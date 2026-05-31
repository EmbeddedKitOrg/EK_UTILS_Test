#include "systick.h"
#include "gd32f4xx_libopt.h"

static volatile uint32_t s_sys_tick = 0;
static volatile uint32_t s_delay_ms_tick = 0;

void systick_init(void)
{
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }
    nvic_irq_enable(SysTick_IRQn, 0, 15);
}

__WEAK uint32_t get_us_counter(void)
{
    return 0;
}

void delay_ms(uint32_t xms)
{
    s_delay_ms_tick = xms;

    while (s_delay_ms_tick);
}

uint32_t get_tick(void)
{
    return s_sys_tick;
}

void tick_inc(uint32_t tick)
{
    s_sys_tick += tick;
    if (s_delay_ms_tick)
    {
        s_delay_ms_tick--;
    }
}
