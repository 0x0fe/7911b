#include "sock_api/sock_api.h"
#include "os/os_api.h"
#include "app_config.h"
#include "system/includes.h"
#include "wifi/wifi_connect.h"
#include "lwip.h"
#include "lwip/sockets.h"
#include "lwip/netdb.h"

#define MAX_RECV_BUF_SIZE   50 //�����ܽ������ݵ�����ֽ���(Bytes)

static int accept_pid = -1;    //tcp_sock_accpet�̵߳ľ��
static int recv_pid = -1;      //tcp_recv_accpet�̵߳ľ��

//tcp_client���ݽṹ
struct tcp_client_info {
    struct list_head entry;         //����ڵ�
    struct sockaddr_in remote_addr; //tcp_client��ַ��Ϣ
    void *fd;                       //�������Ӻ���׽���
};

//tcp_server���ݽṹ
struct tcp_server_info {
    struct list_head client_head;  //����
    struct sockaddr_in local_addr; //tcp_server��ַ��Ϣ
    void *fd;                      //tcp_server�׽���
    OS_MUTEX tcp_mutex;            //������
};

static struct tcp_server_info server_info;

static void tcp_sock_accpet(void);
static void tcp_recv_handler(void);
static struct tcp_client_info *get_tcp_client_info(void);
static void tcp_client_quit(struct tcp_client_info *priv);


//��  �ܣ���tcp_client������Ϣ
//��  ����void *sock_hdl ��socket���
//        const void *buf����Ҫ���͵�����BUF
//        u32 len        : BUF��С
//����ֵ�����ط��ͳɹ������ݵ��ֽ���������-1������ʧ��
static int tcp_send_data(void *sock_hdl, const void *buf, u32 len)
{
    return sock_send(sock_hdl, buf, len, 0);
}


//��  �ܣ�����tcp_client������
//��  ����void *sock_hdl ��socket���
//        void *buf      ����Ҫ��������BUF
//        u32 len        : BUF��С
//����ֵ�����ؽ��ճɹ������ݵ��ֽ���������-1������ʧ��
static int tcp_recv_data(void *sock_hdl, void *buf, u32 len)
{
    return sock_recv(sock_hdl, buf, len, 0);
}


//��  �ܣ�tcp_server�����̣߳����ڽ���tcp_client������
//��  ��: ��
//����ֵ����
static void tcp_recv_handler(void)
{
    struct tcp_client_info *client_info = NULL;
    char recv_buf[MAX_RECV_BUF_SIZE] = {0};

_reconnect_:

    do {
        client_info = get_tcp_client_info();
        os_time_dly(5);
    } while (client_info == NULL);

    for (;;) {
        if (tcp_recv_data(client_info->fd, recv_buf, sizeof(recv_buf)) > 0) {
			printf("Received data from (ip : %s, port : %d)\r\n", inet_ntoa(client_info->remote_addr.sin_addr), client_info->remote_addr.sin_port);
            printf("recv_buf = %s.\n", recv_buf);
            memset(recv_buf, 0, sizeof(recv_buf));
            //�˴���������ݴ�����
			tcp_send_data(client_info->fd, "Data received successfully!", strlen("Data received successfully!"));
        } else {
            tcp_client_quit(client_info);
            goto _reconnect_;
        }
    }
}

//��  �ܣ�tcp_sock_accpet�̣߳����ڽ���tcp_client����������
//��  ��: void *priv��NULL
//����ֵ����
static void tcp_sock_accpet(void)
{
    socklen_t len = sizeof(server_info.local_addr);

    for (;;) {
        struct tcp_client_info *client_info = calloc(1, sizeof(struct tcp_client_info));
        if (client_info == NULL) {
            printf(" %s calloc fail\n", __FILE__);
            return;
        }

        client_info->fd  = sock_accept(server_info.fd, (struct sockaddr *)&client_info->remote_addr, &len, NULL, NULL);
        if (client_info->fd == NULL) {
            printf("%s socket_accept fail\n",  __FILE__);
            return;
        }

        os_mutex_pend(&server_info.tcp_mutex, 0);
        list_add_tail(&client_info->entry, &server_info.client_head);
        os_mutex_post(&server_info.tcp_mutex);

        printf("%s, build connnect success.\n", inet_ntoa(client_info->remote_addr.sin_addr));
    }
}

//��  �ܣ��tcp_server
//��  ��: int port���˿ں�
//����ֵ������0���ɹ�������-1��ʧ��
static int tcp_server_init(int port)
{
    u32 opt = 1;

    memset(&server_info, 0, sizeof(server_info));

    server_info.local_addr.sin_family = AF_INET;
    server_info.local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_info.local_addr.sin_port = htons(port);

    server_info.fd = sock_reg(AF_INET, SOCK_STREAM, 0, NULL, NULL);
    if (server_info.fd == NULL) {
        printf("%s build socket fail\n",  __FILE__);
        return -1;
    }

    if (sock_setsockopt(server_info.fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        printf("%s sock_setsockopt fail\n", __FILE__);
        return -1;
    }

    if (sock_bind(server_info.fd, (struct sockaddr *)&server_info.local_addr, sizeof(struct sockaddr))) {
        printf("%s sock_bind fail\n", __FILE__);
        return -1;
    }

    if (sock_listen(server_info.fd, 0x2) != 0) {
        printf("%s sock_listen fail\n", __FILE__);
        return -1;
    }

    if (os_mutex_create(&server_info.tcp_mutex) != OS_NO_ERR) {
        printf("%s os_mutex_create fail\n", __FILE__);
        return -1;
    }

    INIT_LIST_HEAD(&server_info.client_head);

    //�����̣߳����ڽ���tcp_client����������
    if (thread_fork("tcp_sock_accpet", 26, 256, 0, &accept_pid, tcp_sock_accpet, NULL) != OS_NO_ERR) {
        printf("%s thread fork fail\n", __FILE__);
        return -1;
    }

    //�����̣߳����ڽ���tcp_client������
    if (thread_fork("tcp_recv_handler", 25, 512, 0, &recv_pid, tcp_recv_handler, NULL) != OS_NO_ERR) {
        printf("%s thread fork fail\n", __FILE__);
        return -1;
    }

    return 0;
}

//��  �ܣ�ע��tcp_client
//��  ��: struct tcp_client_info *priv����Ҫע����tcp_client
//����ֵ����
static void tcp_client_quit(struct tcp_client_info *priv)
{
    list_del(&priv->entry);
    sock_set_quit(priv->fd);
    sock_unreg(priv->fd);
    priv->fd = NULL;
    free(priv);
}

//��  �ܣ���ȡ��ǰ���µ�tcp_client
//��  ��: ��
//����ֵ�����ػ�ȡ��tcp_client
static struct tcp_client_info *get_tcp_client_info(void)
{
    struct list_head *pos = NULL;
    struct tcp_client_info *client_info = NULL;
    struct tcp_client_info *old_client_info = NULL;


    os_mutex_pend(&server_info.tcp_mutex, 0);

    list_for_each(pos, &server_info.client_head) {
        client_info = list_entry(pos, struct tcp_client_info, entry);
    }

    os_mutex_post(&server_info.tcp_mutex);

    return client_info;
}

//��  �ܣ�ע��tcp_server
//��  ��: ��
//����ֵ����
static void tcp_server_exit(void)
{
    struct list_head *pos = NULL;
    struct tcp_client_info *client_info = NULL;

    thread_kill(&accept_pid, KILL_WAIT);
    thread_kill(&recv_pid, KILL_WAIT);

    os_mutex_pend(&server_info.tcp_mutex, 0);

    list_for_each(pos, &server_info.client_head) {
        client_info = list_entry(pos, struct tcp_client_info, entry);
        if (client_info) {
            list_del(&client_info->entry);
            sock_unreg(client_info->fd);
            client_info->fd = NULL;
            free(client_info);
        }
    }

    os_mutex_post(&server_info.tcp_mutex);

    os_mutex_del(&server_info.tcp_mutex, OS_DEL_NO_PEND);
}

static void tcp_server_start(void *priv)
{
    int err;
	enum wifi_sta_connect_state state;
	
    while(1)
    {
         printf("Connecting to the network...\n");
		 state = wifi_get_sta_connect_state();
         if(WIFI_STA_NETWORK_STACK_DHCP_SUCC == state)
         {
			printf("Network connection is successful!\n");
            break;
         }
		 
		 os_time_dly(1000);
    }
	
	err = tcp_server_init(32769);
    if(err == -1)
    {
        printf("tcp_server_init faile\n");
        tcp_server_exit();
    }
}

//Ӧ�ó������,��Ҫ������STAģʽ��
void c_main(void *priv)
{
    if (thread_fork("tcp_server_start", 10, 512, 0, NULL, tcp_server_start, NULL) != OS_NO_ERR) {
        printf("thread fork fail\n");
    }
}

late_initcall(c_main);
