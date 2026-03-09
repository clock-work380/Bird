#pragma once
// =============================================================================
//  NeuralPanel.h  -  Right panel 540px, matching reference design
//  Sections: NEURAL NET | RTL PIPELINE | LOGIC ANALYZER | BIT REGISTERS | RTL SOURCE
// =============================================================================
#include "Bird.h"
#include "GeneticAlgorithm.h"
#include <SFML/Graphics.hpp>
#include <deque>
#include <sstream>
#include <iomanip>
#include <bitset>
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>

namespace Evo {

constexpr int PANEL_W = 600;
constexpr int WIN_H   = 1080;

// Colour palette matching reference (dark theme, orange section headers)
static const sf::Color C_BG    = {28, 30, 35};
static const sf::Color C_HDR   = {45, 48, 55};
static const sf::Color C_SEC   = {38, 42, 48};   // section bg
static const sf::Color C_SHDR  = {42, 48, 58};   // section header bg
static const sf::Color C_ACC   = {200, 130, 50};  // orange accent (ref uses this)
static const sf::Color C_TXT   = {235, 238, 242};
static const sf::Color C_DIM   = {155, 163, 175};
static const sf::Color C_GRN   = {80,  210, 100};
static const sf::Color C_RED   = {220, 80,  80};
static const sf::Color C_BLU   = {80,  160, 255};
static const sf::Color C_CYN   = {60,  195, 195};
static const sf::Color C_YLW   = {240, 200, 60};
static const sf::Color C_PRP   = {170, 100, 240};
static const sf::Color C_ORG   = {220, 140, 50};

static const sf::Color INPUT_COLS[4]={C_BLU,C_CYN,C_PRP,{255,170,60}};

static sf::Vector2f V(float x,float y){return {x,y};}

static void Rect(sf::RenderTarget& rt,float x,float y,float w,float h,
                 sf::Color fill,sf::Color bdr={0,0,0,0},float bt=0){
    sf::RectangleShape r(sf::Vector2f(w,h));
    r.setPosition(V(x,y)); r.setFillColor(fill);
    if(bt>0){r.setOutlineThickness(bt);r.setOutlineColor(bdr);}
    rt.draw(r);
}
static void Line(sf::RenderTarget& rt,float x1,float y1,float x2,float y2,
                 sf::Color c,float t=1.5f){
    float dx=x2-x1,dy=y2-y1,len=std::sqrt(dx*dx+dy*dy);
    if(len<0.1f)return;
    sf::RectangleShape r(sf::Vector2f(len,t));
    r.setOrigin(V(0,t/2)); r.setPosition(V(x1,y1));
    r.setRotation(sf::radians(std::atan2(dy,dx)));
    r.setFillColor(c); rt.draw(r);
}
static std::string Hex16(float v){
    int16_t q=(int16_t)(v*256.f);
    std::ostringstream s;
    s<<std::hex<<std::uppercase<<std::setw(4)<<std::setfill('0')<<(uint16_t)q;
    return s.str();
}
static std::string F2(float v,int d=2){
    std::ostringstream s;s<<std::fixed<<std::setprecision(d)<<v;return s.str();
}

class NeuralPanel {
public:
    void init(unsigned,unsigned){
        (void)rt_.resize({(unsigned)PANEL_W,(unsigned)WIN_H});
    }

    void pushSample(bool flap,float hN,float vN,float dN,float /*clk*/){
        auto push=[](std::deque<float>& d,float v){
            d.push_back(v);if((int)d.size()>240)d.pop_front();};
        push(wFlap_,flap?1.f:0.f);
        push(wH_,hN); push(wV_,vN); push(wD_,dN);
        ++parallelClks_;
        totalBirdEvals_+=POP_SIZE;
    }
    void updateParallelStats(float dt, int clkCycles){
        // Sequential animation sweep (one bird lit per step)
        seqAnimTimer_+=dt*8.f; // speed of sequential sweep
        if(seqAnimTimer_>=1.f){ seqAnimTimer_=0.f; clkEdgeAnim_=(clkEdgeAnim_+1)%POP_SIZE; }
        // Throughput: birds evaluated per second
        throughputTimer_+=dt;
        if(throughputTimer_>=0.5f){
            float delta=(float)(clkCycles-lastClkCount_);
            currentThroughput_=delta*(float)POP_SIZE/throughputTimer_;
            lastClkCount_=(float)clkCycles;
            throughputTimer_=0.f;
            throughputHistory_.push_back(currentThroughput_);
            if((int)throughputHistory_.size()>60) throughputHistory_.pop_front();
        }
    }

    void render(const sf::Font& font,
                const std::vector<Bird>& pop,
                const GenStats& stats,
                int alive, int topScore, int clkCycles,
                float fps, float speedMult)
    {
        rt_.clear(C_BG);

        // ── Top title bar ────────────────────────────────────────────────────
        Rect(rt_,0,0,(float)PANEL_W,38,C_HDR);
        Line(rt_,0,38,(float)PANEL_W,38,C_ACC,2);
        Tx(font,"EVOLUTION-V",15,C_YLW,10,10);
        Tx(font,"4-in | 3-hidden | 1-out",10,C_DIM,160,14);
        float y=40;

        // ── Sections ─────────────────────────────────────────────────────────
        updateParallelStats(1.f/60.f, clkCycles);
        y=drawNeuralNet(font,y,pop);
        y=drawPipeline(font,y,pop,clkCycles);
        y=drawWaveform(font,y);
        y=drawBitRegs(font,y,pop);
        y=drawParallelViz(font,y,pop,clkCycles,fps);
        drawRTLSource(font,y,stats,alive,topScore,clkCycles,fps,speedMult,pop);

        rt_.display();
    }

    const sf::RenderTexture& texture() const {return rt_;}

private:
    sf::RenderTexture rt_{{(unsigned)PANEL_W,(unsigned)WIN_H}};
    std::deque<float> wFlap_,wH_,wV_,wD_;
    int parallelClks_=0;
    int totalBirdEvals_=0;
    // Parallel viz state
    int clkEdgeAnim_=0;       // 0..29 sweeping bird index for sequential animation
    float seqAnimTimer_=0.f;  // timer for sequential sweep
    std::deque<float> throughputHistory_; // birds/sec over time
    float lastClkCount_=0;
    float throughputTimer_=0.f;
    float currentThroughput_=0.f;

    void Tx(const sf::Font& f,const std::string& s,unsigned sz,sf::Color c,
            float x,float y){
        sf::Text t(f,s,sz); t.setFillColor(c); t.setPosition(V(x,y));
        rt_.draw(t);
    }

    // Section header matching reference: dark bg, orange left bar, white title
    float SecHdr(const sf::Font& f,float y,const std::string& title,
                 const std::string& sub, sf::Color ac=C_ACC){
        Rect(rt_,0,y,(float)PANEL_W,30,C_SHDR);
        Line(rt_,0,y,0,y+30,ac,4);
        Line(rt_,0,y+29,(float)PANEL_W,y+29,{50,55,62},1);
        Tx(f,title,13,{245,248,252},10,y+9);
        Tx(f,"// "+sub,11,{170,178,190},14+(int)title.size()*9,y+9);
        return y+31;
    }

    // ── NEURAL NET ────────────────────────────────────────────────────────────
    float drawNeuralNet(const sf::Font& f,float y,const std::vector<Bird>& pop){
        y=SecHdr(f,y,"NEURAL NET","4-IN  3-HIDDEN  1-OUT",C_BLU);
        const float H=215.f;
        // Tan/beige background matching reference screenshot
        Rect(rt_,0,y,(float)PANEL_W,H,{58,52,38});

        const Bird* best=nullptr;
        for(const auto& b:pop) if(b.alive&&(!best||b.score>best->score)) best=&b;
        if(!best&&!pop.empty()) best=&pop[0];

        const float LX=75,MX=PANEL_W/2.f+10,RX=PANEL_W-45.f;
        const float iY[4]={y+22,y+72,y+122,y+172};
        const float hY[3]={y+55,y+110,y+165};
        const float oY=y+110;
        const char* iLbl[4]={"H_ERR","VEL","DIST","GAP"};

        // Wires input->hidden
        for(int h=0;h<3;h++) for(int i=0;i<4;i++){
            float wv=best?best->w[h][i]:0.f;
            float th=std::clamp(std::abs(wv)*2.f,0.5f,4.5f);
            sf::Color wc=wv>=0?INPUT_COLS[i]:C_RED;
            wc.a=(uint8_t)(60+std::min(1.f,std::abs(wv))*180.f);
            Line(rt_,LX+18,iY[i],MX-20,hY[h],wc,th);
        }
        // Wires hidden->output
        for(int h=0;h<3;h++){
            float wv=best?best->wo[h]:0.f;
            float th=std::clamp(std::abs(wv)*2.f,0.5f,4.5f);
            sf::Color wc=wv>=0?C_GRN:C_RED;
            wc.a=(uint8_t)(60+std::min(1.f,std::abs(wv))*180.f);
            Line(rt_,MX+20,hY[h],RX-24,oY,wc,th);
        }
        // Input nodes
        for(int i=0;i<4;i++){
            sf::CircleShape c(16); c.setOrigin(V(16,16));
            c.setPosition(V(LX,iY[i]));
            c.setFillColor({35,38,48}); c.setOutlineThickness(2.f);
            c.setOutlineColor(INPUT_COLS[i]); rt_.draw(c);
            Tx(f,iLbl[i],11,INPUT_COLS[i],4,iY[i]-7);
            if(best) Tx(f,std::to_string((int)best->in_regs[i]),
                        10,{220,225,235},LX-14,iY[i]+5);
        }
        // Hidden nodes
        for(int h=0;h<3;h++){
            float act=best?(float)best->h_regs[h]/256.f:0.f;
            float ab=std::abs(act);
            sf::Color fill={(uint8_t)(30+ab*100),(uint8_t)(80+ab*130),(uint8_t)(30+ab*70)};
            sf::CircleShape c(20); c.setOrigin(V(20,20));
            c.setPosition(V(MX,hY[h]));
            c.setFillColor(fill); c.setOutlineThickness(2.5f);
            c.setOutlineColor(C_GRN); rt_.draw(c);
            Tx(f,"H"+std::to_string(h),12,{240,243,248},MX-9,hY[h]-9);
            Tx(f,F2(act),10,{175,182,195},MX-16,hY[h]+10);
        }
        // Output node
        bool fl=best&&best->flap_out;
        sf::CircleShape oc(24); oc.setOrigin(V(24,24));
        oc.setPosition(V(RX,oY));
        oc.setFillColor(fl?sf::Color{40,190,60}:sf::Color{170,50,50});
        oc.setOutlineThickness(3); oc.setOutlineColor(fl?C_GRN:C_RED);
        rt_.draw(oc);
        Tx(f,"FLAP",11,{240,243,248},RX-14,oY-10);
        Tx(f,fl?"1":"0",17,fl?sf::Color{80,255,100}:sf::Color{255,100,100},RX-6,oY+2);

        // Population dots
        Tx(f,"POPULATION",10,{170,178,190},8,y+H-16);
        for(int i=0;i<(int)pop.size();i++){
            bool isBest=best&&(&pop[i]==best);
            sf::Color dc=!pop[i].alive?C_RED:isBest?C_GRN:C_YLW;
            sf::CircleShape d(5); d.setOrigin(V(5,5));
            d.setPosition(V(100+i*14.f,y+H-10));
            d.setFillColor(dc); rt_.draw(d);
        }
#ifdef USE_VERILATOR
        Tx(f,"[HIL] Verilator RTL active",11,{80,225,105},8,y+H-28);
#else
        Tx(f,"[STUB] Software emulation",11,{215,165,65},8,y+H-28);
#endif
        return y+H;
    }

    // ── RTL PIPELINE + PARALLEL THROUGHPUT ────────────────────────────────────
    float drawPipeline(const sf::Font& f,float y,
                       const std::vector<Bird>& pop, int clkCycles){
        y=SecHdr(f,y,"RTL PIPELINE","5 STAGES  LIVE  30x PARALLEL",C_ORG);
        const float H=108.f;
        Rect(rt_,0,y,(float)PANEL_W,H,C_SEC);

        const Bird* best=nullptr;
        for(const auto& b:pop) if(b.alive&&(!best||b.score>best->score)) best=&b;
        if(!best&&!pop.empty()) best=&pop[0];

        // Stage cards S1-S5 matching reference exactly
        const char* sNames[5]={"S1","S2","S3","S4","S5"};
        const char* sDesc[5]={"MUL","ACC","ReLU","MUL","FLAP"};
        float vals[5]={0,0,0,0,0};
        bool  active[5]={false,false,false,false,false};
        if(best){
            vals[1]=(float)best->h_regs[0]/256.f; // S2 hidden pre-act
            vals[2]=(float)best->h_regs[0]/256.f; // S3 post-tanh
            vals[3]=(float)best->h_regs[0]*((best->wo[0])/256.f); // S4
            vals[4]=(float)best->activation/256.f; // S5
            active[0]=true;
            active[1]=best->h_regs[0]!=0||best->h_regs[1]!=0||best->h_regs[2]!=0;
            active[2]=active[1];
            active[3]=active[2];
            active[4]=best->flap_out;
        }
        float cw=(float)(PANEL_W-16)/5.f;
        for(int s=0;s<5;s++){
            float cx=8+s*cw;
            sf::Color bg=active[s]?sf::Color{40,60,50}:sf::Color{35,38,45};
            sf::Color bc=active[s]?C_GRN:sf::Color{60,65,75};
            Rect(rt_,cx,y+6,cw-4,72,bg,bc,1.5f);
            Tx(f,sNames[s],15,active[s]?sf::Color{80,225,105}:sf::Color{155,163,175},cx+6,y+11);
            Tx(f,sDesc[s],10,{170,178,190},cx+6,y+29);
            Tx(f,active[s]?"ACTIVE":"idle",10,active[s]?sf::Color{80,225,105}:sf::Color{140,148,160},cx+6,y+43);
            Tx(f,F2(vals[s]),11,{220,225,235},cx+6,y+57);
        }

        // CLK count bottom of pipeline
        Tx(f,"CLK: "+std::to_string(clkCycles),11,{165,173,185},10,y+84);
        Tx(f,"5 CYCLES PER EVAL",11,{80,180,110},120,y+84);

        return y+H;
    }

    // ── LOGIC ANALYZER ────────────────────────────────────────────────────────
    float drawWaveform(const sf::Font& f,float y){
        y=SecHdr(f,y,"LOGIC ANALYZER","4 SIGNAL TRACES",C_GRN);
        const float H=115.f;
        Rect(rt_,0,y,(float)PANEL_W,H,{18,24,18});

        struct Tr{const std::deque<float>*d;sf::Color c;const char*lbl;bool step;};
        std::vector<Tr> trs={
            {&wFlap_,C_GRN,  "FLAP [1b]",true},
            {&wH_,   C_BLU,  "HEIGHT_ERR",false},
            {&wV_,   C_CYN,  "VELOCITY",  false},
            {&wD_,   C_PRP,  "PIPE_DIST", false},
        };
        const float LX=72,TW=PANEL_W-LX-4;
        float ty=y+4, th=25.f;
        for(auto& tr:trs){
            Rect(rt_,LX,ty,TW,th-2,{22,28,22},{50,60,50},1);
            Tx(f,tr.lbl,10,tr.c,4,ty+th/2-6);
            if(tr.d->size()>=2){
                const auto& dq=*tr.d;
                sf::VertexArray va(sf::PrimitiveType::LineStrip);
                for(int i=0;i<(int)dq.size();i++){
                    float fx=LX+(i/(float)(dq.size()-1))*TW;
                    float fy=ty+2+(1.f-dq[i])*(th-6);
                    if(tr.step&&i>0&&dq[i]!=dq[i-1])
                        va.append({V(fx,ty+2+(1.f-dq[i-1])*(th-6)),tr.c});
                    va.append({V(fx,fy),tr.c});
                }
                rt_.draw(va);
            }
            ty+=th+2;
        }
        return y+H;
    }

    // ── BIT REGISTERS ─────────────────────────────────────────────────────────
    float drawBitRegs(const sf::Font& f,float y,const std::vector<Bird>& pop){
        y=SecHdr(f,y,"BIT REGISTERS","LIVE RTL READBACK",C_PRP);
        const float H=72.f;
        Rect(rt_,0,y,(float)PANEL_W,H,C_SEC);

        const Bird* best=nullptr;
        for(const auto& b:pop) if(b.alive&&(!best||b.score>best->score)) best=&b;
        if(!best&&!pop.empty()) best=&pop[0];

        const char* names[4]={"H_ERR","VEL","DIST","GAP"};
        const char* lbl2[4]={"[8b]","[8b]","[8b]","[8b]"};
        float bx=8;
        for(int i=0;i<4;i++){
            float cx=bx;
            Tx(f,names[i],11,INPUT_COLS[i],cx,y+5);
            Tx(f,lbl2[i], 9,{155,163,175},cx,y+19);
            // 8 bits
            int8_t val=best?best->in_regs[i]:0;
            std::string bin=std::bitset<8>((uint8_t)val).to_string();
            for(int b2=0;b2<8;b2++){
                bool hi=(bin[b2]=='1');
                sf::Color fc=hi?sf::Color{INPUT_COLS[i].r,INPUT_COLS[i].g,INPUT_COLS[i].b,130}
                               :sf::Color{35,40,50};
                Rect(rt_,cx+b2*11.f,y+30,10,17,fc,{60,65,80},1);
            }
            // Hex value
            std::ostringstream ss;
            ss<<"0x"<<std::hex<<std::uppercase<<std::setw(2)<<std::setfill('0')
              <<(int)(uint8_t)val;
            Tx(f,ss.str(),10,{175,182,195},cx,y+51);
            Tx(f,"DEC:"+std::to_string((int)val),10,{175,182,195},cx+46,y+51);
            bx+=130;
        }
        return y+H;
    }

    // ── PARALLEL VISUALIZER ──────────────────────────────────────────────────
    float drawParallelViz(const sf::Font& f, float y,
                          const std::vector<Bird>& pop,
                          int clkCycles, float fps)
    {
        y=SecHdr(f,y,"PARALLEL EVAL","1 CLK = 30 BIRDS  SEQ:150cyc  PAR:5cyc  30x",{80,220,120});
        const float H=145.f;
        Rect(rt_,0,y,(float)PANEL_W,H,{18,26,20});

        // ── Row 1: Bird grid — all 30 lit simultaneously vs sequential ────────
        // Left half: PARALLEL — all 30 lit on each clock
        // Right half: SEQUENTIAL — one at a time sweep
        const float MID=PANEL_W/2.f;
        Tx(f,"PARALLEL (actual RTL)",11,{80,225,105},8,y+3);
        Tx(f,"SEQUENTIAL (software)",11,{220,100,100},MID+6,y+3);

        // Divider
        Rect(rt_,MID-1,y+2,2,H-4,{50,60,50});

        const int N=POP_SIZE; // 30
        const float DOT=7.f, GAP=3.f;
        const float rowW=(DOT+GAP)*N;
        // Center each half
        float parX=( MID - rowW)/2.f;
        float seqX=(MID + MID - rowW)/2.f;

        // Two rows of 15 each
        for(int row=0;row<2;row++){
            for(int col=0;col<15;col++){
                int idx=row*15+col;
                bool alive_b=(idx<(int)pop.size())&&pop[idx].alive;

                // Parallel: all alive birds lit simultaneously on each CLK edge
                // Flash effect: all bright on even CLK div, slightly dimmer on odd
                bool parFlash=(clkCycles%2==0);
                sf::Color parC;
                if(!alive_b) parC={40,50,40};
                else if(parFlash) parC={60,255,100};
                else parC={40,160,60};

                float px=parX+col*(DOT+GAP);
                float py=y+18+row*(DOT+GAP+2);
                Rect(rt_,px,py,DOT,DOT,parC,{0,0,0,0},0);

                // Sequential: only one bird lit at a time (sweeping)
                bool isSeq=(idx==clkEdgeAnim_);
                sf::Color seqC=!alive_b?sf::Color{40,40,35}
                              :isSeq?sf::Color{255,120,60}:sf::Color{50,40,35};
                float sx=seqX+col*(DOT+GAP);
                float sy=py;
                Rect(rt_,sx,sy,DOT,DOT,seqC,{0,0,0,0},0);
            }
        }

        // Labels
        Tx(f,"ALL 30 at once",10,{80,210,100},8,y+48);
        Tx(f,"1 of 30 at once",10,{215,115,70},MID+6,y+48);

        // ── Row 2: Cycle cost comparison bar chart ────────────────────────────
        float barY=y+56;
        Tx(f,"CYCLE COST PER GENERATION:",11,{220,225,235},8,barY);

        // Sequential bar (150 cycles) — full width
        float maxBar=PANEL_W-80.f;
        float seqBar=maxBar;       // 150 cycles = full
        float parBar=maxBar/30.f;  // 5 cycles = 1/30th

        Rect(rt_,60,barY+14,seqBar,12,{160,60,60});
        Tx(f,"SEQ",10,{220,100,100},8,barY+14);
        Tx(f,"150 cyc",10,{220,100,100},60+seqBar+2,barY+14);

        Rect(rt_,60,barY+30,parBar,12,{60,200,100});
        Tx(f,"PAR",10,{80,220,100},8,barY+30);
        Tx(f,"5 cyc",10,{80,220,100},60+parBar+2,barY+30);

        // Speedup badge
        Rect(rt_,PANEL_W-72,barY+10,64,36,{20,60,30},{60,200,80},2);
        Tx(f,"30x",22,{60,255,100},PANEL_W-58,barY+10);
        Tx(f,"FASTER",10,{80,220,100},PANEL_W-64,barY+32);

        // ── Row 3: Throughput counter ─────────────────────────────────────────
        float tpY=y+H-44.f;
        // Sparkline of throughput history
        if(throughputHistory_.size()>=2){
            float maxTp=*std::max_element(throughputHistory_.begin(),throughputHistory_.end());
            if(maxTp>0){
                const float spW=PANEL_W-180.f, spH=28.f;
                Rect(rt_,8,tpY,spW,spH,{22,30,22},{50,80,50},1);
                sf::VertexArray va(sf::PrimitiveType::LineStrip);
                for(int i=0;i<(int)throughputHistory_.size();i++){
                    float fx=8+(i/(float)(throughputHistory_.size()-1))*spW;
                    float fy=tpY+spH-2-(throughputHistory_[i]/maxTp)*(spH-4);
                    va.append({V(fx,fy),{80,220,120}});
                }
                rt_.draw(va);
            }
        }
        // Throughput number
        std::ostringstream tpss;
        tpss<<std::fixed<<std::setprecision(0)<<currentThroughput_;
        Rect(rt_,PANEL_W-168,tpY,160,28,{20,40,25},{60,160,80},1.5f);
        Tx(f,"THROUGHPUT",10,{165,173,185},PANEL_W-162,tpY+2);
        Tx(f,tpss.str()+" birds/s",12,{80,225,105},PANEL_W-162,tpY+14);

        return y+H;
    }

    // ── RTL SOURCE ────────────────────────────────────────────────────────────
    void drawRTLSource(const sf::Font& f,float y,const GenStats& st,
                       int /*alive*/,int /*ts*/,int /*cc*/,
                       float /*fps*/,float /*sm*/,
                       const std::vector<Bird>& /*pop*/)
    {
        y=SecHdr(f,y,"RTL SOURCE","FlappyController.v  LIVE",C_YLW);
        Rect(rt_,0,y,(float)PANEL_W,WIN_H-y,{18,22,18});

        // Brief explanation line
        Rect(rt_,0,y,PANEL_W,18,{22,28,22});
        Tx(f,"5-stage pipeline  |  weights evolve each gen  |  gates drive flap",
           11,{130,160,130},6,y+4);
        y+=20;

        sf::Color kw={100,155,255},cm={90,120,75},id={100,215,100},
                  num={255,155,75},nm={190,195,205},hi={255,220,80};
        float ly=y+4, lh=16.f;
        auto L=[&](const std::string& s,sf::Color c,int ind=0){
            if(ly>=WIN_H-28) return;
            Tx(f,std::string(ind*2,' ')+s,12,c,8,ly); ly+=lh;};

        L("module FlappyController (clk, in[3:0],",kw);
        L("  wh[3][4], wo[2:0], flap);",nm);
        L("// S1:MUL  S2:ACC  S3:ReLU  S4:MUL  S5:FLAP",cm);
        ly+=4;
        // Hidden weights with clear labels
        L("// -- HIDDEN WEIGHTS (evolving each gen) --",cm);
        for(int h=0;h<N_HIDDEN&&ly<WIN_H-60;h++){
            std::string ws="wh"+std::to_string(h)+":";
            std::string vals=" [";
            for(int i=0;i<N_IN;i++)
                vals+=Hex16(h<(int)st.eliteW.size()?st.eliteW[h][i]:0.f)
                    +(i<N_IN-1?", ":"");
            vals+="]";
            Tx(f,ws,12,{140,180,255},8,ly);
            Tx(f,vals,12,num,44,ly);
            ly+=lh;
        }
        ly+=3;
        {
            std::string vals="[";
            for(int h=0;h<N_HIDDEN;h++)
                vals+=Hex16(h<(int)st.eliteWo.size()?st.eliteWo[h]:0.f)
                    +(h<N_HIDDEN-1?", ":"");
            vals+="]";
            Tx(f,"wo:",12,{140,220,140},8,ly);
            Tx(f,vals,12,num,34,ly);
            ly+=lh;
        }
        ly+=4;
        L("always @(posedge clk) begin",kw);
        L("  s3 <= ReLU(dot(in, wh));  // hidden",id);
        L("  flap <= (dot(s3, wo)>0); // output",id);
        L("end  endmodule",kw);

        // Status bar
        Rect(rt_,0,WIN_H-26,(float)PANEL_W,26,{20,55,20},{55,180,55},1.5f);
#ifdef USE_VERILATOR
        Tx(f,"VERILATOR HIL ACTIVE  30x PARALLEL  gates drive flap",11,{80,225,105},8,WIN_H-20);
#else
        Tx(f,"SOFTWARE STUB  run 'make hil' for Verilator RTL",11,{215,175,70},8,WIN_H-20);
#endif
    }
};

} // namespace Evo
