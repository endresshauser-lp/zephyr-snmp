#include <zephyr/logging/log.h>
#include <zephyr/types.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys_clock.h>

#include <zephyr/kernel.h>

#include <lwip/opt.h>
#include <lwip/apps/snmp_mib2.h>

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

    u8_t sysdescr[256] = "Example Description";
    u16_t sysdescr_len = strlen(sysdescr);
    snmp_mib2_set_sysdescr(sysdescr, &sysdescr_len);

    u8_t syscontact[256] = "admin@example.com";
    u16_t syscontact_len = strlen(syscontact);
    u16_t syscontact_bufsize = sizeof(syscontact);
    snmp_mib2_set_syscontact(syscontact, &syscontact_len, syscontact_bufsize);
    
    u8_t sysname[256] = "Example Name";
    u16_t sysname_len = strlen(sysname);
    u16_t sysname_bufsize = sizeof(sysname);
    snmp_mib2_set_sysname(sysname, &sysname_len, sysname_bufsize);

    u8_t syslocation[256] = "Example Room";
    u16_t syslocation_len = strlen(syslocation);
    u16_t syslocation_bufsize = sizeof(syslocation);
    snmp_mib2_set_syslocation(syslocation, &syslocation_len, syslocation_bufsize);

    snmp_init();

    while (1)
    {
        snmp_loop();
    }
    
}
