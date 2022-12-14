#include "app_config.h"
#include "system/includes.h"
#include "asm/power_interface.h"
#include "device/device.h"

#ifdef 	USE_SOFT_POWER_OFF_TEST_DEMO    //测试OK

static void poweroff_test(void)
{
    void power_set_soft_poweroff(u32 ms);
    os_time_dly(200);
    printf("---> poweroff_test \n\n");
    power_set_soft_poweroff(0);
}
static int c_main(void)
{
    os_task_create(poweroff_test, NULL, 12, 1000, 0, "poweroff_test");
    return 0;
}
late_initcall(c_main);
#endif
