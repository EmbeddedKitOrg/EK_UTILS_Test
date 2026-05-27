#ifndef BSP_UART_H
#define BSP_UART_H

#include "main.h"

#define BSP_UART         USART0
#define BSP_UART_RCU     RCU_USART0
#define BSP_UART_TX_PORT GPIOA
#define BSP_UART_RX_PORT GPIOA
#define BSP_UART_TX_PIN  GPIO_PIN_9
#define BSP_UART_RX_PIN  GPIO_PIN_10

void bsp_uart_init(void);
int bsp_uart_send_data(uint8_t ch);

#endif //  BSP_UART_H
