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
#include "cpu.h"
#include "io.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "dram.h"
#include "spr.h"

#define print_spr(spr)		\
	puts(#spr);		\
	puts(" = 0x");		\
	put_hex(mfspr(spr));	\
	puts("\n");

extern volatile unsigned int illegal_instruction;

void putchar(char c)
{
	uart0_putc(c);
}

void puts(char *str)
{
	uart0_puts(str);
}

static char nibble_to_hex(unsigned char byte)
{
	switch (byte) {
	default:
	case 0x0:
		return '0';
	case 0x1:
		return '1';
	case 0x2:
		return '2';
	case 0x3:
		return '3';
	case 0x4:
		return '4';
	case 0x5:
		return '5';
	case 0x6:
		return '6';
	case 0x7:
		return '7';
	case 0x8:
		return '8';
	case 0x9:
		return '9';
	case 0xa:
		return 'a';
	case 0xb:
		return 'b';
	case 0xc:
		return 'c';
	case 0xd:
		return 'd';
	case 0xe:
		return 'e';
	case 0xf:
		return 'f';
	}
}

static void put_hex(unsigned int hex)
{
	int i;

	for (i = 0; i < 8; i++) {
		uart0_putc(nibble_to_hex((hex & 0xf0000000)>>28));
		hex <<= 4;
	}
}

#define BUF_LEN 12
static void put_uint(unsigned int value)
{
	char buf[BUF_LEN];
	char *p = &buf[BUF_LEN-1];

	*p = '\0';

	while (value) {
		*--p = '0' + value%10;
		value /= 10;
	}

	puts(p);
}

void gpio_init()
{
	/* setup UART0 to PORTF */
	/* disable GPH20,21 as uart0 tx,rx to avoid conflict */
	gpio_direction_input(SUNXI_GPH(20));
	gpio_direction_input(SUNXI_GPH(21));
	/* Setup GPF2,4 as uart0 tx,rx */
	sunxi_gpio_set_cfgpin(SUNXI_GPF(2), SUNXI_GPF2_UART0_TX);
	sunxi_gpio_set_cfgpin(SUNXI_GPF(4), SUNXI_GPF4_UART0_RX);
}

/* Write a couple of words into the DRAM and read them back */
void test_dram(void)
{
	int i;
	volatile unsigned int *dram = (volatile unsigned int *)0x40000000;

	for (i = 0; i < 1024; i++)
		dram[i] = i;

	for (i = 0; i < 1024; i++) {
		if (dram[i] != i) {
			puts("FAIL\n");
			return;
		}

	}

	puts("OK\n");
}

void test_timer(void)
{
	unsigned int time0, time1;
	timer_enable();
	timer_reset_ticks();
	time0 = timer_get_ticks();
	time1 = timer_get_ticks();

	/* Check that the tick value has changed */
	if (time0 != time1)
		puts("OK\n");
	else
		puts("FAIL\n");
}

/*
 * Crude clock frequency detection routine.
 * Prints a string on the uart and measures the number of ticks (clock cycles)
 * that has elapsed.
 * The real time that has elapsed can roughly be calculated as:
 * time = num_chars * bits_per_char / baud_rate
 * and the clock frequency can be calculated as:
 * freq = ticks / time
 */
void test_clk_freq(void)
{
	unsigned int time0, time1;
	unsigned int clk_freq;

	timer_enable();
	timer_reset_ticks();

	time0 = timer_get_ticks();
	puts("Test CLK freq...");
	time1 = timer_get_ticks();

	clk_freq = (time1 - time0)*115200/(10*16);
	clk_freq /= 1e6;

	put_uint(clk_freq);
	puts(" MHz\n");
}

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

int main(void)
{
	unsigned int upr = mfspr(SPR_UPR);
	unsigned int vr = mfspr(SPR_VR);
	unsigned int iccfgr = mfspr(SPR_ICCFGR);
	unsigned int dccfgr = mfspr(SPR_DCCFGR);
	unsigned int immucfgr = mfspr(SPR_IMMUCFGR);
	unsigned int dmmucfgr = mfspr(SPR_DMMUCFGR);
	unsigned int cpucfgr = mfspr(SPR_CPUCFGR);
	unsigned int ver = (vr & SPR_VR_VER) >> 24;
	unsigned int rev = vr & SPR_VR_REV;
	unsigned int block_size;
	unsigned int set_size;
	unsigned int ways;

	gpio_init();
	uart0_init();

	puts("\n");

	puts("OpenRISC-");
	putchar(nibble_to_hex((char)(ver >> 4)));
	putchar(nibble_to_hex((char)ver & 0xf));
	puts("00 (rev ");
	put_uint(rev);
	puts(")\n");

	if (upr & SPR_UPR_DCP) {
		block_size = (dccfgr & SPR_DCCFGR_CBS) ? 32 : 16;
		ways = 1 << (dccfgr & SPR_DCCFGR_NCW);
		set_size = 1 << ((dccfgr & SPR_DCCFGR_NCS) >> 3);
		puts("D-Cache: ");
		put_uint(block_size*set_size*ways);
		puts(" bytes, ");
		put_uint(block_size);
		puts(" bytes/line, ");
		put_uint(ways);
		puts(" way(s)\n");
	} else {
		puts("D-Cache: no\n");
	}

	if (upr & SPR_UPR_ICP) {
		block_size = (iccfgr & SPR_ICCFGR_CBS) ? 32 : 16;
		ways = 1 << (iccfgr & SPR_ICCFGR_NCW);
		set_size = 1 << ((iccfgr & SPR_ICCFGR_NCS) >> 3);
		puts("I-Cache: ");
		put_uint(block_size*set_size*ways);
		puts(" bytes, ");
		put_uint(block_size);
		puts(" bytes/line, ");
		put_uint(ways);
		puts(" way(s)\n");
	} else {
		puts("I-Cache: no\n");
	}

	if (upr & SPR_UPR_DMP) {
		set_size = 1 << ((dmmucfgr & SPR_DMMUCFGR_NTS) >> 2);
		ways = (dmmucfgr & SPR_DMMUCFGR_NTW) + 1;
		puts("DMMU: ");
		put_uint(set_size);
		puts(" sets, ");
		put_uint(ways);
		puts(" way(s)\n");
	} else {
		puts("DMMU: no\n");
	}

	if (upr & SPR_UPR_IMP) {
		set_size = 1 << ((immucfgr & SPR_IMMUCFGR_NTS) >> 2);
		ways = (immucfgr & SPR_IMMUCFGR_NTW) + 1;
		puts("IMMU: ");
		put_uint(set_size);
		puts(" sets, ");
		put_uint(ways);
		puts(" way(s)\n");
	} else {
		puts("IMMU: no\n");
	}

	puts("MAC unit: ");
	(upr & SPR_UPR_MP) ? puts("yes\n") : puts("no\n");
	puts("Debug unit: ");
	(upr & SPR_UPR_DUP) ? puts("yes\n") : puts("no\n");
	puts("Performance counters: ");
	(upr & SPR_UPR_PCUP) ? puts("yes\n") : puts("no\n");
	puts("Power management: ");
	(upr & SPR_UPR_PMP) ? puts("yes\n") : puts("no\n");
	puts("Interrupt controller: ");
	(upr & SPR_UPR_PICP) ? puts("yes\n") : puts("no\n");
	puts("Timer: ");
	(upr & SPR_UPR_TTP) ? puts("yes\n") : puts("no\n");
	puts("Custom unit(s): ");
	(upr & SPR_UPR_CUP) ? puts("yes\n") : puts("no\n");

	puts("ORBIS32: ");
	(cpucfgr & SPR_CPUCFGR_OB32S) ? puts("yes\n") : puts("no\n");
	puts("ORBIS64: ");
	(cpucfgr & SPR_CPUCFGR_OB64S) ? puts("yes\n") : puts("no\n");
	puts("ORFPX32: ");
	(cpucfgr & SPR_CPUCFGR_OF32S) ? puts("yes\n") : puts("no\n");
	puts("ORFPX64: ");
	(cpucfgr & SPR_CPUCFGR_OF64S) ? puts("yes\n") : puts("no\n");

	puts("Test support for l.addc...");
	test_addc() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cmov...");
	test_cmov() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cust1...");
	test_cust1() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cust2...");
	test_cust2() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cust3...");
	test_cust3() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cust4...");
	test_cust4() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cust5...");
	test_cust5() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cust6...");
	test_cust6() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cust7...");
	test_cust7() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.cust8...");
	test_cust8() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.div...");
	test_div() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.divu...");
	test_divu() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.extbs...");
	test_extbs() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.extbz...");
	test_extbz() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.exths...");
	test_exths() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.exthz...");
	test_exthz() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.extws...");
	test_extws() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.extwz...");
	test_extwz() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.ff1...");
	test_ff1() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.fl1...");
	test_fl1() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.lws...");
	test_lws() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.mac...");
	test_mac() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.maci...");
	test_maci() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.macrc...");
	test_macrc() ? puts("yes\n") : puts("no\n");
/*
	puts("Test support for l.macu...");
	test_macu() ? puts("yes\n") : puts("no\n");
*/
	puts("Test support for l.mul...");
	test_mul() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.muli...");
	test_muli() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.mulu...");
	test_mulu() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.ror...");
	test_ror() ? puts("yes\n") : puts("no\n");
	puts("Test support for l.rori...");
	test_rori() ? puts("yes\n") : puts("no\n");

	puts("Init DRAM...");
	mctl_init();
	puts("done\n");
	puts("Test DRAM read/write...");
	test_dram();

	puts("Test timer functionality...");
	test_timer();

	test_clk_freq();

	puts("done\n");
	for (;;);
}
