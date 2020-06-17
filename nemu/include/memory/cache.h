#ifndef _CACHE_H_
#define _CACHE_H_
#include "nemu.h"

struct CacheLine
{
    bool valid_bit;
    uint32_t tab;
    char data[64];
};

void init_cache(); 
uint32_t cache_read(paddr_t paddr, size_t len); 
void cache_write(paddr_t paddr, size_t len, uint32_t data); 

#endif
