#include "cpu/instr.h"

make_instr_func(leave)
{
    cpu.esp = cpu.ebp;
    OPERAND rel;
    rel.data_size = 32;
    rel.type = OPR_MEM;
    rel.addr = cpu.esp;
    rel.sreg = SREG_SS;
    operand_read(&rel);
    cpu.ebp = rel.val;
    cpu.esp += 4;
    print_asm_0("leave", "", 1);
    return 1;

}
