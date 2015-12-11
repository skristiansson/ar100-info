/*
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * Early uart print for debugging.
 *
 * See file CREDITS for list of people who contributed to this
 * project.
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

#include "io.h"
#include "uart.h"
#include "clock.h"

#define CONFIG_CONS_INDEX 1

#define APB2_DIV	(AW_CCM_BASE + 0x058)
#define APB2_GATE	(AW_CCM_BASE + 0x06C)
#define APB2_RESET	(AW_CCM_BASE + 0x2D8)

void clock_init_uart(void)
{
	/* uart clock source is apb2 */
	writel(APB2_CLK_SRC_OSC24M|
	       APB2_CLK_RATE_N_1|
	       APB2_CLK_RATE_M(1),
	       APB2_GATE);

	/* open the clock for uart */
	set_wbit(APB2_GATE, 1 << (APB2_GATE_UART_SHIFT +
				  CONFIG_CONS_INDEX - 1));

	/* deassert uart reset */
	set_wbit(APB2_RESET, 1 << (APB2_RESET_UART_SHIFT +
				   CONFIG_CONS_INDEX - 1));
}

void uart0_init(void)
{
	clock_init_uart();

	/* select dll dlh */
	writel(0x80, UART0_LCR);
	/* set baudrate */
	writel(0, UART0_DLH);
	writel(BAUD_115200, UART0_DLL);
	/* set line control */
	writel(LC_8_N_1, UART0_LCR);
}

#define TX_READY (readl(UART0_LSR) & (1 << 6))
void uart0_putc(char c)
{
	while(!TX_READY)
		;
	writel(c, UART0_THR);
}

void uart0_puts(const char *s)
{
	while(*s) {
		if (*s == '\n')
			uart0_putc('\r');
		uart0_putc(*s++);
	}
}
