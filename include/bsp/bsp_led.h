#ifndef BSP_LED_H
#define BSP_LED_H

#include "main.h"

#define BSP_LED1_PORT GPIOE
#define BSP_LED2_PORT GPIOD
#define BSP_LED3_PORT GPIOG
#define BSP_LED4_PORT GPIOA
#define BSP_LED1_PIN  GPIO_PIN_3
#define BSP_LED2_PIN  GPIO_PIN_7
#define BSP_LED3_PIN  GPIO_PIN_3
#define BSP_LED4_PIN  GPIO_PIN_5

#define LED1_TOGGLE() gpio_bit_toggle(BSP_LED1_PORT, BSP_LED1_PIN)
#define LED2_TOGGLE() gpio_bit_toggle(BSP_LED2_PORT, BSP_LED2_PIN)
#define LED3_TOGGLE() gpio_bit_toggle(BSP_LED3_PORT, BSP_LED3_PIN)
#define LED4_TOGGLE() gpio_bit_toggle(BSP_LED4_PORT, BSP_LED4_PIN)

#define LED1_SET(X)   gpio_bit_write(BSP_LED1_PORT, BSP_LED1_PIN, (bit_status)(X))
#define LED2_SET(X)   gpio_bit_write(BSP_LED2_PORT, BSP_LED2_PIN, (bit_status)(X))
#define LED3_SET(X)   gpio_bit_write(BSP_LED3_PORT, BSP_LED3_PIN, (bit_status)(X))
#define LED4_SET(X)   gpio_bit_write(BSP_LED4_PORT, BSP_LED4_PIN, (bit_status)(X))

void bsp_led_init(void);

#endif // BSP_LED_H
