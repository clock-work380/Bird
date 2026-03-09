#include "PhysicsEngine.h"
#include <algorithm>
#include <cstdlib>
#include <cmath>

namespace Evo {

PhysicsEngine::PhysicsEngine(std::vector<Bird>& pop) : pop_(pop) {
#ifdef USE_VERILATOR
    for (int i = 0; i < POP_SIZE; i++)
        ctls_[i] = std::make_unique<VFlappyController>(getVerilatedContext());
#else
    for (int i = 0; i < POP_SIZE; i++)
        stubs_[i] = std::make_unique<RTLStub>();
#endif
    reset();
}

void PhysicsEngine::reset() {
    pipes_.clear();
    frame_ = 0; clk_cycles_ = 0;
    pipes_.push_back({(float)(WORLD_W + 80),
        PIPE_GAP * 0.7f + (float)(rand() % (WORLD_H - (int)(PIPE_GAP * 1.4f))), false});

    // Reset all RTL controllers
#ifdef USE_VERILATOR
    for (int i = 0; i < POP_SIZE; i++) {
        auto& c = ctls_[i];
        c->rst = 1; c->clk = 0; c->eval();
        c->clk = 1; c->eval();
        c->clk = 0; c->eval();
        c->rst = 0;
    }
#else
    for (int i = 0; i < POP_SIZE; i++) {
        stubs_[i]->rst = 1; stubs_[i]->eval();
        stubs_[i]->rst = 0; stubs_[i]->eval();
    }
#endif
    for (auto& b : pop_) {
        b.y = WORLD_H / 2.f; b.vy = 0.f; b.alive = true;
        b.score = 0; b.frames = 0; b.flap_out = false;
    }
}

void PhysicsEngine::spawnPipe() {
    float minG = PIPE_GAP * 0.6f, maxG = WORLD_H - PIPE_GAP * 0.6f;
    float gapY = minG + (float)rand() / RAND_MAX * (maxG - minG);
    pipes_.push_back({(float)(WORLD_W + PIPE_W), gapY, false});
}

const Pipe* PhysicsEngine::nearestPipe() const {
    const Pipe* best = nullptr;
    for (const auto& p : pipes_)
        if (p.x + PIPE_W > BIRD_X - BIRD_R)
            if (!best || p.x < best->x) best = &p;
    return best ? best : (pipes_.empty() ? nullptr : &pipes_.front());
}

int8_t  PhysicsEngine::toS8(float v) { int i = (int)v; return (int8_t)(i < -128 ? -128 : i > 127 ? 127 : i); }
int16_t PhysicsEngine::toQ8(float v) { int i = (int)(v * Q8_SCALE); return (int16_t)(i < -32768 ? -32768 : i > 32767 ? 32767 : i); }

void PhysicsEngine::driveAll(const Pipe* p) {
    float gapY     = p ? p->gapY : (float)WORLD_H / 2.f;
    float pipeDist = p ? (p->x - BIRD_X) : (float)WORLD_W;

    for (int i = 0; i < POP_SIZE; i++) {
        Bird& b = pop_[i];

        float hErr  = (b.y - gapY)           / (WORLD_H * 0.5f) * 127.f;
        float velS  =  b.vy                   * 10.f;
        float distS =  pipeDist              / (float)WORLD_W * 127.f;
        float gapS  = (gapY - WORLD_H * 0.5f) / (WORLD_H * 0.5f) * 127.f;

        int8_t i0 = toS8(hErr), i1 = toS8(velS), i2 = toS8(distS), i3 = toS8(gapS);
        b.in_regs[0] = i0; b.in_regs[1] = i1;
        b.in_regs[2] = i2; b.in_regs[3] = i3;

        if (i == 0) lastInputs = {hErr, velS, distS, gapS};

#ifdef USE_VERILATOR
        auto& c = ctls_[i];
        c->in0 = i0; c->in1 = i1; c->in2 = i2; c->in3 = i3;
        c->wh00 = toQ8(b.w[0][0]); c->wh01 = toQ8(b.w[0][1]);
        c->wh02 = toQ8(b.w[0][2]); c->wh03 = toQ8(b.w[0][3]);
        c->wh10 = toQ8(b.w[1][0]); c->wh11 = toQ8(b.w[1][1]);
        c->wh12 = toQ8(b.w[1][2]); c->wh13 = toQ8(b.w[1][3]);
        c->wh20 = toQ8(b.w[2][0]); c->wh21 = toQ8(b.w[2][1]);
        c->wh22 = toQ8(b.w[2][2]); c->wh23 = toQ8(b.w[2][3]);
        c->wo0  = toQ8(b.wo[0]);
        c->wo1  = toQ8(b.wo[1]);
        c->wo2  = toQ8(b.wo[2]);

        // Clock 5 times to flush the 5-stage pipeline
        for (int cyc = 0; cyc < 5; cyc++) {
            getVerilatedContext()->timeInc(1); c->clk = 0; c->eval();
            getVerilatedContext()->timeInc(1); c->clk = 1; c->eval();
        }

        b.flap_out   = (c->flap != 0);
        b.activation =  c->activation;
        b.h_regs[0]  =  c->s3_h0;
        b.h_regs[1]  =  c->s3_h1;
        b.h_regs[2]  =  c->s3_h2;
#else
        auto& s = stubs_[i];
        s->in0 = i0; s->in1 = i1; s->in2 = i2; s->in3 = i3;
        s->wh00 = toQ8(b.w[0][0]); s->wh01 = toQ8(b.w[0][1]);
        s->wh02 = toQ8(b.w[0][2]); s->wh03 = toQ8(b.w[0][3]);
        s->wh10 = toQ8(b.w[1][0]); s->wh11 = toQ8(b.w[1][1]);
        s->wh12 = toQ8(b.w[1][2]); s->wh13 = toQ8(b.w[1][3]);
        s->wh20 = toQ8(b.w[2][0]); s->wh21 = toQ8(b.w[2][1]);
        s->wh22 = toQ8(b.w[2][2]); s->wh23 = toQ8(b.w[2][3]);
        s->wo0  = toQ8(b.wo[0]);
        s->wo1  = toQ8(b.wo[1]);
        s->wo2  = toQ8(b.wo[2]);
        s->clk  = 0; s->eval();
        s->clk  = 1; s->eval();
        b.flap_out   = (s->flap != 0);
        b.activation =  s->activation;
        b.h_regs[0]  =  s->s3_h0;
        b.h_regs[1]  =  s->s3_h1;
        b.h_regs[2]  =  s->s3_h2;
        b.in_regs[0] =  s->in0; b.in_regs[1] = s->in1;
        b.in_regs[2] =  s->in2; b.in_regs[3] = s->in3;
#endif
    }
    ++clk_cycles_;
}

bool PhysicsEngine::collides(const Bird& b, const Pipe& p) const {
    bool inX = (BIRD_X + BIRD_R > p.x) && (BIRD_X - BIRD_R < p.x + PIPE_W);
    if (!inX) return false;
    float top = p.gapY - PIPE_GAP / 2.f, bot = p.gapY + PIPE_GAP / 2.f;
    return (b.y - BIRD_R < top) || (b.y + BIRD_R > bot);
}

int PhysicsEngine::tick(float speedMult) {
    int steps = std::max(1, (int)std::round(speedMult));
    int alive = 0;
    for (int s = 0; s < steps; s++) {
        ++frame_;
        for (auto& p : pipes_) p.x -= PIPE_SPEED;
        pipes_.erase(std::remove_if(pipes_.begin(), pipes_.end(),
            [](const Pipe& p){ return p.x + PIPE_W < -20; }), pipes_.end());
        if (pipes_.empty() || pipes_.back().x < WORLD_W - 220) spawnPipe();

        const Pipe* nearPipe = nearestPipe();
        if (frame_ % CLK_DIV == 0) driveAll(nearPipe);

        alive = 0;
        for (auto& b : pop_) {
            if (!b.alive) continue;
            if (b.flap_out) {
                b.vy = FLAP_VY;
                b.flap_latch = true;
                b.flap_latch_ttl = CLK_DIV * 2;
            }
            if (b.flap_latch_ttl > 0) { --b.flap_latch_ttl; }
            else { b.flap_latch = false; }
            b.vy = std::min(b.vy + GRAVITY, 12.f);
            b.y += b.vy;
            ++b.frames;
            for (auto& p : pipes_) {
                if (!p.scored && p.x + PIPE_W / 2.f < BIRD_X) {
                    p.scored = true; ++b.score;
                    if (b.score > top_score_) top_score_ = b.score;
                }
            }
            bool wallHit = (b.y - BIRD_R < 0) || (b.y + BIRD_R > WORLD_H);
            bool pipeHit = false;
            for (const auto& p : pipes_) if (collides(b, p)) { pipeHit = true; break; }
            if (wallHit || pipeHit) { b.alive = false; continue; }
            ++alive;
        }
    }
    return alive;
}

} // namespace Evo
