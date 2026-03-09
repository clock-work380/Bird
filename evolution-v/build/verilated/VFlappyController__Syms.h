// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Symbol table internal header
//
// Internal details; most calling programs do not need this header,
// unless using verilator public meta comments.

#ifndef VERILATED_VFLAPPYCONTROLLER__SYMS_H_
#define VERILATED_VFLAPPYCONTROLLER__SYMS_H_  // guard

#include "verilated.h"

// INCLUDE MODEL CLASS

#include "VFlappyController.h"

// INCLUDE MODULE CLASSES
#include "VFlappyController___024root.h"

// SYMS CLASS (contains all model state)
class alignas(VL_CACHE_LINE_BYTES)VFlappyController__Syms final : public VerilatedSyms {
  public:
    // INTERNAL STATE
    VFlappyController* const __Vm_modelp;
    VlDeleter __Vm_deleter;
    bool __Vm_didInit = false;

    // MODULE INSTANCE STATE
    VFlappyController___024root    TOP;

    // CONSTRUCTORS
    VFlappyController__Syms(VerilatedContext* contextp, const char* namep, VFlappyController* modelp);
    ~VFlappyController__Syms();

    // METHODS
    const char* name() { return TOP.name(); }
};

#endif  // guard
