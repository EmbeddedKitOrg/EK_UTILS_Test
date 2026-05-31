#include "test.h"
#include "ek_picothread.h"
#include "bsp_led.h"

EK_LOG_FILE_TAG("test_picothread.c");

static void _led_task1(ek_pt_handle_t pt, void *arg);
static void _led_task2(ek_pt_handle_t pt, void *arg);

static ek_pt_handle_t s_pt1;
static ek_pt_handle_t s_pt2;

void picothread_test_init(void)
{
    s_pt1 = ek_pt_create("led_task1", _led_task1, 1, NULL);
    s_pt2 = ek_pt_create("led_task2", _led_task2, 2, NULL);

    EK_LOG_INFO("picothread init ok");
}

static void _led_task1(ek_pt_handle_t pt, void *arg)
{
    EK_PT_BEGIN(pt);

    LED1_TOGGLE();
    EK_LOG_INFO("LED1 Toggle");
    EK_PT_DELAY(pt, 500);
    LED2_TOGGLE();
    EK_LOG_INFO("LED2 Toggle");
    EK_PT_DELAY(pt, 500);

    EK_PT_END(pt);
}

static void _led_task2(ek_pt_handle_t pt, void *arg)
{
    EK_PT_BEGIN(pt);

    LED3_TOGGLE();
    EK_LOG_INFO("LED3 Toggle");
    EK_PT_DELAY(pt, 250);
    LED4_TOGGLE();
    EK_LOG_INFO("LED4 Toggle");
    EK_PT_DELAY(pt, 250);

    EK_PT_END(pt);
}
