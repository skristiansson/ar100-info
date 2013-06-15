#ifndef _SPR_H
#define _SPR_H
#include "spr-defs.h"

static inline unsigned long mfspr(unsigned long add)
{
	unsigned long ret;

	__asm__ __volatile__ ("l.mfspr %0,r0,%1" : "=r" (ret) : "K" (add));

	return ret;
}

static inline void mtspr(unsigned long add, unsigned long val)
{
	__asm__ __volatile__ ("l.mtspr r0,%1,%0" : : "K" (add), "r" (val));
}
#endif
