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

#include <or1k-support.h>
#include "cpu.h"
#include "io.h"
#include "gpio.h"
#include "uart.h"
#include "timer.h"
#include "dram.h"
#include "clock.h"
#include "spr.h"
#include "instruction_tests.h"

#define print_spr(spr)		\
	puts(#spr);		\
	puts(" = 0x");		\
	put_hex(mfspr(spr));	\
	puts("\n");

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

	if (!value)
		*--p = '0';

	while (value) {
		*--p = '0' + value%10;
		value /= 10;
	}

	puts(p);
}

/* Present as floating point with one decimal digit in the fractional part */
static void put_uint_div_by_10(unsigned int value)
{
	if (value >= 10)
		put_uint(value / 10);
	else
		puts("0");
	puts(".");
	put_uint(value % 10);
}

#define SRAM_VER_REG	(AW_SRAMCTRL_BASE + 0x24)

void soc_detection_init(void)
{
	set_wbit(SRAM_VER_REG, 1 << 15);
}

int soc_is_a31(void)
{
	return (readl(SRAM_VER_REG) >> 16) == 0x1633;
}

int soc_is_h3(void)
{
	return (readl(SRAM_VER_REG) >> 16) == 0x1680;
}

int dram_is_clocked(void)
{
    return readl(CCU_PLL5CFG) & (1 << 31);
}

void gpio_init()
{
	if (soc_is_h3()) {
		sunxi_gpio_set_cfgpin(SUNXI_GPA(4), SUN8I_H3_GPA_UART0);
		sunxi_gpio_set_cfgpin(SUNXI_GPA(5), SUN8I_H3_GPA_UART0);
	} else {
		/* setup UART0 to PORTF */
		/* disable GPH20,21 as uart0 tx,rx to avoid conflict */
		gpio_direction_input(SUNXI_GPH(20));
		gpio_direction_input(SUNXI_GPH(21));
		/* Setup GPF2,4 as uart0 tx,rx */
		sunxi_gpio_set_cfgpin(SUNXI_GPF(2), SUNXI_GPF2_UART0_TX);
		sunxi_gpio_set_cfgpin(SUNXI_GPF(4), SUNXI_GPF4_UART0_RX);
	}
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
 * Clock frequency detection routine.
 * Compares the internal AR100 timer with the AVS0 timer.
 */
#define TMR_AVS0_VAL	(AW_TIMER_BASE + 0x0084)
#define TMR_AVS_DIV	(AW_TIMER_BASE + 0x008C)
#define AVS_CLK		(AW_CCM_BASE   + 0x0144)
#define AVS_CNT_CTL	(AW_TIMER_BASE + 0x0080)
void test_clk_freq(void)
{
	int i;
	unsigned int tmr_avs0_val0, tmr_avs0_val1;
	unsigned int tmr_avs0_div;
	unsigned int time0, time1;
	unsigned int delta;
	unsigned int clk_freq;

	puts("Test CLK freq...");
	timer_enable();
	timer_reset_ticks();

	set_wbit(AVS_CLK, 1 << 31);
	set_wbit(AVS_CNT_CTL, 1);

	tmr_avs0_div = (readl(TMR_AVS_DIV) & 0x7ff) + 1;

	/*
	 * AVS0 reads high 32 bits of a 33-bit counter, so effectively we are
	 * dealing with a counter, which runs at (12 MHz / div) clock speed.
	 */
	tmr_avs0_val0 = readl(TMR_AVS0_VAL);
	time0 = timer_get_ticks();
	do {
		tmr_avs0_val1 = readl(TMR_AVS0_VAL);
		if (tmr_avs0_val1 >= tmr_avs0_val0)
			delta = tmr_avs0_val1 - tmr_avs0_val0;
		else
			delta = 0xffffffff - (tmr_avs0_val0 - tmr_avs0_val1);
	} while (delta < 1000);

	time1 = timer_get_ticks();
	delta = time1 - time0;

	clk_freq = (uint64_t)delta * 12000 / tmr_avs0_div;
	clk_freq = (clk_freq + 500) / 1000;

	if (clk_freq < 1000) {
		put_uint(clk_freq);
		puts(" KHz\n");
	} else {
		clk_freq = (clk_freq + 500) / 1000;
		put_uint(clk_freq);
		puts(" MHz\n");
	}
}

void pointer_chasing(int count, void **addr)
{
	void *old = *addr;
	*addr = addr;
	count = (count + 25) / 50;
	while (count--) {
		asm volatile(".rept 50\n"
			     "    l.lwz %0, 0(%0)\n"
			     ".endr\n"
			     : "+r" (addr));
	}
	*addr = old;
}

void pointer_chasing_plus_nop(int count, void **addr)
{
	void *old = *addr;
	*addr = addr;
	count = (count + 25) / 50;
	while (count--) {
		asm volatile(".rept 50\n"
			     "    l.lwz %0, 0(%0)\n"
			     "    l.nop\n"
			     ".endr\n"
			     : "+r" (addr));
	}
	*addr = old;
}

void unrolled_nops(int count)
{
	count = (count + 25) / 50;
	while (count--) {
		asm volatile(".rept 50\n"
			     "    l.nop\n"
			     ".endr\n");
	}
}

void empty_loop(int count)
{
	count = (count + 25) / 50;
	while (count--) {
		asm volatile("");
	}
}

void enable_caches(void)
{
	unsigned addr;
	/* Flush I-cache for the whole SRAM A2 (cargo cult?) */
	for (addr = 0; addr < 16 * 1024 + 32 * 1024; addr += 16)
		or1k_icache_flush(addr);
	or1k_icache_enable();
}

void disable_caches(void)
{
	or1k_icache_disable();
}

void test_mem_latency(char *label, void **addr)
{
	int count = 1000000;
	unsigned t1, t2, t3, t4;
	unsigned loop_overhead;

	timer_enable();
	timer_reset_ticks();

	t1 = timer_get_ticks();
	pointer_chasing(count, addr);
	t2 = timer_get_ticks();
	pointer_chasing_plus_nop(count, addr);
	t3 = timer_get_ticks();
	unrolled_nops(count);
	t4 = timer_get_ticks();
	empty_loop(count);
	loop_overhead = timer_get_ticks() - t4;

	puts(label);

	puts("\n");
	puts("      Instructions fetch : ");
	put_uint_div_by_10((t4 - t3 - loop_overhead) * 10 / count);
	puts(" cycles per instruction\n");

	puts("      Back-to-back L.LWZ : ");
	put_uint_div_by_10((t2 - t1 - loop_overhead) * 10 / count);
	puts(" cycles per 32-bit read\n");

	puts("           L.LWZ + L.NOP : ");
	put_uint_div_by_10((t3 - t2 - loop_overhead) * 10 / count);
	puts(" cycles per 32-bit read\n");
}

void benchmark(void)
{
	void *dummy;

	enable_caches();
	puts("\n  == Benchmark ==\n");
	test_mem_latency("   == Code in SRAM A2 (I-cache ON), data in SRAM A2 ==",
			 &dummy);
	test_mem_latency("   == Code in SRAM A2 (I-cache ON), data in SRAM A1 ==",
			 (void *)0x40000);
	if (dram_is_clocked()) {
		test_mem_latency(
			 "   == Code in SRAM A2 (I-cache ON), data in DRAM ==",
			 (void *)0x40000000);
	}

	disable_caches();
	puts("\n");
	test_mem_latency("   == Code in SRAM A2 (I-cache OFF), data in SRAM A2 ==",
			 &dummy);
	test_mem_latency("   == Code in SRAM A2 (I-cache OFF), data in SRAM A1 ==",
			 (void *)0x40000);
	if (dram_is_clocked()) {
		test_mem_latency(
			 "   == Code in SRAM A2 (I-cache OFF), data in DRAM ==",
			 (void *)0x40000000);
	}

	puts("\n");
}


void test_clk_config(void)
{
	unsigned int ar100_clkcfg_reg;
	unsigned int old_ar100_clkcfg_reg;
	unsigned int pll6_ctrl_reg;
	unsigned int old_pll6_ctrl_reg;

	puts("Set clock source to LOSC...");
	old_ar100_clkcfg_reg = ar100_clkcfg_reg = readl(AR100_CLKCFG_REG);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_SRC_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_SRC_LOSC;
	writel(ar100_clkcfg_reg, AR100_CLKCFG_REG);
	puts("done\n");

	test_clk_freq();

	puts("Set clock source to HOSC (POSTDIV=0, DIV=0)...");
	ar100_clkcfg_reg = readl(AR100_CLKCFG_REG);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_SRC_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_SRC_HOSC;
	ar100_clkcfg_reg &= ~AR100_CLKCFG_POSTDIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_POSTDIV(0);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_DIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_DIV(0);
	writel(ar100_clkcfg_reg, AR100_CLKCFG_REG);
	puts("done\n");

	test_clk_freq();

	puts("Set clock source to HOSC (POSTDIV=0, DIV=1)...");
	ar100_clkcfg_reg = readl(AR100_CLKCFG_REG);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_SRC_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_SRC_HOSC;
	ar100_clkcfg_reg &= ~AR100_CLKCFG_POSTDIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_POSTDIV(0);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_DIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_DIV(1);
	writel(ar100_clkcfg_reg, AR100_CLKCFG_REG);
	puts("done\n");

	test_clk_freq();

	puts("Set clock source to HOSC (POSTDIV=1, DIV=1)...");
	ar100_clkcfg_reg = readl(AR100_CLKCFG_REG);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_SRC_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_SRC_HOSC;
	ar100_clkcfg_reg &= ~AR100_CLKCFG_POSTDIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_POSTDIV(1);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_DIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_DIV(1);
	writel(ar100_clkcfg_reg, AR100_CLKCFG_REG);
	puts("done\n");

	test_clk_freq();

	puts("Setup PLL6 (M=1, K=1, N=24)...");
	old_pll6_ctrl_reg = pll6_ctrl_reg = readl(PLL6_CTRL_REG);
	pll6_ctrl_reg &= ~PLL6_CTRL_M_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_M(1);
	pll6_ctrl_reg &= ~PLL6_CTRL_K_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_K(1);
	pll6_ctrl_reg &= ~PLL6_CTRL_N_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_N(24);
	pll6_ctrl_reg |= (PLL6_CTRL_ENABLE | PLL6_CTRL_CLK_OUTEN);
	writel(pll6_ctrl_reg, PLL6_CTRL_REG);
	puts("done\n");

	puts("Set clock source to PLL6 (POSTDIV=1, DIV=0)...");
	ar100_clkcfg_reg = readl(AR100_CLKCFG_REG);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_SRC_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_SRC_PLL6;
	ar100_clkcfg_reg &= ~AR100_CLKCFG_POSTDIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_POSTDIV(1);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_DIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_DIV(0);
	writel(ar100_clkcfg_reg, AR100_CLKCFG_REG);
	puts("done\n");

	test_clk_freq();
	benchmark();

	puts("Set clock source to PLL6 (POSTDIV=2, DIV=0)...");
	ar100_clkcfg_reg = readl(AR100_CLKCFG_REG);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_SRC_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_SRC_PLL6;
	ar100_clkcfg_reg &= ~AR100_CLKCFG_POSTDIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_POSTDIV(2);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_DIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_DIV(0);
	writel(ar100_clkcfg_reg, AR100_CLKCFG_REG);
	puts("done\n");

	test_clk_freq();

	puts("Set clock source to PLL6 (POSTDIV=2, DIV=1)...");
	ar100_clkcfg_reg = readl(AR100_CLKCFG_REG);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_SRC_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_SRC_PLL6;
	ar100_clkcfg_reg &= ~AR100_CLKCFG_POSTDIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_POSTDIV(2);
	ar100_clkcfg_reg &= ~AR100_CLKCFG_DIV_MASK;
	ar100_clkcfg_reg |= AR100_CLKCFG_DIV(1);
	writel(ar100_clkcfg_reg, AR100_CLKCFG_REG);
	puts("done\n");

	test_clk_freq();

	puts("Setup PLL6 (M=2, K=1, N=24)...");
	pll6_ctrl_reg &= ~PLL6_CTRL_M_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_M(2);
	pll6_ctrl_reg &= ~PLL6_CTRL_K_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_K(1);
	pll6_ctrl_reg &= ~PLL6_CTRL_N_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_N(24);
	writel(pll6_ctrl_reg, PLL6_CTRL_REG);
	puts("done\n");

	test_clk_freq();

	puts("Setup PLL6 (M=1, K=2, N=24)...");
	pll6_ctrl_reg &= ~PLL6_CTRL_M_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_M(1);
	pll6_ctrl_reg &= ~PLL6_CTRL_K_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_K(2);
	pll6_ctrl_reg &= ~PLL6_CTRL_N_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_N(24);
	writel(pll6_ctrl_reg, PLL6_CTRL_REG);
	puts("done\n");

	test_clk_freq();

	puts("Setup PLL6 (M=1, K=1, N=12)...");
	pll6_ctrl_reg &= ~PLL6_CTRL_M_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_M(1);
	pll6_ctrl_reg &= ~PLL6_CTRL_K_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_K(1);
	pll6_ctrl_reg &= ~PLL6_CTRL_N_MASK;
	pll6_ctrl_reg |= PLL6_CTRL_N(12);
	writel(pll6_ctrl_reg, PLL6_CTRL_REG);
	puts("done\n");

	test_clk_freq();

	/* restore regs */
	puts("Restore clock config to original state...");
	writel(old_ar100_clkcfg_reg, AR100_CLKCFG_REG);
	writel(old_pll6_ctrl_reg, PLL6_CTRL_REG);
	puts("done\n");
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

	soc_detection_init();
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

	if (soc_is_a31() && !dram_is_clocked()) {
		puts("Init DRAM...");
		mctl_init();
		puts("done\n");
	}

	if (dram_is_clocked()) {
		puts("Test DRAM read/write...");
		test_dram();
	}

	puts("Test timer functionality...");
	test_timer();

	test_clk_config();

	puts("All tests completed!\n");
	for (;;);
}
