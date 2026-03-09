#pragma once
// =============================================================================
//  GameRenderer.h  -  Game world 1380x1080, clean layout matching ref design
// =============================================================================
#include "Bird.h"
#include "Assets.h"
#include "Particles.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>
#include <algorithm>

namespace Evo {

constexpr int   GAME_RT_W  = 1320;
constexpr int   GAME_RT_H  = 1080;
constexpr int   TOP_BAR_H  = 90;
constexpr float BG_SCALE   = (float)(GAME_RT_H - TOP_BAR_H) / 512.f;
constexpr float BIRD_SCALE = 3.5f;

inline float groundY() { return GAME_RT_H - 112.f * BG_SCALE + TOP_BAR_H; }

static const sf::Color MUT_TINTS[4]={
    {120,180,255},{120,255,140},{255,200,80},{220,120,255}};

class GameRenderer {
public:
    explicit GameRenderer(Assets& assets) : assets_(assets) {
        (void)rt_.resize({(unsigned)GAME_RT_W,(unsigned)GAME_RT_H});
        initFont();
    }

    void onBirdDied(float x,float y,sf::Color c){ particles_.burst(x,y,c); }

    void update(float dt){
        baseScroll_ += PIPE_SPEED*60.f*dt*BG_SCALE;
        float baseW=336.f*BG_SCALE;
        if(baseScroll_>=baseW) baseScroll_-=baseW;
        particles_.update();
    }

    void render(const std::vector<Bird>& pop,
                const std::vector<Pipe>& pipes,
                const GenStats& stats,
                int alive, int topScore, int clkCycles,
                int frame, float speedMult)
    {
        rt_.clear({100,180,240});
        drawBackground();
        drawPipes(pipes);
        drawGround();
        drawBirds(pop, frame);
        particles_.draw(rt_, 2.f);
        drawScore(topScore);
        drawTopBar(stats, alive, topScore, clkCycles, speedMult, pop);
        drawOverlays(stats, alive, topScore, clkCycles, speedMult);
        rt_.display();
    }

    const sf::RenderTexture& texture() const { return rt_; }

private:
    Assets&           assets_;
    sf::RenderTexture rt_{{(unsigned)GAME_RT_W,(unsigned)GAME_RT_H}};
    sf::Font          font_;
    ParticleSystem    particles_;
    float             baseScroll_=0;
    bool              showSpeed_=false;
    float             speedPopTimer_=0;
    float             lastSpeed_=1;

    void initFont(){
        const char* fonts[]={
            "C:/Windows/Fonts/consola.ttf","C:/Windows/Fonts/cour.ttf",
            "C:/Windows/Fonts/arial.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf",nullptr};
        for(int i=0;fonts[i];i++) if(font_.openFromFile(fonts[i])) break;
    }

    void T(const std::string& s,unsigned sz,sf::Color c,float x,float y){
        sf::Text t(font_,s,sz); t.setFillColor(c); t.setPosition({x,y});
        rt_.draw(t);
    }
    void box(float x,float y,float w,float h,sf::Color fill,sf::Color bdr={0,0,0,0},float bt=0){
        sf::RectangleShape r(sf::Vector2f(w,h));
        r.setPosition({x,y}); r.setFillColor(fill);
        if(bt>0){r.setOutlineThickness(bt);r.setOutlineColor(bdr);}
        rt_.draw(r);
    }

    // ── Slim top bar (46px) matching reference ────────────────────────────────
    void drawTopBar(const GenStats& stats, int alive, int /*topScore*/,
                    int /*clkCycles*/, float speedMult,
                    const std::vector<Bird>& pop)
    {
        // Dark background
        box(0,0,(float)GAME_RT_W,(float)TOP_BAR_H,{18,18,24});
        // Bottom border line
        box(0,TOP_BAR_H-2,(float)GAME_RT_W,2,{60,120,80});

        // Title
        T("EVOLUTION-V",20,{255,200,60},12,10);
        T("4-in | 3-hidden | 1-out",11,{100,120,100},12,36);

        // Stat cells
        float cx=210;
        auto cell=[&](const std::string& lbl,const std::string& val,sf::Color vc){
            T(lbl,10,{120,130,110},cx,12);
            T(val,24,vc,cx,30);
            cx+=140;
        };

        int tot=(int)pop.size();
        int al=alive;
        sf::Color aliveC=al>0?sf::Color{80,220,100}:sf::Color{220,80,80};
        cell("GEN",   std::to_string(stats.generation),{255,220,60});
        cell("ALIVE", std::to_string(al)+"/"+std::to_string(tot), aliveC);
        cell("SCORE", std::to_string(stats.eliteScore),{200,200,255});
        cell("FN",    std::to_string(stats.eliteFrames),{180,180,220});
        cell("SPEED", std::to_string((int)speedMult)+"x",
             speedMult>1?sf::Color{60,255,120}:sf::Color{180,180,200});
    }

    // ── Game overlays: LEAD BIRD box, SYS_CLK box ────────────────────────────
    void drawOverlays(const GenStats& /*stats*/, int /*alive*/, int /*topScore*/,
                      int clkCycles, float /*speedMult*/)
    {
        // SYS_CLK bottom right
        float bx=GAME_RT_W-300.f, by=GAME_RT_H-70.f;
        box(bx,by,290,60,{20,30,40},{60,120,160},2);
        T("SYS_CLK: 12.0 MHz",13,{100,160,220},bx+10,by+8);
        std::ostringstream ss;
        ss<<"CLK: "<<std::setw(9)<<std::setfill('0')<<clkCycles;
        T(ss.str(),13,{100,200,255},bx+10,by+30);
    }

    // ── Background ───────────────────────────────────────────────────────────
    void drawBackground(){
        float bgW=288.f*BG_SCALE;
        int tiles=(int)std::ceil((float)GAME_RT_W/bgW)+1;
        for(int i=0;i<tiles;i++){
            sf::Sprite s(assets_.bg);
            s.setScale({BG_SCALE,BG_SCALE});
            s.setPosition({i*bgW,(float)TOP_BAR_H});
            rt_.draw(s);
        }
    }

    void drawPipes(const std::vector<Pipe>& pipes){
        for(const auto& p:pipes){
            float gTop=p.gapY-PIPE_GAP/2.f+(float)TOP_BAR_H;
            float gBot=p.gapY+PIPE_GAP/2.f+(float)TOP_BAR_H;
            // Bottom pipe
            if(gBot<(float)GAME_RT_H){
                sf::Sprite s(assets_.pipe);
                s.setScale({BG_SCALE,BG_SCALE});
                s.setPosition({p.x,gBot});
                rt_.draw(s);
            }
            // Top pipe - only below top bar
            if(gTop>(float)TOP_BAR_H){
                sf::Sprite s2(assets_.pipe);
                s2.setScale({BG_SCALE,-BG_SCALE});
                s2.setPosition({p.x,gTop});
                rt_.draw(s2);
            }
        }
        // Redraw top bar to clip any pipe bleed
        box(0,0,(float)GAME_RT_W,(float)TOP_BAR_H,{18,18,24});
        box(0,TOP_BAR_H-2,(float)GAME_RT_W,2,{60,120,80});
    }

    void drawGround(){
        float baseW=336.f*BG_SCALE;
        float gy=groundY();
        int tiles=(int)std::ceil((float)GAME_RT_W/baseW)+2;
        for(int i=0;i<tiles;i++){
            sf::Sprite s(assets_.base);
            s.setScale({BG_SCALE,BG_SCALE});
            s.setPosition({i*baseW-baseScroll_,gy});
            rt_.draw(s);
        }
    }

    void drawBirds(const std::vector<Bird>& pop, int frame){
        const Bird* best=nullptr;
        for(const auto& b:pop)
            if(b.alive&&(!best||b.score>best->score||
                (b.score==best->score&&b.frames>best->frames))) best=&b;

        float bw=34.f*BIRD_SCALE, bh=24.f*BIRD_SCALE;
        float sw=34.f*BIRD_SCALE*0.5f, sh=24.f*BIRD_SCALE*0.5f;

        // Ghost dead birds
        for(const auto& b:pop){
            if(b.alive) continue;
            float cy=b.y+(float)TOP_BAR_H;
            cy=std::max(cy,(float)TOP_BAR_H+10.f);
            if(cy>(float)GAME_RT_H) continue;
            sf::Sprite s(assets_.birdMid);
            s.setScale({BIRD_SCALE*0.5f,BIRD_SCALE*0.5f});
            s.setColor({255,255,255,18});
            s.setPosition({(float)BIRD_X-sw*0.5f, cy-sh*0.5f});
            rt_.draw(s);
        }
        // Non-elite alive - small tinted sprites
        for(const auto& b:pop){
            if(!b.alive||&b==best) continue;
            float cy=b.y+(float)TOP_BAR_H;
            cy=std::max(cy,(float)TOP_BAR_H+10.f);
            if(cy>(float)GAME_RT_H) continue;
            sf::Color tc=MUT_TINTS[b.id%4];
            const sf::Texture& tex=getBirdTex(b.id,frame);
            sf::Sprite s(tex);
            s.setScale({BIRD_SCALE*0.55f,BIRD_SCALE*0.55f});
            s.setColor({tc.r,tc.g,tc.b,160});
            s.setPosition({(float)BIRD_X-sw*0.25f, cy-sh*0.25f});
            rt_.draw(s);
        }
        // Elite bird with glow
        if(best){
            float cy=std::max(best->y+(float)TOP_BAR_H, (float)TOP_BAR_H+20.f);
            for(int r=0;r<3;r++){
                sf::CircleShape h(bw*0.5f+r*6);
                h.setOrigin({bw*0.5f+r*6,bw*0.5f+r*6});
                h.setPosition({(float)BIRD_X+bw/2,cy});
                h.setFillColor({0,0,0,0});
                h.setOutlineThickness(3);
                h.setOutlineColor({255,220,0,(uint8_t)(50-r*15)});
                rt_.draw(h);
            }
            const sf::Texture& tex=best->flap_out?assets_.birdUp
                :(frame/8)%2==0?assets_.birdMid:assets_.birdDown;
            sf::Sprite s(tex);
            s.setScale({BIRD_SCALE,BIRD_SCALE});
            s.setOrigin({17,12});
            float rot=std::clamp(best->vy*3.f,-25.f,35.f);
            s.setPosition({(float)BIRD_X+bw/2, cy});
            s.setRotation(sf::degrees(rot));
            rt_.draw(s);
            drawBirdHUD(*best);
        }
    }

    const sf::Texture& getBirdTex(int id,int frame){
        int ph=(frame/8+id)%3;
        return ph==0?assets_.birdDown:ph==1?assets_.birdMid:assets_.birdUp;
    }

    void drawBirdHUD(const Bird& b){
        float bx=10.f, by=(float)GAME_RT_H-148.f;
        auto toQ=[](float v)->std::string{
            std::ostringstream ss;
            int16_t q=(int16_t)(v*256.f);
            ss<<"0x"<<std::hex<<std::uppercase<<std::setw(4)
              <<std::setfill('0')<<(uint16_t)q;
            return ss.str();
        };
        box(bx,by,240,118,{16,20,30,210},{80,140,200},2);
        // Header bar
        box(bx,by,240,22,{30,40,70,220});
        T("LEAD BIRD",12,{255,200,0},bx+8,by+4);
        T("B"+std::to_string(b.id),10,{120,140,180},bx+192,by+5);
        // Stats
        T("W_H:",11,{80,140,200},bx+8,by+28);
        T(toQ(b.wHeight()),11,{160,210,255},bx+50,by+28);
        T("W_V:",11,{80,200,140},bx+8,by+46);
        T(toQ(b.wVel()),11,{160,255,200},bx+50,by+46);
        // FLAP indicator — big, colored, can't miss it
        bool fl=b.flap_latch;
        sf::Color fBg=fl?sf::Color{20,80,20}:sf::Color{30,20,20};
        sf::Color fTx=fl?sf::Color{60,255,80}:sf::Color{180,80,80};
        box(bx+8,by+62,224,26,fBg,fl?sf::Color{60,200,60}:sf::Color{140,60,60},1.5f);
        T("FLAP: "+(fl?std::string("1  [FLAP  ^]"):std::string("0  [fall  v]")),12,fTx,bx+14,by+66);
        T("SCORE: "+std::to_string(b.score),12,{220,220,100},bx+8,by+96);
    }

    void drawScore(int topScore){
        if(topScore<=0) return;
        std::string s=std::to_string(topScore);
        float dw=28.f;
        float sx=(GAME_RT_W-s.size()*dw)/2.f;
        float sy=(float)TOP_BAR_H+30.f;
        for(char c:s){
            int d=c-'0';
            sf::Sprite sp(assets_.digits[d]);
            sp.setScale({1.8f,1.8f});
            sp.setPosition({sx,sy});
            rt_.draw(sp);
            sx+=dw;
        }
    }
};

} // namespace Evo
