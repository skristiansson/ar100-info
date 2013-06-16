/*
 * (C) Copyright 2013 Stefan Kristiansson <stefan.kristiansson@saunalahti.fi>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston,
 * MA 02111-1307 USA
 */
#ifndef _INSTRUCTION_TESTS_H
#define _INSTRUCTION_TESTS_H
extern volatile unsigned int illegal_instruction;

static int test_addc(void)
{
	volatile unsigned int ra = 1, rb = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.addc %0,%1,%2" : "=r" (rd) : "r" (ra), "r" (rb));

	return !illegal_instruction;
}

static int test_cmov(void)
{
	volatile unsigned int ra = 1, rb = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.cmov %0,%1,%2" : "=r" (rd) : "r" (ra), "r" (rb));

	return !illegal_instruction;
}

static int test_cust1(void)
{
	illegal_instruction = 0;
	asm volatile("l.cust1");
}

static int test_cust2(void)
{
	illegal_instruction = 0;
	asm volatile("l.cust2");
}

static int test_cust3(void)
{
	illegal_instruction = 0;
	asm volatile("l.cust3");
}

static int test_cust4(void)
{
	illegal_instruction = 0;
	asm volatile("l.cust4");
}

static int test_cust5(void)
{
	illegal_instruction = 0;
	asm volatile("l.cust5");
}

static int test_cust6(void)
{
	illegal_instruction = 0;
	asm volatile("l.cust6");
}

static int test_cust7(void)
{
	illegal_instruction = 0;
	asm volatile("l.cust7");
}

static int test_cust8(void)
{
	illegal_instruction = 0;
	asm volatile("l.cust8");
}

static int test_div(void)
{
	volatile unsigned int ra = 1, rb = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.div %0,%1,%2" : "=r" (rd) : "r" (ra), "r" (rb));

	return !illegal_instruction;
}

static int test_divu(void)
{
	volatile unsigned int ra = 1, rb = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.divu %0,%1,%2" : "=r" (rd) : "r" (ra), "r" (rb));

	return !illegal_instruction;
}

static int test_extbs(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.extbs %0,%1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_extbz(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.extbz %0,%1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_exths(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.exths %0,%1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_exthz(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.exthz %0,%1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_extws(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.exths %0,%1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_extwz(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.exthz %0,%1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_ff1(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.ff1 %0,%1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_fl1(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.fl1 %0,%1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_lws(void)
{
	volatile unsigned int ra = 0, rd;

	illegal_instruction = 0;
	asm volatile("l.lws %0,0(%1)" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_mac(void)
{
	volatile unsigned int ra = 1, rb = 1;

	illegal_instruction = 0;
	asm volatile("l.mac %0,%1" : : "r" (ra), "r" (rb));

	return !illegal_instruction;
}

static int test_maci(void)
{
	volatile unsigned int rd;

	illegal_instruction = 0;
	asm volatile("l.maci %0,1" : "=r" (rd));

	return !illegal_instruction;
}

static int test_macrc(void)
{
	volatile unsigned int rd;

	illegal_instruction = 0;
	asm volatile("l.macrc %0" : "=r" (rd));

	return !illegal_instruction;
}

static int test_macu(void)
{
	volatile unsigned int ra = 1, rb = 1;

	illegal_instruction = 0;
	asm volatile("l.macu %0,%1" : : "r" (ra), "r" (rb));

	return !illegal_instruction;
}


static int test_mul(void)
{
	volatile unsigned int ra = 1, rb = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.mul %0,%1,%2" : "=r" (rd) : "r" (ra), "r" (rb));

	return !illegal_instruction;
}

static int test_muli(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.muli %0,%1,1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}

static int test_mulu(void)
{
	volatile unsigned int ra = 1, rb = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.mulu %0,%1,%2" : "=r" (rd) : "r" (ra), "r" (rb));

	return !illegal_instruction;
}

static int test_ror(void)
{
	volatile unsigned int ra = 1, rb = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.ror %0,%1,%2" : "=r" (rd) : "r" (ra), "r" (rb));

	return !illegal_instruction;
}

static int test_rori(void)
{
	volatile unsigned int ra = 1, rd;

	illegal_instruction = 0;
	asm volatile("l.rori %0,%1,1" : "=r" (rd) : "r" (ra));

	return !illegal_instruction;
}
#endif
