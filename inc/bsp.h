/*
 * bsp.h
 *
 *  Created on: May 4, 2021
 *      Author: shapa
 */

#pragma once

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    GPIO_UART_TX,
    GPIO_UART_RX,
    GPIO_LED_BLUE,
    GPIO_LED_1,
    GPIO_LED_2,
    GPIO_LED_3,
    GPIO_LED_GND,

    GPIO_Last,
} GPIO_t;

int BSP_Init(void);

int BSP_GPIO_set(int pin, int val);
int BSP_GPIO_get(int pin);

int BSP_trace(const void *data, size_t size);

#ifdef __cplusplus
}
#endif
