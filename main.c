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
#include "spr.h"

#define print_spr(spr)	\
	puts(#spr);		\
	puts(" = 0x");		\
	put_hex(mfspr(spr));		\
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

int main(void)
{
	unsigned int spr;

	gpio_init();
	uart0_init();

	puts("Dumping AR100 SPRs...\n");
	print_spr(SPR_VR);
	print_spr(SPR_UPR);
	print_spr(SPR_CPUCFGR);
	print_spr(SPR_DMMUCFGR);
	print_spr(SPR_IMMUCFGR);
	print_spr(SPR_DCCFGR);
	print_spr(SPR_ICCFGR);

	for (;;);
}
