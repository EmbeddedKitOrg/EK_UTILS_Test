#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>

void systick_init(void);
void delay_ms(uint32_t xms);
uint32_t get_tick(void);
void tick_inc(uint32_t tick);

uint32_t get_us_counter(void);

#endif
