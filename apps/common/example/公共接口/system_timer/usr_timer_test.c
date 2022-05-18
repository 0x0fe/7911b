#include "app_config.h"
#include "system/includes.h"
#include "system/timer.h"
#include "system/init.h"

static int timer_id;
static int j;   //jΪ�ص�����
static const u8 test_array[10] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static void array_sequence(int sign)    //����˳����
{
    u8 i = 0;
    j++;
    printf("Callback_No%d_array_all:\r\n",j);
     for(i = 0; i < 10; i++)
    {
        if(sign = 0)    //����������ʾ
            printf("%d\r\n", test_array[i]);
        else            //���鵹����ʾ
            printf("%d\r\n", test_array[9-i]);

    }
     if(j == 3)
    {
        usr_timer_del(timer_id);    //ɾ��timer���ص�3�μ�ɾ��timer
        timer_id=0;
    }
}

static void callback_example(int i)   //�ص�����
{
  switch (i)
  {
    case 0:
    array_sequence(0); break;
    case 1:
    array_sequence(1); break;
  }
}

static int  usr_timer_test(void)
{
    printf("\r\n\r\n-------%s-------%d\r\n\r\n", __func__, __LINE__);
    timer_id = usr_timer_add((void *)1, callback_example, 2*1000, 1);    //ע��һ���߾���timer����ʱʱ��2�룬������id
#if 0   //����0ִ���޸ĺ�Ķ�ʱʱ��500���룬����1ִ�ж�ʱʱ��2��
    printf("timer_id = %d\r\n", timer_id);
#else
    if(timer_id)
    {
        printf("timer_id = %d\r\n", timer_id);
        usr_timer_modify(timer_id, 500);  //�޸�timer�Ķ�ʱʱ��Ϊ500����
    }
#endif
    return 0;
}

late_initcall(usr_timer_test);
