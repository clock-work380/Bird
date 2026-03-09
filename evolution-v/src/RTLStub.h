#pragma once
// =============================================================================
//  RTLStub.h  -  Software fallback ONLY (used when Verilator not available).
//  When USE_VERILATOR is defined, VFlappyController is used instead.
//  Interface mirrors the Verilated class naming.
// =============================================================================
#include <cstdint>
#include <array>
#include <cmath>

struct RTLStub {
    uint8_t clk=0, rst=0;

    // Ports matching FlappyController.v
    int8_t  in0=0, in1=0, in2=0, in3=0;
    int16_t wh00=0,wh01=0,wh02=0,wh03=0;
    int16_t wh10=0,wh11=0,wh12=0,wh13=0;
    int16_t wh20=0,wh21=0,wh22=0,wh23=0;
    int16_t wo0=0, wo1=0, wo2=0;

    uint8_t  flap=0;
    int16_t  activation=0;

    // Pipeline viz ports
    int32_t s2_h0=0,s2_h1=0,s2_h2=0;
    int16_t s3_h0=0,s3_h1=0,s3_h2=0;
    int32_t s5_acc=0;

    // Internal pipeline regs (for s1/s4 which aren't output ports)
    int32_t _s1_h0=0,_s1_h1=0,_s1_h2=0;
    int32_t _s4_h0=0,_s4_h1=0,_s4_h2=0;

    // Arrays for convenient access
    std::array<int32_t,3> s1_mul{};
    std::array<int32_t,3> s2_acc_arr{};
    std::array<int16_t,3> s3_act{};
    std::array<int32_t,3> s4_mul{};

    uint64_t wave_log=0;

    void eval() {
        if (rst) {
            flap=0; activation=0; wave_log=0;
            s2_h0=s2_h1=s2_h2=0;
            s3_h0=s3_h1=s3_h2=0;
            s5_acc=0; _s1_h0=_s1_h1=_s1_h2=0;
            _s4_h0=_s4_h1=_s4_h2=0;
            s1_mul.fill(0); s2_acc_arr.fill(0);
            s3_act.fill(0); s4_mul.fill(0);
            return;
        }
        static uint8_t prev=0;
        bool posedge=(clk==1&&prev==0); prev=clk;
        if (!posedge) return;

        // Correct tanh-based network (combinatorial, no pipeline delay)
        float ins[4]={(float)in0,(float)in1,(float)in2,(float)in3};
        float wh[3][4]={
            {(float)wh00/256,(float)wh01/256,(float)wh02/256,(float)wh03/256},
            {(float)wh10/256,(float)wh11/256,(float)wh12/256,(float)wh13/256},
            {(float)wh20/256,(float)wh21/256,(float)wh22/256,(float)wh23/256}};
        float wo[3]={(float)wo0/256,(float)wo1/256,(float)wo2/256};

        float hidden[3];
        for(int h=0;h<3;h++){
            float s=0; for(int i=0;i<4;i++) s+=ins[i]*wh[h][i]/128.f;
            hidden[h]=std::tanh(s);
        }
        float out=0; for(int h=0;h<3;h++) out+=hidden[h]*wo[h];

        flap=(out>0)?1:0;
        activation=(int16_t)(out*256.f);

        // Populate viz registers
        int16_t iins[4]={(int16_t)in0,(int16_t)in1,(int16_t)in2,(int16_t)in3};
        int16_t iwh[3][4]={{wh00,wh01,wh02,wh03},{wh10,wh11,wh12,wh13},{wh20,wh21,wh22,wh23}};
        int32_t s2[3]={0,0,0};
        for(int h=0;h<3;h++) for(int i=0;i<4;i++) s2[h]+=(int32_t)iins[i]*(int32_t)iwh[h][i];
        s2_h0=s2[0]; s2_h1=s2[1]; s2_h2=s2[2];
        s3_h0=(int16_t)(hidden[0]*256); s3_h1=(int16_t)(hidden[1]*256); s3_h2=(int16_t)(hidden[2]*256);
        s5_acc=(int32_t)(out*65536.f);

        // Mirror to arrays
        s1_mul={s2[0]/4,s2[1]/4,s2[2]/4};
        s2_acc_arr={s2[0],s2[1],s2[2]};
        s3_act={s3_h0,s3_h1,s3_h2};
        s4_mul={(int32_t)(hidden[0]*wo[0]*65536),(int32_t)(hidden[1]*wo[1]*65536),(int32_t)(hidden[2]*wo[2]*65536)};

        wave_log=(wave_log<<1)|flap;
    }
    void final(){}
};
