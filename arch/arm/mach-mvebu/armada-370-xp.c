/*
 * Device Tree support for Armada 370 and XP platforms.
 *
 * Copyright (C) 2012 Marvell
 *
 * Lior Amsalem <alior@marvell.com>
 * Gregory CLEMENT <gregory.clement@free-electrons.com>
 * Thomas Petazzoni <thomas.petazzoni@free-electrons.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/of_platform.h>
#include <linux/io.h>
#include <linux/time-armada-370-xp.h>
#include <linux/clk/mvebu.h>
#include <linux/dma-mapping.h>
#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/mach/time.h>
#include "armada-370-xp.h"
#include "common.h"
#include "coherency.h"

static struct map_desc armada_370_xp_io_desc[] __initdata = {
	{
		.virtual	= (unsigned long) ARMADA_370_XP_REGS_VIRT_BASE,
		.pfn		= __phys_to_pfn(ARMADA_370_XP_REGS_PHYS_BASE),
		.length		= ARMADA_370_XP_REGS_SIZE,
		.type		= MT_DEVICE,
	},
};

void __init armada_370_xp_map_io(void)
{
	iotable_init(armada_370_xp_io_desc, ARRAY_SIZE(armada_370_xp_io_desc));
}

void __init armada_370_xp_timer_and_clk_init(void)
{
	mvebu_clocks_init();
	armada_370_xp_timer_init();
}

void __init armada_370_xp_init_early(void)
{
	/*
	 * Some Armada 370/XP devices allocate their coherent buffers
	 * from atomic context. Increase size of atomic coherent pool
	 * to make sure such the allocations won't fail.
	 */
	init_dma_coherent_pool_size(SZ_1M);
}

struct sys_timer armada_370_xp_timer = {
	.init		= armada_370_xp_timer_and_clk_init,
};

static void __init armada_370_xp_dt_init(void)
{
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
	coherency_init();
}

static const char * const armada_370_xp_dt_compat[] = {
	"marvell,armada-370-xp",
	NULL,
};

DT_MACHINE_START(ARMADA_XP_DT, "Marvell Armada 370/XP (Device Tree)")
	.smp		= smp_ops(armada_xp_smp_ops),
	.init_machine	= armada_370_xp_dt_init,
	.map_io		= armada_370_xp_map_io,
	.init_early	= armada_370_xp_init_early,
	.init_irq	= armada_370_xp_init_irq,
	.handle_irq     = armada_370_xp_handle_irq,
	.timer		= &armada_370_xp_timer,
	.restart	= mvebu_restart,
	.dt_compat	= armada_370_xp_dt_compat,
MACHINE_END
