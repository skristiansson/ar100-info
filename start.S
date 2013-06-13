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

	/*
	* The A31 SoC have address decode logic for the exception vectors,
	* leaving only room for a jump instruction at each exception vector,
	* so we can't do the setup here but have to jump into the actual
	* SRAM at 0x4000
	*
	* NOTE: we are missing all but the reset vector here,
	* but since the l.nop actually is hardwired, the "0" instruction data
	* will turn into an infinite loop (l.j 0) at the unhandled exceptions.
	*/
	.org 0x100
	.global _start
_start:
	l.j	reset
	 l.nop

	.org 0x4000
	/* include the arm "boot loader" binary at a fixed address */
	.global ar100_boot_start
ar100_boot:
	.incbin	"ar100-boot/ar100-boot.code"
	.align 4

reset:
	l.movhi	r0,0
	l.ori	r1, r0, 0x8000
	l.j	main
	 l.nop