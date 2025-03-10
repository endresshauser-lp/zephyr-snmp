#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys_clock.h>

#include <zephyr/kernel.h>

void snmp_init(void);
void snmp_loop();

static void zephyr_snmp_agent(void *data0, void *data1, void *data2);

K_THREAD_DEFINE(zephyr_snmp_thread, CONFIG_SNMP_STACK_SIZE, zephyr_snmp_agent, NULL, NULL,
        NULL, CONFIG_SNMP_THREAD_PRIORITY, K_ESSENTIAL, 0);


static void zephyr_snmp_agent(void *data0, void *data1, void *data2)
{
    ARG_UNUSED(data0);
    ARG_UNUSED(data1);
    ARG_UNUSED(data2);

    snmp_init();

    while (1)
    {
        snmp_loop();
    }
    
}
