#include "app_config.h"
#include "system/includes.h"

extern void put_float(double fv);

static int c_main(void)
{
    float a = 3.3f;
    float b = 5.6f;
    float c = a*b+rand32();

    printf("\r\n\r\n\r\n\r\n\r\n -----------float run %f -------------\r\n", c); //��ӡ�õ�˫���ȸ���,�����õ���������
    puts("put_float:            ");
    put_float(c);

    return 0;
}
late_initcall(c_main);

