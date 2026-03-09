// =============================================================================
//  main.cpp  -  Evolution-V  1920x1080
//  Speed: +/- keys  (1x to 10x, shown in HUD)
// =============================================================================
#include "Bird.h"
#include "Assets.h"
#include "PhysicsEngine.h"
#include "GeneticAlgorithm.h"
#include "GameRenderer.h"
#include "NeuralPanel.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <optional>

// Required by Verilator when not using SystemC
#ifdef USE_VERILATOR
double sc_time_stamp() { return 0; }
#endif

int main() {
    std::srand((unsigned)std::time(nullptr));

    sf::RenderWindow window(sf::VideoMode({1920u,1080u}),
        "EVOLUTION-V", sf::State::Fullscreen);
    window.setFramerateLimit(60);

    sf::Font font;
    const char* fonts[]={
        "C:/Windows/Fonts/consola.ttf","C:/Windows/Fonts/cour.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",nullptr};
    for(int i=0;fonts[i];i++) if(font.openFromFile(fonts[i])) break;

    Evo::Assets assets;
    if (!assets.load("assets/") && !assets.load("../assets/"))
        std::cerr << "Warning: some assets missing.\n";

    Evo::GeneticAlgorithm ga(Evo::MUTATION_SIG);
    std::vector<Evo::Bird> population;
    ga.initPopulation(population, Evo::POP_SIZE);

    Evo::PhysicsEngine physics(population);
    Evo::GameRenderer  gameRend(assets);
    Evo::NeuralPanel   neuralPanel;
    neuralPanel.init(Evo::PANEL_W, Evo::WIN_H);

    Evo::GenStats stats;
    stats.generation = 1;

    int   topScore=0, frame=0;
    float simHz=60.f, speedMult=1.f;
    sf::Clock clock, fpsClock;
    int fpsFrames=0;
    std::vector<bool> wasAlive(Evo::POP_SIZE, true);
    int lastScore=0;
    bool lastFlap=false;

    while (window.isOpen()) {
        float dt = std::min(clock.restart().asSeconds(), 0.05f);

        while (const std::optional<sf::Event> ev = window.pollEvent()) {
            if (ev->is<sf::Event::Closed>()) window.close();
            if (const auto* kp = ev->getIf<sf::Event::KeyPressed>()) {
                if (kp->scancode == sf::Keyboard::Scan::Escape) window.close();
                // Speed control: + / - (also = and numpad)
                if (kp->scancode == sf::Keyboard::Scan::Equal ||
                    kp->scancode == sf::Keyboard::Scan::NumpadPlus)
                    speedMult = std::min(speedMult + 1.f, 10.f);
                if (kp->scancode == sf::Keyboard::Scan::Hyphen ||
                    kp->scancode == sf::Keyboard::Scan::NumpadMinus)
                    speedMult = std::max(speedMult - 1.f, 1.f);
            }
        }

        if (fpsClock.getElapsedTime().asSeconds() >= 1.f) {
            simHz = (float)fpsFrames / fpsClock.restart().asSeconds();
            fpsFrames = 0;
        }
        fpsFrames++;

        int alive = physics.tick(speedMult);
        frame++;

        // Death particles + hit sound
        for (int i=0;i<(int)population.size();i++) {
            if (wasAlive[i] && !population[i].alive) {
                sf::Color pc = Evo::MUT_TINTS[population[i].id%4];
                float px = (float)Evo::BIRD_X + 17*Evo::BIRD_SCALE;
                gameRend.onBirdDied(px, population[i].y, pc);
                if (speedMult <= 2.f) assets.play(assets.sndHit);
            }
            wasAlive[i] = population[i].alive;
        }

        if (topScore > lastScore) {
            if (speedMult <= 2.f) assets.play(assets.sndPoint);
            lastScore = topScore;
        }

        {
            const Evo::Bird* best=nullptr;
            for(const auto& b:population)
                if(b.alive&&(!best||b.score>best->score)) best=&b;
            if(best&&best->flap_out&&!lastFlap&&speedMult<=2.f)
                assets.play(assets.sndWing);
            lastFlap = best ? best->flap_out : false;
        }

        // Waveform samples
        if (physics.frame()%Evo::CLK_DIV==0) {
            const Evo::Bird* best=nullptr;
            for(const auto& b:population)
                if(b.alive&&(!best||b.score>best->score)) best=&b;
            if(best){
                float hN=std::clamp(physics.lastInputs[0]/127.f*0.5f+0.5f,0.f,1.f);
                float vN=std::clamp(physics.lastInputs[1]/127.f*0.5f+0.5f,0.f,1.f);
                float dN=std::clamp(physics.lastInputs[2]/127.f*0.5f+0.5f,0.f,1.f);
                float clkPh=(physics.frame()/Evo::CLK_DIV)%2==0?1.f:0.f;
                neuralPanel.pushSample(best->flap_out,hN,vN,dN,clkPh);
            }
        }

        if (alive==0) {
            int bestIdx=0;
            for(int i=1;i<(int)population.size();i++)
                if(population[i].score>population[bestIdx].score||
                  (population[i].score==population[bestIdx].score&&
                   population[i].frames>population[bestIdx].frames))
                    bestIdx=i;
            if(population[bestIdx].score>topScore)
                topScore=population[bestIdx].score;

            if (speedMult<=2.f) assets.play(assets.sndDie);
            std::cout<<"[GEN "<<std::setw(4)<<ga.generation()<<"]"
                     <<"  score="<<population[bestIdx].score<<"\n";

            stats=ga.evolve(population);
            stats.eliteScore=topScore;
            physics.reset();
            std::fill(wasAlive.begin(),wasAlive.end(),true);
            lastScore=topScore;
        }

        gameRend.update(dt);
        gameRend.render(population, physics.pipes(), stats,
                        alive, topScore, physics.clkCycles(), frame, speedMult);
        neuralPanel.render(font, population, stats,
                           alive, topScore, physics.clkCycles(), simHz, speedMult);

        window.clear();
        sf::Sprite gs(gameRend.texture().getTexture()); window.draw(gs);
        sf::Sprite ps(neuralPanel.texture().getTexture());
        ps.setPosition(sf::Vector2f(1320.f,0.f)); window.draw(ps);
        sf::RectangleShape div(sf::Vector2f(3.f,1080.f));
        div.setPosition(sf::Vector2f(1320.f,0.f));
        div.setFillColor({80,80,80}); window.draw(div);
        window.display();
    }
    return 0;
}
