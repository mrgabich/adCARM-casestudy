#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "functions.h"
#include <string.h>

//Loop Body Size
#define BASE_LOOP_SIZE 256
#define INST_LOOP_SIZE 256

/*-------------------------------------------------------INTEL ISA------------------------------------------------------------------*/	
//Sifive FU540 comprises U54 - RV64IMAFDC application cores with
//an E51 - RV64IMAC monitor core 
#if defined (RV)
#if defined (RV64)
	#define ISA "rv64"
	#define DP_ALIGN 32
	#define SP_ALIGN 32
#else //if defined (RV32)
	#define ISA "rv32"
	#define DP_ALIGN 16
	#define SP_ALIGN 16
#endif
#if defined (FP)
	#define NUM_REGISTER 32
	#define REGISTER "f"
	//#define REGISTER "x"
	//#define COBLERED "\"%%x0\",\"%%x1\",\"%%x2\",\"%%x3\",\"%%x4\",\"%%x5\",\"%%x6\",\"%%x7\",\"%%x8\",\"%%x9\",\"%%x10\",\"%%x11\",\"%%x12\",\"%%x13\",\"%%x14\",\"%%x15\",\"memory\""
	#define COBLERED "\"%%f0\",\"%%f1\",\"%%f2\",\"%%f3\",\"%%f4\",\"%%f5\",\"%%f6\",\"%%f7\",\"%%f8\",\"%%f9\",\"%%f10\",\"%%f11\",\"%%f12\",\"%%f13\",\"%%f14\",\"%%f15\",\"memory\""
	#define DP_OPS 2
	#define DP_DIV "fdiv.d"
	#define DP_ADD "fadd.d"
	#define DP_MUL "fmul.d"
	#define DP_FMA "fmadd.d"
	#define DP_MEM "fmv.d"

	#define SP_OPS 4
	#define SP_DIV "fdiv.s"
	#define SP_ADD "fadd.s"
	#define SP_MUL "fmul.s"
	#define SP_FMA "fmadd.s"
	#define SP_MEM "fmv.s"
#else //if defined (I)
	#define NUM_REGISTER 32
	#define REGISTER "x"
	#define COBLERED "\"%%x0\",\"%%x1\",\"%%x2\",\"%%x3\",\"%%x4\",\"%%x5\",\"%%x6\",\"%%x7\",\"%%x8\",\"%%x9\",\"%%x10\",\"%%x11\",\"%%x12\",\"%%x13\",\"%%x14\",\"%%x15\",\"memory\""
	#define SP_OPS 4
	#define SP_DIV "div"
	#define SP_ADD "add"
	#define SP_MUL "mul"
	#define SP_FMA "fmadd"
	#define SP_MEM "fmv"
#endif
#elif defined (AVX512)
	#define ISA "avx512"
	#define NUM_REGISTER 32
	#define REGISTER "zmm"
	#define DP_ALIGN 64
	#define SP_ALIGN 64
	#define COBLERED "\"%%zmm0\",\"%%zmm1\",\"%%zmm2\",\"%%zmm3\",\"%%zmm4\",\"%%zmm5\",\"%%zmm6\",\"%%zmm7\",\"%%zmm8\",\"%%zmm9\",\"%%zmm10\",\"%%zmm11\",\"%%zmm12\",\"%%zmm13\",\"%%zmm14\",\"%%zmm15\",\"memory\""
	#define DP_OPS 8
	#define DP_DIV "vdivpd"
	#define DP_ADD "vaddpd"
	#define DP_MUL "vmulpd"
	#define DP_FMA "vfmadd132pd"
	#define DP_MEM "vmovapd"

	#define SP_OPS 16
	#define SP_DIV "vdivps"
	#define SP_ADD "vaddps"
	#define SP_MUL "vmulps"
	#define SP_FMA "vfmadd132ps"
	#define SP_MEM "vmovaps"
#elif defined (AVX256)
	#define ISA "avx256"
	#define NUM_REGISTER 32
	#define REGISTER "ymm"
	#define DP_ALIGN 32
	#define SP_ALIGN 32
	#define COBLERED "\"%%ymm0\",\"%%ymm1\",\"%%ymm2\",\"%%ymm3\",\"%%ymm4\",\"%%ymm5\",\"%%ymm6\",\"%%ymm7\",\"%%ymm8\",\"%%ymm9\",\"%%ymm10\",\"%%ymm11\",\"%%ymm12\",\"%%ymm13\",\"%%ymm14\",\"%%ymm15\",\"memory\""
	#define DP_OPS 4
	#define DP_DIV "vdivpd"
	#define DP_ADD "vaddpd"
	#define DP_MUL "vmulpd"
	#define DP_FMA "vfmadd132pd"
	#define DP_MEM "vmovapd"

	#define SP_OPS 8
	#define SP_DIV "vdivps"
	#define SP_ADD "vaddps"
	#define SP_MUL "vmulps"
	#define SP_FMA "vfmadd132ps"
	#define SP_MEM "vmovaps"
#elif defined (AVX)
	#define ISA "avx"
	#define NUM_REGISTER 16
	#define REGISTER "ymm"
	#define DP_ALIGN 32
	#define SP_ALIGN 32
	#define COBLERED "\"%%ymm0\",\"%%ymm1\",\"%%ymm2\",\"%%ymm3\",\"%%ymm4\",\"%%ymm5\",\"%%ymm6\",\"%%ymm7\",\"%%ymm8\",\"%%ymm9\",\"%%ymm10\",\"%%ymm11\",\"%%ymm12\",\"%%ymm13\",\"%%ymm14\",\"%%ymm15\",\"memory\""
	#define DP_OPS 4
	#define DP_DIV "vdivpd"
	#define DP_ADD "vaddpd"
	#define DP_MUL "vmulpd"
	#define DP_FMA "vfmadd132pd"
	#define DP_MEM "vmovapd"

	#define SP_OPS 8
	#define SP_DIV "vdivps"
	#define SP_ADD "vaddps"
	#define SP_MUL "vmulps"
	#define SP_FMA "vfmadd132ps"
	#define SP_MEM "vmovaps"
#elif defined(SSE)
	#define ISA "sse"
	#define NUM_REGISTER 16
	#define REGISTER "xmm"
	#define DP_ALIGN 16
	#define SP_ALIGN 16
	#define COBLERED "\"%%xmm0\",\"%%xmm1\",\"%%xmm2\",\"%%xmm3\",\"%%xmm4\",\"%%xmm5\",\"%%xmm6\",\"%%xmm7\",\"%%xmm8\",\"%%xmm9\",\"%%xmm10\",\"%%xmm11\",\"%%xmm12\",\"%%xmm13\",\"%%xmm14\",\"%%xmm15\",\"memory\""
	#define DP_OPS 2
	#define DP_DIV "divpd"
	#define DP_ADD "addpd"
	#define DP_MUL "mulpd"
	#define DP_FMA "vfmadd132pd"
	#define DP_MEM "movapd"

	#define SP_OPS 4
	#define SP_DIV "divps"
	#define SP_ADD "addps"
	#define SP_MUL "mulps"
	#define SP_FMA "vfmadd132ps"
	#define SP_MEM "movaps"
#else
	#define ISA "scalar"
	#define NUM_REGISTER 16
	#define REGISTER "xmm"
	#define DP_ALIGN 8
	#define SP_ALIGN 4
	#define COBLERED "\"%%xmm0\",\"%%xmm1\",\"%%xmm2\",\"%%xmm3\",\"%%xmm4\",\"%%xmm5\",\"%%xmm6\",\"%%xmm7\",\"%%xmm8\",\"%%xmm9\",\"%%xmm10\",\"%%xmm11\",\"%%xmm12\",\"%%xmm13\",\"%%xmm14\",\"%%xmm15\",\"memory\""

	#define DP_OPS 1
	#define DP_DIV "vdivsd"
	#define DP_ADD "vaddsd"
	#define DP_MUL "vmulsd"
	#define DP_FMA "vfmadd132sd"
	#define DP_MEM "movsd"
	
	#define SP_OPS 1
	#define SP_DIV "vdivss"
	#define SP_ADD "vaddss"
	#define SP_MUL "vmulss"
	#define SP_FMA "vfmadd132ss"
	#define SP_MEM "movss"
#endif

