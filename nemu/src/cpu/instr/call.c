#include "cpu/instr.h"

make_instr_func(call) {
        OPERAND rel;
        cpu.esp -= 4;
        rel.type = OPR_MEM;
        rel.data_size = data_size;
        rel.sreg = SREG_SS;
        rel.val = eip + 1 + data_size / 8;
        rel.addr = cpu.esp;
        operand_write(&rel);//push eip

        OPERAND addr;
        addr.type = OPR_IMM;
        addr.sreg = SREG_CS;
        addr.addr = eip + 1;
        addr.data_size = data_size;

        operand_read(&addr);

        int offset = sign_ext(addr.val, data_size);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("call", "", 1 + data_size / 8, &addr);

        cpu.eip += offset;

        return 1 + data_size / 8;
}

make_instr_func(call_near_indirect)
{
        int len = 1;
        OPERAND rel;
        cpu.esp -= 4;
        rel.type = OPR_MEM;
        rel.data_size = data_size;
        rel.sreg = SREG_SS;
        rel.val = eip + 2;
        rel.addr = cpu.esp;
        operand_write(&rel);//push eip

        OPERAND rel2;
        rel2.data_size = data_size;
        len += modrm_rm(eip+1, &rel2);
        operand_read(&rel2);

        int offset = sign_ext(rel2.val, data_size);
        //printf("0x%x\n", offset);
        // thank Ting Xu from CS'17 for finding this bug
        print_asm_1("call", "", 1 + data_size / 8 + offset, &rel2);

        cpu.eip = offset;
        //assert(0);
        return 0;
}
