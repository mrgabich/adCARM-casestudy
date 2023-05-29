static inline __attribute__((always_inline)) void test_function(double* test_var, int long long num_reps_t) {
    __asm__ __volatile__ (
        "ld t2, %0\n\t\t"                  // Load the value of num_reps_t into register t2
        "ld a0, %1\n\t\t"                  // Load the address of test_var into register a0
        "outer_loop_%=:\n\t\t"
        "li t1, 15\n\t\t"                  // Initialize a counter register t1 with 15

        "beqz t1, outer_loop_exit_%=\n\t\t" // Exit the outer loop if t1 is zero
        "inner_loop_%=:\n\t\t"

        "fld f0, 0(a0)\n\t\t"              // Move the value from memory (test_var) to floating-point register f0
        "fld f1, 8(a0)\n\t\t"              // Move the value from memory (test_var) to floating-point register f1
        "fsd f2, 16(a0)\n\t\t"             // Move the value from floating-point register f2 to memory (test_var)
        "fld f3, 24(a0)\n\t\t"             // Move the value from memory (test_var) to floating-point register f3
        "fld f4, 32(a0)\n\t\t"             // Move the value from memory (test_var) to floating-point register f4
        "fsd f5, 40(a0)\n\t\t"             // Move the value from floating-point register f5 to memory (test_var)
        "fld f6, 48(a0)\n\t\t"             // Move the value from memory (test_var) to floating-point register f6
        "fld f7, 56(a0)\n\t\t"             // Move the value from memory (test_var) to floating-point register f7
        "fsd f8, 64(a0)\n\t\t"             // Move the value from floating-point register f8 to memory (test_var)
        "fld f9, 72(a0)\n\t\t"             // Move the value from memory (test_var) to floating-point register f9
        "fld f10, 80(a0)\n\t\t"            // Move the value from memory (test_var) to floating-point register f10
        "fsd f11, 88(a0)\n\t\t"            // Move the value from floating-point register f11 to memory (test_var)
        "fld f12, 96(a0)\n\t\t"            // Move the value from memory (test_var) to floating-point register f12
        "fld f13, 104(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f13
        "fsd f14, 112(a0)\n\t\t"           // Move the value from floating-point register f14 to memory (test_var)
        "fld f15, 120(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f15
        "fld f16, 128(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f16
        "fsd f17, 136(a0)\n\t\t"           // Move the value from floating-point register f17 to memory (test_var)
        "fld f18, 144(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f18
        "fld f19, 152(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f19
        "fsd f20, 160(a0)\n\t\t"           // Move the value from floating-point register f20 to memory (test_var)
        "fld f21, 168(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f21
        "fld f22, 176(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f22
        "fsd f23, 184(a0)\n\t\t"           // Move the value from floating-point register f23 to memory (test_var)
        "fld f24, 192(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f24
        "fld f25, 200(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f25
        "fsd f26, 208(a0)\n\t\t"           // Move the value from floating-point register f26 to memory (test_var)
        "fld f27, 216(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f27
        "fld f28, 224(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f28
        "fsd f29, 232(a0)\n\t\t"           // Move the value from floating-point register f29 to memory (test_var)
        "fld f30, 240(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f30
        "fld f31, 248(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f31
        "fsd f0, 256(a0)\n\t\t"            // Move the value from floating-point register f0 to memory (test_var)
        "fld f1, 264(a0)\n\t\t"            // Move the value from memory (test_var) to floating-point register f1
        "fld f2, 272(a0)\n\t\t"            // Move the value from memory (test_var) to floating-point register f2
        "fsd f3, 280(a0)\n\t\t"            // Move the value from floating-point register f3 to memory (test_var)
        "fld f4, 288(a0)\n\t\t"            // Move the value from memory (test_var) to floating-point register f4
        "fld f5, 296(a0)\n\t\t"            // Move the value from memory (test_var) to floating-point register f5
        "fsd f6, 304(a0)\n\t\t"            // Move the value from floating-point register f6 to memory (test_var)
        "fld f7, 312(a0)\n\t\t"            // Move the value from memory (test_var) to floating-point register f7
        "fld f8, 320(a0)\n\t\t"            // Move the value from memory (test_var) to floating-point register f8
        "fsd f9, 328(a0)\n\t\t"            // Move the value from floating-point register f9 to memory (test_var)
        "fld f10, 336(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f10
        "fld f11, 344(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f11
        "fsd f12, 352(a0)\n\t\t"           // Move the value from floating-point register f12 to memory (test_var)
        "fld f13, 360(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f13
        "fld f14, 368(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f14
        "fsd f15, 376(a0)\n\t\t"           // Move the value from floating-point register f15 to memory (test_var)
        "fld f16, 384(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f16
        "fld f17, 392(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f17
        "fsd f18, 400(a0)\n\t\t"           // Move the value from floating-point register f18 to memory (test_var)
        "fld f19, 408(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f19
        "fld f20, 416(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f20
        "fsd f21, 424(a0)\n\t\t"           // Move the value from floating-point register f21 to memory (test_var)
        "fld f22, 432(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f22
        "fld f23, 440(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f23
        "fsd f24, 448(a0)\n\t\t"           // Move the value from floating-point register f24 to memory (test_var)
        "fld f25, 456(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f25
        "fld f26, 464(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f26
        "fsd f27, 472(a0)\n\t\t"           // Move the value from floating-point register f27 to memory (test_var)
        "fld f28, 480(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f28
        "fld f29, 488(a0)\n\t\t"           // Move the value from memory (test_var) to floating-point register f29
        "fsd f30, 496(a0)\n\t\t"           // Move the value from floating-point register f30 to memory (test_var)
        "addi a0, a0, 504\n\t\t"           // Increment the memory address by 504 (size of 8 double-precision floats)
        "addi t1, t1, -1\n\t\t"            // Decrement the counter register t1 by 1
        "bnez t1, inner_loop_%=\n\t\t"     // Repeat the inner loop until t1 is zero
        "outer_loop_exit_%=:\n\t\t"
        "addi t2, t2, -1\n\t\t"            // Decrement the counter register t2 by 1
        "bnez t2, outer_loop_%=\n\t\t"     // Repeat the outer loop until t2 is zero
        :
        : "m"(num_reps_t), "m"(test_var)
        : "a0", "t1", "t2", "f0", "f1", "f2", "f3", "f4", "f5", "f6", "f7", "f8", "f9", "f10", "f11", "f12", "f13", "f14", "f15",
          "f16", "f17", "f18", "f19", "f20", "f21", "f22", "f23", "f24", "f25", "f26", "f27", "f28", "f29", "f30", "f31"
    );
}


int main() {
    double test_var;
    int long long num_reps = 1000000;

    test_function(&test_var, num_reps);

    return 0;
}
