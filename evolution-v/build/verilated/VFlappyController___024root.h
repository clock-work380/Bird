// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design internal header
// See VFlappyController.h for the primary calling header

#ifndef VERILATED_VFLAPPYCONTROLLER___024ROOT_H_
#define VERILATED_VFLAPPYCONTROLLER___024ROOT_H_  // guard

#include "verilated.h"


class VFlappyController__Syms;

class alignas(VL_CACHE_LINE_BYTES) VFlappyController___024root final : public VerilatedModule {
  public:

    // DESIGN SPECIFIC STATE
    VL_IN8(clk,0,0);
    VL_IN8(rst,0,0);
    VL_IN8(in0,7,0);
    VL_IN8(in1,7,0);
    VL_IN8(in2,7,0);
    VL_IN8(in3,7,0);
    VL_OUT8(flap,0,0);
    CData/*0:0*/ __Vtrigprevexpr___TOP__clk__0;
    CData/*0:0*/ __Vtrigprevexpr___TOP__rst__0;
    CData/*0:0*/ __VactContinue;
    VL_IN16(wh00,15,0);
    VL_IN16(wh01,15,0);
    VL_IN16(wh02,15,0);
    VL_IN16(wh03,15,0);
    VL_IN16(wh10,15,0);
    VL_IN16(wh11,15,0);
    VL_IN16(wh12,15,0);
    VL_IN16(wh13,15,0);
    VL_IN16(wh20,15,0);
    VL_IN16(wh21,15,0);
    VL_IN16(wh22,15,0);
    VL_IN16(wh23,15,0);
    VL_IN16(wo0,15,0);
    VL_IN16(wo1,15,0);
    VL_IN16(wo2,15,0);
    VL_OUT16(activation,15,0);
    VL_OUT16(s3_h0,15,0);
    VL_OUT16(s3_h1,15,0);
    VL_OUT16(s3_h2,15,0);
    SData/*15:0*/ FlappyController__DOT__s3_h0_r;
    SData/*15:0*/ FlappyController__DOT__s3_h1_r;
    SData/*15:0*/ FlappyController__DOT__s3_h2_r;
    VL_OUT(s2_h0,31,0);
    VL_OUT(s2_h1,31,0);
    VL_OUT(s2_h2,31,0);
    VL_OUT(s5_acc,31,0);
    IData/*31:0*/ FlappyController__DOT__s1_h0;
    IData/*31:0*/ FlappyController__DOT__s1_h1;
    IData/*31:0*/ FlappyController__DOT__s1_h2;
    IData/*31:0*/ FlappyController__DOT__s2_h0_r;
    IData/*31:0*/ FlappyController__DOT__s2_h1_r;
    IData/*31:0*/ FlappyController__DOT__s2_h2_r;
    IData/*31:0*/ FlappyController__DOT__s4_h0;
    IData/*31:0*/ FlappyController__DOT__s4_h1;
    IData/*31:0*/ FlappyController__DOT__s4_h2;
    IData/*31:0*/ __VactIterCount;
    QData/*33:0*/ FlappyController__DOT__s5_r;
    VlTriggerVec<2> __VactTriggered;
    VlTriggerVec<2> __VnbaTriggered;

    // INTERNAL VARIABLES
    VFlappyController__Syms* const vlSymsp;

    // CONSTRUCTORS
    VFlappyController___024root(VFlappyController__Syms* symsp, const char* v__name);
    ~VFlappyController___024root();
    VL_UNCOPYABLE(VFlappyController___024root);

    // INTERNAL METHODS
    void __Vconfigure(bool first);
};


#endif  // guard
