/*
 * linux/arch/arm/mach-omap2/board-mapphone-camera.c
 *
 * Copyright (C) 2009 Motorola, Inc.
 *
 * Derived from mach-omap3/board-3430sdp.c
 *
 * Copyright (C) 2007 Texas Instruments
 *
 * Modified from mach-omap2/board-generic.c
 *
 * Initial code: Syed Mohammed Khasim
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/mm.h>
#include <linux/platform_device.h>
#include <linux/regulator/consumer.h>
#include <plat/mux.h>
#include <plat/board-mapphone.h>
#include <plat/omap-pm.h>
#include <plat/control.h>
#include <plat/resource.h>

#if defined(CONFIG_VIDEO_OMAP3)
#include <media/v4l2-int-device.h>
#include <../drivers/media/video/omap34xxcam.h>
#include <../drivers/media/video/isp/ispreg.h>
#include <../drivers/media/video/isp/isp.h>
#include <../drivers/media/video/isp/ispcsi2.h>
#elif defined(CONFIG_VIDEO_OLDOMAP3)
#include <media/v4l2-int-device.h>
#include <../drivers/media/video/oldomap34xxcam.h>
#include <../drivers/media/video/oldisp/ispreg.h>
#include <../drivers/media/video/oldisp/isp.h>
#include <../drivers/media/video/oldisp/ispcsi2.h>
#endif
#if defined(CONFIG_VIDEO_OV8810) || defined(CONFIG_VIDEO_OV8810_MODULE)
#include <media/ov8810.h>
#if defined(CONFIG_LEDS_FLASH_RESET)
#include <linux/spi/cpcap.h>
#include <linux/spi/cpcap-regbits.h>
#endif
#define OV8810_CSI2_CLOCK_POLARITY	0
#define OV8810_CSI2_DATA0_POLARITY	0
#define OV8810_CSI2_DATA1_POLARITY	0
#define OV8810_CSI2_CLOCK_LANE		1
#define OV8810_CSI2_DATA0_LANE		2
#define OV8810_CSI2_DATA1_LANE		3
#define OV8810_CSI2_PHY_THS_TERM	1
#define OV8810_CSI2_PHY_THS_SETTLE	21
#define OV8810_CSI2_PHY_TCLK_TERM	0
#define OV8810_CSI2_PHY_TCLK_MISS	1
#define OV8810_CSI2_PHY_TCLK_SETTLE	14
#define CPUCLK_LOCK_VAL			    0x5
#define OV8810_XCLK_27MHZ			27000000
#endif
#ifdef CONFIG_VIDEO_OMAP3_HPLENS
#include <../drivers/media/video/hplens.h>
#endif

static void mapphone_camera_lines_safe_mode(void);
static void mapphone_camera_lines_func_mode(void);

#ifdef CONFIG_VIDEO_OMAP3_HPLENS
static int hplens_power_set(enum v4l2_power power)
{
	(void)power;

	return 0;
}

static int hplens_set_prv_data(void *priv)
{
	struct omap34xxcam_hw_config *hwc = priv;

	hwc->dev_index = 0;
	hwc->dev_minor = 0;
	hwc->dev_type = OMAP34XXCAM_SLAVE_LENS;

	return 0;
}

struct hplens_platform_data mapphone_hplens_platform_data = {
	.power_set = hplens_power_set,
	.priv_data_set = hplens_set_prv_data,
};
#endif

#if defined(CONFIG_VIDEO_OV8810)

static struct omap34xxcam_sensor_config ov8810_cam_hwc = {
	.sensor_isp = 0,
	.xclk = OMAP34XXCAM_XCLK_A,
	.capture_mem = PAGE_ALIGN(3264 * 2448 * 2 * 2) * 4,
};

static void mapphone_lock_cpufreq(int lock){
	static struct device *ov_dev;
	static int flag;
	if (lock == 1) {
		resource_request("vdd1_opp", ov_dev, CPUCLK_LOCK_VAL);
		flag = 1;
	}
	else {
		if (flag == 1) {
			resource_release("vdd1_opp", ov_dev);
			flag = 0;
		}
	}
}

static int ov8810_sensor_set_prv_data(void *priv)
{
	struct omap34xxcam_hw_config *hwc = priv;

	hwc->u.sensor.xclk = ov8810_cam_hwc.xclk;
	hwc->u.sensor.sensor_isp = ov8810_cam_hwc.sensor_isp;
	hwc->u.sensor.capture_mem = ov8810_cam_hwc.capture_mem;
	hwc->dev_index = 0;
	hwc->dev_minor = 0;
	hwc->dev_type = OMAP34XXCAM_SLAVE_SENSOR;
	hwc->interface_type = ISP_CSIA;

	hwc->csi2.hw_csi2.lanes.clock.polarity = OV8810_CSI2_CLOCK_POLARITY;
	hwc->csi2.hw_csi2.lanes.clock.position = OV8810_CSI2_CLOCK_LANE;
	hwc->csi2.hw_csi2.lanes.data[0].polarity = OV8810_CSI2_DATA0_POLARITY;
	hwc->csi2.hw_csi2.lanes.data[0].position = OV8810_CSI2_DATA0_LANE;
	hwc->csi2.hw_csi2.lanes.data[1].polarity = OV8810_CSI2_DATA1_POLARITY;
	hwc->csi2.hw_csi2.lanes.data[1].position = OV8810_CSI2_DATA1_LANE;
	hwc->csi2.hw_csi2.phy.ths_term = OV8810_CSI2_PHY_THS_TERM;
	hwc->csi2.hw_csi2.phy.ths_settle = OV8810_CSI2_PHY_THS_SETTLE;
	hwc->csi2.hw_csi2.phy.tclk_term = OV8810_CSI2_PHY_TCLK_TERM;
	hwc->csi2.hw_csi2.phy.tclk_miss = OV8810_CSI2_PHY_TCLK_MISS;
	hwc->csi2.hw_csi2.phy.tclk_settle = OV8810_CSI2_PHY_TCLK_SETTLE;

	return 0;
}

static struct isp_interface_config ov8810_if_config = {
	.ccdc_par_ser = ISP_CSIA,
	.dataline_shift = 0x0,
	.hsvs_syncdetect = ISPCTRL_SYNC_DETECT_VSRISE,
	.vdint0_timing = 0x0,
	.vdint1_timing = 0x0,
	.strobe = 0x0,
	.prestrobe = 0x0,
	.shutter = 0x0,
	.wenlog = ISPCCDC_CFG_WENLOG_OR,
	.dcsub = OV8810_BLACK_LEVEL_10BIT,
	.raw_fmt_in = ISPCCDC_INPUT_FMT_BG_GR,
	.wbal.coef0		= 0x23,
	.wbal.coef1		= 0x20,
	.wbal.coef2		= 0x20,
	.wbal.coef3		= 0x39,
	.u.csi.crc = 0x0,
	.u.csi.mode = 0x0,
	.u.csi.edge = 0x0,
	.u.csi.signalling = 0x0,
	.u.csi.strobe_clock_inv = 0x0,
	.u.csi.vs_edge = 0x0,
	.u.csi.channel = 0x1,
	.u.csi.vpclk = 0x1,
	.u.csi.data_start = 0x0,
	.u.csi.data_size = 0x0,
	.u.csi.format = V4L2_PIX_FMT_SGRBG10,
};

static int ov8810_sensor_power_set(struct device *dev, \
	struct i2c_client *i2c_client, enum v4l2_power power)
{

	struct isp_csi2_lanes_cfg lanecfg;
	struct isp_csi2_phy_cfg phyconfig;

	static enum v4l2_power previous_power = V4L2_POWER_OFF;
	static struct regulator *regulator_vcam;
	static struct regulator *regulator_vwlan1;
	/*Basic turn on operation is will be first one time executed.*/
	static bool regulator_poweron = 0;
	
#if defined(CONFIG_LEDS_FLASH_RESET)
	static enum detect_type {
		FLASH_COUPLE_LINE = 0,
		FLASH_SINGLE_LINE,
		FLASH_NOT_DETECTED,
	} flash_detected = FLASH_NOT_DETECTED;
#endif

	switch (power) {
	case V4L2_POWER_OFF:

		/* Power Down Sequence */
		isp_csi2_complexio_power(ISP_CSI2_POWER_OFF);


		/* Release pm constraints */
		omap_pm_set_min_bus_tput(dev, OCP_INITIATOR_AGENT, 0);
		/* Turn off power */
		if (regulator_vcam != NULL) {
			regulator_disable(regulator_vcam);
			regulator_put(regulator_vcam);
			regulator_vcam = NULL;
		} else {
			mapphone_camera_lines_safe_mode();
			pr_err("%s: Regulator for vcam is not "\
					"initialized\n", __func__);
			return -EIO;
		}

		/* Delay 6 msec for vcam to drop (4.7uF to 10uF change) */
		msleep(6);

		/* Turn off power */
		if (regulator_vwlan1 != NULL) {
			regulator_disable(regulator_vwlan1);
			regulator_put(regulator_vwlan1);
			regulator_vwlan1 = NULL;
		} else {
			mapphone_camera_lines_safe_mode();
			pr_err("%s: Regulator for vwlan1 is not "\
					"initialized\n", __func__);
			return -EIO;
		}
		gpio_set_value(GPIO_OV8810_RESET, 0);
		gpio_set_value(GPIO_OV8810_STANDBY, 0);

#if defined(CONFIG_LEDS_FLASH_RESET)
		/*If Xenon flash module didn't detected,
			FLASH_RESET pin control.*/
		if (flash_detected == FLASH_COUPLE_LINE)
			cpcap_direct_misc_write(CPCAP_REG_GPIO0,\
				0, CPCAP_BIT_GPIO0DRV);
#endif
		gpio_free(GPIO_OV8810_RESET);
		gpio_free(GPIO_OV8810_STANDBY);

		mapphone_camera_lines_safe_mode();
	break;
	case V4L2_POWER_ON:

		mapphone_camera_lines_func_mode();
	        /* Set min throughput to:
	         *  2592 x 1944 x 2bpp x 30fps x 3 L3 accesses */
	         omap_pm_set_min_bus_tput(dev, OCP_INITIATOR_AGENT, 885735);

		printk(KERN_DEBUG "ov8810_sensor_power_set(ON)\n");
		/*if (previous_power == V4L2_POWER_OFF)*/
			isp_csi2_reset();

		lanecfg.clk.pol = OV8810_CSI2_CLOCK_POLARITY;
		lanecfg.clk.pos = OV8810_CSI2_CLOCK_LANE;
		lanecfg.data[0].pol = OV8810_CSI2_DATA0_POLARITY;
		lanecfg.data[0].pos = OV8810_CSI2_DATA0_LANE;
		lanecfg.data[1].pol = OV8810_CSI2_DATA1_POLARITY;
		lanecfg.data[1].pos = OV8810_CSI2_DATA1_LANE;
		lanecfg.data[2].pol = 0;
		lanecfg.data[2].pos = 0;
		lanecfg.data[3].pol = 0;
		lanecfg.data[3].pos = 0;
		isp_csi2_complexio_lanes_config(&lanecfg);
		isp_csi2_complexio_lanes_update(true);

		phyconfig.ths_term = OV8810_CSI2_PHY_THS_TERM;
		phyconfig.ths_settle = OV8810_CSI2_PHY_THS_SETTLE;
		phyconfig.tclk_term = OV8810_CSI2_PHY_TCLK_TERM;
		phyconfig.tclk_miss = OV8810_CSI2_PHY_TCLK_MISS;
		phyconfig.tclk_settle = OV8810_CSI2_PHY_TCLK_SETTLE;
		isp_csi2_phy_config(&phyconfig);
		isp_csi2_phy_update(true);

		isp_configure_interface(&ov8810_if_config);

		if ((previous_power == V4L2_POWER_OFF) && (regulator_poweron == 0)){

			/* Disable Interface. */
			isp_csi2_ctrl_config_if_enable(false);
			isp_csi2_ctrl_update(false);

			/* Configure pixel clock divider (here?) */
			omap_writel(OMAP_MCAM_SRC_DIV, 0x48004f40);

			/* turn on VWLAN1 power */
			if (regulator_vwlan1 != NULL) {
				pr_warning("%s: Already have "\
						"regulator_vwlan1 \n", __func__);
			} else {
				regulator_vwlan1 = regulator_get(NULL, "vwlan1");
				if (IS_ERR(regulator_vwlan1)) {
					pr_err("%s: Cannot get vwlan1 "\
						"regulator_vwlan1, err=%ld\n",
						__func__, PTR_ERR(regulator_vwlan1));
					return PTR_ERR(regulator_vwlan1);
				}
			}
			
			if (regulator_enable(regulator_vwlan1) != 0) {
				pr_err("%s: Cannot enable vcam regulator_vwlan1\n",
						__func__);
				return -EIO;
			}

			/* turn on VCAM power */
			if (regulator_vcam != NULL) {
				pr_warning("%s: Already have "\
						"regulator_vcam\n", __func__);
			} else {
				regulator_vcam = regulator_get(NULL, "vcam");
				if (IS_ERR(regulator_vcam)) {
					pr_err("%s: Cannot get vcam "\
						"regulator_vcam, err=%ld\n",
						__func__, PTR_ERR(regulator_vcam));
					return PTR_ERR(regulator_vcam);
				}
			}

			if (regulator_enable(regulator_vcam) != 0) {
				pr_err("%s: Cannot enable vcam regulator_vcam\n",
						__func__);
				return -EIO;
			}

			mdelay(5);

			/* Request and configure gpio pins */
			if (gpio_request(GPIO_OV8810_STANDBY,
						"ov8810 camera standby") != 0)
				return -EIO;

			/* set to output mode */
			gpio_direction_output(GPIO_OV8810_STANDBY, 0);
			gpio_set_value(GPIO_OV8810_STANDBY, 0);

			if (gpio_request(GPIO_OV8810_RESET,
						"ov8810 camera reset") != 0)
				return -EIO;

			/* trigger reset */
			gpio_direction_output(GPIO_OV8810_RESET, 1);

			/* nRESET is active LOW. set HIGH to release reset */
			gpio_set_value(GPIO_OV8810_RESET, 1);
			
#if defined(CONFIG_LEDS_FLASH_RESET)
			if (flash_detected == FLASH_NOT_DETECTED) {
				if (bd7885_device_detection())
					flash_detected = FLASH_SINGLE_LINE;
				else
					flash_detected = FLASH_COUPLE_LINE;
			}
			/*If Xenon flash module didn't detected,
				FLASH_RESET pin control.*/
			if (flash_detected == FLASH_COUPLE_LINE)
				cpcap_direct_misc_write(CPCAP_REG_GPIO0,\
					CPCAP_BIT_GPIO0DRV, CPCAP_BIT_GPIO0DRV);
#endif
			/* give sensor sometime to get out of the reset.
			 * Datasheet says 2400 xclks. At 6 MHz, 400 usec is
			 * enough
			 */
			mdelay(5);

			ov8810_write_reg(i2c_client, OV8810_IMAGE_SYSTEM, 0x00);

			pr_err("%s: OV8810 streaming off.\n",
					__func__);

			/* Enable Interface. */
			isp_csi2_ctrl_config_if_enable(true);
			isp_csi2_ctrl_update(false);

                     /*Set regulator turned on.*/
			/*regulator_poweron = 1;*/
		}
		break;
	case V4L2_POWER_STANDBY:
		/* stand by */
		break;
	}
	/* Save powerstate to know what was before calling POWER_ON. */
	previous_power = power;
	return 0;
}

struct ov8810_platform_data mapphone_ov8810_platform_data = {
	.power_set      = ov8810_sensor_power_set,
	.priv_data_set  = ov8810_sensor_set_prv_data,
	.lock_cpufreq   = mapphone_lock_cpufreq,
	.default_regs   = NULL,
};

#endif

void mapphone_camera_lines_safe_mode(void)
{
	omap_writew(0x0704, 0x4800207C);
}

void mapphone_camera_lines_func_mode(void)
{
	omap_writew(0x0704, 0x4800207C);
}

void __init mapphone_camera_init(void)
{
    omap_cfg_reg(C25_34XX_CAM_XCLKA);
	omap_cfg_reg(C23_34XX_CAM_FLD);
    omap_cfg_reg(H2_34XX_GPMC_A3);
    omap_cfg_reg(AG17_34XX_CAM_D0);
	omap_cfg_reg(AH17_34XX_CAM_D1);
	omap_cfg_reg(AD17_34XX_CSI2_DX0);
	omap_cfg_reg(AE18_34XX_CSI2_DY0);
	omap_cfg_reg(AD16_34XX_CSI2_DX1);
	omap_cfg_reg(AE17_34XX_CSI2_DY1);

    /*Initialize F_RDY_N pin for Xenon flash control.*/
    if (gpio_request(36, "xenon flash ready pin") != 0)
	pr_err("%s: Xenon flash ready pin control failure.\n",__func__);

    gpio_direction_input(36);
}

