#include "cpu/instr.h"

static void instr_execute_2op()
{
	operand_read(&opr_src);
	operand_read(&opr_dest);
	alu_sub(opr_src.val, opr_dest.val, opr_dest.data_size);
	//Only change flags
}

make_instr_impl_2op(cmp, r, rm, b)
make_instr_impl_2op(cmp, r, rm, v)
make_instr_impl_2op(cmp, rm, r, b)
make_instr_impl_2op(cmp, rm, r, v)
make_instr_impl_2op(cmp, i, rm, b)
make_instr_impl_2op(cmp, i, rm, v)

make_instr_func(cmp_si2rm_v)
{
    //printf("%x\n", eip);
	int len = 1;
	OPERAND rm, imm;
	rm.data_size  = data_size;
	len += modrm_rm(eip + 1, &rm);
    operand_read(&rm);

	imm.data_size = 8;
	imm.type = OPR_IMM;
	imm.addr = eip + len;

	operand_read(&imm);

    uint32_t tmp = sign_ext(imm.val, imm.data_size);
   // printf("%d %d", imm.val, rm.val);
	alu_sub(tmp, rm.val, rm.data_size);
    print_asm_2("cmp", "", len+1, &imm, &rm);
     //The value of imm is before sign_ext
    //printf("%d", cpu.eflags.ZF);
	return len + 1;
}

make_instr_func(cmp_i2eax_b)
{
    OPERAND imm;
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = 8;
    operand_read(&imm);
    
    OPERAND myeax;
    myeax.type = OPR_REG;
    myeax.addr = REG_AL;
    myeax.data_size = 8;
    operand_read(&myeax);
    //printf("%x %x\n", imm.val, myeax.val);
    alu_sub(imm.val, myeax.val, myeax.data_size);

    print_asm_2("cmp", "", 2 , &imm, &myeax);
    return 2;
}

make_instr_func(cmp_i2eax_v)
{
    OPERAND imm;
    imm.type = OPR_IMM;
    imm.addr = eip + 1;
    imm.data_size = data_size;
    operand_read(&imm);
    
    OPERAND myeax;
    myeax.type = OPR_REG;
    myeax.addr = ((data_size == 32) ? REG_EAX : REG_AX);
    myeax.data_size = data_size;
    operand_read(&myeax);
    //printf("%x %x\n", imm.val, myeax.val);
    alu_sub(imm.val, myeax.val, myeax.data_size);

    print_asm_2("cmp", "", 2 , &imm, &myeax);
    return 1 + data_size / 8;
}
