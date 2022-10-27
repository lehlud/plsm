CXX = clang++
CXX_FLAGS = -O3 -Isrc/ `llvm-config --cxxflags`
LD_FLAGS = `llvm-config --ldflags --system-libs --libs all` -flto -lLLVM

.PHONY: all clean create-build-dir

all: create-build-dir build/plsm

create-build-dir:
	@mkdir -p build
	@mkdir -p build/AST
	@mkdir -p build/Compiler

clean:
	rm -rf build/

build/plsm: build/main.o build/AST/Function.o build/AST/Number.o build/AST/Type.o
	$(CXX) $(LD_FLAGS) -o $@ $^

build/%.o: src/%.cpp
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
