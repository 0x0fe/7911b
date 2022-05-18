#include "app_config.h"
#include "system/includes.h"
#include "os/os_api.h"
#include "asm/clock.h"

#define TEMP_SAMPLE_TIMER   (500) //�¶Ȳ���ʱ��ms,�������100ms����
#define TEMP_MAX            (125) //������������ʾ�¶�
#define TEMP_MIN            (-55) //������������ʾ�¶�
#define TEMP_FILTER_CNT     (20)  //����ʱ�������ݴ���(����ʱ�����ݲ�����������Ҫ�򿪹��ˣ����˴������������)
#define TEMP_RANK_NUM       (9)  //��ֵ�˲�ʹ���¶����ݸ�����ð��������˴�Сֵ�����ȡ�м�ֵ
#define SENSOR_CHECK_CNT    (5)  //���������ʧ�ܴ�����Ϊ����������

#define USER_FUNC_INT_SRAM  1   //�жϴ������Ƿ�����ڲ�sram

#if USER_FUNC_INT_SRAM
#define FUNC_IN SEC_USED(.volatile_ram_code)
#else
#define FUNC_IN
#endif
#define AWINLINE   __attribute__((always_inline))


//������Ӳ��IO����
#define wire_1_bit      BIT(7) //IO�ڣ�ֻ�ܼĴ�������
#define DQ_INIT         {JL_PORTA->DIR &= ~wire_1_bit;JL_PORTA->PU &= ~wire_1_bit;JL_PORTA->PD &= ~wire_1_bit;}//��عر��ڲ�������
#define DQ_OUT          {JL_PORTA->DIR &= ~wire_1_bit;}
#define DQ_IN           {JL_PORTA->DIR |= wire_1_bit;}
#define DQ_R            ((JL_PORTA->IN & wire_1_bit) ? 1 : 0)
#define DQ_W_H         JL_PORTA->OUT  |= wire_1_bit
#define DQ_W_L         JL_PORTA->OUT  &=~ wire_1_bit


//#define db_io           BIT(0) //IO�ڣ�ֻ�ܼĴ�������
//#define DB_INIT         {JL_PORTH->DIR &= ~db_io;JL_PORTH->HD |= db_io;}
//#define DB_H            JL_PORTH->OUT  |= db_io
//#define DB_L            JL_PORTH->OUT  &=~ db_io
//#define DB_AND          JL_PORTH->OUT  ^= db_io


enum {
    TMP_ERR = 0,
    TMP_NONE,
    TMP_INIT_START,
    TMP_INIT_ACK,
    TMP_INIT_ERR,
    TMP_INIT_OK,

    TMP_WTITE_BYTE_START,
    TMP_WTITE_BYTE_ERR,
    TMP_WTITE_BYTE_OK,

    TMP_READ_BYTE_START,
    TMP_READ_BYTE_ERR,
    TMP_READ_BYTE_OK,
};

enum read_temp_step {
    STEP_SEND_INIT = 0,
    STEP_SEND_0XCC1,
    STEP_SEND_0X44,
    STEP_SEND_REINIT,
    STEP_SEND_0XCC2,
    STEP_SEND_0XBE,
    STEP_READ_BYT_L,
    STEP_READ_BYT_H,
    STEP_READ_DATA_OK,
    STEP_READ_REDO,
};

enum read_temp_am2301_step {
    STEP_AM_SEND_INIT = 0,
    STEP_AM_SEND_READ,
    STEP_AM_SEND_REINIT,
    STEP_AM_READ_DATA_OK,
    STEP_AM_READ_REDO,
};

enum sensor_type {
    SENSOR_ERR = -1,
    SENSOR_NONE = 0,
    SENSOR_DS1B820,
    SENSOR_AM2301,
};



struct temp_strut {
    volatile u8 mode_type;
    volatile u8 mode_cnt;
    volatile u8 byte_num;
    volatile u8 temp_h;
    volatile u8 temp_l;
    volatile enum read_temp_step read_step;
    volatile u8 write_data;
    volatile u8 read_data;
    volatile u8 wr_bit;
    u16 temp;
    u16 filter_cnt;
    u8 ftemp_rank_id;
    float ftemp;//�¶�
    float fhumidity;//ʪ��
    volatile float ftemp_rank[TEMP_RANK_NUM];
    volatile u8 recv_data[5];//D��ʪ�ȴ�����
    u8 is_ds18b20;//DS1B820�¶ȼ�
    u8 timer_close;
    char sensor_type;
    u8 sensor_check;
    u32 ready_readed_tocnt;//û�б������ݳ�ʱ����
} temp_info = {0};

static JL_TIMER_TypeDef *TMR = JL_TIMER4;//ѡ��ʱ��4
static u8 timer_irq = IRQ_TIMER4_IDX;//ѡ��ʱ��4
static const u8 timer_index[16] =  {0, 4, 1, 5, 2,  6,  3,  7,   8,   12,  9,    13,   10,   14,   11,    15};
static const u32 timer_table[16] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
static u32 timer_clk = 0;
static FUNC_IN ___interrupt void ds18b20_timer_isr(void);
static FUNC_IN ___interrupt void am2301_timer_isr(void);

static AWINLINE float temp_rank(float *buff, u8 lenth)//up rank
{
    u8 i, j;
    float temp;
    for (i = 0; i < lenth - 1; i++) {
        for (j = 0; j < lenth - 1 - i; j++) {
            if (buff[j] > buff[j + 1]) {
                temp = buff[j + 1];
                buff[j + 1] = buff[j];
                buff[j] = temp;
            }
        }
    }
    temp = (lenth % 2) ? buff[lenth / 2] : (buff[lenth / 2] + buff[lenth / 2 - 1]) / 2;
    return temp;//����������м���ֵ��ż���������м�2��ֵƽ��ֵ
}
static AWINLINE void timer_cfg(u32 freq, u32 us)
{
    u32 clock = timer_clk;
    u8 psc = 0;
    u8 tmp = 0;
    u32 prd = 0;
    u32 ts = us / (1000 * 1000);//������
    u32 tu = us % (1000 * 1000);//������ʣ��us
    u8 i;
    float tp = 0;

    if (freq >= clock) {
        freq = clock;
    } else if (freq <= 1) {
        freq = 1;
        if (ts) {
            tp = (float)tu / (1000 * 1000);
        }
    }
    /*��prdֵ*/
    prd = clock / freq;
    if (prd > 65535) {
        for (psc = 0; psc < 16; psc++) {
            prd = (u32)(clock / (timer_table[psc]) / freq);
            if (prd <= 65535) {
                break;
            } else if (psc == 15) {
                prd = 65535;
                break;
            }
        }
    }
    prd = ts ? (prd * ts + tp * prd) : prd;
    psc = timer_index[psc];
    TMR->CON = 0;
    TMR->CNT = 0;
    TMR->CON |= BIT(14);
    TMR->PRD = prd;
    TMR->CON |= psc << 4; //lsb_clk��Ƶ
    TMR->CON |= BIT(0);
}
static AWINLINE void timer_init(u32 us)
{
    u32 freq = 1000000 / us;
    timer_cfg(freq, us);
}
/*****************************************************************************
                DS18B20��ʼ��
*****************************************************************************/
static AWINLINE void temp_mode_reinit(void)
{
    temp_info.byte_num = 0;
    temp_info.mode_cnt = 0;
    temp_info.wr_bit = 0;
    temp_info.timer_close = 0;
    temp_info.mode_type = TMP_NONE;
}
static AWINLINE void ds18b20_timer_exit(void)
{
    if (!(TMR->CON & BIT(15))) {
        TMR->CON &= ~BIT(0);
    } else {
        temp_info.timer_close = TRUE;
    }
}
static AWINLINE void ds18b20_timer_init(void)
{
    timer_clk = clk_get("timer");
    temp_info.sensor_check = 0;
    temp_info.sensor_type = SENSOR_NONE;
    temp_info.mode_type = TMP_NONE;
    temp_info.read_step = STEP_SEND_INIT;
    temp_mode_reinit();
    request_irq(timer_irq, 3, ds18b20_timer_isr, 0);
    timer_init(50 * 1000); //��ʼ��50ms���һ���豸
}
static AWINLINE void ds18b20_io_init(void)
{
    DQ_OUT;
    DQ_W_H;
//    printf("ds18b20_io_init\n");
}
static AWINLINE void temp_redo_next_time(u32 us)
{
    DQ_OUT;
    DQ_W_H;
    timer_init(us);//�´μ���¶�ʱ��
}
static AWINLINE void temp0_reset(void)//�ߵ�ƽ�����ź�
{
    DQ_OUT;
    timer_init(10 * 1000); //10ms�����ź�
    DQ_W_H;
}
static AWINLINE void temp1_reset_start(void)//������λ�ź�
{
    timer_init(600);//����600us��λ�ź�
    DQ_W_L;
}
static AWINLINE void temp2_release_bus(void)
{
    timer_init(65);//�ͷ����ߣ��ȴ�60us  (15 - 60us)
    DQ_IN;
}
static AWINLINE u8 temp3_reset_get_ack(void)//��λ�źŴӻ���ACK�Ƿ�����
{
    u8 ack = !DQ_R;
    timer_init(480);//�ȴ������480us
    if (!ack) { //���ACK���
        temp_redo_next_time(50 * 1000); //���ʧ�ܣ�����һ�μ�⣬50ms
    }
    return ack;
}
static AWINLINE void temp4_reset_end(void)
{
    DQ_OUT;
    DQ_W_H;
    timer_init(10);//10us���д�ֽ�
}
static FUNC_IN u8 ds18b20_init(void)
{
    u8 ret;
    if (temp_info.mode_type == TMP_INIT_OK) {
        return TMP_INIT_OK;
    }
    temp_info.mode_type = (temp_info.mode_type == TMP_INIT_ACK) ? temp_info.mode_type : TMP_INIT_START;
    switch (temp_info.mode_cnt) {
    case 0:
        temp0_reset();
        break;
    case 1:
        temp1_reset_start();
        break;
    case 2:
        temp2_release_bus();
        break;
    case 3:
        ret = temp3_reset_get_ack();
        temp_info.mode_type = ret ? TMP_INIT_ACK : TMP_INIT_ERR;
        if (!ret) {
            temp_info.mode_cnt = 0;
            if (++temp_info.sensor_check > SENSOR_CHECK_CNT) {
                temp_info.sensor_type = SENSOR_ERR;
                temp_info.sensor_check = 0;
            }
        }
        break;
    case 4:
        temp4_reset_end();
        temp_info.mode_type = TMP_INIT_OK;
        temp_info.mode_cnt = 0;
        temp_info.sensor_check = 0;
        temp_info.sensor_type = SENSOR_DS1B820;
        return temp_info.mode_type;
    default:
        break;
    }
    temp_info.mode_cnt++;
    return temp_info.mode_type;
}

/*****************************************************************************
                DS18B20��д�ֽ�
*****************************************************************************/
static AWINLINE void temp_write_start_bit(void)
{
    DQ_OUT;
    timer_init(10);//���15usҪ����
    DQ_W_L;
}
static AWINLINE void temp_write_end_bit(u8 bit)
{
    timer_init(60);//��ʱ60us��60 - 120us
    if (bit) {
        DQ_W_H;
    } else {
        DQ_W_L;
    }
}
static AWINLINE void temp_write_bit_complet(void)
{
    timer_init(10);//5us�������ддһ��bit��> 1us
    DQ_W_H;
}

static AWINLINE void temp_read_start_bit(void)
{
    DQ_OUT;
    timer_init(1);//Ҫ�������1us
    DQ_W_L;
}
static AWINLINE void temp_read_release_bus(void)
{
    DQ_IN;
    timer_init(10);//15us֮�ڲ���
}
static AWINLINE u8 temp_read_end_bit(void)
{
    u8 bit;
    bit = DQ_R;
    timer_init(60);//��ʱ60us��60 - 120us
    return bit;
}
static AWINLINE void temp_read_bit_complet(void)
{
    timer_init(10);//10us�������ддһ��bit��> 1us
    DQ_OUT;
    DQ_W_H;
}
static FUNC_IN u8 ds18b20_write_byte(u8 byte)
{
    if (temp_info.mode_type == TMP_WTITE_BYTE_OK) {
        return TMP_WTITE_BYTE_OK;
    }
    if (!temp_info.mode_cnt && !temp_info.wr_bit) {
        temp_info.write_data = byte;
    }
    temp_info.mode_type = TMP_WTITE_BYTE_START;
    switch (temp_info.wr_bit) {
    case 0:
        temp_write_start_bit();
        break;
    case 1:
        temp_write_end_bit(temp_info.write_data & 0x1);
        break;
    case 2:
        temp_write_bit_complet();
        temp_info.write_data >>= 1;
    default:
        break;
    }
    temp_info.wr_bit++;
    if (temp_info.wr_bit > 2) { //�������0-2
        temp_info.mode_cnt++;
        temp_info.wr_bit = 0;
    }
    if (temp_info.mode_cnt >= 8) {
        temp_info.mode_type = TMP_WTITE_BYTE_OK;
        temp_info.mode_cnt = 0;
        temp_info.wr_bit = 0;
        temp_info.write_data = 0;
    }
    return temp_info.mode_type;
}
static FUNC_IN u8 ds18b20_read_byte(void)
{
    u8 ret;
    if (temp_info.mode_type == TMP_READ_BYTE_OK) {
        return TMP_READ_BYTE_OK;
    }
    if (!temp_info.mode_cnt && !temp_info.wr_bit) {
        temp_info.read_data = 0;
    }
    temp_info.mode_type = TMP_READ_BYTE_START;
    switch (temp_info.wr_bit) {
    case 0:
        temp_read_start_bit();
        temp_info.read_data >>= 1;
        break;
    case 1:
        temp_read_release_bus();
        break;
    case 2:
        ret = temp_read_end_bit();
        temp_info.read_data |= ret ? 0x80 : 0x0;
        break;
    case 3:
        temp_read_bit_complet();
    default:
        break;
    }
    temp_info.wr_bit++;
    if (temp_info.wr_bit > 3) { //�������0-3
        temp_info.mode_cnt++;
        temp_info.wr_bit = 0;
    }
    if (temp_info.mode_cnt >= 8) {
        temp_info.mode_type = TMP_READ_BYTE_OK;
        temp_info.mode_cnt = 0;
        temp_info.wr_bit = 0;
        temp_info.write_data = 0;
    }
    return temp_info.mode_type;
}
u8 ds18b20_temp_read(float *temp_buf)//�̶߳�ȡ�¶����ݣ�temp_buf ���¶����ݴ洢��ַ
{
    if (temp_info.read_step == STEP_READ_DATA_OK) { //ds18b20�¶ȶ�ȡ����
        *temp_buf = temp_info.ftemp;
        temp_info.read_step = STEP_READ_REDO;//���¶�ȡ�¶ȱ��
        return 1;
    }
    return 0;
}
/*****************************************************************************
                DS18B20�жϴ�����
*****************************************************************************/
static FUNC_IN ___interrupt void ds18b20_timer_isr(void)
{
    if (TMR->CON & BIT(15)) {
        //��ȡ�¶Ȳ���
        switch (temp_info.read_step) {
        case STEP_SEND_INIT:
            ds18b20_init();             //1.��ʼ��
            break;
        case STEP_SEND_0XCC1:
            ds18b20_write_byte(0xCC);   //2,����ROMָ��
            break;
        case STEP_SEND_0X44:
            ds18b20_write_byte(0x44);   //3.�¶�ת��ָ��
            break;
        case STEP_SEND_REINIT:
            ds18b20_init();             //4.��ʼ��
            break;
        case STEP_SEND_0XCC2:
            ds18b20_write_byte(0xCC);   //5.����ROMָ��
            break;
        case STEP_SEND_0XBE:
            ds18b20_write_byte(0xBE);   //6.���Ĵ���ָ��
            break;
        case STEP_READ_BYT_L:
            ds18b20_read_byte();        //7.��ȡ���ֽ�
            break;
        case STEP_READ_BYT_H:
            ds18b20_read_byte();        //8.��ȡ���ֽ�
            break;
        case STEP_READ_REDO:
            temp_info.read_step = STEP_SEND_INIT;
            if (temp_info.filter_cnt < TEMP_FILTER_CNT || (temp_info.ftemp_rank_id && temp_info.ftemp_rank_id < TEMP_RANK_NUM)) { //����ʱ���ˣ��洢������BUFF���ٶ�ȡ
                timer_init(2 * 1000); //2ms���ٹ���
            } else {
                timer_init(TEMP_SAMPLE_TIMER * 1000); //�´μ���¶�ʱ��
            }
            break;
        default:
            break;
        }

        //��дһ���ֽڽ������
        switch (temp_info.mode_type) {
        case TMP_INIT_OK:
            if (temp_info.read_step == STEP_SEND_INIT) {
                temp_info.read_step = STEP_SEND_0XCC1;
            } else {
                temp_info.read_step = STEP_SEND_0XCC2;
            }
            temp_mode_reinit();
            break;
        case TMP_WTITE_BYTE_OK:
            if (temp_info.read_step == STEP_SEND_0XCC1) {
                temp_info.read_step = STEP_SEND_0X44;
            } else if (temp_info.read_step == STEP_SEND_0X44) {
                temp_info.read_step = STEP_SEND_REINIT;
            } else if (temp_info.read_step == STEP_SEND_0XCC2) {
                temp_info.read_step = STEP_SEND_0XBE;
            } else if (temp_info.read_step == STEP_SEND_0XBE) {
                temp_info.read_step = STEP_READ_BYT_L;
            }
            temp_mode_reinit();
            break;
        case TMP_READ_BYTE_OK:
            if (temp_info.read_step == STEP_READ_BYT_L) {
                temp_info.read_step = STEP_READ_BYT_H;
                temp_info.temp_l = temp_info.read_data;
            } else {
                temp_info.temp_h = temp_info.read_data;
                temp_info.temp = (temp_info.temp_h << 8) | temp_info.temp_l;
                temp_info.ftemp = (float)temp_info.temp / 16;//* 0.0625;//�¶�ת��
                if ((temp_info.filter_cnt >= TEMP_FILTER_CNT || !TEMP_FILTER_CNT) && //������ɻ����޹���
                    (temp_info.ftemp > TEMP_MIN && temp_info.ftemp < TEMP_MAX)) { //ds18b20�¶�������Χ

                    temp_info.ftemp_rank[temp_info.ftemp_rank_id++] = temp_info.ftemp;
                    if (temp_info.ftemp_rank_id >= TEMP_RANK_NUM) {
                        temp_info.read_step = STEP_READ_DATA_OK;//��ȡ���
                        temp_info.ftemp = temp_rank(temp_info.ftemp_rank, TEMP_RANK_NUM);//��ֵ�˲�
                        temp_info.ftemp_rank_id = 0;
                    } else {
                        temp_info.read_step = STEP_READ_REDO;//���¼��
                    }
                    temp_redo_next_time(2 * 1000);
                } else {
                    if (TEMP_FILTER_CNT) {
                        temp_info.filter_cnt++;
                    }
                    temp_info.read_step = STEP_READ_REDO;//���¼��
                    temp_redo_next_time(10 * 1000); //10ms�����߳��Ƿ��ȡ�¶Ƚ��
                }
            }
            temp_mode_reinit();
            break;
        case TMP_ERR:
        case TMP_INIT_ERR:
        case TMP_WTITE_BYTE_ERR:
        case TMP_READ_BYTE_ERR:
            temp_info.read_step = STEP_SEND_INIT;
            temp_info.filter_cnt = 0;
            temp_info.ftemp_rank_id = 0;
            temp_mode_reinit();
            temp_redo_next_time(50 * 1000); //�´μ���¶�
            break;
        default :
            break;
        }

        if (temp_info.timer_close) {
            temp_info.timer_close = 0;
            TMR->CON &= ~BIT(0);
        }
        TMR->CON |= BIT(14);
    }
}
/*************************************************************************************************************************************************************/


/*****************************************************************************
                AM2301��ʼ��
*****************************************************************************/
#define AM2301_DATA_L_TIMER     58 //����0/1�͵�ƽʱ��58 - 62 us��ȡ��Сֵ��ʾ����ץ�����ģ��ĵ�����Ӧ
#define AM2301_DATA1_H_TIMER    72 //����1�ߵ�ƽʱ��72 - 76 us��ȡ��Сֵ��ʾ����ץ�����ģ��ĵ�����Ӧ

static AWINLINE void am2301_timer_init(void)
{
    timer_clk = clk_get("timer");
    temp_info.sensor_check = 0;
    temp_info.sensor_type = SENSOR_NONE;
    temp_info.mode_type = TMP_NONE;
    temp_info.read_step = STEP_AM_SEND_INIT;
    temp_mode_reinit();
    request_irq(timer_irq, 3, am2301_timer_isr, 0);
    timer_init(50 * 1000); //��ʼ��100ms���һ���豸
}
static AWINLINE void am2301_timer_exit(void)
{
    if (!(TMR->CON & BIT(15))) {
        TMR->CON &= ~BIT(0);
    } else {
        temp_info.timer_close = TRUE;
    }
}
static AWINLINE void am2301_io_init(void)
{
    DQ_OUT;
    DQ_W_H;
//    printf("am2301_io_init\n");
}
static AWINLINE void am2301_temp_redo_next_time(u32 us)
{
    DQ_OUT;
    DQ_W_H;
    timer_init(us);//�´μ���¶�ʱ��
}
static AWINLINE void am2301_temp0_reset(void)//�ߵ�ƽ�����ź�
{
    timer_init(10 * 1000); //10ms�����ź�
    DQ_OUT;
    DQ_W_H;
}
static AWINLINE void am2301_temp1_reset_start(void)//������λ�ź�
{
    timer_init(1000);//����>800us��λ�ź�
    DQ_W_L;
}
static AWINLINE void am2301_temp2_release_bus(void)//������λ�ź�
{
    timer_init(30);//�ͷ����ߣ��ȴ�30us
    DQ_IN;
}
static AWINLINE void am2301_temp3_delay(void)
{
    timer_init(40);//40us���ȡ�ӻ��ĵ͵�ƽ
}
static AWINLINE u8 am2301_temp4_read_lack(void)
{
    u8 ack = !DQ_R;
    if (ack) {
        timer_init(80);//80us���ȡ�ӻ��ĸߵ�ƽ
    } else {
        timer_init(50 * 1000); //ACKʧ�ܣ�100ms��Ⳣ��
    }
    return ack;
}
static AWINLINE u8 am2301_temp5_read_hack(void)
{
    u8 ack = DQ_R;
    if (ack) {
        timer_init(40 + AM2301_DATA_L_TIMER / 2); //65us���ȡ��һλ�����źŵĵ͵�ƽʱ��
    } else {
        timer_init(50 * 1000); //ACKʧ�ܣ�100ms��Ⳣ��
    }
    return ack;
}
static AWINLINE void am2301_temp6_first_bit_l(void)
{
#if USER_FUNC_INT_SRAM
    timer_init(AM2301_DATA_L_TIMER / 2 + 26 + AM2301_DATA_L_TIMER / 2); //76us���ȡ��һλ����
#else
    timer_init(AM2301_DATA_L_TIMER / 2 + 26 + AM2301_DATA_L_TIMER / 2 - 30); //��������flash��Ҫ��ȥ30us
#endif
}
static FUNC_IN u8 am2301_init(void)
{
    u8 ret;
    if (temp_info.mode_type == TMP_INIT_OK) {
        return TMP_INIT_OK;
    }
    temp_info.mode_type = (temp_info.mode_type == TMP_INIT_ACK) ? temp_info.mode_type : TMP_INIT_START;
    switch (temp_info.mode_cnt) {
    case 0:
        am2301_temp0_reset();
        break;
    case 1:
        am2301_temp1_reset_start();
        break;
    case 2:
        am2301_temp2_release_bus();
        break;
    case 3:
        am2301_temp3_delay();
        break;
    case 4:
        ret = am2301_temp4_read_lack();
        temp_info.mode_type = ret ? TMP_INIT_ACK : TMP_INIT_ERR;
        if (!ret) {
            temp_info.mode_cnt = 0;
            if (++temp_info.sensor_check > SENSOR_CHECK_CNT) {
                temp_info.sensor_type = SENSOR_ERR;
                temp_info.sensor_check = 0;
            }
            return temp_info.mode_type;
        }
        break;
    case 5:
        ret = am2301_temp5_read_hack();
        temp_info.mode_type = ret ? TMP_INIT_ACK : TMP_INIT_ERR;
        if (!ret) {
            temp_info.mode_cnt = 0;
            if (++temp_info.sensor_check > SENSOR_CHECK_CNT) {
                temp_info.sensor_type = SENSOR_ERR;
                temp_info.sensor_check = 0;
            }
            return temp_info.mode_type;
        }
        break;
    case 6:
        am2301_temp6_first_bit_l();
        temp_info.mode_type = TMP_INIT_OK;
        temp_info.mode_cnt = 0;
        temp_info.sensor_check = 0;
        temp_info.sensor_type = SENSOR_AM2301;
        return temp_info.mode_type;
        break;
    default:
        break;
    }
    temp_info.mode_cnt++;
    return temp_info.mode_type;
}
static AWINLINE u8 am2301_temp_read_bit(void)
{
    u8 bit;
    bit = DQ_R;
    if (bit) {
        timer_init(((AM2301_DATA1_H_TIMER - 26 - (AM2301_DATA_L_TIMER / 2)) + AM2301_DATA_L_TIMER / 2) + (AM2301_DATA_L_TIMER / 2 + 26 + AM2301_DATA_L_TIMER / 2) + ((temp_info.wr_bit) ? 1 : 0) + temp_info.wr_bit / 2); //((temp_info.wr_bit) ? 1 : 0) + temp_info.wr_bit / 2 ���ڲ���
    } else {
        timer_init(AM2301_DATA_L_TIMER / 2 + 26 + AM2301_DATA_L_TIMER / 2 + ((temp_info.wr_bit % 3 == 0) ? 1 : 0)); //((temp_info.wr_bit % 3 == 0) ? 1 : 0) ���ڲ���
    }
    return bit;
}
static FUNC_IN u8 am2301_read_bytes(void)
{
    if (temp_info.mode_type == TMP_READ_BYTE_OK) {
        return TMP_READ_BYTE_OK;
    }
    if (!temp_info.byte_num && !temp_info.wr_bit) {
        temp_info.read_data = 0;
    }
    temp_info.mode_type = TMP_READ_BYTE_START;
    temp_info.read_data <<= 1;
    temp_info.read_data |= am2301_temp_read_bit() ? 0x1 : 0x0;
    temp_info.wr_bit++;
    if (temp_info.wr_bit >= 8) {
        temp_info.recv_data[temp_info.byte_num] = temp_info.read_data;
        temp_info.read_data = 0;
        temp_info.wr_bit = 0;
        temp_info.byte_num++;
        if (temp_info.byte_num >= 5) {
            temp_info.byte_num = 0;
            temp_info.mode_type = TMP_READ_BYTE_OK;
            timer_init(50 * 1000);
        }
    }
    return temp_info.mode_type;
}
u8 am2301_read(float *temp, float *humidity)//�̶߳�ȡ�¶����ݣ�temp_buf ���¶����ݴ洢��ַ
{
    if (temp_info.read_step == STEP_AM_READ_DATA_OK) { //ds18b20�¶ȶ�ȡ����
        *temp = temp_info.ftemp;
        *humidity = temp_info.fhumidity;
        temp_info.read_step = STEP_AM_READ_REDO;//���¶�ȡ�¶ȱ��
        return 1;
    }
    return 0;
}

/*****************************************************************************
                AM2301�жϴ�����
*****************************************************************************/
static FUNC_IN ___interrupt void am2301_timer_isr(void)
{
    if (TMR->CON & BIT(15)) {
        //��ȡ��ʪ�Ȳ���
        switch (temp_info.read_step) {
        case STEP_AM_SEND_INIT:
            am2301_init();//1 . ��ʼ��
            break;
        case STEP_AM_SEND_READ:
            am2301_read_bytes();//2 . ��40�ֽ�
            break;
        case STEP_AM_READ_REDO:
            temp_info.read_step = STEP_AM_SEND_INIT;
            temp_mode_reinit();
            temp_redo_next_time(3 * 1000 * 1000); //3S�ٴζ�ȡ
            break;
        case STEP_AM_READ_DATA_OK:
            ;
            u8 to_s = 10;//10sû�ж�ȡ�����¶�ȡ
            if (temp_info.ready_readed_tocnt > (((to_s * 1000) / 100) * 2)) { //10s���¶�ȡ
                temp_info.ready_readed_tocnt = 0;
                temp_info.read_step = STEP_AM_SEND_INIT;
                temp_mode_reinit();
            } else if (!temp_info.ready_readed_tocnt) {
                temp_redo_next_time(100 * 1000); //100ms����Ƿ��ȡ�¶�
            }
            temp_info.ready_readed_tocnt++;
            break;
        default:
            break;
        }
        switch (temp_info.mode_type) {
        case TMP_INIT_OK:
            if (temp_info.read_step == STEP_AM_SEND_INIT) {
                temp_info.read_step = STEP_AM_SEND_READ;
            }
            break;
        case TMP_READ_BYTE_OK:
            ;
            short hum = 0, tmp = 0, check_sum = 0, sum = 0;
            hum = ((temp_info.recv_data[0] << 8) | temp_info.recv_data[1]) & 0xFFFF;
            tmp = ((temp_info.recv_data[2] << 8) | temp_info.recv_data[3]) & 0xFFFF;
            temp_info.ftemp = (float)tmp / 10;//�¶�
            temp_info.fhumidity = (float)hum / 10;//ʪ��
            sum = temp_info.recv_data[0] + temp_info.recv_data[1] + temp_info.recv_data[2] + temp_info.recv_data[3];
            check_sum = temp_info.recv_data[4];
            check_sum |= (sum > 255) ? (sum & 0xFF00) : 0;//����У�飬��255����8λ��sum�ģ�������BUG
            if (check_sum == sum) {
                temp_info.read_step = STEP_AM_READ_DATA_OK;
            } else {
                temp_info.read_step = STEP_AM_READ_REDO;//���¶�ȡ�¶ȱ��
                printf("AM3201 data check sum err\n");
            }
            temp_info.mode_type = TMP_NONE;
            temp_info.ready_readed_tocnt = 0;
            temp_info.recv_data[0] = temp_info.recv_data[1] = temp_info.recv_data[2] = temp_info.recv_data[3] = temp_info.recv_data[4] = 0;
            temp_redo_next_time(100 * 1000); //100ms����Ƿ��ȡ�¶�
            break;
        case TMP_ERR:
        case TMP_INIT_ERR:
        case TMP_WTITE_BYTE_ERR:
        case TMP_READ_BYTE_ERR:
            temp_info.read_step = STEP_AM_SEND_INIT;
            temp_info.filter_cnt = 0;
            temp_info.ftemp_rank_id = 0;
            temp_mode_reinit();
            temp_redo_next_time(50 * 1000); //100ms����Ƿ��ȡ�¶�
            break;
        default :
            break;
        }

        if (temp_info.timer_close) {
            temp_info.timer_close = 0;
            TMR->CON &= ~BIT(0);
        }
        TMR->CON |= BIT(14);
    }
}
static void temp_app_task(void *priv)
{
    u32 ret;
    float temp;
    float humidity;

    temp_info.is_ds18b20 = TRUE;

    os_time_dly(100);
    while (1) {
        if (temp_info.is_ds18b20) {
            if (temp_info.sensor_type == SENSOR_ERR) {
                os_time_dly(10);
            }
            ds18b20_io_init();
            ds18b20_timer_init();
            while (temp_info.sensor_type == SENSOR_NONE) {
                os_time_dly(2);
            }
            if (temp_info.sensor_type == SENSOR_ERR) {
                printf("DS18B20 check err\n\n");
                ds18b20_timer_exit();
            }
        }
        if (temp_info.sensor_type == SENSOR_DS1B820) {
            while (1) {
                ret = ds18b20_temp_read(&temp);//��ȡ�¶Ƚ��
                if (ret) { //ds18b20�¶ȶ�ȡ����
                    /*
                    ����ʾ��......
                    */
                    printf("---> DS18B20 : temp = %f \n", temp);
                }
                if (temp_info.sensor_type == SENSOR_ERR) {
                    ds18b20_timer_exit();
                    temp_info.is_ds18b20 = 0;
                    break;
                }
                os_time_dly(10); //��ʱ100ms�����ͷ�CPU
            }
        } else {
            if (temp_info.sensor_type == SENSOR_ERR) {
                os_time_dly(10);
            }
            am2301_io_init();
            am2301_timer_init();
            while (temp_info.sensor_type == SENSOR_NONE) {
                os_time_dly(2);
            }
            if (temp_info.sensor_type == SENSOR_ERR) {
                printf("AM3201 check err \n\n");
                am2301_timer_exit();
            }
            while (1) {
                ret = am2301_read(&temp, &humidity);//��ȡ�¶Ƚ��
                if (ret) { //ds18b20�¶ȶ�ȡ����
                    /*
                    ����ʾ��......
                    */
                    printf("---> AM3201 : temp = %f ,humidity = %f\n", temp, humidity);
                }
                if (temp_info.sensor_type == SENSOR_ERR) {
                    am2301_timer_exit();
                    temp_info.is_ds18b20 = TRUE;
                    break;
                }
                os_time_dly(00); //��ʱ100ms�����ͷ�CPU
            }
        }
    }
}

static int temp_init(void *priv)
{
    return os_task_create(temp_app_task, NULL, 16, 512, 0, "temp_app_task");
//    return thread_fork("temp_app_task", 16, 512, 0, 0, temp_app_task, (void *)NULL);
}
late_initcall(temp_init);

