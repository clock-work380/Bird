// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VFlappyController.h for the primary calling header

#include "VFlappyController__pch.h"
#include "VFlappyController__Syms.h"
#include "VFlappyController___024root.h"

#ifdef VL_DEBUG
VL_ATTR_COLD void VFlappyController___024root___dump_triggers__act(VFlappyController___024root* vlSelf);
#endif  // VL_DEBUG

void VFlappyController___024root___eval_triggers__act(VFlappyController___024root* vlSelf) {
    VL_DEBUG_IF(VL_DBG_MSGF("+    VFlappyController___024root___eval_triggers__act\n"); );
    VFlappyController__Syms* const __restrict vlSymsp VL_ATTR_UNUSED = vlSelf->vlSymsp;
    auto& vlSelfRef = std::ref(*vlSelf).get();
    // Body
    vlSelfRef.__VactTriggered.setBit(0U, ((IData)(vlSelfRef.clk) 
                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__clk__0))));
    vlSelfRef.__VactTriggered.setBit(1U, ((IData)(vlSelfRef.rst) 
                                          & (~ (IData)(vlSelfRef.__Vtrigprevexpr___TOP__rst__0))));
    vlSelfRef.__Vtrigprevexpr___TOP__clk__0 = vlSelfRef.clk;
    vlSelfRef.__Vtrigprevexpr___TOP__rst__0 = vlSelfRef.rst;
#ifdef VL_DEBUG
    if (VL_UNLIKELY(vlSymsp->_vm_contextp__->debug())) {
        VFlappyController___024root___dump_triggers__act(vlSelf);
    }
#endif
}
