COMPILER = gcc
FLAGS = -g -Wall -Wextra -O2

ifeq ($(isa),sse)
	FLAGS += -DSSE
else ifeq ($(isa),avx)
	FLAGS += -DAVX
else ifeq ($(isa),avx256)
	FLAGS += -DAVX256
else ifeq ($(isa),avx512)
	FLAGS += -DAVX512
else ifeq ($(isa),scalar)
	FLAGS += -DSCALAR
else ifeq ($(isa),rv64)
	FLAGS += -DRV -DRV64 -DFP
else
	FLAGS += -DARMV7A
endif

Bench:
	$(COMPILER) $(FLAGS) Bench.c select_op.c create_bench.c write_asm.c calc_param.c -o Bench -lm

clean:
	rm -f Bench
