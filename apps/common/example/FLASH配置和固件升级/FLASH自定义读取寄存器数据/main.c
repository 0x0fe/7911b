#include "app_config.h"
#include "system/includes.h"
#include "asm/sfc_norflash_api.h"


//���еĺ�������Ҫ������spi_code��
#define AT_SPI_CODE SEC_USED(.spi_code)

AT_SPI_CODE
static u32 user_flash_spi_test(void) 
{
    u8 reg1 = 0,reg2 = 0;
    int ret;

    printf("---> user_flash_spi_test \n");
    /******************************************************
	  ��norflash_enter_spi_code��norflash_exit_spi_code���ڣ�
	  ���д�������ݺ�const����ͱ���ֻ����sdram�����ڲ�sram��
	  ���ݺ�const����ͱ�������ָ���ڲ�sram��SEC_USED(.sram)��
	  ��sdram��SEC_USED(.data)�����ϵͳ�Ĵ�������flash��������CONFIG_SFC_ENABLE�꣩��
	  ���ܼӴ�ӡ��Ϣ
	*******************************************************/
    //1 �û����Բ���flash��norflash_enter_spi_code
    norflash_enter_spi_code(); 
    
    //2 �ȴ�flashæ����
    ret = norflash_wait_busy();
    if(!ret){//��æ��ȡ�Ĵ�������
		//3 ��ȡ�Ĵ���1
		norflash_spi_cs(0); 
		norflash_spi_write_byte(0x05);
		reg1 = norflash_spi_read_byte(); 
		norflash_spi_cs(1); 

		//4 ��ȡ�Ĵ���2
		norflash_spi_cs(0); 
		norflash_spi_write_byte(0x35);
		reg2 = norflash_spi_read_byte(); 
		norflash_spi_cs(1);
    }

    //5 �û����Բ���flash�������Ҫnorflash_exit_spi_code
    norflash_exit_spi_code(); 
   
    /******************************************************
	   ��norflash_enter_spi_code��norflash_exit_spi_code���ڣ�
	  ���д�������ݺ�const����ͱ���ֻ����sdram�����ڲ�sram��
	  ���ݺ�const����ͱ�������ָ���ڲ�sram��SEC_USED(.sram)��
	  ��sdram��SEC_USED(.data)�����ϵͳ�Ĵ�������flash��������CONFIG_SFC_ENABLE�꣩��
	  ���ܼӴ�ӡ��Ϣ
	*******************************************************/
    //6 ������д�ӡ��Ҫ�����˳�֮�����ʹ��
    printf("---> reg1 = 0x%x , reg2 = 0x%x \n",reg1,reg2);
}

static int c_main(void)
{
    user_flash_spi_test();
}
late_initcall(c_main);
