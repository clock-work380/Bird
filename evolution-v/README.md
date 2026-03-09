# Evolution-V

> Flappy Bird where the bird's brain is a **real Verilog neural network**, evolved by a genetic algorithm.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue) ![Verilog](https://img.shields.io/badge/Verilog-RTL-orange) ![Verilator](https://img.shields.io/badge/Verilator-5.x-green) ![SFML](https://img.shields.io/badge/SFML-3.x-red)

---

## What is this?

30 birds play Flappy Bird simultaneously. Each bird's brain is a **2-layer neural network implemented in Verilog RTL** — not simulated in software, but compiled into cycle-accurate gate logic via [Verilator](https://www.veripool.org/verilator/). A genetic algorithm evolves the network weights each generation.

Every frame, real clock edges are driven into the RTL. The hardware pipeline computes the flap decision in **5 stages** across 5 clock cycles — multiply, accumulate, ReLU, multiply, threshold — exactly as it would on an FPGA.

```
┌─────────────────────────────────────────────────────────┐
│  Genetic Algorithm (C++)                                │
│  evolves weights each generation                        │
│         │                                               │
│         ▼                                               │
│  Verilator HIL  ←── FlappyController.v (Verilog RTL)   │
│  30 instances, one per bird                             │
│  cycle-accurate 5-stage pipeline                        │
│         │                                               │
│         ▼                                               │
│  Physics Engine (C++) + SFML Renderer                   │
└─────────────────────────────────────────────────────────┘
```

---

## Neural Network Architecture

- **Inputs (4):** height error, velocity, pipe distance, gap center
- **Hidden layer:** 3 neurons, ReLU activation (approx_tanh via saturating shift)
- **Output:** 1 bit — flap or don't flap
- **Weights:** Q8.8 fixed-point, 15 total (12 hidden + 3 output)
- **Genome:** the 15 weights, evolved by crossover + gaussian mutation

### RTL Pipeline (FlappyController.v)

```
CLK 1: S1 — multiply inputs × first weights
CLK 2: S2 — accumulate dot products
CLK 3: S3 — ReLU activation (approx_tanh)
CLK 4: S4 — multiply hidden × output weights
CLK 5: S5 — sum + threshold → flap bit
```

---

## Hardware Parallelism

Sequential software would evaluate 30 birds × 5 stages = **150 cycles**.  
With 30 Verilator instances sharing one clock, all 30 birds evaluate in **5 cycles** — a **30x speedup** visible live in the panel.

---

## Right Panel

| Section | What it shows |
|---|---|
| **Neural Net** | Live weight visualization, node activations, population status |
| **RTL Pipeline** | S1–S5 stage values, active/idle state per stage |
| **Logic Analyzer** | FLAP, HEIGHT_ERR, VELOCITY, PIPE_DIST waveforms |
| **Bit Registers** | Raw 8-bit values of all 4 inputs in binary + hex |
| **Parallel Eval** | Bird grid (parallel vs sequential), cycle cost bar chart, throughput counter |
| **RTL Source** | Live Verilog with current best bird's weights |

---

## Building

### Requirements

- MSYS2 UCRT64 (Windows) or GCC on Linux
- [SFML 3.x](https://www.sfml-dev.org/)
- [Verilator 5.x](https://www.veripool.org/verilator/) (for HIL build)

### Software stub (no Verilator needed)

```bash
make
./build/evolution-v
```

### Hardware-in-the-loop (Verilator RTL)

```bash
make hil
./build/evolution-v_hil
```

---

## Controls

| Key | Action |
|---|---|
| `+` / `=` | Speed up (1x → 10x) |
| `-` | Slow down |
| `Escape` | Quit |

---

## Project Structure

```
evolution-v/
├── rtl/
│   ├── FlappyController.v   # 5-stage pipeline neural net (the brain)
│   └── FlappyArray.v        # 30x parallel instantiation
├── src/
│   ├── main.cpp             # window, game loop
│   ├── PhysicsEngine.*      # drives RTL, applies physics
│   ├── GeneticAlgorithm.h   # selection, crossover, mutation
│   ├── GameRenderer.h       # game world, birds, pipes
│   ├── NeuralPanel.h        # right panel, all visualizations
│   ├── Bird.h               # bird state + genome
│   └── RTLStub.h            # software fallback
├── assets/                  # sprites + sounds
└── Makefile
```

---

## Why Verilator?

Verilator compiles Verilog into C++ that models exact gate-level behavior — every flip-flop, every pipeline register, every clock edge. It's how real chips are verified before tape-out. The neural net here has real bit-width constraints, real pipeline latency, and real fixed-point arithmetic — bugs that pure software simulation would never catch.

The next step would be synthesizing `FlappyController.v` with Yosys + nextpnr and loading it onto an FPGA, where the same Verilog would run as actual silicon logic gates.

---

## License

MIT
