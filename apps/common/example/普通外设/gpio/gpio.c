#include "asm/gpio.h"
#include "system/includes.h"
#include "app_config.h"
#include "asm/port_waked_up.h"
#include "device/gpio.h"


static void PA0IntHandler(int i)   //�ⲿ�жϴ���

{ int a;
  a=i;
  switch (a)
  {
   case 0:
   printf( "hello,jl"); break;
   case 1:
   printf( "hi,jl"); break;
   case 2:
   printf( "bye,jl");break;
  }
}

static void c_main(void *priv)
{

 __gpio_direction_input(IO_PORTH_12);       //GPIO����   ͬһ������֮��û�л���, ����û��и��ٶ�Ҫ��, ���Ҳ���Ҫ����ͬһ������֮��������������ʹ��
 __gpio_set_pull_up(IO_PORTH_12,1);         //��������

 gpio_direction_output(IO_PORTC_01,1);   //GPIO���
 gpio_set_hd(IO_PORTC_01,1);             //ǿ������

//gpio_direction_input(IO_PORTA_01);         //GPIO����
//gpio_set_pull_down(IO_PORTA_01,1);
 os_time_dly(300);


Port_Wakeup_Reg(EVENT_IO_0, IO_PORTA_01, EDGE_POSITIVE, PA0IntHandler, (void *)2); //GPIO�ж�ģʽʹ�ã�I/O�����ⲿ�жϣ��ⲿ�ж����ţ������ش������ж���Ӧ���������������EVENT_IO_0����ӳ�䵽��������
//Port_Wakeup_Reg(EVENT_IO_1, IO_PORTA_01, EDGE_NEGATIVE, PA0IntHandler, (void *)1); //GPIO�ж�ģʽʹ�ã�I/O�����ⲿ�жϣ��ⲿ�ж����ţ��½��ش������ж���Ӧ���������������EVENT_IO_1����ӳ�䵽��������



gpio_direction_output(IO_PORT_USB_DPA,1);
gpio_direction_output(IO_PORT_USB_DMA,1);   //����������USB/����ΪGPIOʹ�÷���

gpio_direction_input(IO_PORTH_04);         //GPIO����,����PH4��PH6��˫�ذ�IO
gpio_set_pull_down(IO_PORTH_04,1);         //��������

os_time_dly(3000);
gpio_direction_output(IO_PORTH_06,1);

 //gpio_output_channle(IO_PORTE_00,00);
//gpio_read(IO_PORTE_00);
 //ret=gpio_read(IO_PORTE_00);
//printf("ret:",ret);



}
late_initcall(c_main);



