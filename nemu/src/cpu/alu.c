#include "cpu/cpu.h"
// CF contains information relevant to unsigned integers
void set_CF_add(uint32_t result, uint32_t src, size_t data_size) {	
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = result < src;
}

void set_CF_adc(uint32_t result, uint32_t src, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF ==1)
		cpu.eflags.CF = result <= src;
	else
		cpu.eflags.CF = result < src;
}

void set_CF_sub(uint32_t dest, uint32_t src, size_t data_size) {	
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.CF = dest < src;
}

void set_CF_sbb(uint32_t dest, uint32_t src, size_t data_size) {	
	dest = sign_ext(dest & (0xFFFFFFFF >> (32 - data_size)), data_size);
	src = sign_ext(src & (0xFFFFFFFF >> (32 - data_size)), data_size);
	if(cpu.eflags.CF == 1 )
		cpu.eflags.CF = dest <= src;
	else
		cpu.eflags.CF = dest < src;
}


void set_ZF(uint32_t result, size_t data_size) {
	result = result & (0xFFFFFFFF >> (32 - data_size));
	cpu.eflags.ZF = (result == 0);
}

// SF and OF contain information relevant to signed integers
void set_SF(uint32_t result, size_t data_size) {
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	cpu.eflags.SF = sign(result);
}

void set_PF(uint32_t result, size_t data_size){
	result = sign_ext(result & (0xFFFFFFFF >> (32 - data_size)), data_size);
	int numof1 = 0;
	for(int i = 0; i < 8; i++)
	{
		if((result & 1 )!= 0)
			numof1++;
		result /= 2;
	}
	cpu.eflags.PF = ((numof1 & 1 )== 0);
} 

void set_OF_add(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
	case 8:
		result = sign_ext(result & 0xFF, 8);
		src = sign_ext(src & 0xFF, 8);
		dest = sign_ext(dest & 0xFF, 8);
		break;
	case 16:
		result = sign_ext(result & 0xFFFF, 16);
		src = sign_ext(src & 0xFFFF, 16);
		dest = sign_ext(dest & 0xFFFF, 16);
		break;
	default: break;// do nothing
	}
	if(sign(src) == sign(dest)) {
		if(sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}
       	else {
		cpu.eflags.OF = 0;
	}
}


void set_OF_adc(uint32_t result, uint32_t src, uint32_t dest, size_t data_size)
{
	if(cpu.eflags.CF == 0)
		set_OF_add(result,src,dest,data_size);
	else
	{	switch(data_size) {
	case 8:
		result = sign_ext(result & 0xFF, 8);
		src = sign_ext(src & 0xFF, 8);
		dest = sign_ext(dest & 0xFF, 8);
		break;
	case 16:
		result = sign_ext(result & 0xFFFF, 16);
		src = sign_ext(src & 0xFFFF, 16);
		dest = sign_ext(dest & 0xFFFF, 16);
		break;
	default: break;// do nothing
	}
	if(sign(src) == sign(dest)) {
		if(sign(src) != sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}
       	else {
		cpu.eflags.OF = 0;
	}
	}

}

void set_OF_sub(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
	case 8:
		result = sign_ext(result & 0xFF, 8);
		src = sign_ext(src & 0xFF, 8);
		dest = sign_ext(dest & 0xFF, 8);
		break;
	case 16:
		result = sign_ext(result & 0xFFFF, 16);
		src = sign_ext(src & 0xFFFF, 16);
		dest = sign_ext(dest & 0xFFFF, 16);
		break;
	default: break;// do nothing
	}
	if(sign(src) != sign(dest)) {
		if(sign(src) == sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}
       	else {
		cpu.eflags.OF = 0;
	}
}

void set_OF_sbb(uint32_t result, uint32_t src, uint32_t dest, size_t data_size) {
	switch(data_size) {
	case 8:
		result = sign_ext(result & 0xFF, 8);
		src = sign_ext(src & 0xFF, 8);
		dest = sign_ext(dest & 0xFF, 8);
		break;
	case 16:
		result = sign_ext(result & 0xFFFF, 16);
		src = sign_ext(src & 0xFFFF, 16);
		dest = sign_ext(dest & 0xFFFF, 16);
		break;
	default: break;// do nothing
	}
	if(sign(src) != sign(dest)) {
		if(sign(src) == sign(result))
			cpu.eflags.OF = 1;
		else
			cpu.eflags.OF = 0;
	}
       	else {
		cpu.eflags.OF = 0;
	}
}




uint32_t alu_add(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_add(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src;
	set_CF_add(res, src, data_size);
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_add(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_adc(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_adc(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest + src + cpu.eflags.CF;
	set_CF_adc(res, src, data_size);
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_adc(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sub(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sub(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - src;
	set_CF_sub(dest, src, data_size);
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sub(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_sbb(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sbb(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest - src - cpu.eflags.CF;
	set_CF_sbb(dest, src, data_size);
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	set_OF_sbb(res, src, dest, data_size);
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint64_t alu_mul(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mul(src, dest, data_size);
#else
	uint64_t res = 0;
	res = (uint64_t)dest * src;
	if((res >> data_size) == 0)
	{	cpu.eflags.CF = 0;
       		cpu.eflags.OF = 0;}
	else
	{	cpu.eflags.CF = 1;
      		cpu.eflags.OF = 1;}
	return res & (0xFFFFFFFFFFFFFFFF >> (64 - data_size * 2));
#endif
}

int64_t alu_imul(int32_t src, int32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imul(src, dest, data_size);
#else
	int64_t res = 0;
	res = (int64_t)dest * src;
	if((res >> 63)==0)
	{
		if((res>>data_size) == 0)
		{	cpu.eflags.CF = 0;
			cpu.eflags.OF = 0;}
		else
		{	cpu.eflags.CF = 1;
			cpu.eflags.OF = 1;}
	}
	else
	{
		if((res >> data_size) == (0xFFFFFFFFFFFFFFFF >> (64 - data_size)))
		{	cpu.eflags.CF = 0;
		       	cpu.eflags.OF = 0;}
		else
		{	cpu.eflags.CF = 1;
			cpu.eflags.OF = 1;}
	}
	return res;
#endif
}

// need to implement alu_mod before testing
uint32_t alu_div(uint64_t src, uint64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_div(src, dest, data_size);
#else
	if(src == 0)
	{
		printf("\e[0;31mDivisor can not be ZERO!\e[0m\n");
		assert(0);
		return 0;
	}
	else
	{
		uint32_t res = dest / src;
		return res & (0xFFFFFFFF >> (32 - data_size));
	}
#endif
}

// need to implement alu_imod before testing
int32_t alu_idiv(int64_t src, int64_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_idiv(src, dest, data_size);
#else
	if(src == 0)
	{
		printf("\e[0;31mDivisor can not be ZERO!\e[0m\n");
		assert(0);
		return 0;
	}
	else
	{
		int32_t res = dest / src;
		return res;
	}
#endif
}

uint32_t alu_mod(uint64_t src, uint64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_mod(src, dest);
#else
	if(src == 0)
	{
		printf("\e[0;31mDivisor can not be ZERO!\e[0m\n");
		assert(0);
		return 0;
	}
	else
	{
		uint32_t res = dest % src;
		return res;
	}
#endif
}

int32_t alu_imod(int64_t src, int64_t dest)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_imod(src, dest);
#else
	if(src == 0)
	{
		printf("\e[0;31mDivisor can not be ZERO!\e[0m\n");
		assert(0);
		return 0;
	}
	else
	{
		int32_t res = dest % src;
		return res;
	}
#endif
}

uint32_t alu_and(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_and(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest & src;
	cpu.eflags.CF = 0;
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_xor(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_xor(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest ^ src;
	cpu.eflags.CF = 0;
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_or(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_or(src, dest, data_size);
#else
	uint32_t res = 0;
	res = dest | src;
	cpu.eflags.CF = 0;
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
	cpu.eflags.OF = 0;
	return res & (0xFFFFFFFF >> (32 - data_size));
#endif
}

uint32_t alu_shl(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shl(src, dest, data_size);
#else
	uint32_t res = dest & (0xFFFFFFFF >> (32 - data_size));
	int mysign = data_size - 1;
    for(int i = 0; i<src; i++)
    {
        cpu.eflags.CF = res >> mysign;
        res <<= 1;
    }
    res = res & (0xFFFFFFFF >> (32 - data_size));
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);

	return res;

#endif
}

uint32_t alu_shr(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_shr(src, dest, data_size);
#else
	uint32_t res = dest & (0xFFFFFFFF >> (32 - data_size));
	int mysign = data_size - 1;
    for(int i = 0; i<src; i++)
    {
        cpu.eflags.CF = res >> mysign;
        res >>= 1;
    }
    res = res & (0xFFFFFFFF >> (32 - data_size));
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);

	return res;

#endif
}

uint32_t alu_sar(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sar(src, dest, data_size);
#else
	uint32_t res = dest & (0xFFFFFFFF >> (32 - data_size));
	int mysign = data_size - 1;
	for(int i = 0; i < src; i++)
    {
        cpu.eflags.CF = (res << 31) >> 31;
        uint32_t tmp = 1 << mysign;
        res >>= 1;
        if((res >> (mysign - 1)) == 1) res += tmp;
    }
	res = res & (0xFFFFFFFF >> (32 - data_size));
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);

	return res;


#endif
}

uint32_t alu_sal(uint32_t src, uint32_t dest, size_t data_size)
{
#ifdef NEMU_REF_ALU
	return __ref_alu_sal(src, dest, data_size);
#else
	uint32_t res = dest & (0xFFFFFFFF >> (32 - data_size));
	uint32_t res_t = res;
    int mysign = data_size - 1;
    for(int i = 0; i < src; ++i)
    {
        cpu.eflags.CF = res >> mysign;
        res <<= 1;
    }
	res = res & (0xFFFFFFFF >> (32 - data_size));
	set_PF(res, data_size);
	set_ZF(res, data_size);
	set_SF(res, data_size);
    if(data_size == 1)
    {
        cpu.eflags.OF = (res >> mysign) == (res_t >> mysign);
    }

	return res ;

#endif
}
