#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>

namespace Evo {

struct Particle {
    float x, y, vx, vy, life, maxLife;
    sf::Color col;
};

class ParticleSystem {
public:
    void burst(float x, float y, sf::Color col, int n = 14) {
        for (int i = 0; i < n; i++) {
            float angle = (float)(rand() % 628) / 100.f;
            float speed = 0.5f + (float)(rand() % 30) / 10.f;
            Particle p;
            p.x=x; p.y=y;
            p.vx=std::cos(angle)*speed;
            p.vy=std::sin(angle)*speed - 1.2f;
            p.life=p.maxLife=28.f+(rand()%20);
            p.col=col;
            parts_.push_back(p);
        }
    }

    void update() {
        for(auto& p:parts_){ p.x+=p.vx; p.y+=p.vy; p.vy+=0.12f; p.life-=1.f; }
        parts_.erase(std::remove_if(parts_.begin(),parts_.end(),
            [](const Particle& p){ return p.life<=0; }),parts_.end());
    }

    void draw(sf::RenderTarget& rt, float ps) {
        sf::VertexArray va(sf::PrimitiveType::Triangles);
        for(const auto& p:parts_){
            float a=p.life/p.maxLife;
            sf::Color c=p.col; c.a=(uint8_t)(a*220);
            float sz=ps*(0.5f+a);
            float x=p.x,y=p.y,w=sz,h=sz;
            va.append({{x,  y  },c}); va.append({{x+w,y  },c}); va.append({{x,  y+h},c});
            va.append({{x,  y+h},c}); va.append({{x+w,y  },c}); va.append({{x+w,y+h},c});
        }
        rt.draw(va);
    }
private:
    std::vector<Particle> parts_;
};

} // namespace Evo
