#include "config_test.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																					WRITE FP TEST
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void write_asm_fp (int long long fp, char * op, int flops, char * assembly_op_flops_1, char * assembly_op_flops_2, char * precision){
	
	int i, j;
	FILE * file,*file_header;
	int long long iter;
	j = 0;
		
	file_header =  fopen("Test/test_params.h", "w");
	file = file_header;
	
	//Specific test data
	if(strcmp(op,"div") == 0){
		fprintf(file_header,"#define DIV 1\n");
		fprintf(file_header,"#define NUM_LD 1\n");
		fprintf(file_header,"#define NUM_ST 0\n");
		fprintf(file_header,"#define OPS %d\n",flops);
		fprintf(file_header,"#define NUM_REP 1\n");
		if(strcmp(precision, "dp") == 0){
			fprintf(file_header,"#define PRECISION double\n");
			fprintf(file_header,"#define ALIGN %d\n", (int) DP_ALIGN);
		}else{
			fprintf(file_header,"#define PRECISION float\n");
			fprintf(file_header,"#define ALIGN %d\n", (int) SP_ALIGN);
		}
	}

	fprintf(file_header,"#define FP_INST %lld\n",fp);
	
	iter = flops_math(fp); //Calculate necessary iterations
	
	//Creating Test Function
	if(strcmp(op,"div") == 0){
		fprintf(file,"static inline __attribute__((always_inline)) void test_function(PRECISION * test_var, int long long num_rep_max){\n");
	}else{
		fprintf(file,"static inline __attribute__((always_inline)) void test_function(int long long num_rep_max){\n");
	}
	
	fprintf(file,"\t__asm__ __volatile__ (\n");
	fprintf(file,"\t\t\"movq %%0, %%%%r8\\n\\t\\t\"\n");
	if(strcmp(op,"div") == 0){
		fprintf(file,"\t\t\"movq %%1, %%%%rax\\n\\t\\t\"\n");
		if(strcmp(precision, "dp") == 0){
			fprintf(file,"\t\t\"%s (%%%%rax), %%%%%s0\\n\\t\\t\"\n", DP_MEM, REGISTER);	
		}else{
			fprintf(file,"\t\t\"%s (%%%%rax), %%%%%s0\\n\\t\\t\"\n", SP_MEM, REGISTER);	
		}
	}
	fprintf(file,"\t\t\"Loop2_%%=:\\n\\t\\t\"\n");
	if(iter > 1){
		fprintf(file,"\t\t\"movl $%lld, %%%%edi\\n\\t\\t\"\n",iter);
		fprintf(file,"\t\t\"Loop1_%%=:\\n\\t\\t\"\n");
		for(i = 0; i < BASE_LOOP_SIZE; i++){
			if(i % NUM_REGISTER == 0){
				j = 0;
			}
			#if defined(RV64)
				if(strcmp(op,"div") == 0){
					fprintf(file,"\t\t\"%s %%%%%s0, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, REGISTER, j, REGISTER, j);
				}else if(strcmp(op,"mad") == 0){
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
					j++;
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_2, REGISTER, j, REGISTER, j, REGISTER, j);
				}else{	
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
				}	
			#elif defined(AVX) || defined(AVX256) || defined(AVX512) || defined(SSE)
				if(strcmp(op,"div") == 0){
					fprintf(file,"\t\t\"%s %%%%%s0, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, REGISTER, j, REGISTER, j);
				}else if(strcmp(op,"mad") == 0){
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
					j++;
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_2, REGISTER, j, REGISTER, j, REGISTER, j);
				}else{	
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
				}	
			#else
				if(strcmp(op,"div") == 0){
					fprintf(file,"\t\t\"%s %%%%%s0, %%%%%s%d;\"\n", assembly_op_flops_1, REGISTER, REGISTER, j);
				}else if(strcmp(op,"mad") == 0){
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d;\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j);
					j++;
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					fprintf(file,"\t\t\"%s %%%%%st%d, %%%%%s%d;\"\n", assembly_op_flops_2, REGISTER, j, REGISTER, j);
				}else if(strcmp(op,"FMA") == 0){
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
				}else{
					fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d;\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j);
				}	
			#endif
			j++;
			fp -= iter;
		}	
		fprintf(file,"\t\t\"subl $1, %%%%edi\\n\\t\\t\"\n");
		fprintf(file,"\t\t\"jnz Loop1_%%=\\n\\t\\t\"\n");
	}
	
	for(i = 0; i < fp; i++){
		if(i % 16 == 0){
			j = 0;
		}
		#if defined(RV64)
			if(strcmp(op,"div") == 0){
				fprintf(file,"\t\t\"%s %%%%%s0, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, REGISTER, j, REGISTER, j);
			}else if(strcmp(op,"mad") == 0){
				if(j  >= NUM_REGISTER){
					j = 0;
				}
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
				j++;
				if(j  >= NUM_REGISTER){
					j = 0;
				}
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_2, REGISTER, j, REGISTER, j, REGISTER, j);
			}else{	
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
			}	
		#elif defined (AVX512) || defined (AVX256) || defined (AVX)
			if(strcmp(op,"div") == 0){
				fprintf(file,"\t\t\"%s %%%%%s0, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, REGISTER, j, REGISTER, j);
			}else if(strcmp(op,"mad") == 0){
				if(j  >= NUM_REGISTER){
					j = 0;
				}
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
				j++;
				if(j  >= NUM_REGISTER){
					j = 0;
				}
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_2, REGISTER, j, REGISTER, j, REGISTER, j);
			}else{	
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);
			}	
		#else
			if(strcmp(op,"div") == 0){
				fprintf(file,"\t\t\"%s %%%%%s0, %%%%%s%d;\"\n", assembly_op_flops_1, REGISTER, REGISTER, j);
			}else if(strcmp(op,"mad") == 0){
				if(j  >= NUM_REGISTER){
					j = 0;
				}
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d;\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j);
				j++;
				if(j  >= NUM_REGISTER){
					j = 0;
				}
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d;\"\n", assembly_op_flops_2, REGISTER, j, REGISTER, j);
			}else if(strcmp(op,"FMA") == 0){
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d, %%%%%s%d\\n\\t\\t\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j, REGISTER, j);	
			}else{
				fprintf(file,"\t\t\"%s %%%%%s%d, %%%%%s%d;\"\n", assembly_op_flops_1, REGISTER, j, REGISTER, j);
			}	
		#endif
		j++;
	}
	
	fprintf(file,"\t\t\"sub $1, %%%%r8\\n\\t\\t\"\n");
	fprintf(file,"\t\t\"jnz Loop2_%%=\\n\\t\\t\"\n");
	
	//End Test Function
	if(strcmp(op,"div") == 0){
		fprintf(file,"\t\t:\n\t\t:\"r\"(num_rep_max),\"r\" (test_var)\n\t\t:\"%%rax\",\"%%rdi\","COBLERED"\n\t);\n");
	}else{
		fprintf(file,"\t\t:\n\t\t:\"r\"(num_rep_max)\n\t\t:\"%%rax\",\"%%rdi\","COBLERED"\n\t);\n");
	}
	
	fprintf(file,"}\n\n");
	
	//fclose(file);
	fclose(file_header);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//																					WRITE MEM TEST
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void write_asm_mem (int long long num_rep, int align, int ops, int num_ld, int num_st, char * assembly_op, char * precision){
	
	int offset = 0;
	int aux = num_rep;
	int i, j = 0, k;
	FILE * file, * file_header;
	int num_aux;
	int long long iter;
	
	file_header =  fopen("Test/test_params.h", "w");
	file = file_header;
	
	//Specific Test Data
	fprintf(file_header,"#define MEM 1\n");	
	fprintf(file_header,"#define NUM_LD %d\n",num_ld);
	fprintf(file_header,"#define NUM_ST %d\n",num_st);
	fprintf(file_header,"#define OPS %d\n",ops);
	fprintf(file_header,"#define NUM_REP %lld\n",num_rep);
	if(strcmp(precision, "dp") == 0){
			fprintf(file_header,"#define PRECISION double\n");
	}else{
			fprintf(file_header,"#define PRECISION float\n");
	}
	fprintf(file_header,"#define ALIGN %d\n\n", align);
	fprintf(file_header,"#define FP_INST 1\n\n");
	
	iter = mem_math (num_rep, num_ld, num_st,&num_aux); //Calculate number of iterations
	
	//Create Test Function
	fprintf(file,"static inline __attribute__((always_inline)) void test_function(PRECISION * test_var, int long long num_reps_t){\n");
	fprintf(file,"\t__asm__ __volatile__ (\n");

#if defined(RV64)
	fprintf(file,"\t\t\"ld t2, %%0\\n\\t\\t\"\t\t\n"); // Move num_reps_t to t2
	fprintf(file,"\t\t\"ld a0, %%1\\n\\t\\t\"\n"); // Load the address of test_var into t0
	fprintf(file,"\t\t\"outer_loop_%%=:\\n\\t\\t\"\n"); //outer loop
	if(iter > 1){
		fprintf(file,"\t\t\"li t1, %lld\\n\\t\\t\"\n",iter); //Load the immediate loop size to t0
		fprintf(file,"\t\t\"beqz t1, outer_loop_exit_%%=\\n\\t\\t\"\n"); //Load the immediate loop size to t0
		fprintf(file,"\t\t\"inner_loop_%%=:\\n\\t\\t\"\n"); //inner loop
	}
		for(i = 0; i < num_aux-1; i++){
			for(k = 0;k < num_ld;k++){
				if(j  >= NUM_REGISTER){
					j = 0;
				}
				//fprintf(file,"\t\t\"ld a0, %d(t0)\\n\\t\\t\"\n", offset); // move from t0
				fprintf(file,"\t\t\"fld %s%d, %d(a0)\\n\\t\\t\"\n", REGISTER, j, offset); // move from t0
				j++;
				offset += align;
			}
			for(k = 0;k < num_st;k++){
				if(j  >= NUM_REGISTER){
					j = 0;
				}
				//fprintf(file,"\t\t\"ld t0, %d(t0)\\n\\t\\t\"\n", offset); // move from t0
				fprintf(file,"\t\t\"fsd %s%d, %d(a0)\\n\\t\\t\"\n", REGISTER, j, offset); // move from t0
				j++;
				offset += align;
			}
			aux -= iter;
		}
		fprintf(file,"\t\t\"addi a0, a0, %d\\n\\t\\t\"\n",offset); // Increment t0 with offset
	if(iter > 1){
		fprintf(file,"\t\t\"addi t1, t1, -1\\n\\t\\t\"\n"); // Decrement t1 by 1 (loop control)
		fprintf(file,"\t\t\"bnez t1, inner_loop_%%=\\n\\t\\t\"\n"); // Jump to Loop1_ if t1 is not equal to zero
	}
	fprintf(file,"\t\t\"outer_loop_exit_%%=:\\n\\t\\t\"\n"); // Outer Loop Exit	
	fprintf(file,"\t\t\"addi t2, t2, -1\\n\\t\\t\"\n"); // Decrement t2 by 1 (loop control)
	fprintf(file,"\t\t\"bnez t2, outer_loop_%%=\\n\\t\\t\"\n"); // Jump to Loop2_ if t2 is not equal to zero

	//End Test Function
	fprintf(file,"\t\t:\n\t\t:\"m\"(num_reps_t),\"m\" (test_var)\n\t\t:\"a0\",\"t1\",\"t2\","COBLERED"\n\t);\n");

#elif defined(armv7a)

	fprintf(file,"\t\t\"mov %%0, r8\\n\\t\\t\"\t\t\n"); // Move num_reps_t to r8
	fprintf(file,"\t\t\"Loop2_%%=:\\n\\t\\t\"\n"); //outer loop
	fprintf(file,"\t\t\"mov %%1, r0\\n\\t\\t\"\n"); // Load the address of test_var into r0
	if(iter > 1){
		fprintf(file,"\t\t\"mov $%lld, r1\\n\\t\\t\"\n",iter); //Load the immediate loop size to t0
		fprintf(file,"\t\t\"Loop1_%%=:\\n\\t\\t\"\n"); //inner loop
		for(i = 0; i < num_aux; i++){
				for(k = 0;k < num_ld;k++){
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					//TODO
					//fprintf(file,"\t\t\"%s %s%d, %d(a0)\\n\\t\\t\"\n", assembly_op, offset, REGISTER,j); // move from a0
					j++;
					offset += align;
				}
				for(k = 0;k < num_st;k++){
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					//TODO
					//fprintf(file,"\t\t\"%s %d(a0), %s%d\\n\\t\\t\"\n", assembly_op, REGISTER, j, offset); // move to a0
					j++;
					offset += align;
				}
				aux -= iter;
		}
		fprintf(file,"\t\t\"add r0, r0, #%d\\n\\t\\t\"\n",offset); // Increment a0 with offset
		fprintf(file,"\t\t\"sub r1, r1, #%1\\n\\t\\t\"\n"); // Decrement t1 by 1 (loop control)
		fprintf(file,"\t\t\"cmp r1, #%0\\n\\t\\t\"\n"); // Compare r1 with 0
		fprintf(file,"\t\t\"bne Loop1_%%=\\n\\t\\t\"\n"); // Jump to Loop1_ if t1 is not equal to zero
	}

	num_rep = aux;
	offset = 0;

	for(i = 0; i < num_rep; i++){
		for(k = 0;k < num_ld;k++){
			if(j  >= NUM_REGISTER){
				j = 0;
			}
			//TODO
			//fprintf(file,"\t\t\"%s %s%d, %d(a0)\\n\\t\\t\"\n", assembly_op, offset, REGISTER,j);
			j++;
			offset += align;
			
		}
		for(k = 0;k < num_st;k++){
			if(j  >= NUM_REGISTER){
				j = 0;
			}
			//TODO
			//fprintf(file,"\t\t\"%s %d(a0), %s%d\\n\\t\\t\"\n", assembly_op, REGISTER, j, offset);
			j++;
			offset += align;
		}
	}
	fprintf(file,"\t\t\"sub r8, r8, #%1\\n\\t\\t\"\n"); // Decrement t1 by 1 (loop control)
	fprintf(file,"\t\t\"cmp r8, #%0\\n\\t\\t\"\n"); // Compare r8 with 0
	fprintf(file,"\t\t\"bne Loop2_%%=\\n\\t\\t\"\n"); // Jump to Loop1_ if t1 is not equal to zero

	//End Test Function
	fprintf(file,"\t\t:\n\t\t:\"r\"(num_reps_t),\"r\" (test_var)\n\t\t:\"%%r0\",\"%%r1\",\"%%r8\","COBLERED"\n\t);\n");

#else

	fprintf(file,"\t\t\"movq %%0, %%%%r8\\n\\t\\t\"\n");
	fprintf(file,"\t\t\"Loop2_%%=:\\n\\t\\t\"\n");
	fprintf(file,"\t\t\"movq %%1, %%%%rax\\n\\t\\t\"\n");
	if(iter > 1){
		fprintf(file,"\t\t\"movq $%lld, %%%%rdi\\n\\t\\t\"\n",iter);
		fprintf(file,"\t\t\"Loop1_%%=:\\n\\t\\t\"\n");

		
		for(i = 0; i < num_aux; i++){
				for(k = 0;k < num_ld;k++){
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					fprintf(file,"\t\t\"%s %d(%%%%rax), %%%%%s%d\\n\\t\\t\"\n", assembly_op, offset, REGISTER,j);
					j++;
					offset += align;
				}
				for(k = 0;k < num_st;k++){
					if(j  >= NUM_REGISTER){
						j = 0;
					}
					fprintf(file,"\t\t\"%s %%%%%s%d, %d(%%%%rax)\\n\\t\\t\"\n", assembly_op, REGISTER, j, offset);
					j++;
					offset += align;
				}
				aux -= iter;
		}	
	
		fprintf(file,"\t\t\"addq $%d, %%%%rax\\n\\t\\t\"\n",offset);
		fprintf(file,"\t\t\"subq $1, %%%%rdi\\n\\t\\t\"\n");
		fprintf(file,"\t\t\"jnz Loop1_%%=\\n\\t\\t\"\n");
	}
	
	num_rep = aux;
	offset = 0;
	
	for(i = 0; i < num_rep; i++){
		for(k = 0;k < num_ld;k++){
			if(j  >= NUM_REGISTER){
				j = 0;
			}
			fprintf(file,"\t\t\"%s %d(%%%%rax), %%%%%s%d\\n\\t\\t\"\n", assembly_op, offset, REGISTER,j);
			j++;
			offset += align;
			
		}
		for(k = 0;k < num_st;k++){
			if(j  >= NUM_REGISTER){
				j = 0;
			}
			fprintf(file,"\t\t\"%s %%%%%s%d, %d(%%%%rax)\\n\\t\\t\"\n", assembly_op, REGISTER, j, offset);
			j++;
			offset += align;
		}
	}
	fprintf(file,"\t\t\"subq $1, %%%%r8\\n\\t\\t\"\n");
	fprintf(file,"\t\t\"jnz Loop2_%%=\\n\\t\\t\"\n");
	
	
	//End Test Function
	fprintf(file,"\t\t:\n\t\t:\"r\"(num_reps_t),\"r\" (test_var)\n\t\t:\"%%rax\",\"%%rdi\",\"%%r8\","COBLERED"\n\t);\n");
#endif
	fprintf(file,"}\n\n");
	
	fclose(file_header);
}	
