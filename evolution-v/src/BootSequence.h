#pragma once
// =============================================================================
//  BootSequence.h  -  Retro DE1-SoC FPGA boot terminal
// =============================================================================
#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <chrono>

namespace Evo {

constexpr int BOOT_W = 1920, BOOT_H = 1080;

struct BootLine {
    std::string prefix;   // e.g. "[  OK  ]"
    std::string text;
    sf::Color   prefixCol;
    sf::Color   textCol;
    float       delay;    // seconds before appearing
    bool        blink;    // cursor blink after this line
};

static const std::vector<BootLine> BOOT_LINES = {
    {"",        "DE1-SoC FPGA BOOT SEQUENCE v3.1.4",          {0,255,65},{180,255,180},  0.0f, false},
    {"",        "Copyright (c) EVOLUTION-V Systems Lab",       {0,80,20}, {0,100,30},    0.1f, false},
    {"",        "--------------------------------------------",{0,40,10}, {0,40,10},     0.15f,false},
    {"[INIT] ", "Checking SDRAM controller...",                {0,255,65},{0,200,50},    0.3f, false},
    {"[  OK  ]","SDRAM 256MB @ 400MHz",                       {0,255,65},{180,255,180},  0.6f, false},
    {"[INIT] ", "Loading PLL configurations...",               {0,255,65},{0,200,50},    0.8f, false},
    {"[  OK  ]","PLL locked: 12.0 MHz SYS_CLK",              {0,255,65},{180,255,180},  1.1f, false},
    {"[INIT] ", "Mapping NNE_WEIGHT_REGS...",                  {0,255,65},{0,200,50},    1.3f, false},
    {"[  OK  ]","Registers: W_HEIGHT @ 0x0000, W_VEL @ 0x0002",{0,255,65},{180,255,180},1.6f, false},
    {"[LOAD] ", "Uploading bitstream FlappyController.sv...", {255,176,0},{200,140,0},   1.9f, false},
    {"[....] ", "Configuring 17,408 logic elements",          {255,176,0},{160,120,0},   2.2f, false},
    {"[  OK  ]","Bitstream verified. CRC: 0xDEADBEEF",        {0,255,65},{180,255,180},  2.6f, false},
    {"[INIT] ", "Spawning GA population (N=30)...",            {0,255,65},{0,200,50},    2.8f, false},
    {"[  OK  ]","Population initialised. sigma=0.120",        {0,255,65},{180,255,180},  3.1f, false},
    {"[INIT] ", "Starting Verilator HIL bridge...",            {0,255,65},{0,200,50},    3.3f, false},
    {"[  OK  ]","RTL<->Physics bridge ONLINE",                {0,255,65},{180,255,180},  3.6f, false},
    {"[INIT] ", "Calibrating VGA output (1920x1080)...",       {0,255,65},{0,200,50},    3.8f, false},
    {"[  OK  ]","Display: 60Hz, pixel-perfect scaling",       {0,255,65},{180,255,180},  4.0f, false},
    {"",        "--------------------------------------------",{0,40,10}, {0,40,10},     4.2f, false},
    {"",        "ALL SYSTEMS NOMINAL. LAUNCHING SIMULATION...",{0,255,65},{0,255,65},    4.4f, true },
};

// --- Chip SVG drawn in SFML primitives ---------------------------------------
static void drawChip(sf::RenderTarget& rt, float cx, float cy, float sz,
                     float progress /* 0..1 */)
{
    // Chip body
    sf::RectangleShape body(sf::Vector2f(sz, sz));
    body.setOrigin(sf::Vector2f(sz/2,sz/2));
    body.setPosition(sf::Vector2f(cx,cy));
    body.setFillColor({4,20,8});
    body.setOutlineThickness(3);
    body.setOutlineColor({0,255,65});
    rt.draw(body);

    // Pins
    int pins=8;
    for(int i=0;i<pins;i++){
        float t=(float)i/(pins-1);
        float px1=cx-sz/2-18, py1=cy-sz*0.4f+t*sz*0.8f;
        sf::RectangleShape pin(sf::Vector2f(18,3));
        pin.setPosition(sf::Vector2f(px1,py1));
        pin.setFillColor(t<=progress?sf::Color{0,255,65}:sf::Color{0,40,15});
        rt.draw(pin);
        pin.setPosition(sf::Vector2f(cx+sz/2,py1));
        rt.draw(pin);
    }

    // Internal grid filling in as progress advances
    int grid=8;
    float cell=sz/(grid+1);
    int filled=(int)(progress*grid*grid);
    int cnt=0;
    for(int r=0;r<grid&&cnt<filled;r++){
        for(int c=0;c<grid&&cnt<filled;c++,cnt++){
            float gx=cx-sz/2+cell*(c+1)-cell/2+4;
            float gy=cy-sz/2+cell*(r+1)-cell/2+4;
            sf::RectangleShape dot(sf::Vector2f(cell-6,cell-6));
            dot.setPosition(sf::Vector2f(gx,gy));
            dot.setFillColor({0,(uint8_t)(80+cnt*2),20,180});
            dot.setOutlineThickness(1);
            dot.setOutlineColor({0,80,20});
            rt.draw(dot);
        }
    }

    // Neural connections radiating outward
    int nc=6;
    for(int i=0;i<nc;i++){
        float a=(float)i/nc*3.14159f*2;
        float r=sz*0.55f*progress;
        float ex=cx+std::cos(a)*r;
        float ey=cy+std::sin(a)*r;
        sf::Color lc={0,255,65,(uint8_t)(60+progress*100)};
        sf::Vertex line[]={{sf::Vector2f(cx,cy),lc},{sf::Vector2f(ex,ey),lc}};
        rt.draw(line,2,sf::PrimitiveType::Lines);
        sf::CircleShape nd(4);
        nd.setOrigin(sf::Vector2f(4,4));
        nd.setPosition(sf::Vector2f(ex,ey));
        nd.setFillColor({0,200,50,(uint8_t)(120*progress)});
        rt.draw(nd);
    }
}

// --- BootSequence -------------------------------------------------------------
class BootSequence {
public:
    BootSequence() : start_(std::chrono::steady_clock::now()) {}

    // Returns true when boot is complete and sim should start
    bool update(float dt){
        elapsed_ += dt;
        // Auto-finish after last line + 0.8s
        if(!done_ && elapsed_ > BOOT_LINES.back().delay + 0.8f){
            done_=true;
        }
        blinkTimer_+=dt;
        return done_;
    }

    void draw(sf::RenderWindow& win, const sf::Font& font){
        win.clear({2,8,4});

        // Left: terminal
        float tx=60, ty=80;
        for(const auto& line : BOOT_LINES){
            if(elapsed_ < line.delay) break;
            if(!line.prefix.empty()){
                sf::Text pre(font, line.prefix, 15);
                pre.setFillColor(line.prefixCol);
                pre.setPosition(sf::Vector2f(tx,ty));
                win.draw(pre);
            }
            sf::Text txt(font, line.text, 15);
            txt.setFillColor(line.textCol);
            txt.setPosition(sf::Vector2f(tx+110,ty));
            win.draw(txt);
            ty+=22;
        }

        // Blinking cursor
        if((int)(blinkTimer_*2)%2==0){
            sf::RectangleShape cur(sf::Vector2f(10,16));
            cur.setPosition(sf::Vector2f(tx+110,ty));
            cur.setFillColor({0,255,65});
            win.draw(cur);
        }

        // Right: chip animation
        float chipProgress = std::min(1.f, elapsed_/4.4f);
        drawChip(win, 1400, 540, 220, chipProgress);

        // Progress bar
        float barW=(BOOT_W-120)*chipProgress;
        sf::RectangleShape bar(sf::Vector2f(barW, 8));
        bar.setPosition(sf::Vector2f(60, BOOT_H-60));
        bar.setFillColor({0,255,65});
        win.draw(bar);
        sf::RectangleShape barBg(sf::Vector2f(BOOT_W-120,8));
        barBg.setPosition(sf::Vector2f(60,BOOT_H-60));
        barBg.setFillColor({0,0,0,0});
        barBg.setOutlineThickness(1);
        barBg.setOutlineColor({0,60,20});
        win.draw(barBg);

        // Scanlines
        for(int y=0;y<BOOT_H;y+=3){
            sf::RectangleShape sl(sf::Vector2f((float)BOOT_W,1));
            sl.setPosition(sf::Vector2f(0,(float)y));
            sl.setFillColor({0,0,0,28});
            win.draw(sl);
        }
    }

private:
    float elapsed_=0, blinkTimer_=0;
    bool  done_=false;
    std::chrono::steady_clock::time_point start_;
};

} // namespace Evo
