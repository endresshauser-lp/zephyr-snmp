#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys_clock.h>

#include <zephyr/kernel.h>

#include <lwip/opt.h>

void snmp_init(void);
void snmp_loop();

void snmp_mib2_set_syscontact(u8_t *ocstr, u16_t *ocstrlen, u16_t bufsize);
void snmp_mib2_set_syslocation(u8_t *ocstr, u16_t *ocstrlen, u16_t bufsize);

static void zephyr_snmp_agent(void *data0, void *data1, void *data2);

K_THREAD_DEFINE(zephyr_snmp_thread, CONFIG_SNMP_STACK_SIZE, zephyr_snmp_agent, NULL, NULL,
        NULL, CONFIG_SNMP_THREAD_PRIORITY, K_ESSENTIAL, 0);


static void zephyr_snmp_agent(void *data0, void *data1, void *data2)
{
    ARG_UNUSED(data0);
    ARG_UNUSED(data1);
    ARG_UNUSED(data2);

    u8_t syscontact[] = "admin@example.com";
    u16_t syscontact_len = sizeof(syscontact) - 1;
    u16_t syscontact_bufsize = sizeof(syscontact);
    snmp_mib2_set_syscontact(syscontact, &syscontact_len, syscontact_bufsize);

    u8_t syslocation[] = "Example Room";
    u16_t syslocation_len = sizeof(syslocation) - 1;
    u16_t syslocation_bufsize = sizeof(syslocation);
    snmp_mib2_set_syslocation(syslocation, &syslocation_len, syslocation_bufsize);

    snmp_init();

    while (1)
    {
        snmp_loop();
    }
    
}
