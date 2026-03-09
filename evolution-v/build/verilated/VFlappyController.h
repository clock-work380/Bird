// Verilated -*- C++ -*-
// DESCRIPTION: Verilator output: Primary model header
//
// This header should be included by all source files instantiating the design.
// The class here is then constructed to instantiate the design.
// See the Verilator manual for examples.

#ifndef VERILATED_VFLAPPYCONTROLLER_H_
#define VERILATED_VFLAPPYCONTROLLER_H_  // guard

#include "verilated.h"

class VFlappyController__Syms;
class VFlappyController___024root;

// This class is the main interface to the Verilated model
class alignas(VL_CACHE_LINE_BYTES) VFlappyController VL_NOT_FINAL : public VerilatedModel {
  private:
    // Symbol table holding complete model state (owned by this class)
    VFlappyController__Syms* const vlSymsp;

  public:

    // CONSTEXPR CAPABILITIES
    // Verilated with --trace?
    static constexpr bool traceCapable = false;

    // PORTS
    // The application code writes and reads these signals to
    // propagate new values into/out from the Verilated model.
    VL_IN8(&clk,0,0);
    VL_IN8(&rst,0,0);
    VL_IN8(&in0,7,0);
    VL_IN8(&in1,7,0);
    VL_IN8(&in2,7,0);
    VL_IN8(&in3,7,0);
    VL_OUT8(&flap,0,0);
    VL_IN16(&wh00,15,0);
    VL_IN16(&wh01,15,0);
    VL_IN16(&wh02,15,0);
    VL_IN16(&wh03,15,0);
    VL_IN16(&wh10,15,0);
    VL_IN16(&wh11,15,0);
    VL_IN16(&wh12,15,0);
    VL_IN16(&wh13,15,0);
    VL_IN16(&wh20,15,0);
    VL_IN16(&wh21,15,0);
    VL_IN16(&wh22,15,0);
    VL_IN16(&wh23,15,0);
    VL_IN16(&wo0,15,0);
    VL_IN16(&wo1,15,0);
    VL_IN16(&wo2,15,0);
    VL_OUT16(&activation,15,0);
    VL_OUT16(&s3_h0,15,0);
    VL_OUT16(&s3_h1,15,0);
    VL_OUT16(&s3_h2,15,0);
    VL_OUT(&s2_h0,31,0);
    VL_OUT(&s2_h1,31,0);
    VL_OUT(&s2_h2,31,0);
    VL_OUT(&s5_acc,31,0);

    // CELLS
    // Public to allow access to /* verilator public */ items.
    // Otherwise the application code can consider these internals.

    // Root instance pointer to allow access to model internals,
    // including inlined /* verilator public_flat_* */ items.
    VFlappyController___024root* const rootp;

    // CONSTRUCTORS
    /// Construct the model; called by application code
    /// If contextp is null, then the model will use the default global context
    /// If name is "", then makes a wrapper with a
    /// single model invisible with respect to DPI scope names.
    explicit VFlappyController(VerilatedContext* contextp, const char* name = "TOP");
    explicit VFlappyController(const char* name = "TOP");
    /// Destroy the model; called (often implicitly) by application code
    virtual ~VFlappyController();
  private:
    VL_UNCOPYABLE(VFlappyController);  ///< Copying not allowed

  public:
    // API METHODS
    /// Evaluate the model.  Application must call when inputs change.
    void eval() { eval_step(); }
    /// Evaluate when calling multiple units/models per time step.
    void eval_step();
    /// Evaluate at end of a timestep for tracing, when using eval_step().
    /// Application must call after all eval() and before time changes.
    void eval_end_step() {}
    /// Simulation complete, run final blocks.  Application must call on completion.
    void final();
    /// Are there scheduled events to handle?
    bool eventsPending();
    /// Returns time at next time slot. Aborts if !eventsPending()
    uint64_t nextTimeSlot();
    /// Trace signals in the model; called by application code
    void trace(VerilatedTraceBaseC* tfp, int levels, int options = 0) { contextp()->trace(tfp, levels, options); }
    /// Retrieve name of this model instance (as passed to constructor).
    const char* name() const;

    // Abstract methods from VerilatedModel
    const char* hierName() const override final;
    const char* modelName() const override final;
    unsigned threads() const override final;
    /// Prepare for cloning the model at the process level (e.g. fork in Linux)
    /// Release necessary resources. Called before cloning.
    void prepareClone() const;
    /// Re-init after cloning the model at the process level (e.g. fork in Linux)
    /// Re-allocate necessary resources. Called after cloning.
    void atClone() const;
  private:
    // Internal functions - trace registration
    void traceBaseModel(VerilatedTraceBaseC* tfp, int levels, int options);
};

#endif  // guard
