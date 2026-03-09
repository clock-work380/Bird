#pragma once
#include "Bird.h"
#include <vector>
#include <array>
#include <memory>

#ifdef USE_VERILATOR
  #include "VFlappyController.h"
  #include "verilated.h"
  static VerilatedContext* getVerilatedContext() {
      static VerilatedContext ctx;
      return &ctx;
  }
#endif
#include "RTLStub.h"

namespace Evo {

class PhysicsEngine {
public:
    explicit PhysicsEngine(std::vector<Bird>& pop);
    ~PhysicsEngine() = default;

    void  reset();
    int   tick(float speedMult);

    const std::vector<Pipe>& pipes()     const { return pipes_; }
    int                      frame()     const { return frame_; }
    int                      clkCycles() const { return clk_cycles_; }
    int                      topScore()  const { return top_score_; }

    std::array<float,4> lastInputs{};

private:
    std::vector<Bird>& pop_;
    std::vector<Pipe>  pipes_;
    int  frame_=0, clk_cycles_=0, top_score_=0;

#ifdef USE_VERILATOR
    // One VFlappyController per bird — simple, correct, guaranteed to work
    std::array<std::unique_ptr<VFlappyController>, POP_SIZE> ctls_;
#else
    std::array<std::unique_ptr<RTLStub>, POP_SIZE> stubs_;
#endif

    void spawnPipe();
    const Pipe* nearestPipe() const;
    bool collides(const Bird& b, const Pipe& p) const;
    void driveAll(const Pipe* near);

    static int8_t  toS8(float v);
    static int16_t toQ8(float v);
};

} // namespace Evo
