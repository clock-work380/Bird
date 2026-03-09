#pragma once
#include <cstdint>
#include <array>
#include <memory>

#ifdef USE_VERILATOR
  #include "VFlappyController.h"
  #include "verilated.h"
#endif
#include "RTLStub.h"

namespace Evo {

constexpr float GRAVITY      = 0.33f;
constexpr float FLAP_VY      = -7.4f;
constexpr float PIPE_SPEED   = 2.2f;
constexpr int   PIPE_W       = 52;
constexpr int   PIPE_GAP     = 148;
constexpr int   BIRD_X       = 120;
constexpr float BIRD_R       = 14.f;
constexpr int   BIRD_W       = 16;
constexpr int   BIRD_H       = 12;
constexpr int   WORLD_W      = 1380;
constexpr int   WORLD_H      = 886;
constexpr int   POP_SIZE     = 30;
constexpr int   CLK_DIV      = 4;
constexpr float MUTATION_SIG = 0.12f;
constexpr float Q8_SCALE     = 256.f;
constexpr int   N_IN         = 4;
constexpr int   N_HIDDEN     = 3;

struct Pipe { float x, gapY; bool scored; };

#ifdef USE_VERILATOR
inline VerilatedContext* getVerilatedContext() {
    static VerilatedContext ctx;
    return &ctx;
}
#endif

// Per-bird state -- NO RTL model here anymore, array module owns all birds
struct Bird {
    float y=WORLD_H/2.f, vy=0.f;
    bool  alive=true;
    int   score=0, frames=0, id=0;

    std::array<std::array<float,N_IN>, N_HIDDEN> w{};
    std::array<float, N_HIDDEN>                  wo{};

    // RTL readback (filled by PhysicsEngine from array module)
    std::array<int8_t,  N_IN>     in_regs{};
    std::array<int16_t, N_HIDDEN> h_regs{};
    bool     flap_out=false;
    bool     flap_latch=false;  // stays true for CLK_DIV frames for HUD display
    int      flap_latch_ttl=0;
    int16_t  activation=0;

    Bird() {
        for(auto& row:w) row.fill(0.f);
        wo.fill(0.f);
    }

    float wHeight() const { return w[0][0]; }
    float wVel()    const { return w[0][1]; }
};

} // namespace Evo
