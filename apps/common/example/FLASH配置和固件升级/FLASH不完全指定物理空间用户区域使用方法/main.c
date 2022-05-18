#include "app_config.h"
#include "system/includes.h"
#include "fs/fs.h"
#include "asm/sfc_norflash_api.h"

static void user_flash_test(void)
{
    void flash_test(void);
    void flash_read_write_init(void);
    int flash_write_buff(u8 *buff, u32 addr, u32 len);
    int flash_read_buff(u8 *buff, u32 addr, u32 len);

#if 0
    flash_test();//flash���ԣ�flash_test�������Բ�����ʼ��ַ��������ַ
#else
    //1.��ʼ��flashӲ��������ȡID��������
    flash_read_write_init();//��ʼ��flashӲ��������ȡID��������

    unsigned char *buff = malloc(4096);
    if(buff){//4096�ֽڶ�д����
        for (int i = 0; i < 4096; i++) {
            buff[i] = (unsigned char)i;
        }
        //2.д����
        flash_write_buff(buff, 14*1024*1024, 4096);//14M��ַ��ʼ���ԣ�д
        memset(buff, 0, 4096);

        //3.������
        flash_read_buff(buff, 14*1024*1024, 4096);//14M��ַ��ʼ���ԣ���
        for (int i = 0; i < 4096; i++) {
            if (buff[i] != (unsigned char)i) {//У��
                printf("user_flash_test test err buff[%d] = %d \n",i,buff[i]);
                break;
            }
        }
        free(buff);
    }

    printf("---> user_flash_test over \n");
#endif
    while (1) {
        os_time_dly(10);
    }
}
static int c_main(void)
{
    os_task_create(user_flash_test, NULL, 12, 1000, 0, "user_flash_test");
    return 0;
}
late_initcall(c_main);
