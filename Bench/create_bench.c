#include "config_test.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																					CREATE FP TEST
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_benchmark_flops(char * op, char * precision, int long long fp){
	int sys_out;
	int flops;
	char command[128];
	char * assembly_op_flops_1, * assembly_op_flops_2;
	
	if(strcmp(op,"mad") == 0){
		select_ISA_flops(&flops, &assembly_op_flops_1, "mul", precision);	//Select FP operation based on the ISA
		select_ISA_flops(&flops, &assembly_op_flops_2, "add", precision);	//Select FP operation based on the ISA
	}else{
		select_ISA_flops(&flops, &assembly_op_flops_1,op, precision);	//Select FP operation based on the ISA
	}
		
	write_asm_fp (fp, op, flops, assembly_op_flops_1, assembly_op_flops_2, precision); 	//Write Assembly Code
	
	//Free auxiliary variables
	free(assembly_op_flops_1);
	if(strcmp(op,"mad") == 0) free(assembly_op_flops_2);
	
	sprintf(command,"make isa=%s -f Test/Makefile_Benchmark\n",ISA);
	sys_out = system(command);
	if (sys_out != 0) {
		printf("ERROR: Make %s FP test failed!\n",ISA);
		exit(2);
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																					CREATE MEM TEST
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void create_benchmark_mem(int long long num_rep, int num_ld, int num_st, char * precision){
	int sys_out;
	char command[128];
	char * assembly_op_mem_1, * assembly_op_mem_2;
	int align, ops;

#if defined(RV64)
	select_ISA_mem(&align, &ops, &assembly_op_mem_1, "load", precision);   //Select memory operation based on the ISA
	select_ISA_mem(&align, &ops, &assembly_op_mem_2, "store", precision);   //Select memory operation based on the ISA
#else
	select_ISA_mem(&align, &ops, &assembly_op_mem_1, "move", precision);   //Select memory operation based on the ISA
#endif
	write_asm_mem(num_rep, align, ops, num_ld, num_st, assembly_op_mem_1, assembly_op_mem_2, precision); //Write ASM code
		
	//Free auxiliary variables
	free(assembly_op_mem_1);
#if defined(RV64)
	free(assembly_op_mem_2);
#endif
	sprintf(command,"make isa=%s -f Test/Makefile_Benchmark\n",ISA);
	sys_out = system(command);
	if (sys_out != 0) {
		printf("ERROR: Make %s MEM test failed!\n",ISA);
		exit(3);
	}
}
