#include "app_config.h"
#include "system/includes.h"
#include "system/timer.h"
#include "system/init.h"

static int timeout_id;
static int j;   //jΪ�ص�����,sys_timeoutֻ�ص�һ��
static const u8 test_array[10] =  { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

static void callback_example(void)  //�ص�����
{
    u8 i = 0;
    j++;
    printf("Callback_No%d_array_all:\r\n",j);
     for(i = 0; i < 10; i++)
    {
        printf("%d\r\n", test_array[i]);
//      printf("array_no.%d��%d\r\n", i, test_array[i]);
    }
}

static int sys_timeout_test(void)
{
    printf("\r\n\r\n-------%s-------%d\r\n\r\n", __func__, __LINE__);
    timeout_id = sys_timeout_add(NULL, callback_example, 5 * 1000);    //ע��һ��timeout��������id����ʱʱ�䵽���Զ�ɾ��timeout
#if 0   //����0ִ���޸ĺ�Ķ�ʱʱ��2��;����1ɾ����ʱʱ��Ϊ5���timeout
    printf("timeout_id = %d\r\n", timeout_id);
    sys_timeout_del(timeout_id);    //ɾ��timeout������sys_timeout�Զ�ɾ��timeout���ò����ú�����
#else
    if(timeout_id)
    {
        printf("timeout_id = %d\r\n", timeout_id);
//      sys_timeout_modify(timeout_id, 2 * 1000);  //������ʱû���������,���Գ���ʵ�ʻ���ִ��5��
    }
#endif
    return 0;
}
late_initcall(sys_timeout_test);
