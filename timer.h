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
#ifndef _TIMER_H
#define _TIMER_H
#include "spr.h"

static inline void timer_enable(void)
{
	mtspr(SPR_TTMR, SPR_TTMR_CR);
}

static inline void timer_disable(void)
{
	mtspr(SPR_TTMR, 0);
}

static inline void timer_reset_ticks(void)
{
	mtspr(SPR_TTCR, 0);
}

static inline unsigned timer_get_ticks(void)
{
	return mfspr(SPR_TTCR);
}
#endif
