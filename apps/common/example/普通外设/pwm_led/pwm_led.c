//#include "app_config.h"
//#include "system/includes.h"
//#include "asm/pwm.h"
//#include "device/device.h"
//
///*********************************PWM_LED�豸����****************************************************
//  ע�⣺��ʼ��֮��ֻ֧�ֵ�ͨ��ѡ��
//  PWMͨ��dev_writeд������ֵΪ�ɹ�0��ʧ�ܷ�0��
//  PWMͨ��dev_read��������ֵΪͨ����ռ�ձ�ֵ(0-100%)��
//  PWMͨ��dev_ioctl���ƣ��β�argΪPWMͨ����
//************************************************************************************************/
//static void pwm_led_test(void)
//{
//    int ret;
//    u32 duty;
//    u32 channel;
//	void *pwm_dev_handl = NULL;
//    struct pwm_platform_data pwm = {0};
//
//    /*1.open ��open֮���PWMͨ�����֧��ͬһ��ͨ����H��L���ã����ܽ��ж�ͨ��������*/
//    pwm_dev_handl = dev_open("pwm1", &pwm);//��PWM1�豸�����ο��Ի�ȡboard.c���õĲ���
//    if (!pwm_dev_handl) {
//        printf("open pwm err !!!\n\n");
//        return ;
//    }
//    printf("pwm : ch=0x%x,duty=%2f%%,pbit=%d,freq=%dHz\n", pwm.pwm_ch, pwm.duty, pwm.point_bit, pwm.freq);
//    os_time_dly(200);
///*open PWM�豸֮��ͻ��ʼ��PWM��PWM��ز���Ϊboard.c���ã��ڲ���Ҫ���Ĳ���ʱ��ֻ��Ҫopen���У�����Ҫ�������²���*/
//    #if 0
//    /*write and read ����ռ�ձ�*/
//
//    pwm.pwm_ch = PWMCH0_H | PWMCH0_L;//����ͨ��
//    pwm.duty = 80;//����ռ�ձ�
//    dev_write(pwm_dev_handl, (void *)&pwm, 0);
//    ret = dev_read(pwm_dev_handl, (void *)&pwm, 0);//dev_read��������ֵΪռ�ձȣ�����С���㣩0-100
//    printf("pwm0 read duty : %d \n", ret);
//    os_time_dly(200);
//#endif
//
//     /*2.ioctl����ռ�ձ�*/
//    int i;
//	while(1)
//	{
//		//ֵ��Χ��0~100  �ƽ���
//		for( i = 0; i<= 100 ; i++)
//		{
//		    pwm.duty = i;
//            dev_ioctl(pwm_dev_handl, PWM_SET_DUTY, (u32)&pwm);//����ռ�ձ�
//            os_time_dly(10);
//
//		}
//		//�ƽ���
//		for( i = 100; i>= 0 ; i--)
//		{
//			pwm.duty = i;
//            dev_ioctl(pwm_dev_handl, PWM_SET_DUTY, (u32)&pwm);//����ռ�ձ�
//            os_time_dly(10);
//		}
//	}
//
//}
//static int c_main(void)
//{
//    os_task_create(pwm_led_test, NULL, 12, 1000, 0, "pwm_led_test");
//    return 0;
//}
//late_initcall(pwm_led_test);
