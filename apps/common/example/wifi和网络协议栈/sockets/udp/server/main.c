#include "sock_api/sock_api.h"
#include "os/os_api.h"
#include "app_config.h"
#include "system/includes.h"
#include "wifi/wifi_connect.h"
#include "lwip.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

static void *socket_fd = 0;
static int recv_pid = 0;

//��  �ܣ�udp_server�����̣߳����ڽ���udp_client������
//��  ��: ��
//����ֵ����
static void udp_recv_handler(void)
{
    struct sockaddr_in remote_addr = {0};
    socklen_t len = sizeof(remote_addr);
    int recv_len = 0;
    u8 recv_buf[50] = {0};

    for (;;) {

        //����udp_client�����ݣ�����ȡudp_client�ĵ�ַ��Ϣ
        recv_len = sock_recvfrom(socket_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&remote_addr, &len);
        printf("\n recv_len is %d  \n", recv_len);

        //�����յ������ݷ�����udp_client
        sock_sendto(socket_fd, recv_buf, sizeof(recv_buf), 0, (struct sockaddr *)&remote_addr, sizeof(struct sockaddr_in));

        memset(recv_buf, 0, sizeof(recv_buf));
    }
}

//��  �ܣ��udp_server
//��  ��: int port���˿ں�
//����ֵ������0���ɹ�������-1��ʧ��
static int udp_server_init(int port)
{
    struct sockaddr_in local_addr = {0};

    local_addr.sin_family = AF_INET;
    local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    local_addr.sin_port = htons(port);

    socket_fd = sock_reg(AF_INET, SOCK_DGRAM, 0, NULL, NULL);
    if (socket_fd == NULL) {
        printf("%s build socket fail\n",  __FILE__);
        return -1;
    }

    if (sock_bind(socket_fd, (struct sockaddr *)&local_addr, sizeof(struct sockaddr))) {
        printf("%s sock_bind fail\n", __FILE__);
        return -1;
    }

    //�����̣߳����ڽ���tcp_client������
    if (thread_fork("udp_recv_handler", 25, 512, 0, &recv_pid, udp_recv_handler, NULL) != OS_NO_ERR) {
        printf("%s thread fork fail\n", __FILE__);
        return -1;
    }

    return 0;
}

//��  �ܣ�ע��udp_server
//��  ��: ��
//����ֵ����
static void udp_server_exit(void)
{
    thread_kill(&recv_pid, KILL_WAIT);
    sock_unreg(socket_fd);
}

static void udp_server_start(void *priv)
{
    int err;
	enum wifi_sta_connect_state state;
	
    while(1)
    {  
		 state = wifi_get_sta_connect_state();
		 printf("Connecting to the network...\n");
         if(WIFI_STA_NETWORK_STACK_DHCP_SUCC == state)
         {
			printf("Network connection is successful!\n");
            break;
         }
		 
		 os_time_dly(1000);
    }
	
	err = udp_server_init(32769);
    if(err == -1)
    {
        printf("udp_server_init faile\n");
        udp_server_exit();
    }
}

//Ӧ�ó������,��Ҫ������STAģʽ��
void c_main(void *priv)
{
    if (thread_fork("udp_server_start", 10, 512, 0, NULL, udp_server_start, NULL) != OS_NO_ERR) {
        printf("thread fork fail\n");
    }
}

late_initcall(c_main);
