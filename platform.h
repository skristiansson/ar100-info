/*
 * arch/arm/mach-sun6i/include/mach/platform.h
 *
 * Copyright (c) Allwinner.  All rights reserved.
 * Benn Huang (benn@allwinnertech.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __ASM_ARCH_PLATFORM_H
#define __ASM_ARCH_PLATFORM_H

/*
 * Memory definitions
 */
#define AW_IO_PHYS_BASE				0x01c00000
#define AW_IO_SIZE					0x00400000  /* 4MB(Max) */
#define AW_SRAM_A1_BASE				0x00000000
#define AW_SRAM_A1_SIZE				0x00008000
#define AW_SRAM_A2_BASE				0x00040000
#define AW_SRAM_A2_SIZE				0x00014000
#define AW_SRAM_D_BASE				0x00010000
#define AW_SRAM_D_SIZE				0x00001000
#define AW_SRAM_B_BASE				0x00020000 /* Secure, 64KB */
#define AW_SRAM_B_SIZE				0x00010000
#define AW_SDRAM_BASE				0x40000000
#define AW_BROM_BASE				0xffff0000
#define AW_BROM_SIZE				0x00008000 /* 32KB*/
#define	AW_MTC_ACC					0x00080000

/*
 *  device physical addresses
 */
#define	AW_SRAMCTRL_BASE			0x01c00000
#define	AW_DMA_BASE                 0x01c02000
#define	AW_NANDFLASHC0_BASE			0x01c03000
#define	AW_TS_BASE					0x01c04000
#define	AW_NANDFLASHC1_BASE			0x01c05000
#define	AW_LCD0_BASE				0x01c0c000
#define	AW_LCD1_BASE				0x01c0d000
#define AW_VE_BASE					0x01c0e000
#define AW_SDMMC0_BASE				0x01c0f000
#define AW_SDMMC1_BASE				0x01c10000
#define AW_SDMMC2_BASE				0x01c11000
#define AW_SDMMC3_BASE				0x01c12000
#define AW_SS_BASE					0x01c15000
#define AW_HDMI_BASE				0x01c16000
#define AW_MSGBOX_BASE				0x01c17000
#define AW_SPINLOCK_BASE			0x01c18000
#define AW_USB_OTG_BASE				0x01c19000
#define	AW_USB_EHCI0_BASE			0x01c1a000
#define	AW_USB_OHCI0_BASE			0x01c1a000
#define	AW_USB_EHCI1_BASE			0x01c1b000
#define	AW_USB_OHCI1_BASE			0x01c1b000
#define	AW_USB_OHCI2_BASE			0x01c1c000
#define	AW_TZASC_BASE				0x01c1e000
#define AW_CCM_BASE					0x01c20000
#define AW_PIO_BASE                 0x01c20800
#define AW_TIMER_BASE               0x01c20c00
#define	AW_SPDIF_BASE				0x01c21000
#define	AW_PWM_BASE					0x01c21400
#define	AW_DAUDIO0_BASE				0x01c22000
#define	AW_DAUDIO1_BASE				0x01c22400
#define	AW_LRADC01_BASE				0x01c22800
#define	AW_AUDIOCODEC_BASE			0x01c22C00
#define	AW_TZPC_BASE				0x01c23400
#define	AW_SID_BASE					0x01c23800
#define	AW_SJTAG_BASE				0x01c23c00
#define	AW_TP_BASE					0x01c25000
#define	AW_DMIC_BASE				0x01c25400
#define AW_UART0_BASE				0x01c28000 /* UART 0 */
#define AW_UART1_BASE				0x01c28400 /* UART 1 */
#define AW_UART2_BASE				0x01c28800 /* UART 2 */
#define AW_UART3_BASE				0x01c28c00 /* UART 3 */
#define AW_UART4_BASE				0x01c29000 /* UART 4 */
#define AW_UART5_BASE				0x01c29400 /* UART 5 */
#define AW_TWI0_BASE				0x01c2ac00
#define AW_TWI1_BASE				0x01c2b000
#define AW_TWI2_BASE				0x01c2b400
#define	AW_TWI3_BASE				0x01c2b800
#define	AW_GMAC_BASE				0x01c30000
#define	AW_GPU_BASE					0x01c40000
#define	AW_HSTMR_BASE				0x01c60000
#define	AW_DRAMCOM_BASE				0x01c62000
#define	AW_DRAMCTL0_BASE			0x01c63000
#define	AW_DRAMCTL1_BASE			0x01c64000
#define	AW_DRAMPHY0_BASE			0x01c65000
#define	AW_DRAMPHY1_BASE			0x01c66000
#define	AW_SPI0_BASE				0x01c68000
#define	AW_SPI1_BASE				0x01c69000
#define	AW_SPI2_BASE				0x01c6a000
#define	AW_SPI3_BASE				0x01c6b000
#define	AW_SCU_BASE					0x01c80000
#define	AW_MIPI_DSI0_BASE			0x01ca0000
#define	AW_MIPI_DSI0_PHY_BASE		0x01ca1000
#define	AW_CSI0_BASE				0x01cb0000
#define	AW_MIPI_CSI0_BASE			0x01cb1000
#define	AW_MIPI_CSI0_PHY_BASE		0x01cb2000
#define	AW_CSI1_BASE				0x01cb3000
#define	AW_ISP_BASE					0x01cb8000
#define	AW_ISP_MEM_BASE				0x01cc0000
#define	AW_SRAM_C_BASE				0x01d00000
#define	AW_DE_FE0_BASE				0x01e00000
#define	AW_DE_FE1_BASE				0x01e20000
#define	AW_DE_BE1_BASE				0x01e40000
#define	AW_DRC1_BASE				0x01e50000
#define	AW_DE_BE0_BASE				0x01e60000
#define	AW_DRC0_BASE				0x01e70000
#define	AW_MP_BASE					0x01e80000
#define	AW_DEU1_BASE				0x01ea0000
#define	AW_DEU0_BASE				0x01eb0000
#define	AW_PS_BASE					0x01ef0000
#define	AW_RTC_BASE					0x01f00000
#define	AW_R_TIMER_BASE				0x01f00800
#define	AW_R_INTC_BASE				0x01f00c00
#define	AW_R_WDOG_BASE				0x01f01000
#define	AW_R_PRCM_BASE				0x01f01400
#define AW_R_CPUCFG_BASE            0x01f01c00
#define AW_R_CIR_BASE				0x01f02000
#define	AW_R_TWI_BASE				0x01f02400
#define AW_R_UART_BASE              0x01f02800 /* R_UART */
#define AW_R_PIO_BASE               0x01f02c00 /* for r-pio */
#define AW_R_ONE_WIRE_BASE			0x01f03000
#define	AW_R_P2WI_BASE				0x01f03400
#define	AW_CDM_BASE					0x3f500000/*coresight debug module*/
#define	AW_TSGEN_RO_BASE			0x3f506000
#define	AW_TSGEN_CTRL_BASE			0x3f507000




#define AW_GIC_DIST_BASE            0x01c81000
#define AW_GIC_CPU_BASE             0x01c82000
#define AW_TIMER_G_BASE             0x01c80200 /* CPU global timer, not used */
#define AW_TIMER_P_BASE             0x01c80600 /* CPU private timer, not used */


/*
 * Peripheral addresses
 */
#define AW_RTC_REG					AW_RTC_BASE
#define	AW_RPIO_BASE				AW_R_PIO_BASE










/*
 *  device virtual addresses
 */
#define	AW_VIR_SRAMCTRL_BASE			0xf1c00000
#define	AW_VIR_DMA_BASE                 0xf1c02000
#define	AW_VIR_NANDFLASHC0_BASE			0xf1c03000
#define	AW_VIR_TS_BASE					0xf1c04000
#define	AW_VIR_NANDFLASHC1_BASE			0xf1c05000
#define	AW_VIR_LCD0_BASE				0xf1c0c000
#define	AW_VIR_LCD1_BASE				0xf1c0d000
#define AW_VIR_VE_BASE					0xf1c0e000
#define AW_VIR_SDMMC0_BASE				0xf1c0f000
#define AW_VIR_SDMMC1_BASE				0xf1c10000
#define AW_VIR_SDMMC2_BASE				0xf1c11000
#define AW_VIR_SDMMC3_BASE				0xf1c12000
#define AW_VIR_SS_BASE					0xf1c15000
#define AW_VIR_HDMI_BASE				0xf1c16000
#define AW_VIR_MSGBOX_BASE				0xf1c17000
#define AW_VIR_SPINLOCK_BASE			0xf1c18000
#define AW_VIR_USB_OTG_BASE				0xf1c19000
#define	AW_VIR_USB_EHCI0_BASE			0xf1c1a000
#define	AW_VIR_USB_OHCI0_BASE			0xf1c1a000
#define	AW_VIR_USB_EHCI1_BASE			0xf1c1b000
#define	AW_VIR_USB_OHCI1_BASE			0xf1c1b000
#define	AW_VIR_USB_OHCI2_BASE			0xf1c1c000
#define	AW_VIR_TZASC_BASE				0xf1c1e000
#define AW_VIR_CCM_BASE					0xf1c20000
#define AW_VIR_PIO_BASE                 0xf1c20800
#define AW_VIR_TIMER_BASE               0xf1c20c00
#define	AW_VIR_SPDIF_BASE				0xf1c21000
#define	AW_VIR_PWM_BASE					0xf1c21400
#define	AW_VIR_DAUDIO0_BASE				0xf1c22000
#define	AW_VIR_DAUDIO1_BASE				0xf1c22400
#define	AW_VIR_LRADC01_BASE				0xf1c22800
#define	AW_VIR_AUDIOCODEC_BASE			0xf1c22C00
#define	AW_VIR_TZPC_BASE				0xf1c23400
#define	AW_VIR_SID_BASE					0xf1c23800
#define	AW_VIR_SJTAG_BASE				0xf1c23c00
#define	AW_VIR_TP_BASE					0xf1c25000
#define	AW_VIR_DMIC_BASE				0xf1c25400
#define AW_VIR_UART0_BASE				0xf1c28000 /* UART 0 */
#define AW_VIR_UART1_BASE				0xf1c28400 /* UART 1 */
#define AW_VIR_UART2_BASE				0xf1c28800 /* UART 2 */
#define AW_VIR_UART3_BASE				0xf1c28c00 /* UART 3 */
#define AW_VIR_UART4_BASE				0xf1c29000 /* UART 4 */
#define AW_VIR_UART5_BASE				0xf1c29400 /* UART 5 */
#define AW_VIR_TWI0_BASE				0xf1c2ac00
#define AW_VIR_TWI1_BASE				0xf1c2b000
#define AW_VIR_TWI2_BASE				0xf1c2b400
#define	AW_VIR_TWI3_BASE				0xf1c2b800
#define	AW_VIR_GMAC_BASE				0xf1c30000
#define	AW_VIR_GPU_BASE					0xf1c40000
#define	AW_VIR_HSTMR_BASE				0xf1c60000
#define	AW_VIR_DRAMCOM_BASE				0xf1c62000
#define	AW_VIR_DRAMCTL0_BASE			0xf1c63000
#define	AW_VIR_DRAMCTL1_BASE			0xf1c64000
#define	AW_VIR_DRAMPHY0_BASE			0xf1c65000
#define	AW_VIR_DRAMPHY1_BASE			0xf1c66000
#define	AW_VIR_SPI0_BASE				0xf1c68000
#define	AW_VIR_SPI1_BASE				0xf1c69000
#define	AW_VIR_SPI2_BASE				0xf1c6a000
#define	AW_VIR_SPI3_BASE				0xf1c6b000
#define	AW_VIR_SCU_BASE					0xf1c80000
#define	AW_VIR_MIPI_DSI0_BASE			0xf1ca0000
#define	AW_VIR_MIPI_DSI0_PHY_BASE		0xf1ca1000
#define	AW_VIR_CSI0_BASE				0xf1cb0000
#define	AW_VIR_MIPI_CSI0_BASE			0xf1cb1000
#define	AW_VIR_MIPI_CSI0_PHY_BASE		0xf1cb2000
#define	AW_VIR_CSI1_BASE				0xf1cb3000
#define	AW_VIR_ISP_BASE					0xf1cb8000
#define	AW_VIR_ISP_MEM_BASE				0xf1cc0000
#define	AW_VIR_SRAM_C_BASE				0xf1d00000
#define	AW_VIR_DE_FE0_BASE				0xf1e00000
#define	AW_VIR_DE_FE1_BASE				0xf1e20000
#define	AW_VIR_DE_BE1_BASE				0xf1e40000
#define	AW_VIR_DRC1_BASE				0xf1e50000
#define	AW_VIR_DE_BE0_BASE				0xf1e60000
#define	AW_VIR_DRC0_BASE				0xf1e70000
#define	AW_VIR_MP_BASE					0xf1e80000
#define	AW_VIR_DEU1_BASE				0xf1ea0000
#define	AW_VIR_DEU0_BASE				0xf1eb0000
#define	AW_VIR_PS_BASE					0xf1ef0000
#define	AW_VIR_RTC_BASE					0xf1f00000
#define	AW_VIR_R_TIMER_BASE				0xf1f00800
#define	AW_VIR_R_INTC_BASE				0xf1f00c00
#define	AW_VIR_R_WDOG_BASE				0xf1f01000
#define	AW_VIR_R_PRCM_BASE				0xf1f01400
#define AW_VIR_R_CPUCFG_BASE            0xf1f01c00
#define AW_VIR_R_CIR_BASE				0xf1f02000
#define	AW_VIR_R_TWI_BASE				0xf1f02400
#define AW_VIR_R_UART_BASE              0xf1f02800 /* R_UART */
#define AW_VIR_R_PIO_BASE               0xf1f02c00 /* for r-pio */
#define AW_VIR_R_ONE_WIRE_BASE			0xf1f03000
#define	AW_VIR_R_P2WI_BASE				0xf1f03400




/*
 * Timer registers
 */
#define AW_TMR_IRQ_EN_REG           0x0000
#define AW_TMR_IRQ_STA_REG          0x0004
#define AW_TMR0_CTRL_REG            0x0010
#define AW_TMR0_INTV_VALUE_REG      0x0014
#define AW_TMR0_CUR_VALUE_REG       0x0018

#define AW_AVS_CNT_CTL_REG          0x0080
#define AW_AVS_CNT0_REG             0x0084
#define AW_AVS_CNT1_REG             0x0088
#define AW_AVS_CNT_DIV_REG          0x008c

#define	AW_WDOG1_IRQ_EN_REG			0xa0
#define	AW_WDOG1_IRQ_STA_REG		0xa4
#define	AW_WDOG1_CTRL_REG			0xb0
#define	AW_WDOG1_CFG_REG			0xb4
#define	AW_WDOG1_MODE_REG			0xb8

/* r-watchdog0 reg offset define */
#define	AW_WDOG0_IRQ_EN_REG		0x0
#define	AW_WDOG0_IRQ_STA_REG		0x4
#define	AW_WDOG0_CTRL_REG		0x10
#define	AW_WDOG0_CFG_REG		0x14
#define	AW_WDOG0_MODE_REG		0x18

/*
 * CPUCFG
 */
#define AW_CPUCFG_P_REG0            0x01a4
#define AW_CPUCFG_P_REG1            0x01a8
#define CPUX_RESET_CTL(x) (0x40 + (x)*0x40)
#define CPUX_CONTROL(x)   (0x44 + (x)*0x40)
#define CPUX_STATUS(x)    (0x48 + (x)*0x40)
#define AW_CPUCFG_GENCTL            0x0184
#define AW_CPUCFG_DBGCTL0           0x01e0
#define AW_CPUCFG_DBGCTL1           0x01e4


/*
 * PRCM
 */
#define AW_CPU_PWROFF_REG			0x100
/* cpu0 has no clmap register! */
#define AW_CPUX_PWR_CLAMP(x)		(0x140 + (x)*0x04)
#define AW_CPUX_PWR_CLAMP_STATUS(x)	(0x64 + (x)*0x40)

/*
 * UART
 */
#define AW_UART_RBR 0x00 /* Receive Buffer Register */
#define AW_UART_THR 0x00 /* Transmit Holding Register */
#define AW_UART_DLL 0x00 /* Divisor Latch Low Register */
#define AW_UART_DLH 0x04 /* Diviso Latch High Register */
#define AW_UART_IER 0x04 /* Interrupt Enable Register */
#define AW_UART_IIR 0x08 /* Interrrupt Identity Register */
#define AW_UART_FCR 0x08 /* FIFO Control Register */
#define AW_UART_LCR 0x0c /* Line Control Register */
#define AW_UART_MCR 0x10 /* Modem Control Register */
#define AW_UART_LSR 0x14 /* Line Status Register */
#define AW_UART_MSR 0x18 /* Modem Status Register */
#define AW_UART_SCH 0x1c /* Scratch Register */
#define AW_UART_USR 0x7c /* Status Register */
#define AW_UART_TFL 0x80 /* Transmit FIFO Level */
#define AW_UART_RFL 0x84 /* RFL */
#define AW_UART_HALT 0xa4 /* Halt TX Register */

#define UART_USR        	(AW_UART_USR >> 2)
#define UART_HALT       	(AW_UART_HALT >> 2)
#define UART_SCH			(AW_UART_SCH >> 2)
#define UART_FORCE_CFG  	(1 << 1)
#define UART_FORCE_UPDATE	(1 << 2)

#define AW_UART_LOG(fmt, args...) do {} while(0)
#if 0
#define AW_UART_LOG(fmt, args...)                                       \
        do {                                                            \
                aw_printk((u32)AW_UART0_BASE, "[%s]"fmt"\n", __FUNCTION__, ##args);   \
        } while (0)
#endif

#define AW_R_UART_LOG(fmt, args...)                                       \
        do {                                                              \
                aw_printk((u32)AW_R_UART_BASE, "[%s]"fmt"\n", __FUNCTION__, ##args);   \
        } while (0)


#endif	/* __ASM_ARCH_PLATFORM_H */
