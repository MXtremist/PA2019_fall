#ifndef _INSTR_MYINTR_H_
#define _INSTR_MYINTR_H_

make_instr_func(int_i_b);
make_instr_func(lidt);
make_instr_func(cli);
make_instr_func(sti);
make_instr_func(pusha_v);
make_instr_func(popa_v);
make_instr_func(iret);

#endif
