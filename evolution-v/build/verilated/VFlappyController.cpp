// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Model implementation (design independent parts)

#include "VFlappyController__pch.h"

//============================================================
// Constructors

VFlappyController::VFlappyController(VerilatedContext* _vcontextp__, const char* _vcname__)
    : VerilatedModel{*_vcontextp__}
    , vlSymsp{new VFlappyController__Syms(contextp(), _vcname__, this)}
    , clk{vlSymsp->TOP.clk}
    , rst{vlSymsp->TOP.rst}
    , in0{vlSymsp->TOP.in0}
    , in1{vlSymsp->TOP.in1}
    , in2{vlSymsp->TOP.in2}
    , in3{vlSymsp->TOP.in3}
    , flap{vlSymsp->TOP.flap}
    , wh00{vlSymsp->TOP.wh00}
    , wh01{vlSymsp->TOP.wh01}
    , wh02{vlSymsp->TOP.wh02}
    , wh03{vlSymsp->TOP.wh03}
    , wh10{vlSymsp->TOP.wh10}
    , wh11{vlSymsp->TOP.wh11}
    , wh12{vlSymsp->TOP.wh12}
    , wh13{vlSymsp->TOP.wh13}
    , wh20{vlSymsp->TOP.wh20}
    , wh21{vlSymsp->TOP.wh21}
    , wh22{vlSymsp->TOP.wh22}
    , wh23{vlSymsp->TOP.wh23}
    , wo0{vlSymsp->TOP.wo0}
    , wo1{vlSymsp->TOP.wo1}
    , wo2{vlSymsp->TOP.wo2}
    , activation{vlSymsp->TOP.activation}
    , s3_h0{vlSymsp->TOP.s3_h0}
    , s3_h1{vlSymsp->TOP.s3_h1}
    , s3_h2{vlSymsp->TOP.s3_h2}
    , s2_h0{vlSymsp->TOP.s2_h0}
    , s2_h1{vlSymsp->TOP.s2_h1}
    , s2_h2{vlSymsp->TOP.s2_h2}
    , s5_acc{vlSymsp->TOP.s5_acc}
    , rootp{&(vlSymsp->TOP)}
{
    // Register model with the context
    contextp()->addModel(this);
}

VFlappyController::VFlappyController(const char* _vcname__)
    : VFlappyController(Verilated::threadContextp(), _vcname__)
{
}

//============================================================
// Destructor

VFlappyController::~VFlappyController() {
    delete vlSymsp;
}

//============================================================
// Evaluation function

#ifdef VL_DEBUG
void VFlappyController___024root___eval_debug_assertions(VFlappyController___024root* vlSelf);
#endif  // VL_DEBUG
void VFlappyController___024root___eval_static(VFlappyController___024root* vlSelf);
void VFlappyController___024root___eval_initial(VFlappyController___024root* vlSelf);
void VFlappyController___024root___eval_settle(VFlappyController___024root* vlSelf);
void VFlappyController___024root___eval(VFlappyController___024root* vlSelf);

void VFlappyController::eval_step() {
    VL_DEBUG_IF(VL_DBG_MSGF("+++++TOP Evaluate VFlappyController::eval_step\n"); );
#ifdef VL_DEBUG
    // Debug assertions
    VFlappyController___024root___eval_debug_assertions(&(vlSymsp->TOP));
#endif  // VL_DEBUG
    vlSymsp->__Vm_deleter.deleteAll();
    if (VL_UNLIKELY(!vlSymsp->__Vm_didInit)) {
        vlSymsp->__Vm_didInit = true;
        VL_DEBUG_IF(VL_DBG_MSGF("+ Initial\n"););
        VFlappyController___024root___eval_static(&(vlSymsp->TOP));
        VFlappyController___024root___eval_initial(&(vlSymsp->TOP));
        VFlappyController___024root___eval_settle(&(vlSymsp->TOP));
    }
    VL_DEBUG_IF(VL_DBG_MSGF("+ Eval\n"););
    VFlappyController___024root___eval(&(vlSymsp->TOP));
    // Evaluate cleanup
    Verilated::endOfEval(vlSymsp->__Vm_evalMsgQp);
}

//============================================================
// Events and timing
bool VFlappyController::eventsPending() { return false; }

uint64_t VFlappyController::nextTimeSlot() {
    VL_FATAL_MT(__FILE__, __LINE__, "", "No delays in the design");
    return 0;
}

//============================================================
// Utilities

const char* VFlappyController::name() const {
    return vlSymsp->name();
}

//============================================================
// Invoke final blocks

void VFlappyController___024root___eval_final(VFlappyController___024root* vlSelf);

VL_ATTR_COLD void VFlappyController::final() {
    VFlappyController___024root___eval_final(&(vlSymsp->TOP));
}

//============================================================
// Implementations of abstract methods from VerilatedModel

const char* VFlappyController::hierName() const { return vlSymsp->name(); }
const char* VFlappyController::modelName() const { return "VFlappyController"; }
unsigned VFlappyController::threads() const { return 1; }
void VFlappyController::prepareClone() const { contextp()->prepareClone(); }
void VFlappyController::atClone() const {
    contextp()->threadPoolpOnClone();
}
