CXX = gcc
CXX_FLAGS = -O3

.PHONY: all clean create-build-dir

all: create-build-dir build/plsm

create-build-dir:
	@mkdir -p build

clean:
	rm -rf build/

build/plsm: build/main.o build/memory.o build/inst.o
	$(CXX) $(CXX_FLAGS) -o $@ $^

build/%.o: src/%.c
	$(CXX) $(CXX_FLAGS) -c -o $@ $<
