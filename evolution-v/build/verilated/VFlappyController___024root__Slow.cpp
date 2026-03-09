// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Design implementation internals
// See VFlappyController.h for the primary calling header

#include "VFlappyController__pch.h"
#include "VFlappyController__Syms.h"
#include "VFlappyController___024root.h"

void VFlappyController___024root___ctor_var_reset(VFlappyController___024root* vlSelf);

VFlappyController___024root::VFlappyController___024root(VFlappyController__Syms* symsp, const char* v__name)
    : VerilatedModule{v__name}
    , vlSymsp{symsp}
 {
    // Reset structure values
    VFlappyController___024root___ctor_var_reset(this);
}

void VFlappyController___024root::__Vconfigure(bool first) {
    (void)first;  // Prevent unused variable warning
}

VFlappyController___024root::~VFlappyController___024root() {
}
