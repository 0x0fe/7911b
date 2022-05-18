#include "system/includes.h"
#include "app_config.h"

static u8 buf[1 * 1024] __attribute__((aligned(32))); //���ڴ��ڽ��ջ������ݵ�ѭ��buf

static void uart_test_main(void *priv)
{
    u8 recv_buf[64];
    int send_buf[64];	//DMA������Ҫ4�ֽڶ���
    int len;
    int cnt = 0;
    void *hdl = dev_open("uart2", NULL);
    if (!hdl) {
        printf("open uart err !!!\n");
        return ;
    }
    /* 1 . ���ô��ڽ��ջ������ݵ�ѭ��buf��ַ */
    dev_ioctl(hdl, UART_SET_CIRCULAR_BUFF_ADDR, (int)buf);

    /* 1 . ���ô��ڽ��ջ������ݵ�ѭ��buf���� */
    dev_ioctl(hdl, UART_SET_CIRCULAR_BUFF_LENTH, sizeof(buf));

    /* 3 . ���ý�������Ϊ������ʽ,��Ҫ����������ȥ��,������ϳ�ʱ���� */
    dev_ioctl(hdl, UART_SET_RECV_BLOCK, 1);

    /* u32 parm = 1000; */
    /* dev_ioctl(hdl, UART_SET_RECV_TIMEOUT, (u32)parm); /*��ʱ����*/*/

    /* 4 . ʹ�����⴮��,�����շ����� */
    dev_ioctl(hdl, UART_START, 0);

    printf("uart_test_task running \n");

    while (1) {
        /* 5 . �������� */
        len = dev_read(hdl, recv_buf, 64);
        if (len <= 0) {
            printf("\n  uart recv err len = %d\n", len);
            if (len == UART_CIRCULAR_BUFFER_WRITE_OVERLAY) {
                printf("\n UART_CIRCULAR_BUFFER_WRITE_OVERLAY err\n");
                dev_ioctl(hdl, UART_FLUSH, 0); //��������û����ڲ�ȡ�߽��յ����ݵ���ѭ��buf���ջؾ���,���ֱ�ӳ��ѭ��buf�����������½���
            } else if (len == UART_RECV_TIMEOUT) {
                puts("UART_RECV_TIMEOUT...\r\n");
            }
            continue;
        }
        printf("\n uart recv len = %d\n", len);
        //�Ѵ��ڽ��յ������ݷ��ͻ�ȥ
        dev_write(hdl, recv_buf, len);
        cnt += len;
        len = sprintf(send_buf, "Rx_Cnt = %d\n", cnt);
        //ͳ�ƴ��ڽ��յ������ݷ��ͻ�ȥ
        dev_write(hdl, send_buf, len);
    }
    dev_close(hdl);
}
static int c_main(void)
{
    os_task_create(uart_test_main, NULL, 10, 1000, 0, "uart_test_main");
    return 0;
}
late_initcall(c_main);