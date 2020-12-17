#include <c_types.h>
#include <osapi.h>
#include <user_interface.h>

#include "user_webserver.h"

/* local defines... */
#define VE_TASKPRIO_LOW    0
#define VE_TASKPRIO_MED    1
#define VE_TASKPRIO_HIGH   2

#define VE_TIMER_NOREPEAT  0
#define VE_TIMER_REPEAT    1

/* local helpers... */
#define __asize(x)  (sizeof(x) / sizeof(x[0]))
#define __d(x) { os_printf("%s:%u %s\n", __FUNCTION__, __LINE__, x); }

/* missing declarations in SDK-includes */
extern int os_printf_plus(const char *format, ...);
extern void ets_timer_setfn(os_timer_t *, os_timer_func_t *, void *);
extern void ets_timer_arm_new(os_timer_t *, uint32_t, bool, bool);
extern void uart_div_modify(uint8 uart_no, uint32 baudrate);

/* local types... */
enum __local_event_t {
    LOCAL_EVENT_WAIT_USER_TO_REPLUG_CABLE,
    LOCAL_EVENT_SAYHELLO
};

uint32 ICACHE_FLASH_ATTR
user_rf_cal_sector_set(void)
{
    enum flash_size_map size_map = system_get_flash_size_map();
    uint32 rf_cal_sec = 0;

    switch (size_map) {
        case FLASH_SIZE_4M_MAP_256_256:
            rf_cal_sec = 128 - 8;
            break;

        case FLASH_SIZE_8M_MAP_512_512:
            rf_cal_sec = 256 - 5;
            break;

        case FLASH_SIZE_16M_MAP_512_512:
        case FLASH_SIZE_16M_MAP_1024_1024:
            rf_cal_sec = 512 - 5;
            break;

        case FLASH_SIZE_32M_MAP_512_512:
        case FLASH_SIZE_32M_MAP_1024_1024:
            rf_cal_sec = 1024 - 5;
            break;

        default:
            rf_cal_sec = 0;
            break;
    }

    return rf_cal_sec;
}

static void ICACHE_FLASH_ATTR
__local_event_sayhello() {

    /* and finally what we are after for */
    os_printf("Hello world!\n");
}

static void ICACHE_FLASH_ATTR
__local_delay_callback(void *arg) {

    /* send the actual SAYHELLO -message */
    system_os_post(VE_TASKPRIO_LOW, LOCAL_EVENT_SAYHELLO, 0);
}

static void ICACHE_FLASH_ATTR
__local_event_waituser_to_replug_cable() {

    static os_timer_t __timer;
    
    /* since you need some time to switch back to working serial
       terminal to actually see the "Hello world", we need to delay
       actual printage. esp boots quite fast */

    /* ...so delay of 10000 milliseconds, increase if you are slow */
    os_timer_setfn(&__timer, __local_delay_callback,
		   NULL /* arg to callback */);
    os_timer_arm(&__timer, 10000, VE_TIMER_NOREPEAT);
}

static void ICACHE_FLASH_ATTR
__local_event_callback(os_event_t *ev) {

    enum __local_event_t event =
	(enum __local_event_t)ev->sig;
    
    switch(event) {
    case LOCAL_EVENT_WAIT_USER_TO_REPLUG_CABLE:
	return __local_event_waituser_to_replug_cable();
    case LOCAL_EVENT_SAYHELLO:
	return __local_event_sayhello();
    }
}

void ICACHE_FLASH_ATTR
user_init() {

    /* either static or declared outside!! */
    static os_event_t __local_task_queue[8];

    /* mandatory initialization
       - baud-rate */
    uart_div_modify(0 /* uart0 */, UART_CLK_FREQ / 115200);

    /* initialize taskqueue */
    system_os_task(__local_event_callback, VE_TASKPRIO_LOW,
		   __local_task_queue, __asize(__local_task_queue));


    /* post callback */
    system_os_post(VE_TASKPRIO_LOW,
		   LOCAL_EVENT_WAIT_USER_TO_REPLUG_CABLE, 0);

    user_webserver_init(80);

}