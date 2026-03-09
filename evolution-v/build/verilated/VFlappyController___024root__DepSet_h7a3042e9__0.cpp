// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VFlappyController.h for the primary calling header

#include "VFlappyController__pch.h"
#include "VFlappyController___024root.h"

void VFlappyController___024root___eval_act(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_act\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

void VFlappyController___024root___nba_sequent__TOP__0(VFlappyController___024root* vlSelf);

void VFlappyController___024root___eval_nba(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_nba\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((3ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VFlappyController___024root___nba_sequent__TOP__0(vlSelf);
    }
}

VL_INLINE_OPT void VFlappyController___024root___nba_sequent__TOP__0(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___nba_sequent__TOP__0\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    SData/*15:0*/ __Vfunc_FlappyController__DOT__approx_tanh__0__Vfuncout;
    __Vfunc_FlappyController__DOT__approx_tanh__0__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_FlappyController__DOT__approx_tanh__0__x;
    __Vfunc_FlappyController__DOT__approx_tanh__0__x = 0;
    IData/*31:0*/ __Vfunc_FlappyController__DOT__approx_tanh__0__scaled;
    __Vfunc_FlappyController__DOT__approx_tanh__0__scaled = 0;
    SData/*15:0*/ __Vfunc_FlappyController__DOT__approx_tanh__1__Vfuncout;
    __Vfunc_FlappyController__DOT__approx_tanh__1__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_FlappyController__DOT__approx_tanh__1__x;
    __Vfunc_FlappyController__DOT__approx_tanh__1__x = 0;
    IData/*31:0*/ __Vfunc_FlappyController__DOT__approx_tanh__1__scaled;
    __Vfunc_FlappyController__DOT__approx_tanh__1__scaled = 0;
    SData/*15:0*/ __Vfunc_FlappyController__DOT__approx_tanh__2__Vfuncout;
    __Vfunc_FlappyController__DOT__approx_tanh__2__Vfuncout = 0;
    IData/*31:0*/ __Vfunc_FlappyController__DOT__approx_tanh__2__x;
    __Vfunc_FlappyController__DOT__approx_tanh__2__x = 0;
    IData/*31:0*/ __Vfunc_FlappyController__DOT__approx_tanh__2__scaled;
    __Vfunc_FlappyController__DOT__approx_tanh__2__scaled = 0;
    // Body
    if (vlSelfRef.rst) {
        vlSelfRef.flap = 0U;
        vlSelfRef.activation = 0U;
        vlSelfRef.s5_acc = 0U;
        vlSelfRef.FlappyController__DOT__s5_r = 0ULL;
        vlSelfRef.FlappyController__DOT__s4_h0 = 0U;
        vlSelfRef.s3_h0 = 0U;
        vlSelfRef.FlappyController__DOT__s4_h1 = 0U;
        vlSelfRef.s3_h1 = 0U;
        vlSelfRef.FlappyController__DOT__s4_h2 = 0U;
        vlSelfRef.s3_h2 = 0U;
        vlSelfRef.FlappyController__DOT__s3_h0_r = 0U;
        vlSelfRef.s2_h0 = 0U;
        vlSelfRef.FlappyController__DOT__s3_h1_r = 0U;
        vlSelfRef.s2_h1 = 0U;
        vlSelfRef.FlappyController__DOT__s3_h2_r = 0U;
        vlSelfRef.s2_h2 = 0U;
        vlSelfRef.FlappyController__DOT__s2_h0_r = 0U;
        vlSelfRef.FlappyController__DOT__s2_h1_r = 0U;
        vlSelfRef.FlappyController__DOT__s2_h2_r = 0U;
        vlSelfRef.FlappyController__DOT__s1_h0 = 0U;
        vlSelfRef.FlappyController__DOT__s1_h1 = 0U;
        vlSelfRef.FlappyController__DOT__s1_h2 = 0U;
    } else {
        vlSelfRef.flap = VL_LTS_IQQ(34, 0ULL, vlSelfRef.FlappyController__DOT__s5_r);
        vlSelfRef.activation = (0xffffU & (IData)((vlSelfRef.FlappyController__DOT__s5_r 
                                                   >> 8U)));
        vlSelfRef.s5_acc = (IData)(vlSelfRef.FlappyController__DOT__s5_r);
        vlSelfRef.FlappyController__DOT__s5_r = (0x3ffffffffULL 
                                                 & ((VL_EXTENDS_QI(34,32, vlSelfRef.FlappyController__DOT__s4_h0) 
                                                     + 
                                                     VL_EXTENDS_QI(34,32, vlSelfRef.FlappyController__DOT__s4_h1)) 
                                                    + 
                                                    VL_EXTENDS_QI(34,32, vlSelfRef.FlappyController__DOT__s4_h2)));
        vlSelfRef.FlappyController__DOT__s4_h0 = VL_MULS_III(32, 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.FlappyController__DOT__s3_h0_r)), 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wo0)));
        vlSelfRef.s3_h0 = vlSelfRef.FlappyController__DOT__s3_h0_r;
        vlSelfRef.FlappyController__DOT__s4_h1 = VL_MULS_III(32, 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.FlappyController__DOT__s3_h1_r)), 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wo1)));
        vlSelfRef.s3_h1 = vlSelfRef.FlappyController__DOT__s3_h1_r;
        vlSelfRef.FlappyController__DOT__s4_h2 = VL_MULS_III(32, 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.FlappyController__DOT__s3_h2_r)), 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wo2)));
        vlSelfRef.s3_h2 = vlSelfRef.FlappyController__DOT__s3_h2_r;
        __Vfunc_FlappyController__DOT__approx_tanh__0__x 
            = vlSelfRef.FlappyController__DOT__s2_h0_r;
        __Vfunc_FlappyController__DOT__approx_tanh__0__scaled 
            = VL_SHIFTRS_III(32,32,32, __Vfunc_FlappyController__DOT__approx_tanh__0__x, 8U);
        __Vfunc_FlappyController__DOT__approx_tanh__0__Vfuncout 
            = (VL_LTS_III(32, 0x7fU, __Vfunc_FlappyController__DOT__approx_tanh__0__scaled)
                ? 0x7fU : (VL_GTS_III(32, 0xffffff80U, __Vfunc_FlappyController__DOT__approx_tanh__0__scaled)
                            ? 0xff80U : (0xffffU & __Vfunc_FlappyController__DOT__approx_tanh__0__scaled)));
        vlSelfRef.s2_h0 = vlSelfRef.FlappyController__DOT__s2_h0_r;
        vlSelfRef.FlappyController__DOT__s3_h0_r = __Vfunc_FlappyController__DOT__approx_tanh__0__Vfuncout;
        __Vfunc_FlappyController__DOT__approx_tanh__1__x 
            = vlSelfRef.FlappyController__DOT__s2_h1_r;
        __Vfunc_FlappyController__DOT__approx_tanh__1__scaled 
            = VL_SHIFTRS_III(32,32,32, __Vfunc_FlappyController__DOT__approx_tanh__1__x, 8U);
        __Vfunc_FlappyController__DOT__approx_tanh__1__Vfuncout 
            = (VL_LTS_III(32, 0x7fU, __Vfunc_FlappyController__DOT__approx_tanh__1__scaled)
                ? 0x7fU : (VL_GTS_III(32, 0xffffff80U, __Vfunc_FlappyController__DOT__approx_tanh__1__scaled)
                            ? 0xff80U : (0xffffU & __Vfunc_FlappyController__DOT__approx_tanh__1__scaled)));
        vlSelfRef.s2_h1 = vlSelfRef.FlappyController__DOT__s2_h1_r;
        vlSelfRef.FlappyController__DOT__s3_h1_r = __Vfunc_FlappyController__DOT__approx_tanh__1__Vfuncout;
        __Vfunc_FlappyController__DOT__approx_tanh__2__x 
            = vlSelfRef.FlappyController__DOT__s2_h2_r;
        __Vfunc_FlappyController__DOT__approx_tanh__2__scaled 
            = VL_SHIFTRS_III(32,32,32, __Vfunc_FlappyController__DOT__approx_tanh__2__x, 8U);
        __Vfunc_FlappyController__DOT__approx_tanh__2__Vfuncout 
            = (VL_LTS_III(32, 0x7fU, __Vfunc_FlappyController__DOT__approx_tanh__2__scaled)
                ? 0x7fU : (VL_GTS_III(32, 0xffffff80U, __Vfunc_FlappyController__DOT__approx_tanh__2__scaled)
                            ? 0xff80U : (0xffffU & __Vfunc_FlappyController__DOT__approx_tanh__2__scaled)));
        vlSelfRef.s2_h2 = vlSelfRef.FlappyController__DOT__s2_h2_r;
        vlSelfRef.FlappyController__DOT__s3_h2_r = __Vfunc_FlappyController__DOT__approx_tanh__2__Vfuncout;
        vlSelfRef.FlappyController__DOT__s2_h0_r = 
            (((vlSelfRef.FlappyController__DOT__s1_h0 
               + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in1)), 
                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh01)))) 
              + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in2)), 
                            VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh02)))) 
             + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in3)), 
                           VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh03))));
        vlSelfRef.FlappyController__DOT__s2_h1_r = 
            (((vlSelfRef.FlappyController__DOT__s1_h1 
               + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in1)), 
                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh11)))) 
              + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in2)), 
                            VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh12)))) 
             + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in3)), 
                           VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh13))));
        vlSelfRef.FlappyController__DOT__s2_h2_r = 
            (((vlSelfRef.FlappyController__DOT__s1_h2 
               + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in1)), 
                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh21)))) 
              + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in2)), 
                            VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh22)))) 
             + VL_MULS_III(32, VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in3)), 
                           VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh23))));
        vlSelfRef.FlappyController__DOT__s1_h0 = VL_MULS_III(32, 
                                                             VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in0)), 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh00)));
        vlSelfRef.FlappyController__DOT__s1_h1 = VL_MULS_III(32, 
                                                             VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in0)), 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh10)));
        vlSelfRef.FlappyController__DOT__s1_h2 = VL_MULS_III(32, 
                                                             VL_EXTENDS_II(32,8, (IData)(vlSelfRef.in0)), 
                                                             VL_EXTENDS_II(32,16, (IData)(vlSelfRef.wh20)));
    }
}

void VFlappyController___024root___eval_triggers__act(VFlappyController___024root* vlSelf);

bool VFlappyController___024root___eval_phase__act(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_phase__act\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    VlTriggerVec<2> __VpreTriggered;
    CData/*0:0*/ __VactExecute;
    // Body
    VFlappyController___024root___eval_triggers__act(vlSelf);
    __VactExecute = vlSelfRef.__VactTriggered.any();
    if (__VactExecute) {
        __VpreTriggered.andNot(vlSelfRef.__VactTriggered, vlSelfRef.__VnbaTriggered);
        vlSelfRef.__VnbaTriggered.thisOr(vlSelfRef.__VactTriggered);
        VFlappyController___024root___eval_act(vlSelf);
    }
    return (__VactExecute);
}

bool VFlappyController___024root___eval_phase__nba(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_phase__nba\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    CData/*0:0*/ __VnbaExecute;
    // Body
    __VnbaExecute = vlSelfRef.__VnbaTriggered.any();
    if (__VnbaExecute) {
        VFlappyController___024root___eval_nba(vlSelf);
        vlSelfRef.__VnbaTriggered.clear();
    }
    return (__VnbaExecute);
}

#ifdef VL_DEBUG
VL_ATTR_COLD void VFlappyController___024root___dump_triggers__nba(VFlappyController___024root* vlSelf);
#endif  // VL_DEBUG
#ifdef VL_DEBUG
VL_ATTR_COLD void VFlappyController___024root___dump_triggers__act(VFlappyController___024root* vlSelf);
#endif  // VL_DEBUG

void VFlappyController___024root___eval(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Init
    IData/*31:0*/ __VnbaIterCount;
    CData/*0:0*/ __VnbaContinue;
    // Body
    __VnbaIterCount = 0U;
    __VnbaContinue = 1U;
    while (__VnbaContinue) {
        if (VL_UNLIKELY(((0x64U < __VnbaIterCount)))) {
#ifdef VL_DEBUG
            VFlappyController___024root___dump_triggers__nba(vlSelf);
#endif
            VL_FATAL_MT("rtl\\FlappyController.v", 8, "", "NBA region did not converge.");
        }
        __VnbaIterCount = ((IData)(1U) + __VnbaIterCount);
        __VnbaContinue = 0U;
        vlSelfRef.__VactIterCount = 0U;
        vlSelfRef.__VactContinue = 1U;
        while (vlSelfRef.__VactContinue) {
            if (VL_UNLIKELY(((0x64U < vlSelfRef.__VactIterCount)))) {
#ifdef VL_DEBUG
                VFlappyController___024root___dump_triggers__act(vlSelf);
#endif
                VL_FATAL_MT("rtl\\FlappyController.v", 8, "", "Active region did not converge.");
            }
            vlSelfRef.__VactIterCount = ((IData)(1U) 
                                         + vlSelfRef.__VactIterCount);
            vlSelfRef.__VactContinue = 0U;
            if (VFlappyController___024root___eval_phase__act(vlSelf)) {
                vlSelfRef.__VactContinue = 1U;
            }
        }
        if (VFlappyController___024root___eval_phase__nba(vlSelf)) {
            __VnbaContinue = 1U;
        }
    }
}

#ifdef VL_DEBUG
void VFlappyController___024root___eval_debug_assertions(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_debug_assertions\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if (VL_UNLIKELY(((vlSelfRef.clk & 0xfeU)))) {
        Verilated::overWidthError("clk");}
    if (VL_UNLIKELY(((vlSelfRef.rst & 0xfeU)))) {
        Verilated::overWidthError("rst");}
}
#endif  // VL_DEBUG
