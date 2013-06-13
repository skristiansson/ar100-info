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
#define CPUCFG_BASE	0x01f01c00

#define readl(addr)	(*((volatile unsigned long  *)(addr)))
#define writel(v, addr)	(*((volatile unsigned long  *)(addr)) = (unsigned long)(v))

/*
 * from U-Boot arch/arm/cpu/armv7/syslib
 * (C) Copyright 2008
 * Texas Instruments, <www.ti.com>
 *
 * Richard Woodruff <r-woodruff2@ti.com>
 * Syed Mohammed Khasim <khasim@ti.com>
 */
void delay(unsigned long loops)
{
	__asm__ volatile ("1:\n" "subs %0, %1, #1\n"
			  "bne 1b":"=r" (loops):"0"(loops));
}

int main(void)
{
	volatile unsigned long cpu_cfg;

	/* assert reset to ar100 */
	cpu_cfg = readl(CPUCFG_BASE);
	cpu_cfg &= ~(1uL);
	writel(cpu_cfg, CPUCFG_BASE);
	delay(1000);
	/* deassert reset to ar100 */
	cpu_cfg |= 1;
	writel(cpu_cfg, CPUCFG_BASE);

	return 0;
}
