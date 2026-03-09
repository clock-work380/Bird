CXX      := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wno-unused-parameter -Wno-sign-compare

SFML_LIBS := $(shell pkg-config --libs sfml-all 2>/dev/null || echo \
             "-lsfml-graphics -lsfml-window -lsfml-audio -lsfml-system")
SFML_INC  := $(shell pkg-config --cflags sfml-all 2>/dev/null || echo "-I/usr/include")

VERILATOR      := verilator
VERILATOR_ROOT := $(shell $(VERILATOR) --getenv VERILATOR_ROOT)
VERILATOR_INC  := $(VERILATOR_ROOT)/include

SRC_DIR := src
RTL_DIR := rtl
VER_DIR := build/verilated

SRCS := $(SRC_DIR)/main.cpp $(SRC_DIR)/PhysicsEngine.cpp

.PHONY: all
all: build/evolution-v
	@echo "Run: ./build/evolution-v"

build/evolution-v: $(SRCS) $(wildcard $(SRC_DIR)/*.h)
	@mkdir -p build
	$(CXX) $(CXXFLAGS) $(SFML_INC) -I$(SRC_DIR) $(SRCS) -o $@ $(SFML_LIBS)

.PHONY: hil
hil: build/evolution-v_hil
	@echo "Run: ./build/evolution-v_hil"

# Verilate just FlappyController - one instance per bird, simple & correct
$(VER_DIR)/VFlappyController__ALL.a: $(RTL_DIR)/FlappyController.v
	@mkdir -p $(VER_DIR)
	$(VERILATOR) --cc \
	    --Mdir $(VER_DIR) \
	    --clk clk \
	    -Wno-BADVLTPRAGMA -Wno-WIDTHEXPAND -Wno-UNPACKED \
	    $(RTL_DIR)/FlappyController.v \
	    --top-module FlappyController
	make -C $(VER_DIR) -f VFlappyController.mk

build/evolution-v_hil: $(VER_DIR)/VFlappyController__ALL.a $(SRCS) $(wildcard $(SRC_DIR)/*.h)
	$(CXX) $(CXXFLAGS) $(SFML_INC) \
	    -I$(SRC_DIR) -I$(VER_DIR) -I$(VERILATOR_INC) \
	    -DUSE_VERILATOR \
	    $(SRCS) \
	    $(VER_DIR)/VFlappyController__ALL.a \
	    $(VER_DIR)/verilated.o \
	    $(VER_DIR)/verilated_threads.o \
	    -o build/evolution-v_hil $(SFML_LIBS) -lpthread

.PHONY: clean
clean:
	rm -rf build
