#include "asm/iic.h"
#include "asm/isp_dev.h"
#include "byd3a03.h"
#include "gpio.h"
#include "generic/jiffies.h"
#include "device/iic.h"
#include "device/device.h"
#include "app_config.h"


static void *iic = NULL;
static u8 BYD3A03_reset_io[2] = {-1, -1};
static u8 BYD3A03_power_io[2] = {-1, -1};

#if (CONFIG_VIDEO_IMAGE_W > 640)
#define BYD3A03_DEVP_INPUT_W 		640
#define BYD3A03_DEVP_INPUT_H		480
#else
#define BYD3A03_DEVP_INPUT_W 		CONFIG_VIDEO_IMAGE_W
#define BYD3A03_DEVP_INPUT_H		CONFIG_VIDEO_IMAGE_H
#endif

#define BYD3A03_WRCMD 0xdc
#define BYD3A03_RDCMD 0xdd


#define	BYD3A03_FPS_20 20
#define	BYD3A03_FPS_15 15
#define	BYD3A03_FPS_10 10

#define	BYD3A03_FPS_5  5

#define CONFIG_INPUT_FPS	BYD3A03_FPS_10

struct reginfo {
    u8 reg;
    u8 val;
};
static const struct reginfo sensor_init_data[] = {
#if 1
    {0x12, 0x80},
    {0x09, 0x55},
    {0x15, 0x02},
    {0x1e, 0x40},
    {0x06, 0x78},
    {0x21, 0x00},
    {0x3e, 0x37},
    {0x29, 0x2b},
    {0x27, 0x98},
    {0x16, 0x25},
    {0xd9, 0x24},
    {0xdf, 0x2f},
    {0x20, 0x00},
    {0xd2, 0x18},
    {0x11, 0x10},
    {0x1b, 0x09},
    {0x2f, 0x4e},
    {0x2b, 0x20},
    {0x92, 0x09},
    {0xcc, 0x77},
    {0xcd, 0x7b},
    {0xce, 0x82},
    {0xcf, 0x98},
    {0xc3, 0xa0},
    {0x3a, 0x02},
    {0x4a, 0x98},
    {0x12, 0x00},
    {0x13, 0x08},
    {0x01, 0x14},
    {0x02, 0x20},
    {0x8c, 0x02},
    {0x8d, 0x4c},
    {0x87, 0x16},
    {0x13, 0x07},
    {0x35, 0x46},
    {0x65, 0x38},
    {0x66, 0x42},
    {0x70, 0x0f},
    {0x3b, 0x00},
    {0x71, 0x0c},
    {0x73, 0x27},
    {0x75, 0x88},
    {0x76, 0xd8},
    {0x77, 0x0a},
    {0x78, 0xff},
    {0x79, 0x14},
    {0x7a, 0x24},
    {0x9e, 0x04},
    {0x7d, 0x2a},
    {0x39, 0xa0},
    {0x3f, 0xa0},
    {0x90, 0x20},
    {0x5f, 0x00},
    {0x40, 0x22},
    {0x41, 0x23},
    {0x42, 0x28},
    {0x43, 0x25},
    {0x44, 0x1d},
    {0x45, 0x17},
    {0x46, 0x13},
    {0x47, 0x12},
    {0x48, 0x10},
    {0x49, 0x0d},
    {0x4b, 0x0b},
    {0x4c, 0x0b},
    {0x4e, 0x09},
    {0x4f, 0x07},
    {0x50, 0x06},
    {0x24, 0x50},
    {0x97, 0x40},
    {0x25, 0x88},
    {0x81, 0x00},
    {0x82, 0x18},
    {0x83, 0x30},
    {0x84, 0x20},
    {0x85, 0x38},
    {0x86, 0x88},
    {0x94, 0x82},
    {0x80, 0x92},
    {0x98, 0x8a},
    {0x89, 0x1d},
    {0x8e, 0x2c},
    {0x8f, 0x82},
    {0x8a, 0x93},
    {0x8b, 0x7a},
    {0x96, 0xb5},
    {0x9a, 0x30},
    {0xf7, 0x8a},
    {0x5a, 0xec},
    {0x51, 0x90},
    {0x52, 0x10},
    {0x53, 0x8d},
    {0x54, 0x88},
    {0x57, 0x82},
    {0x58, 0x8d},
    {0x5a, 0x7c},
    {0x51, 0x80},
    {0x52, 0x04},
    {0x53, 0x8d},
    {0x54, 0x88},
    {0x57, 0x82},
    {0x58, 0x8d},
    {0x5a, 0x6c},
    {0x51, 0x93},
    {0x52, 0x04},
    {0x53, 0x8a},
    {0x54, 0x88},
    {0x57, 0x02},
    {0x58, 0x8d},
    {0xb0, 0xa0},
    {0xb1, 0x26},
    {0xb2, 0x1c},
    {0xb4, 0xfd},
    {0xb0, 0x30},
    {0xb1, 0xd8},
    {0xb2, 0xb0},
    {0xb4, 0xf1},
    {0x3c, 0x40},
    {0x56, 0x48},
    {0x4d, 0x40},
    {0x59, 0x40},
    {0xb4, 0xe1},
    {0x56, 0x40},
    {0x6a, 0x81},
    {0x23, 0x33},
    {0xa2, 0x0b},
    {0xa3, 0x26},
    {0xa4, 0x08},
    {0xa5, 0x26},
    {0xa7, 0x1a},
    {0xa8, 0x14},
    {0xa9, 0x14},
    {0xaa, 0x14},
    {0xab, 0x16},
    {0xac, 0x44},
    {0xad, 0xf0},
    {0xae, 0x57},
    {0xc5, 0x66},
    {0xc7, 0x38},
    {0xc8, 0x0d},
    {0xc9, 0x16},
    {0xd3, 0x09},
    {0xd4, 0x15},
    {0xd0, 0x00},
    {0xd1, 0x00},
#else
    //[Addr][Data]
    //{0x12,0x80},
    {0x09, 0x55},
    {0x15, 0x02},
    {0x1e, 0x40}, //HV mirror

    //Analog signals
    {0x06, 0x78},
    {0x21, 0x00},
    {0x3e, 0x37},
    {0x29, 0x2b},
    {0x27, 0x98},

    //Clock
    {0x2f, 0x4e}, //有绿色雪花点时，写为{0x42},
    {0x11, 0x10}, //分频[1:0]
    {0x1b, 0x09}, //倍频 PLL使能与{0x2f[1]、{0x1b[3]

    //数据输出格式
    {0x4a, 0x18}, //Sub、Win
    /*{0x4a,0x98},//Sub、Win*/
    {0x12, 0x00}, //YUV、sub时序
    {0x3a, 0x02}, //YUV时序

    //Manual
    {0x13, 0x08}, //手写bit[3]GLB GAIN0
    //{0x01,0x14},
    //{0x02,0x20},
    {0x8c, 0x02},
    {0x8d, 0x4c},
    {0x87, 0x16}, //GLB GAIN0

    //Auto
    {0x13, 0x07},

    //Denoise
    {0x70, 0x0f}, //插值模块开关
    {0x3b, 0x00}, //去格子噪声
    {0x71, 0x0e}, //插值模块开关
    {0x73, 0x36}, //Denoise
    {0x75, 0x88}, //Outdoor denoise
    {0x76, 0xd8},
    {0x77, 0x0a}, //Low light denoise
    {0x78, 0xff},
    {0x79, 0x14},
    {0x7a, 0x24}, //边缘增强
    {0x9e, 0x04},
    {0x7d, 0x2a}, //去紫边

    //Gamma default
    {0x39, 0xA0}, //Gamma offset
    {0x3f, 0xA0},
    {0x90, 0x20},
    {0x5f, 0x01}, //Dark_sel gamma
    {0x40, 0x22},
    {0x41, 0x23},
    {0x42, 0x28},
    {0x43, 0x25},
    {0x44, 0x1d},
    {0x45, 0x17},
    {0x46, 0x13},
    {0x47, 0x12},
    {0x48, 0x10},
    {0x49, 0x0d},
    {0x4b, 0x0b},
    {0x4c, 0x0b},
    {0x4e, 0x09},
    {0x4f, 0x07},
    {0x50, 0x06},

    /*//Gamma 低噪
    {0x40,0x24},
    {0x41,0x30},
    {0x42,0x24},
    {0x43,0x1d},
    {0x44,0x1a},
    {0x45,0x14},
    {0x46,0x11},
    {0x47,0x0e},
    {0x48,0x0d},
    {0x49,0x0c},
    {0x4b,0x0b},
    {0x4c,0x09},
    {0x4e,0x09},
    {0x4f,0x08},
    {0x50,0x07},

    //Gamma 过曝过度好
    {0x40,0x18},
    {0x41,0x2c},
    {0x42,0x28},
    {0x43,0x20},
    {0x44,0x16},
    {0x45,0x10},
    {0x46,0x0f},
    {0x47,0x0f},
    {0x48,0x0e},
    {0x49,0x0a},
    {0x4b,0x0b},
    {0x4c,0x09},
    {0x4e,0x09},
    {0x4f,0x08},
    {0x50,0x06},

    //Gamma 清晰亮丽
    {0x40,0x19},
    {0x41,0x1e},
    {0x42,0x1f},
    {0x43,0x20},
    {0x44,0x1d},
    {0x45,0x19},
    {0x46,0x17},
    {0x47,0x17},
    {0x48,0x14},
    {0x49,0x12},
    {0x4b,0x0f},
    {0x4c,0x0c},
    {0x4e,0x08},
    {0x4f,0x06},
    {0x50,0x03},*/

    //AE
    {0x24, 0x48}, //非A光Y_target
    {0x97, 0x40}, //A光Y_target
    {0x25, 0x88}, //AE_LOC
    {0x81, 0x00}, //AE speed
    {0x82, 0x1b},
    {0x83, 0x37},
    {0x84, 0x39},
    {0x85, 0x5d},
    {0x86, 0x99},
    {0x94, 0x82}, //对AE亮度权重调节
    {0x80, 0x96}, //对过曝点数调节,bit[1]choose 50HZ or 60HZ step},
    {0x98, 0x8a}, //对窗口权重调节//8a
    {0x89, 0x7d}, //帧率调节bit[7:3]
    {0x8e, 0x2c},
    {0x8f, 0x82},

    //Banding
    {0x2b, 0x20},
    {0x8a, 0x93}, //50HZ
    {0x8b, 0x7a}, //60HZ
    {0x92, 0x6D},

    //Color
    {0x5a, 0xec}, //Outdoor color
    {0x51, 0x90},
    {0x52, 0x10},
    {0x53, 0x8d},
    {0x54, 0x88},
    {0x57, 0x82},
    {0x58, 0x8d},
    {0x5a, 0x7c}, //A light color
    {0x51, 0x80},
    {0x52, 0x04},
    {0x53, 0x8d},
    {0x54, 0x88},
    {0x57, 0x82},
    {0x58, 0x8d},

    //Color default
    {0x5a, 0x6c}, //Indoor color
    {0x51, 0x90},
    {0x52, 0x04},
    {0x53, 0x8a},
    {0x54, 0x88},
    {0x57, 0x02},
    {0x58, 0x8d},
    /*
    	{0x51,0x93},
    	{0x52,0x04},
    	{0x53,0x8a},
    	{0x54,0x88},
    	{0x57,0x02},
    	{0x58,0x8d},
    */
    /*//Color色彩艳丽
    {0x5a,0x6c},//Indoor color
    {0x51,0xa0},
    {0x52,0x01},
    {0x53,0x8d},
    {0x54,0x85},
    {0x57,0x01},
    {0x58,0x90},

    //Color肤色较好
    {0x5a,0x6c},//Indoor color
    {0x51,0x90},
    {0x52,0x0a},
    {0x53,0x84},
    {0x54,0x05},
    {0x57,0x05},
    {0x58,0x87},

    //Color色彩淡
    {0x5a,0x6c},//Indoor color
    {0x51,0x85},
    {0x52,0x06},
    {0x53,0x8a},
    {0x54,0x81},
    {0x57,0x02},
    {0x58,0x8a},*/

    //Saturation
    {0xb0, 0xa0}, //A光饱和度
    {0xb1, 0x26},
    {0xb2, 0x1c},
    {0xb4, 0xfd},
    {0xb0, 0x30}, //非A光饱和度
    {0xb1, 0xd8},
    {0xb2, 0xb0},
    {0xb4, 0x71},

    //Contrast
    {0x3c, 0x40}, //K1
    {0x56, 0x48}, //K2 {0xb4[4]选择new or old
    {0x4d, 0x40}, //K3
    {0x59, 0x40}, //K4

    //G gain大一些
    {0x35, 0x40}, //shading R
    {0x65, 0x44}, //shading G
    {0x66, 0x44}, //shading B
    //AWB
    {0x6a, 0xd1},
    {0x23, 0x22},
    {0xa2, 0x04},
    {0xa3, 0x13},
    {0xa4, 0x04},
    {0xa5, 0x20},
    {0xa7, 0x1a},
    {0xa8, 0x10},
    {0xa9, 0x1f},
    {0xaa, 0x16},
    {0xab, 0x16},
    {0xac, 0x30},
    {0xad, 0xf0},
    {0xae, 0x57},
    {0xc5, 0xaa},
    {0xc7, 0x38},
    {0xc8, 0x0d},
    {0xc9, 0x16},
    {0xd3, 0x09},
    {0xd4, 0x15},
    {0xd0, 0x00},
    {0xd1, 0x00},
    {0xd2, 0x18},
    {0xee, 0x4c},

    /*//G gain小一些
    {0x35,0x46},//shading R
    {0x65,0x3a},//shading G
    {0x66,0x40},//shading B
    //AWB
    {0x6a,0xd1},//AWB
    {0x23,0x00},//G GAIN
    {0xa2,0x08},//非户外 B上下线
    {0xa3,0x26},
    {0xa4,0x04},//非户外 R上下线
    {0xa5,0x26},
    {0xa7,0x13},//B GAIN
    {0xa8,0x8e},//R GAIN
    {0xa9,0x16},//白点
    {0xaa,0x16},
    {0xab,0x16},
    {0xac,0x30},
    {0xad,0xf0},
    {0xae,0x57},
    {0xc5,0x66},
    {0xc7,0x38},//户内、外开关bit[4]
    {0xc8,0x0d},//非户外 B上下线
    {0xc9,0x16},
    {0xd3,0x09},//非户外 R上下线
    {0xd4,0x15},
    {0xd0,0x00},//A光下的offset
    {0xd1,0x01},//非A光下的offset
    {0xd2,0x58},
    */
#endif
};

static s32 BYD3A03_set_output_size(u16 *width, u16 *height, u8 *freq);
static unsigned char wrBYD3A03Reg(unsigned char regID, unsigned char regDat)
{
    u8 ret = 1;
    dev_ioctl(iic, IIC_IOCTL_START, 0);
    if (dev_ioctl(iic, IIC_IOCTL_TX_WITH_START_BIT, BYD3A03_WRCMD)) {
        ret = 0;
        printf("iic write err!!! line : %d \n", __LINE__);
        goto exit;
    }
    delay(1000);
    if (dev_ioctl(iic, IIC_IOCTL_TX, regID)) {
        ret = 0;
        printf("iic write err!!! line : %d \n", __LINE__);
        goto exit;
    }
    delay(1000);
    if (dev_ioctl(iic, IIC_IOCTL_TX_WITH_STOP_BIT, regDat)) {
        ret = 0;
        printf("iic write err!!! line : %d \n", __LINE__);
        goto exit;
    }
    delay(1000);

exit:
    dev_ioctl(iic, IIC_IOCTL_STOP, 0);
    return ret;
}

static unsigned char rdBYD3A03Reg(unsigned char regID, unsigned char *regDat)
{
    u8 ret = 1;
    dev_ioctl(iic, IIC_IOCTL_START, 0);
    if (dev_ioctl(iic, IIC_IOCTL_TX_WITH_START_BIT, BYD3A03_WRCMD)) {
        ret = 0;
        printf("iic write err!!! line : %d \n", __LINE__);
        goto exit;
    }
    delay(1000);
    if (dev_ioctl(iic, IIC_IOCTL_TX_WITH_STOP_BIT, regID)) {
        ret = 0;
        printf("iic write err!!! line : %d \n", __LINE__);
        goto exit;
    }
    delay(1000);
    if (dev_ioctl(iic, IIC_IOCTL_TX_WITH_START_BIT, BYD3A03_RDCMD)) {
        ret = 0;
        printf("iic write err!!! line : %d \n", __LINE__);
        goto exit;
    }
    delay(1000);
    dev_ioctl(iic, IIC_IOCTL_RX_WITH_STOP_BIT, (u32)regDat);

exit:
    dev_ioctl(iic, IIC_IOCTL_STOP, 0);
    return ret;
}

static void BYD3A03_config_SENSOR(u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    int i;
    for (i = 0; i < sizeof(sensor_init_data) / sizeof(sensor_init_data[0]); i++) {
        wrBYD3A03Reg(sensor_init_data[i].reg, sensor_init_data[i].val);
    }
    *format = SEN_IN_FORMAT_UYVY;
    *frame_freq = CONFIG_INPUT_FPS;
}
static s32 BYD3A03_set_output_size(u16 *width, u16 *height, u8 *freq)
{
    u16 liv_width = *width;
    u16 liv_height = *height;

    printf("BYD3A03 : %d , %d \n", *width, *height);
    return 0;
}
static s32 BYD3A03_power_ctl(u8 isp_dev, u8 is_work)
{
    return 0;
}

static s32 BYD3A03_ID_check(void)
{
    u16 pid = 0x00;
    u8 id0, id1;
    rdBYD3A03Reg(0xfc, &id0);
    delay(40000);
    rdBYD3A03Reg(0xfd, &id1);
    delay(40000);
    pid = (u16)((id0 << 8) | id1);
    printf("BYD3A03 Sensor ID : 0x%x\n", pid);
    if (pid != 0x3a03) {
        return -1;
    }

    return 0;
}

static void BYD3A03_powerio_ctl(u32 _power_gpio, u32 on_off)
{
    gpio_direction_output(_power_gpio, on_off);
}
static void BYD3A03_reset(u8 isp_dev)
{
    u8 res_io;
    u8 powd_io;
    u8 id = 0;
    puts("BYD3A03 reset\n");

    if (isp_dev == ISP_DEV_0) {
        res_io = (u8)BYD3A03_reset_io[0];
        powd_io = (u8)BYD3A03_power_io[0];
    } else {
        res_io = (u8)BYD3A03_reset_io[1];
        powd_io = (u8)BYD3A03_power_io[1];
    }

    if (powd_io != (u8) - 1) {
        BYD3A03_powerio_ctl((u32)powd_io, 0);
    }
    if (res_io != (u8) - 1) {
        gpio_direction_output(res_io, 1);
        delay(40000);
        delay(40000);
        gpio_direction_output(res_io, 0);
        delay(40000);
        delay(40000);
        gpio_direction_output(res_io, 1);
        delay(40000);
        delay(40000);
    }
}


static s32 BYD3A03_check(u8 isp_dev, u32 _reset_gpio, u32 _power_gpio)
{
    if (!iic) {
        if (isp_dev == ISP_DEV_0) {
            iic = dev_open("iic0", 0);
        } else {
            iic = dev_open("iic1", 0);
        }
        BYD3A03_reset_io[isp_dev] = (u8)_reset_gpio;
        BYD3A03_power_io[isp_dev] = (u8)_power_gpio;
    }
    if (iic == NULL) {
        printf("BYD3A03 iic open err!!!\n\n");
        return -1;
    }
    BYD3A03_reset(isp_dev);

    if (0 != BYD3A03_ID_check()) {
        dev_close(iic);
        iic = NULL;
        printf("-------not BYD3A03------\n\n");
        return -1;
    }
    printf("-------hello BYD3A03------\n\n");
    return 0;
}


static s32 BYD3A03_init(u8 isp_dev, u16 *width, u16 *height, u8 *format, u8 *frame_freq)
{
    puts("\n\n BYD3A03 \n\n");

    if (0 != BYD3A03_check(isp_dev, 0, 0)) {
        return -1;
    }
    BYD3A03_config_SENSOR(width, height, format, frame_freq);

    return 0;
}
void set_rev_sensor_BYD3A03(u16 rev_flag)
{
    if (!rev_flag) {
        wrBYD3A03Reg(0x1e, 0x30);
    } else {
        wrBYD3A03Reg(0x1e, 0x00);
    }
}

u16 BYD3A03_dvp_rd_reg(u16 addr)
{
    u8 val;
    rdBYD3A03Reg((u8)addr, &val);
    return val;
}

void BYD3A03_dvp_wr_reg(u16 addr, u16 val)
{
    wrBYD3A03Reg((u8)addr, (u8)val);
}

// *INDENT-OFF*
REGISTER_CAMERA(BYD3A03) = {
    .logo 				= 	"BYD3A03",
    .isp_dev 			= 	ISP_DEV_NONE,
    .in_format 			= 	SEN_IN_FORMAT_UYVY,
    .mbus_type          =   SEN_MBUS_PARALLEL,
    .mbus_config        =   SEN_MBUS_DATA_WIDTH_8B  | SEN_MBUS_HSYNC_ACTIVE_HIGH | \
    						SEN_MBUS_PCLK_SAMPLE_FALLING | SEN_MBUS_VSYNC_ACTIVE_HIGH,
#if (defined CONFIG_BOARD_7916A)
    .sync_config		=   SEN_MBUS_SYNC0_VSYNC_SYNC1_HSYNC,//WL82/AC791才可以H-V SYNC互换，请留意
#endif
    .fps         		= 	CONFIG_INPUT_FPS,
    .out_fps			=   CONFIG_INPUT_FPS,
    .sen_size 			= 	{BYD3A03_DEVP_INPUT_W, BYD3A03_DEVP_INPUT_H},
    .cap_fps         	= 	CONFIG_INPUT_FPS,
    .sen_cap_size 		= 	{BYD3A03_DEVP_INPUT_W, BYD3A03_DEVP_INPUT_H},
    .ops                =   {
        .avin_fps           =   NULL,
        .avin_valid_signal  =   NULL,
        .avin_mode_det      =   NULL,
        .sensor_check 		= 	BYD3A03_check,
        .init 		        = 	BYD3A03_init,
        .set_size_fps 		=	BYD3A03_set_output_size,
        .power_ctrl         =   BYD3A03_power_ctl,
        .sleep 		        =	NULL,
        .wakeup 		    =	NULL,
        .write_reg 		    =	BYD3A03_dvp_wr_reg,
        .read_reg 		    =	BYD3A03_dvp_rd_reg,
        .set_sensor_reverse =   set_rev_sensor_BYD3A03,
    }
};


