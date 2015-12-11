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
#ifndef _CLOCK_H
#define _CLOCK_H
#include "platform.h"

/*
 * PLL6 control register:
 * [31]    Enable
 * [30:29] Reserved
 * [28]    Lock flag
 * [27:26] Reserved
 * [25]    Output bypass enable
 * [24]    Clock output enable
 * [23:19] Reserved
 * [18]    24 MHz clock output enable
 * [17:16] 24 MHz output clock post divider
 * [15:13] Reserved
 * [12:8]  Factor N
 * [7:6]   Reserved
 * [5:4]   Factor K
 * [3:2]   Reserved
 * [1:0]   Factor M
 */
#define PLL6_CTRL_REG			(AW_CCM_BASE + 0x0028)
#define PLL6_CTRL_ENABLE		(1 << 31)
#define PLL6_CTRL_LOCK			(1 << 28)
#define PLL6_CTRL_BYPASS		(1 << 25)
#define PLL6_CTRL_CLK_OUTEN		(1 << 24)
#define PLL6_CTRL_24M_OUTEN		(1 << 18)
#define PLL6_CTRL_24M_POSTDIV(x)	(((x) & 0x3) << 16)
#define PLL6_CTRL_24M_POSTDIV_MASK	(0x3 << 16)
#define PLL6_CTRL_N(x)			(((x) & 0x1f) << 8)
#define PLL6_CTRL_N_MASK		(0x1f << 8)
#define PLL6_CTRL_K(x)			(((x) & 0x3) << 4)
#define PLL6_CTRL_K_MASK		(0x3 << 4)
#define PLL6_CTRL_M(x)			(((x) & 0x3) << 0)
#define PLL6_CTRL_M_MASK		(0x3 << 0)

/* apb2 bit field */
#define APB2_CLK_SRC_LOSC		(0x0 << 24)
#define APB2_CLK_SRC_OSC24M		(0x1 << 24)
#define APB2_CLK_SRC_PLL6		(0x2 << 24)
#define APB2_CLK_SRC_MASK		(0x3 << 24)
#define APB2_CLK_RATE_N_1		(0x0 << 16)
#define APB2_CLK_RATE_N_2		(0x1 << 16)
#define APB2_CLK_RATE_N_4		(0x2 << 16)
#define APB2_CLK_RATE_N_8		(0x3 << 16)
#define APB2_CLK_RATE_N_MASK		(3 << 16)
#define APB2_CLK_RATE_M(m)		(((m)-1) << 0)
#define APB2_CLK_RATE_M_MASK            (0x1f << 0)

/* apb2 gate field */
#define APB2_GATE_UART_SHIFT	(16)
#define APB2_GATE_UART_MASK		(0xff << APB2_GATE_UART_SHIFT)
#define APB2_GATE_TWI_SHIFT	(0)
#define APB2_GATE_TWI_MASK		(0xf << APB2_GATE_TWI_SHIFT)

/* apb2 reset */
#define APB2_RESET_UART_SHIFT		(16)
#define APB2_RESET_UART_MASK		(0xff << APB2_RESET_UART_SHIFT)
#define APB2_RESET_TWI_SHIFT		(0)
#define APB2_RESET_TWI_MASK		(0xf << APB2_RESET_TWI_SHIFT)

/*
 * AR100 clock configuration register:
 * [31:18] Reserved
 * [17:16] Clock source (00: LOSC, 01: HOSC, 10/11: PLL6/PDIV)
 * [15:13] Reserved
 * [12:8]  Post divide (00000: 1 - 11111: 32)
 * [7:6]   Reserved
 * [5:4]   Clock divide ratio (00: 1, 01: 2, 10: 4, 11: 8)
 * [3:0]   Reserved
 */
#define AR100_CLKCFG_REG		(AW_R_PRCM_BASE + 0x000)
#define AR100_CLKCFG_SRC_LOSC		(0 << 16)
#define AR100_CLKCFG_SRC_HOSC		(1 << 16)
#define AR100_CLKCFG_SRC_PLL6		(2 << 16)
#define AR100_CLKCFG_SRC_MASK		(0x3 << 16)
#define AR100_CLKCFG_POSTDIV(x)		(((x) & 0x1f) << 8)
#define AR100_CLKCFG_POSTDIV_MASK	(0x1f << 8)
#define AR100_CLKCFG_DIV(x)		(((x) & 0x3) << 4)
#define AR100_CLKCFG_DIV_MASK		(0x3 << 4)

#endif
