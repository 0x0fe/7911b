#include "app_config.h"
#include "system/includes.h"
#include "system/timer.h"
#include "system/init.h"

static int timer_id;
static int j;   //jΪ�ص�����
static const u8 test_array[10] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static void callback_example(void)  //�ص�����
{
    u8 i = 0;
    j++;
    printf("Callback_No%d_array_all:\r\n",j);
     for(i = 0; i < 10; i++)
    {
        printf("%d\r\n", test_array[i]);
//        printf("array_no.%d��%d\r\n", i, test_array[i]);
    }
     if(j == 3)
    {
        sys_timer_del(timer_id);    //ɾ��timer���ص�3�μ�ɾ��timer
        timer_id=0;
    }
}

static int sys_timer_test(void)
{
    printf("\r\n\r\n-------%s-------%d\r\n\r\n", __func__, __LINE__);
    timer_id = sys_timer_add(NULL, callback_example, 5 * 1000);    //ע��һ����ʱ��timer����ʱʱ��5�룬������id
#if 0   //����0ִ���޸ĺ�Ķ�ʱʱ��2�룬����1ִ�ж�ʱʱ��5��
    printf("timer_id = %d\r\n", timer_id);
#else
    if(timer_id)
    {
        printf("timer_id = %d\r\n", timer_id);
        sys_timer_modify(timer_id, 2 * 1000);  //�޸�timer�Ķ�ʱʱ��Ϊ2��
    }
#endif
    return 0;
}
late_initcall(sys_timer_test);
