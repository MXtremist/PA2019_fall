#include "cpu/instr.h"

static void instr_execute_2op() 
{
    operand_read(&opr_src);
    operand_read(&opr_dest);
	opr_dest.val = alu_and(opr_src.val, opr_dest.val, opr_dest.data_size);
	operand_write(&opr_dest);
}

make_instr_impl_2op(and, i, rm, b)
make_instr_impl_2op(and, i, rm, v)
make_instr_impl_2op(and, r, rm, b)
make_instr_impl_2op(and, r, rm, v)
make_instr_impl_2op(and, rm, r, b)
make_instr_impl_2op(and, rm, r, v)

make_instr_func(and_i2eax_b)
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

    myeax.val = alu_and(imm.val, myeax.val, myeax.data_size);
    operand_write(&myeax);

    print_asm_2("and", "", 2, &imm, &myeax);
    return 2;
}

make_instr_func(and_i2eax_v)
{
    int len = 1;
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

    myeax.val = alu_and(imm.val, myeax.val, myeax.data_size);
    operand_write(&myeax);

    len += data_size / 8;
    print_asm_2("and", "", len + 1, &imm, &myeax);
    return len;
}

make_instr_func(and_si2rm_v)
{
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
	rm.val = alu_and(tmp, rm.val, rm.data_size);
    operand_write(&rm);
    print_asm_2("and", "", len+1, &imm, &rm);
    //The value of imm is before sign_ext
    //printf("%d", cpu.eflags.ZF);
	return len + 1;
}
