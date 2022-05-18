#include "device/device.h"
#include "asm/crc16.h"
#include "asm/sfc_norflash_api.h"

//==========================================�û�flash API����˵��=====================================================//
/*norflash_open�����ǳ�ʼ��flashӲ�������û�ʹ��flash��API�ӿ�������Ҫ�򿪲���ʹ��flash API(��δ�û��Ӱ��)
������name��device��argΪNULL���ɡ�
*/
int norflash_open(const char *name, struct device **device, void *arg);

/*norflash_origin_read������ֱ��SPI��ȡflash�������û�Ӧ��ʹ�õ�API�ӿڡ�
��ͨ��norflash_write����д����norflash_origin_read��ȡ��д����ͬ����Ҫ�ӽ�����Ӧ�ò��û����д���
������buf:���ݵ�ַ��offsetf:flash��ַ��len:���ݳ���
*/
int norflash_origin_read(u8 *buf, u32 offset, u32 len);

/*norflash_read������CPU��ȡflash���û�Ӧʹ�ø�API�ӿ�ʱ��Ҫע�⣺��д��ַС��VM����ʱ�����ӽ��ܴ���FLASH����ȫָ������ռ��û������ֹʹ�á�
������deviceΪNULL��buf:���ݵ�ַ��offsetf:flash��ַ��len:���ݳ���
*/
int norflash_read(struct device *device, void *buf, u32 len, u32 offset);

/*norflash_write������ֱ��SPIдflash�������û�Ӧ��ʹ�õ�API�ӿڡ�
��ͨ��norflash_write����д����norflash_origin_read��ȡ��д����ͬ����Ҫ�ӽ�����Ӧ�ò��û����д���
������deviceΪNULL��buf:���ݵ�ַ��offsetf:flash��ַ��len:���ݳ���
*/
int norflash_write(struct device *device, void *buf, u32 len, u32 offset);

/*norflash_ioctl��flash��������ӿ�
������deviceΪNULL
cmd��
IOCTL_GET_CAPACITY����ȡflash������flashʵ������������С����argΪ�����������ĵ�ַ
IOCTL_ERASE_SECTOR�������������argΪ����������ַ
IOCTL_GET_ID����ȡflash ID���argΪID�������ĵ�ַ
IOCTL_GET_UNIQUE_ID����ȡflash��UUID��argΪUUID�������ĵ�ַ����ַ���ô�СΪ16�ֽ�
IOCTL_SET_WRITE_PROTECT�����flash��д������arg=0��Ĭ��0��
*/
int norflash_ioctl(struct device *device, u32 cmd, u32 arg);



//==================flashд��ַС��MV����norflash_read������ȡ��������Ҫ�����ȼ���д��=========================//
void doe(u16 k, void *pBuf, u32 lenIn, u32 addr);//ϵͳ�ļ��ܺ���
u32 get_system_enc_key(void);//ϵͳ������Կ
u32 get_norflash_vm_addr(void);//��ȡVM��ַ��VM����Ϊisd_config.ini�ļ����õ�VM_LEN

//д��ַС��MV�������ݽ��м��ܣ�norflash_read��ȡ�Ų�����
//��дflash������ʱ����ȡֻ����norflash_origin_read
void flash_data_doe(u8 *buf, int len)
{
    doe(get_system_enc_key(), buf, len, 0);
}



//=======================================flash���Ժ���==========================================================//
void flash_test(void)
{
    void *dev = NULL;

    u8 *mac_addr_data = zalloc(4096);
    if (!mac_addr_data) {
        printf("----> err !!!!!!! malloc \n\n");
        return ;
    }
#define USE_CRC32	1 //ʹ��CRCУ�飬��ʹ����Ĭ�Ϻ�У��
    u32 cap_size = 0;
    u32 id = 0;
    u8 uuid[16] = {0};

    norflash_open(NULL, NULL, NULL);
    norflash_ioctl(NULL, IOCTL_GET_ID, (u32)&id);
    norflash_ioctl(NULL, IOCTL_GET_UNIQUE_ID, (u32)&uuid);
    norflash_ioctl(NULL, IOCTL_GET_CAPACITY, (u32)&cap_size);
    printf("--->flash id = 0x%x \n",id);
    printf("--->flash uuid \n");
    put_buf(uuid, 16);
    printf("--->flash cap_size = %dMB \n", cap_size / 1024 / 1024);

    u32 timer_get_sec(void);
    u32 start_time = timer_get_sec();
    u32 start_addr = 2 * 1024 * 1024;//��ʼ��ַ
    u32 end_addr = start_addr + 2 * 1024 * 1024;//������ַ

    for (u32 addr = start_addr; addr < end_addr; addr += 4096) { //����:start_addr - end_addr
        if (addr % (1024 * 1024) == 0) {
            printf("--->flash test addr = 0x%x \n", addr);
        }
        u32 mac_flash_addr = addr;
        u32 i, sum;
        for (i = 0, sum = 0; i < (4096 - 4); i++) {//����� + ��У������
            mac_addr_data[i] = JL_RAND->R64L & 0xFF;
            sum += mac_addr_data[i];
        }
        u32 *check = (u32 *)&mac_addr_data[4096 - 4];
#if USE_CRC32
        u32 crc32 = CRC32(mac_addr_data, (4096 - 4));//CRCУ��������
        *check = crc32;//CRCУ��
#else
        *check = sum;//��У��
#endif
        norflash_ioctl(NULL, IOCTL_SET_WRITE_PROTECT, 0);
        norflash_ioctl(NULL, IOCTL_ERASE_SECTOR, mac_flash_addr);
        norflash_write(NULL, mac_addr_data, 4096, mac_flash_addr);
        memset(mac_addr_data, 0, 4096);
        norflash_origin_read(mac_addr_data, mac_flash_addr, 4096);

#if USE_CRC32
        u32 check_crc32 = CRC32(mac_addr_data, (4096 - 4));//��ȡ���ݺ�����ݽ���CRCУ��
        if (*check != crc32 || check_crc32 != crc32) {//˫��У�飺��У���ַ��ֵ��У������
            printf("---> flash check err : read crc = 0x%x , write crc = 0x%x , check_crc = 0x%x,addr=0x%x\n", *check, crc32, check_crc32, addr);
        }
#else
        u32 check_sum = 0;
        for (i = 0; i < (4096 - 4); i++) {//��ȡ���ݺ�����ݽ��к�У��
            check_sum += mac_addr_data[i];
        }
        if (*check != sum || check_sum != sum) {//˫��У�飺��У���ַ��ֵ��У������
            printf("---> flash check err : read sum = 0x%x , write sum = 0x%x , check_sum = 0x%x,addr=0x%x\n", *check, sum, check_sum, addr);
        }
#endif
    }
    u32 end_time = timer_get_sec();
    printf("--->flash test susscess , use time = %ds\n", end_time - start_time);
    free(mac_addr_data);
}

//=======================================�û�flash API��д�����û���ֱ��ʹ��===================================================//
//Ӧ�ò��û�ʼ��flashӲ���ӿ�
void flash_read_write_init(void)
{
    u32 cap_size = 0;
    u32 flash_id = 0;
    u8 uuid[16] = {0};

    norflash_open(NULL, NULL, NULL);
    norflash_ioctl(NULL, IOCTL_GET_ID, (u32)&flash_id);//��ȡID
    norflash_ioctl(NULL, IOCTL_GET_UNIQUE_ID, (u32)&uuid);//��ȡUUID
    norflash_ioctl(NULL, IOCTL_GET_CAPACITY, (u32)&cap_size);//��ȡ����
    printf("--->flash id = 0x%x , cap_size = %dMB \n",flash_id, cap_size / 1024 / 1024);
    printf("--->flash uuid \n");
    put_buf(uuid, 16);
}

//Ӧ�ò��û���дAPI��ʹ�øú���ǰ���ȵ���norflash_open������ʼ��flashӲ���ӿ�
int flash_write_buff(u8 *buff, u32 addr, u32 len)
{
#define FLASH_BLOCK 4096
    u32 beginBlock = (addr / FLASH_BLOCK);
    u32 endBlock = (((addr + len) / FLASH_BLOCK));

    u8 *flashBuffer = malloc(FLASH_BLOCK);
    if (!flashBuffer) {
        return -1;
    }
    u32 writeLength = 0;
    u32 length = FLASH_BLOCK - (addr % FLASH_BLOCK);
    norflash_ioctl(NULL, IOCTL_SET_WRITE_PROTECT, 0);
    norflash_origin_read(flashBuffer, beginBlock * FLASH_BLOCK, FLASH_BLOCK);
    if (length >= len) {
        memcpy(flashBuffer + (addr % FLASH_BLOCK), buff, len);
        norflash_ioctl(NULL, IOCTL_ERASE_SECTOR, beginBlock * FLASH_BLOCK);//ÿ���飬д֮ǰ������Ӧ��
        norflash_write(NULL, flashBuffer, FLASH_BLOCK, beginBlock * FLASH_BLOCK);
        free(flashBuffer);
        return 0;
    } else {
        memcpy(flashBuffer + (addr % FLASH_BLOCK), buff, length);
        norflash_ioctl(NULL, IOCTL_ERASE_SECTOR, beginBlock * FLASH_BLOCK);//ÿ���飬д֮ǰ������Ӧ��
        norflash_write(NULL, flashBuffer, FLASH_BLOCK, beginBlock * FLASH_BLOCK);
        writeLength = length;
    }
    for (int i = beginBlock + 1; i < endBlock; i++) {
        norflash_ioctl(NULL, IOCTL_ERASE_SECTOR, i * FLASH_BLOCK);
        norflash_ioctl(NULL, IOCTL_ERASE_SECTOR, i * FLASH_BLOCK);//ÿ���飬д֮ǰ������Ӧ��
        norflash_write(NULL, buff + writeLength, FLASH_BLOCK, i * FLASH_BLOCK);
        writeLength += FLASH_BLOCK;
    }
    if (beginBlock != endBlock && (len > writeLength)) {
        //���һ���飬ʣ������û����һ���飬���ܶ�ʧ����û�õ���ַ������
        //������һ���ȶ����ݣ����ı���д��Ӧ��ַ�����ݣ��ٽ�û���õ���ַ��ԭ������д��ȥ������������ݶ�ʧ
        norflash_origin_read(flashBuffer, endBlock * FLASH_BLOCK, FLASH_BLOCK);
        memcpy(flashBuffer, buff + writeLength, len - writeLength);
        norflash_ioctl(NULL, IOCTL_ERASE_SECTOR, endBlock * FLASH_BLOCK);//ÿ���飬д֮ǰ������Ӧ��
        norflash_write(NULL, flashBuffer, FLASH_BLOCK, endBlock * FLASH_BLOCK);
    }
    free(flashBuffer);
    return 0;
}

//Ӧ�ò��û���дAPI��ʹ�øú���ǰ���ȵ���norflash_open������ʼ��flashӲ���ӿ�
int flash_read_buff(u8 *buff, u32 addr, u32 len)
{
    norflash_origin_read(buff, addr, len);
    return 0;
}
//=======================================�û�flash API��д�����û���ֱ��ʹ��===================================================//
