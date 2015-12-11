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
#include "spr-defs.h"

	/*
	* The A31 SoC have address decode logic for the exception vectors,
	* leaving only room for a jump instruction at each exception vector,
	* so we can't do the setup here but have to jump into the actual
	* SRAM at 0x4000
	*
	* NOTE: we are missing vectors here,  but since the l.nop actually is
	* hardwired, the "0" instruction data will turn into an infinite loop
	* (l.j 0) at the unhandled exceptions.
	*/
	.org 0x100
	.global _start
_start:
	l.j	reset
	 l.nop

	.org 0x700
	l.j illegal_instruction_handler
	 l.nop

	.org 0x4000
	/* include the arm "boot loader" binary at a fixed address */
	.global ar100_boot_start
ar100_boot:
	.incbin	"ar100-boot/ar100-boot.code"
	.align 4

reset:
	l.movhi	r0,0
	l.ori	r1, r0, 0x7ffc
	l.j	main
	 l.nop

	.global illegal_instruction
illegal_instruction:	.long 0x0

illegal_instruction_handler:
	/* Step over red zone */
	l.addi	r1, r1, -128
	l.sw	-4(r1), r3
	l.sw	-8(r1), r4

	/* Set illegal_instruction to 1 */
	l.movhi	r3, hi(illegal_instruction)
	l.ori	r3, r3, lo(illegal_instruction)
	l.ori	r4, r0, 1
	l.sw	0(r3), r4
	/* Step over the illegal instruction */
	l.mfspr	r3, r0, SPR_EPCR_BASE
	l.addi	r3, r3, 4
	l.mtspr	r0, r3, SPR_EPCR_BASE

	/* Restore */
	l.lwz	r3, -4(r1)
	l.lwz	r4, -8(r1)
	l.addi	r1, r1, 128


	l.rfe
	l.nop
