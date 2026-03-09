#pragma once
// =============================================================================
//  Sprites.h  -  Original pixel-art. Bright Flappy-vibe aesthetic.
//  Red chubby bird, green pipes, sunny blue sky. SFML 3 (Triangles only).
// =============================================================================
#include <SFML/Graphics.hpp>
#include <cmath>

namespace Spr {

// -- 2-triangle rectangle (SFML3 has no Quads) --------------------------------
static void rect(sf::VertexArray& va, float x, float y, float w, float h, sf::Color c){
    va.append({{x,  y  },c}); va.append({{x+w,y  },c}); va.append({{x,  y+h},c});
    va.append({{x,  y+h},c}); va.append({{x+w,y  },c}); va.append({{x+w,y+h},c});
}
static void sq(sf::VertexArray& va,float x,float y,float s,sf::Color c){ rect(va,x,y,s,s,c); }

// -- Solid filled rectangle helper (direct draw) -------------------------------
static void fillRect(sf::RenderTarget& rt,float x,float y,float w,float h,sf::Color c){
    sf::VertexArray va(sf::PrimitiveType::Triangles);
    rect(va,x,y,w,h,c);
    rt.draw(va);
}

// =============================================================================
//  BIRD  -  20?18 chubby red cardinal, hand-crafted pixel map
//  Colours: 0=transparent 1=body(red) 2=shade(dark red) 3=belly(orange-red)
//           4=wing(crimson) 5=eye(black) 6=pupil(white) 7=beak(yellow)
//           8=crest(dark red tip) 9=highlight(bright red)
// =============================================================================
static void drawBird(sf::RenderTarget& rt, float bx, float by, float ps,
                     bool flapUp, sf::Color tint={255,255,255}, float alpha=1.f)
{
    auto A=[&](sf::Color c)->sf::Color{
        return {(uint8_t)(c.r*tint.r/255),
                (uint8_t)(c.g*tint.g/255),
                (uint8_t)(c.b*tint.b/255),
                (uint8_t)(c.a*alpha)};
    };
    sf::Color C[10]={
        {0,0,0,0},        // 0 transparent
        A({220, 40, 20}), // 1 body red
        A({150, 15,  5}), // 2 shade dark red
        A({240,100, 40}), // 3 belly orange
        A({180, 20, 10}), // 4 wing crimson
        A({ 15, 10, 10}), // 5 eye black
        A({240,240,255}), // 6 pupil white
        A({255,210,  0}), // 7 beak yellow
        A({100,  0,  0}), // 8 crest tip
        A({255, 90, 70}), // 9 highlight
    };

    // 20 cols ? 18 rows
    static const uint8_t BODY[18][20]={
        {0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,8,1,1,8,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,8,1,1,1,1,8,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,1,1,9,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
        {0,0,1,1,1,1,5,1,1,1,1,1,1,1,0,0,0,0,0,0},
        {0,1,1,1,1,1,5,6,1,1,1,1,1,1,7,7,0,0,0,0},
        {0,1,2,1,1,1,1,1,1,3,3,1,1,1,7,7,0,0,0,0},
        {1,1,2,2,1,1,1,1,3,3,3,3,1,1,1,1,0,0,0,0},
        {1,2,2,2,1,1,1,3,3,3,3,3,3,1,1,1,0,0,0,0},
        {1,4,2,2,2,1,3,3,3,3,3,3,3,1,1,0,0,0,0,0},
        {0,4,4,2,2,2,3,3,3,3,3,3,1,1,0,0,0,0,0,0},
        {0,4,4,4,2,2,2,3,3,3,3,1,1,0,0,0,0,0,0,0},
        {0,0,4,4,4,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,4,4,4,2,2,2,1,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,4,4,4,2,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,4,4,2,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,4,2,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };
    // Wing raised (flap up): wing pixels shift up 3 rows
    static const uint8_t WINGUP[18][20]={
        {0,0,0,0,0,8,8,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,4,4,0,8,1,1,8,0,0,0,0,0,0,0,0,0,0,0,0},
        {4,4,4,8,1,1,1,1,8,0,0,0,0,0,0,0,0,0,0,0},
        {4,4,1,1,1,9,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
        {0,4,1,1,1,1,5,1,1,1,1,1,1,1,0,0,0,0,0,0},
        {0,1,1,1,1,1,5,6,1,1,1,1,1,1,7,7,0,0,0,0},
        {0,1,2,1,1,1,1,1,1,3,3,1,1,1,7,7,0,0,0,0},
        {1,1,2,2,1,1,1,1,3,3,3,3,1,1,1,1,0,0,0,0},
        {1,2,2,2,1,1,1,3,3,3,3,3,3,1,1,1,0,0,0,0},
        {1,1,2,2,2,1,3,3,3,3,3,3,3,1,1,0,0,0,0,0},
        {0,0,1,2,2,2,3,3,3,3,3,3,1,1,0,0,0,0,0,0},
        {0,0,0,1,2,2,2,3,3,3,3,1,1,0,0,0,0,0,0,0},
        {0,0,0,0,1,2,2,2,2,2,1,1,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,1,2,2,2,1,1,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,1,2,1,1,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,2,1,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };
    const auto& M = flapUp ? WINGUP : BODY;
    sf::VertexArray va(sf::PrimitiveType::Triangles);
    for(int r=0;r<18;r++)
        for(int c=0;c<20;c++)
            if(M[r][c]) sq(va, bx+c*ps, by+r*ps, ps, C[M[r][c]]);
    rt.draw(va);
}

// =============================================================================
//  PIPE  -  green pixel-art with cap, light/dark stripes
// =============================================================================
static void drawPipe(sf::RenderTarget& rt,float px,float py,
                     float pw,float ph,bool isTop,float ps)
{
    sf::Color body={60,180,40}, dark={30,110,20}, lite={100,220,60},
              cap={50,200,40},  capDk={20,120,20}, capLt={120,240,80};

    sf::VertexArray va(sf::PrimitiveType::Triangles);
    int cols=(int)(pw/ps)+1, rows=(int)(ph/ps)+1;
    for(int r=0;r<rows;r++)
        for(int c=0;c<cols;c++){
            sf::Color col;
            if     (c==0||c==cols-1) col=dark;
            else if(c==1)            col=lite;
            else                     col=body;
            sq(va,px+c*ps,py+r*ps,ps,col);
        }
    // Cap
    float cX=px-ps*2, cW=pw+ps*4, cH=ps*5;
    float cY=isTop?py+ph-cH:py;
    int cc=(int)(cW/ps)+1, cr=(int)(cH/ps)+1;
    for(int r=0;r<cr;r++)
        for(int c=0;c<cc;c++){
            sf::Color col;
            if     (c==0||c==cc-1) col=capDk;
            else if(c==1||r==0)    col=capLt;
            else                   col=cap;
            sq(va,cX+c*ps,cY+r*ps,ps,col);
        }
    rt.draw(va);
}

// =============================================================================
//  CLOUD  -  fluffy 3-bump cloud, pixel art
// =============================================================================
static void drawCloud(sf::RenderTarget& rt,float cx,float cy,float ps,float scale=1.f){
    // 24?12 map  1=outline 2=fill 3=highlight
    static const uint8_t CL[12][24]={
        {0,0,0,1,1,1,0,0,0,0,1,1,1,0,0,0,0,1,1,1,1,0,0,0},
        {0,0,1,2,2,2,1,0,0,1,2,2,2,1,0,0,1,2,2,2,2,1,0,0},
        {0,1,2,3,2,2,2,1,1,2,3,2,2,2,1,1,2,3,2,2,2,2,1,0},
        {1,2,3,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1},
        {0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0},
        {0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0},
        {0,0,0,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0},
        {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
    };
    sf::VertexArray va(sf::PrimitiveType::Triangles);
    sf::Color outline={180,200,220},fill={250,252,255},hi={255,255,255};
    sf::Color C[]={{0,0,0,0},outline,fill,hi};
    float sp=ps*scale;
    for(int r=0;r<12;r++)
        for(int c=0;c<24;c++)
            if(CL[r][c]) sq(va,cx+c*sp,cy+r*sp,sp,C[CL[r][c]]);
    rt.draw(va);
}

// =============================================================================
//  SUN  -  pixel-art sun with rays
// =============================================================================
static void drawSun(sf::RenderTarget& rt,float cx,float cy,float ps){
    sf::VertexArray va(sf::PrimitiveType::Triangles);
    sf::Color core={255,240,60},mid={255,210,30},ray={255,200,0,180};
    // Rays (8 directions, rectangles)
    float rl=ps*6,rw=ps*1.5f;
    float angles[]={0,45,90,135,180,225,270,315};
    for(float ang:angles){
        float rad=ang*3.14159f/180.f;
        float dx=std::cos(rad),dy=std::sin(rad);
        float sx=cx+dx*ps*7, sy=cy+dy*ps*7;
        // Draw ray as thin rect rotated - approximate with points
        for(int i=0;i<(int)(rl/ps);i++){
            float rx=sx+dx*i*ps-dy*rw/2;
            float ry=sy+dy*i*ps+dx*rw/2;
            sq(va,rx,ry,ps,ray);
        }
    }
    // Core disc
    for(int r=-5;r<=5;r++)
        for(int c=-5;c<=5;c++){
            float d=std::sqrt((float)(r*r+c*c));
            if(d<=5.f){
                sf::Color col=d<3?core:mid;
                sq(va,cx+c*ps,cy+r*ps,ps,col);
            }
        }
    rt.draw(va);
}

// =============================================================================
//  HILLS  -  rounded background hills, two layers
// =============================================================================
static void drawHills(sf::RenderTarget& rt,float w,float groundY,
                      float scroll,float ps)
{
    // Back hills (lighter, slower)
    sf::Color hBack={120,200,80}, hBackLt={150,220,100};
    // Front hills (darker, faster)
    sf::Color hFront={80,170,50}, hFrontLt={110,190,70};

    auto hill=[&](float offX,float baseY,float amp,float period,
                  sf::Color c,sf::Color ct,float spd){
        float sc=std::fmod(scroll*spd,period);
        sf::VertexArray va(sf::PrimitiveType::Triangles);
        for(float x=0;x<w+period;x+=ps){
            float hx=x-sc;
            float hy=baseY-amp*(0.5f+0.5f*std::sin(hx/period*2*3.14159f));
            // Column of pixels from hy down to baseY
            float colH=baseY-hy;
            if(colH>0) rect(va,hx,hy,ps,colH, (int(hx/ps)%3==0)?ct:c);
        }
        rt.draw(va);
    };
    hill(0,groundY-ps*4, ps*20, 280, hBack,  hBackLt,  0.2f);
    hill(0,groundY-ps*2, ps*12, 180, hFront, hFrontLt, 0.35f);
}

// =============================================================================
//  GROUND  -  green top with brown soil, checker pattern
// =============================================================================
static void drawGround(sf::RenderTarget& rt,float y,float w,float scroll,float ps){
    sf::VertexArray va(sf::PrimitiveType::Triangles);
    float off=std::fmod(scroll*0.8f,ps*8);
    int cols=(int)(w/(ps*8))+3;
    for(int c=0;c<cols;c++){
        float gx=c*ps*8-off;
        bool dk=(c%2==0);
        rect(va,gx,y,       ps*8,ps*4, dk?sf::Color{60,180,40}:sf::Color{80,200,55});
        rect(va,gx,y+ps*4,  ps*8,ps*4, dk?sf::Color{140,90,40}:sf::Color{160,110,55});
        rect(va,gx,y+ps*8,  ps*8,ps*20,{120,75,30});
    }
    // Top grass highlight strip
    rect(va,0,y,w,ps,{120,220,80});
    rt.draw(va);
}

// =============================================================================
//  TREES  -  simple pixel-art pine trees in background
// =============================================================================
static void drawTrees(sf::RenderTarget& rt,float groundY,float w,float scroll,float ps){
    sf::VertexArray va(sf::PrimitiveType::Triangles);
    sf::Color trunk={120,70,30}, leaf1={40,140,30}, leaf2={60,170,40}, leaf3={80,190,50};
    float period=120*ps;
    float sc=std::fmod(scroll*0.25f,period);
    for(float tx=-period;tx<w+period;tx+=period){
        float x=tx-sc;
        // Trunk
        rect(va,x+ps*4,groundY-ps*8, ps*4,ps*8,trunk);
        // Three tiers of leaves
        for(int t=0;t<3;t++){
            float tw=(float)(7-t)*ps*2, th=ps*6;
            float ty=groundY-ps*14-t*ps*5;
            float tx2=x+ps*6-tw/2;
            sf::Color lc=t==0?leaf1:t==1?leaf2:leaf3;
            rect(va,tx2,ty,tw,th,lc);
            // highlight
            rect(va,tx2+tw*0.3f,ty+ps,ps,ps*2,{(uint8_t)(lc.r+30),(uint8_t)(lc.g+30),(uint8_t)(lc.b+20)});
        }
    }
    rt.draw(va);
}

} // namespace Spr
