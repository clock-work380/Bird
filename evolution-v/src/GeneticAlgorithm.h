#pragma once
// =============================================================================
//  GeneticAlgorithm.h  -  Elite clone + crossover (top-2) + mutation
// =============================================================================
#include "Bird.h"
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <cmath>

namespace Evo {

struct GenStats {
    int   generation   = 1;
    int   eliteScore   = 0;
    int   eliteFrames  = 0;
    // Keep these for panel display (first hidden neuron, first two inputs)
    float eliteWHeight = 0.f;
    float eliteWVel    = 0.f;
    // Full elite genome snapshot for Verilog snippet
    std::array<std::array<float,N_IN>, N_HIDDEN> eliteW{};
    std::array<float, N_HIDDEN>                  eliteWo{};
};

class GeneticAlgorithm {
public:
    explicit GeneticAlgorithm(float sigma = MUTATION_SIG)
        : sigma_(sigma), rng_(std::random_device{}()) {}

    void initPopulation(std::vector<Bird>& pop, int n) {
        pop.clear();
        pop.reserve(n);
        std::uniform_real_distribution<float> wd(-1.5f, 1.5f);
        for (int i=0;i<n;i++) {
            pop.emplace_back();
            pop.back().id = i;
            for (auto& row : pop.back().w)
                for (auto& ww : row) ww = wd(rng_);
            for (auto& ww : pop.back().wo) ww = wd(rng_);
        }
    }

    GenStats evolve(std::vector<Bird>& pop) {
        // Rank by score then frames
        std::vector<int> idx(pop.size());
        std::iota(idx.begin(), idx.end(), 0);
        std::sort(idx.begin(), idx.end(), [&](int a, int b){
            return pop[a].score != pop[b].score
                ? pop[a].score > pop[b].score
                : pop[a].frames > pop[b].frames;
        });

        int e1 = idx[0];
        int e2 = idx.size()>1 ? idx[1] : idx[0];

        GenStats stats;
        stats.eliteScore   = pop[e1].score;
        stats.eliteFrames  = pop[e1].frames;
        stats.eliteWHeight = pop[e1].w[0][0];
        stats.eliteWVel    = pop[e1].w[0][1];
        stats.eliteW       = pop[e1].w;
        stats.eliteWo      = pop[e1].wo;
        ++generation_;
        stats.generation   = generation_;

        // Stash genomes before clearing pop
        auto w1 = pop[e1].w;  auto wo1 = pop[e1].wo;
        auto w2 = pop[e2].w;  auto wo2 = pop[e2].wo;

        // Reuse existing Bird objects to avoid re-creating Verilated models
        // (VerilatedContext cannot add new models after time > 0)
        std::normal_distribution<float> mut(0.f, sigma_);
        std::uniform_real_distribution<float> pick(0.f, 1.f);

        // Grow population if needed (first generation)
        while ((int)pop.size() < POP_SIZE) {
            pop.emplace_back();
            pop.back().id = (int)pop.size()-1;
        }

        for (int i=0;i<POP_SIZE;i++) {
            pop[i].id = i;
            if (i == 0) {
                pop[i].w  = w1;
                pop[i].wo = wo1;
            } else if (i == 1) {
                pop[i].w  = w2;
                pop[i].wo = wo2;
            } else {
                for (int h=0;h<N_HIDDEN;h++) {
                    for (int j=0;j<N_IN;j++) {
                        float base = pick(rng_) < 0.5f ? w1[h][j] : w2[h][j];
                        pop[i].w[h][j] = base + mut(rng_);
                    }
                    float obase = pick(rng_) < 0.5f ? wo1[h] : wo2[h];
                    pop[i].wo[h] = obase + mut(rng_);
                }
            }
        }
        return stats;
    }

    int generation() const { return generation_; }

private:
    float sigma_;
    int   generation_ = 1;
    std::mt19937 rng_;
};

} // namespace Evo
