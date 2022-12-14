#include "app_config.h"
#include "system/includes.h"
#include "device/iic.h"
#include "server/audio_dev.h"
#include "asm/includes.h"

#if TCFG_USB_SLAVE_ENABLE || TCFG_USB_HOST_ENABLE
#include "otg.h"
#include "usb_host.h"
#include "usb_common_def.h"
#include "usb_storage.h"
#endif

// *INDENT-OFF*

UART0_PLATFORM_DATA_BEGIN(uart0_data)
	.baudrate = 115200,
	/* .baudrate = 1000000, */
	/* .port = PORTA_5_6, */
	/* .port = PORTB_1_0, */
	.port = PORTA_3_4,
	/* .port = PORTH_0_1, */
	/* .tx_pin = IO_PORTA_05, */
	/* .rx_pin = IO_PORTA_06, */
	/* .tx_pin = IO_PORTB_01, */
	/* .rx_pin = IO_PORTB_00, */
	.tx_pin = IO_PORTA_03,
	.rx_pin = IO_PORTA_04,
	/* .tx_pin = IO_PORTH_00, */
	/* .rx_pin = IO_PORTH_01, */
	.max_continue_recv_cnt = 1024,
	.idle_sys_clk_cnt = 500000,
	.clk_src = PLL_48M,
	.flags = UART_DEBUG,
UART0_PLATFORM_DATA_END();


UART1_PLATFORM_DATA_BEGIN(uart1_data)
	.baudrate = 1000000,
	/* .port = PORT_REMAP, */
	/* .port = PORTH_6_7, */
	/* .port = PORTC_3_4, */
	/* .port = PORTUSB_B, */
	.port = PORTUSB_A,
	/* .tx_pin = IO_PORTH_06, */
	/* .rx_pin = IO_PORTH_07, */
	/* .tx_pin = IO_PORTC_03, */
	/* .rx_pin = IO_PORTC_04, */
	/* .tx_pin = IO_PORT_USB_DPB, */
	/* .rx_pin = IO_PORT_USB_DMB, */
	.tx_pin = IO_PORT_USB_DPA,
	.rx_pin = IO_PORT_USB_DMA,
	/* .tx_pin = IO_PORTB_06, */
	/* .rx_pin = IO_PORTB_07, */
	.max_continue_recv_cnt = 1024,
	.idle_sys_clk_cnt = 500000,
	.clk_src = PLL_48M,
	.flags = UART_DEBUG,
UART1_PLATFORM_DATA_END();


UART2_PLATFORM_DATA_BEGIN(uart2_data)
	.baudrate = 1000000,
	/* .port = PORTH_2_3, */
	/* .port = PORTC_9_10, */
	/* .port = PORTB_6_7, */
	/* .port = PORTE_0_1, */
	/* .tx_pin = IO_PORTH_02, */
	/* .rx_pin = IO_PORTH_03, */
	/* .tx_pin = IO_PORTC_09, */
	/* .rx_pin = IO_PORTC_10, */
	/* .tx_pin = IO_PORTB_06, */
	/* .rx_pin = IO_PORTB_07, */
	/* .tx_pin = IO_PORTE_00,*/
	/* .rx_pin = IO_PORTE_01,*/
	.port = PORT_REMAP,
	.output_channel = OUTPUT_CHANNEL0,
	.tx_pin = IO_PORTC_06,
	.rx_pin = -1,
	.max_continue_recv_cnt = 1024,
	.idle_sys_clk_cnt = 500000,
	.clk_src = PLL_48M,
	.flags = UART_DEBUG,
UART2_PLATFORM_DATA_END();



/****************SD??????**********************************************************
 * ????????????1???SD????????????????????????SD???????????????????????????SD0/1_ENABLE???????????????????????????
********************************************************************************/
#if TCFG_SD0_ENABLE
//sd0
#define SD_PLATFORM_DATA_BEGIN() SD0_PLATFORM_DATA_BEGIN(sd0_data)
#define SD_PLATFORM_DATA_END() SD0_PLATFORM_DATA_END()
#define SD_CLK_DETECT_FUNC		sdmmc_0_clk_detect
#elif TCFG_SD1_ENABLE
//sd1
#define SD_PLATFORM_DATA_BEGIN() SD1_PLATFORM_DATA_BEGIN(sd1_data)
#define SD_PLATFORM_DATA_END() SD1_PLATFORM_DATA_END()
#define SD_CLK_DETECT_FUNC		sdmmc_1_clk_detect
#endif

#if (TCFG_SD0_ENABLE || TCFG_SD1_ENABLE)
SD_PLATFORM_DATA_BEGIN()
	.port 					= TCFG_SD_PORTS,
	.priority 				= 3,
	.data_width 			= TCFG_SD_DAT_WIDTH,
	.speed 					= TCFG_SD_CLK,
	.detect_mode 			= TCFG_SD_DET_MODE,
#if (TCFG_SD_DET_MODE == SD_CLK_DECT)
	.detect_func 			= SD_CLK_DETECT_FUNC,
#elif (TCFG_SD_DET_MODE == SD_IO_DECT)
	.detect_func 			= sdmmc_0_io_detect,
#else
	.detect_func 			= NULL,
#endif
SD_PLATFORM_DATA_END()
#endif
/****************SD??????*********************/


HW_IIC0_PLATFORM_DATA_BEGIN(hw_iic1_data)
	/* .clk_pin = IO_PORTC_01, */
	/* .dat_pin = IO_PORTC_02, */
	.clk_pin = IO_PORTH_00,
	.dat_pin = IO_PORTH_01,
	.baudrate = 0x3f,//3f:385k
HW_IIC0_PLATFORM_DATA_END()

SW_IIC_PLATFORM_DATA_BEGIN(sw_iic0_data)
	/*.clk_pin = IO_PORTA_03,*/
	/*.dat_pin = IO_PORTA_04,*/
	.clk_pin = IO_PORTC_01,
	.dat_pin = IO_PORTC_02,
	.sw_iic_delay = 50,
SW_IIC_PLATFORM_DATA_END()
/*
 * spi0???falsh
 */
SPI0_PLATFORM_DATA_BEGIN(spi0_data)
	.clk    = 20000000,
	.mode   = SPI_STD_MODE,
	.port   = 'A',
SPI0_PLATFORM_DATA_END()

SPI1_PLATFORM_DATA_BEGIN(spi1_data)
	.clk    = 80000000,
	.mode   = SPI_STD_MODE,
	.attr   = SPI_MODE_SLAVE | SPI_SCLK_H_UPL_SMPL | SPI_UNIDIR_MODE,//?????????CLK??? ??????????????????????????????(SPI_DO??????)
	.port   = 'B',
SPI1_PLATFORM_DATA_END()

SPI2_PLATFORM_DATA_BEGIN(spi2_data)
	.clk    = 20000000,
	.mode   = SPI_STD_MODE,
	.attr   = SPI_SCLK_H_UPH_SMPL | SPI_BIDIR_MODE,//?????????CLK??? ??????????????????????????????(SPI_DI/SPI_DO??????)
	.port   = 'C',
SPI2_PLATFORM_DATA_END()

static const struct spiflash_platform_data spiflash_data = {
    .name           = "spi0",
    .mode           = FAST_READ_OUTPUT_MODE,//FAST_READ_IO_MODE,
    .sfc_run_mode   = SFC_FAST_READ_DUAL_OUTPUT_MODE,
};

static const struct dac_platform_data dac_data = {
    .pa_auto_mute = 1,
    .pa_mute_port = 0xff,
    .pa_mute_value = 1,
    .differ_output = 0,
	.hw_channel = 0x01,
	.ch_num = 1,	//???????????????????????????
    .vcm_init_delay_ms = 1000,
};
static const struct adc_platform_data adc_data = {
    .mic_channel = LADC_CH_MIC1_P_N,
    .mic_ch_num = 1,
    .isel = 2,
};
static const struct audio_pf_data audio_pf_d = {
    .adc_pf_data = &adc_data,
    .dac_pf_data = &dac_data,
};
static const struct audio_platform_data audio_data = {
    .private_data = (void *) &audio_pf_d,
};


#ifdef CONFIG_VIDEO_ENABLE
#define CAMERA_GROUP_PORT	ISC_GROUPA
/* #define CAMERA_GROUP_PORT	ISC_GROUPC */
static const struct camera_platform_data camera0_data = {
    .xclk_gpio      = IO_PORTC_00,//????????? ????????????xclk????????????IO???????????????OUTPUT_CHANNEL1
    .reset_gpio     = IO_PORTC_03,
    .online_detect  = NULL,
    .pwdn_gpio      = -1,
    .power_value    = 0,
    .interface      = SEN_INTERFACE0,//SEN_INTERFACE_CSI2,
    .dvp={
#if (CAMERA_GROUP_PORT == ISC_GROUPA)
        .pclk_gpio   = IO_PORTA_08,
        .hsync_gpio  = IO_PORTA_09,
        .vsync_gpio  = IO_PORTA_10,
#else
        .pclk_gpio   = IO_PORTC_08,
        .hsync_gpio  = IO_PORTC_09,
        .vsync_gpio  = IO_PORTC_10,
#endif
		.group_port  = CAMERA_GROUP_PORT,
        .data_gpio={
#if (CAMERA_GROUP_PORT == ISC_GROUPA)
                IO_PORTA_07,
                IO_PORTA_06,
                IO_PORTA_05,
                IO_PORTA_04,
                IO_PORTA_03,
                IO_PORTA_02,
                IO_PORTA_01,
                IO_PORTA_00,
				-1,
				-1,
#else
                IO_PORTC_07,
                IO_PORTC_06,
                IO_PORTC_05,
                IO_PORTC_04,
                IO_PORTC_03,
                IO_PORTC_02,
                IO_PORTC_01,
                IO_PORTC_00,
				-1,
				-1,
#endif
        },
    }
};
static const struct video_subdevice_data video0_subdev_data[] = {
    { VIDEO_TAG_CAMERA, (void *)&camera0_data },
};
static const struct video_platform_data video0_data = {
    .data = video0_subdev_data,
    .num = ARRAY_SIZE(video0_subdev_data),
};
#endif

PWM_PLATFORM_DATA_BEGIN(pwm_data0)
	.port	= 0,
    .pwm_ch = PWMCH0_H | PWMCH0_L,//????????????????????????,???:PWMCH0_H | PWMCH0_L | PWMCH1_H ... | PWMCH7_H | PWMCH7_L | PWM_TIMER2_OPCH2 | PWM_TIMER3_OPCH3 ,
    .freq   = 1000,//??????
    .duty   = 50,//?????????
    .point_bit = 0,//??????point_bit????????????????????????????????????: 0<freq<=4K,point_bit=2;4K<freq<=40K,point_bit=1; freq>40K,point_bit=0;
PWM_PLATFORM_DATA_END()

PWM_PLATFORM_DATA_BEGIN(pwm_data1)
	.port	= IO_PORTA_03,//??????????????????TIMER PWM??????IO???pwm_ch??????PWM_TIMER3_OPCH3???PWM_TIMER2_OPCH2??????,?????????2???PWM,??????output_channel2/3?????????????????????output_channel?????????
    .pwm_ch = PWM_TIMER2_OPCH2,//????????????????????????,???:PWMCH0_H | PWMCH0_L | PWMCH1_H ... | PWMCH7_H | PWMCH7_L | PWM_TIMER2_OPCH2 | PWM_TIMER3_OPCH3 ,
    .freq   = 1000,//??????
    .duty   = 50,//?????????
    .point_bit = 0,//??????point_bit????????????????????????????????????: 0<freq<=4K,point_bit=2;4K<freq<=40K,point_bit=1; freq>40K,point_bit=0;
PWM_PLATFORM_DATA_END()

#if TCFG_USB_SLAVE_ENABLE || TCFG_USB_HOST_ENABLE
static const struct otg_dev_data otg_data = {
    .usb_dev_en = 0x03,
#if TCFG_USB_SLAVE_ENABLE
    .slave_online_cnt = 10,
    .slave_offline_cnt = 10,
#endif
#if TCFG_USB_HOST_ENABLE
    .host_online_cnt = 10,
	.host_offline_cnt = 10,
#endif
	.detect_mode = OTG_HOST_MODE | OTG_SLAVE_MODE | OTG_CHARGE_MODE,
    .detect_time_interval = 30,
};
#endif

#if defined CONFIG_BT_ENABLE || defined CONFIG_WIFI_ENABLE
#include "wifi/wifi_connect.h"
const struct wifi_calibration_param wifi_calibration_param = {
    .xosc_l     = 0xb,// ?????????????????????
    .xosc_r     = 0xb,// ?????????????????????
    .pa_trim_data  ={1, 7, 4, 7, 11, 1, 7},// ??????MP????????????PA TRIM???
};
#endif

#ifdef CONFIG_RTC_ENABLE
const struct rtc_init_config rtc_init_data = {
	.rtc_clk_sel = RTC_CLK_SEL_INSIDE,
	.rtc_power_sel = RTCVDD_SUPPLY_OUTSIDE,
};
#endif

REGISTER_DEVICES(device_table) = {
    { "pwm0",  &pwm_dev_ops, (void *)&pwm_data0},
    { "pwm1",  &pwm_dev_ops, (void *)&pwm_data1},

    { "iic0",  &iic_dev_ops, (void *)&sw_iic0_data },
    { "iic1",  &iic_dev_ops, (void *)&hw_iic1_data },

    /*{ "audio", &audio_dev_ops, (void *)&audio_data },*/

#if TCFG_SD0_ENABLE
    { "sd0",  &sd_dev_ops, (void *)&sd0_data },
#endif

#if TCFG_SD1_ENABLE
    { "sd1",  &sd_dev_ops, (void *)&sd1_data },
#endif

#ifdef CONFIG_VIDEO_ENABLE
    { "video0.*",  &video_dev_ops, (void *)&video0_data },
#endif

    { "spi1", &spi_dev_ops, (void *)&spi1_data },
    { "spi2", &spi_dev_ops, (void *)&spi2_data },

    {"rtc", &rtc_dev_ops, NULL},

    {"uart0", &uart_dev_ops, (void *)&uart0_data },
    {"uart1", &uart_dev_ops, (void *)&uart1_data },
    {"uart2", &uart_dev_ops, (void *)&uart2_data },

#if TCFG_USB_SLAVE_ENABLE || TCFG_USB_HOST_ENABLE
    { "otg", &usb_dev_ops, (void *)&otg_data},
#endif
};


/************************** LOW POWER config ****************************/
static const struct low_power_param power_param = {
    .config         = TCFG_LOWPOWER_LOWPOWER_SEL,
    .btosc_disable  = TCFG_LOWPOWER_BTOSC_DISABLE,         //??????????????????BTOSC????????????
    .vddiom_lev     = TCFG_LOWPOWER_VDDIOM_LEVEL,          //???VDDIO??????,?????????2.2V  2.4V  2.6V  2.8V  3.0V  3.2V  3.4V  3.6V
    .vddiow_lev     = TCFG_LOWPOWER_VDDIOW_LEVEL,          //???VDDIO??????,?????????2.1V  2.4V  2.8V  3.2V
    .vdc14_dcdc 	= TRUE,	   							   //????????????1.4VDCDC?????????????????????
    .vdc14_lev		= VDC14_VOL_SEL_LEVEL, 				   //VDD1.4V??????
    .sysvdd_lev		= SYSVDD_VOL_SEL_LEVEL,				   //?????????sdram????????????
	.vlvd_enable	= TRUE,                                //TRUE??????????????????
    .vlvd_value		= VLVD_SEL_25V,                        //????????????????????????
};

/************************** PWR config ****************************/
//#define PORT_VCC33_CTRL_IO		IO_PORTA_03					//VCC33 DCDC????????????,???????????????DCDC???????????????3.3V????????????HPVDD???VDDIO???VDD33
#define PORT_WAKEUP_IO			IO_PORTB_01					//??????????????????????????????
#define PORT_WAKEUP_NUM			(PORT_WAKEUP_IO/IO_GROUP_NUM)//??????:0-7:GPIOA-GPIOH, ????????????0-7???

static const struct port_wakeup port0 = {
    .edge       = FALLING_EDGE,                            //??????????????????,??????????????????\?????????
    .attribute  = BLUETOOTH_RESUME,                        //????????????
    .iomap      = PORT_WAKEUP_IO,                          //???????????????
    .low_power	= POWER_SLEEP_WAKEUP|POWER_OFF_WAKEUP,    //?????????IO??????,????????????0
};

static const struct long_press lpres_port = {
	.enable 	= FALSE,
	.use_sec4 	= TRUE,										//enable = TRUE , use_sec4: TRUE --> 4 sec , FALSE --> 8 sec
	.edge		= FALLING_EDGE,								//????????????,?????????FALLING_EDGE /  RISING_EDGE --> ?????????/?????????
	.iomap 		= PORT_WAKEUP_IO,							//????????????IO???IO??????????????????IO
};

static const struct sub_wakeup sub_wkup = {
    .attribute  = BLUETOOTH_RESUME,
};

static const struct charge_wakeup charge_wkup = {
    .attribute  = BLUETOOTH_RESUME,
};

static const struct wakeup_param wk_param = {
    .port[PORT_WAKEUP_NUM] = &port0,
    .sub = &sub_wkup,
    .charge = &charge_wkup,
	.lpres = &lpres_port,
};

/*???????????????????????????IO???????????????????????????????????????????????????????????????????????????*/
static void board_set_soft_poweroff(void)
{
    u32 IO_PORT;
    JL_PORT_FLASH_TypeDef *gpio[] = {JL_PORTA, JL_PORTB, JL_PORTC, JL_PORTD, JL_PORTE, JL_PORTF, JL_PORTG, JL_PORTH};

    for (u8 p = 0; p < 8; ++p) {
        //flash sdram PD PE PF PG?????????????????????,?????????????????????
        if (gpio[p] == JL_PORTD || gpio[p] == JL_PORTE || gpio[p] == JL_PORTF || gpio[p] == JL_PORTG) {
            continue;
        }
        for (u8 i = 0; i < 16; ++i) {
            IO_PORT = IO_PORTA_00 + p * 16 + i;
            gpio_set_pull_up(IO_PORT, 0);
            gpio_set_pull_down(IO_PORT, 0);
            gpio_set_direction(IO_PORT, 1);
            gpio_set_die(IO_PORT, 0);
            gpio_set_dieh(IO_PORT, 0);
            gpio_set_hd(IO_PORT, 0);
            gpio_set_hd1(IO_PORT, 0);
            gpio_latch_en(IO_PORT, 1);
        }
    }
#ifdef PORT_VCC33_CTRL_IO
    gpio_latch_en(PORT_VCC33_CTRL_IO, 0);
    gpio_direction_output(PORT_VCC33_CTRL_IO, 0);
    gpio_set_pull_up(PORT_VCC33_CTRL_IO, 0);
    gpio_set_pull_down(PORT_VCC33_CTRL_IO, 1);
    gpio_set_direction(PORT_VCC33_CTRL_IO, 1);
    gpio_set_die(PORT_VCC33_CTRL_IO, 0);
    gpio_set_dieh(PORT_VCC33_CTRL_IO, 0);
    gpio_latch_en(PORT_VCC33_CTRL_IO, 1);
#endif
}

static void sleep_exit_callback(u32 usec)
{
#ifdef PORT_VCC33_CTRL_IO
	gpio_direction_output(PORT_VCC33_CTRL_IO, 1);
	gpio_set_pull_up(PORT_VCC33_CTRL_IO, 1);
	gpio_set_pull_down(PORT_VCC33_CTRL_IO,0);
#endif
    /* putchar('-'); */
}

static void sleep_enter_callback(u8  step)
{
    /* ??????????????????????????? */

    if (step == 1) {
        /*dac_power_off();*/
    } else {
#ifdef PORT_VCC33_CTRL_IO
		gpio_direction_output(PORT_VCC33_CTRL_IO, 0);
		gpio_set_pull_up(PORT_VCC33_CTRL_IO, 0);
		gpio_set_pull_down(PORT_VCC33_CTRL_IO, 1);
		gpio_set_direction(PORT_VCC33_CTRL_IO, 1);
		gpio_set_die(PORT_VCC33_CTRL_IO, 0);
#endif
	}
}

static void board_power_init(void)
{
#ifdef PORT_VCC33_CTRL_IO
	gpio_direction_output(PORT_VCC33_CTRL_IO, 1);
    gpio_set_pull_up(PORT_VCC33_CTRL_IO, 1);
	gpio_set_pull_down(PORT_VCC33_CTRL_IO,0);
#endif

    power_init(&power_param);

    power_set_callback(TCFG_LOWPOWER_LOWPOWER_SEL, sleep_enter_callback, sleep_exit_callback, board_set_soft_poweroff);

    power_keep_state(POWER_KEEP_RESET);//0, POWER_KEEP_DACVDD | POWER_KEEP_RTC | POWER_KEEP_RESET

#ifdef CONFIG_RTC_ENABLE
    power_keep_state(POWER_KEEP_RTC);//0, POWER_KEEP_DACVDD | POWER_KEEP_RTC | POWER_KEEP_RESET
#endif

    power_wakeup_init(&wk_param);
}

#ifdef CONFIG_DEBUG_ENABLE
void debug_uart_init()
{
    uart_init(&uart2_data);
}
#endif

void board_early_init()
{
    devices_init();
}

void board_init()
{
    board_power_init();
#ifdef CONFIG_RTC_ENABLE
    rtc_early_init();
#endif
	adc_init();
}

