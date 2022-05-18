#include "app_config.h"
#include "system/includes.h"
#include "system/timer.h"
#include "system/init.h"

static int timeout_id;
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

static int  usr_timeout_test(void)
{
    printf("\r\n\r\n-------%s-------%d\r\n\r\n", __func__, __LINE__);
    timeout_id = usr_timeout_add((void *)1, callback_example, 2*1000, 1);    //ע��һ���߾���timeout����ʱʱ��2�룬������id��ֻ�ص�һ��
#if 0   //����0ִ���޸ĺ�Ķ�ʱʱ��500����;����1ɾ����ʱʱ��Ϊ2���timeout
    printf("timeout_id = %d\r\n", timeout_id);
    usr_timeout_del(timeout_id);    //ɾ��timeout
#else
    if(timeout_id)
    {
        printf("timeout_id = %d\r\n", timeout_id);
        usr_timeout_modify(timeout_id, 500);  //�޸�timeout�Ķ�ʱʱ��Ϊ500����
    }
#endif
    return 0;
}

late_initcall(usr_timeout_test);
