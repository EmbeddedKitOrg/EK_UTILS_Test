#include "main.h"
#include "ek_export.h"
#include "ek_io.h"
#include "ek_heap.h"
#include "ek_log.h"
#include "bsp_uart.h"
#include "bsp_led.h"

EK_LOG_FILE_TAG("main.c");

int main(void)
{
    nvic_priority_group_set(NVIC_PRIGROUP_PRE4_SUB0);
    systick_init();
    ek_heap_init();
    ek_export_init();

    while (1)
    {
        EK_LOG_INFO("hello world");
        LED1_TOGGLE();
        LED2_TOGGLE();
        LED3_TOGGLE();
        LED4_TOGGLE();
        delay_ms(500);
    }
}

EK_IO_FPUTC()
{
    bsp_uart_send_data(ch);
    return ch;
}

EK_LOG_GET_TICK()
{
    return get_tick();
}
