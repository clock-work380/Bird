// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VFlappyController.h for the primary calling header

#include "VFlappyController__pch.h"
#include "VFlappyController___024root.h"

VL_ATTR_COLD void VFlappyController___024root___eval_static(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_static\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst__0 = vlSelfRef.rst;
}

VL_ATTR_COLD void VFlappyController___024root___eval_initial(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_initial\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void VFlappyController___024root___eval_final(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_final\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

VL_ATTR_COLD void VFlappyController___024root___eval_settle(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_settle\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
}

#ifdef VL_DEBUG
VL_ATTR_COLD void VFlappyController___024root___dump_triggers__act(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___dump_triggers__act\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VactTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((2ULL & vlSelfRef.__VactTriggered.word(0U))) {
        VL_DBG_MSGF("         'act' region trigger index 1 is active: @(posedge rst)\n");
    }
}
#endif  // VL_DEBUG

#ifdef VL_DEBUG
VL_ATTR_COLD void VFlappyController___024root___dump_triggers__nba(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___dump_triggers__nba\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    if ((1U & (~ vlSelfRef.__VnbaTriggered.any()))) {
        VL_DBG_MSGF("         No triggers active\n");
    }
    if ((1ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 0 is active: @(posedge clk)\n");
    }
    if ((2ULL & vlSelfRef.__VnbaTriggered.word(0U))) {
        VL_DBG_MSGF("         'nba' region trigger index 1 is active: @(posedge rst)\n");
    }
}
#endif  // VL_DEBUG

VL_ATTR_COLD void VFlappyController___024root___ctor_var_reset(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___ctor_var_reset\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    const uint64_t __VscopeHash = VL_MURMUR64_HASH(vlSelf->name());
    vlSelf->clk = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 16707436170211756652ull);
    vlSelf->rst = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 18209466448985614591ull);
    vlSelf->in0 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 8091406170039428016ull);
    vlSelf->in1 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 15347402500067000120ull);
    vlSelf->in2 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 14260782587271488589ull);
    vlSelf->in3 = VL_SCOPED_RAND_RESET_I(8, __VscopeHash, 12575345787804495474ull);
    vlSelf->wh00 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 18315110287843852929ull);
    vlSelf->wh01 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 9146920425818429713ull);
    vlSelf->wh02 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 5158199404451534687ull);
    vlSelf->wh03 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 4510937600144408184ull);
    vlSelf->wh10 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12247959279731882430ull);
    vlSelf->wh11 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12075766886137372132ull);
    vlSelf->wh12 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14519018474290368796ull);
    vlSelf->wh13 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 4929864178001599275ull);
    vlSelf->wh20 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 3216139252310533166ull);
    vlSelf->wh21 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 7116157284059952705ull);
    vlSelf->wh22 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 13208145381447476035ull);
    vlSelf->wh23 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12070406237301783128ull);
    vlSelf->wo0 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 17250622450538512502ull);
    vlSelf->wo1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 12143151962610423874ull);
    vlSelf->wo2 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 14622220903445743829ull);
    vlSelf->flap = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 7951056075316444093ull);
    vlSelf->activation = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2320115052395106594ull);
    vlSelf->s2_h0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9391324438943763691ull);
    vlSelf->s2_h1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12294859487338717494ull);
    vlSelf->s2_h2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14223659398453224739ull);
    vlSelf->s3_h0 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11509905132934960945ull);
    vlSelf->s3_h1 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 11310770126302304148ull);
    vlSelf->s3_h2 = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 150017094344887814ull);
    vlSelf->s5_acc = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 9113521846449033501ull);
    vlSelf->FlappyController__DOT__s1_h0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14971478133512313337ull);
    vlSelf->FlappyController__DOT__s1_h1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 5194707515304479457ull);
    vlSelf->FlappyController__DOT__s1_h2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 16321238230025412759ull);
    vlSelf->FlappyController__DOT__s2_h0_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14433434672322177775ull);
    vlSelf->FlappyController__DOT__s2_h1_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 822102875723477386ull);
    vlSelf->FlappyController__DOT__s2_h2_r = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 12230036532481047202ull);
    vlSelf->FlappyController__DOT__s3_h0_r = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2865011540144486190ull);
    vlSelf->FlappyController__DOT__s3_h1_r = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 2935851793928579677ull);
    vlSelf->FlappyController__DOT__s3_h2_r = VL_SCOPED_RAND_RESET_I(16, __VscopeHash, 15239383380683074379ull);
    vlSelf->FlappyController__DOT__s4_h0 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 14208626612460900416ull);
    vlSelf->FlappyController__DOT__s4_h1 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 850865261954610040ull);
    vlSelf->FlappyController__DOT__s4_h2 = VL_SCOPED_RAND_RESET_I(32, __VscopeHash, 1412429060479073439ull);
    vlSelf->FlappyController__DOT__s5_r = VL_SCOPED_RAND_RESET_Q(34, __VscopeHash, 5460126908387664194ull);
    vlSelf->__Vtrigprevexpr___TOP__clk__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 9526919608049418986ull);
    vlSelf->__Vtrigprevexpr___TOP__rst__0 = VL_SCOPED_RAND_RESET_I(1, __VscopeHash, 3495601893105415319ull);
}
