/*
 * bsp.c
 *
 *  Created on: May 4, 2021
 *      Author: shapa
 */


#include "bsp.h"

#include "stm32f4xx_hal_rcc.h"
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx_hal_gpio_ex.h"

#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_usart.h"

#include <errno.h>

static void initRcc(void);
static void initGpio(void);
static void initUsart1(void);

#define ARRAY_SIZE(array) \
    ((long)(sizeof(array) / sizeof((array)[0])))

typedef struct {
    GPIO_TypeDef *const port;
    const GPIO_InitTypeDef config;
} GpioCfg_t;

int BSP_Init(void) {
    initRcc();
    initGpio();
    initUsart1();
    return 0;
}

static const GpioCfg_t s_gpioCfg[] = {
        [GPIO_UART_TX] = { GPIOA, { GPIO_PIN_9, MODE_AF, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART1 } }, // tx
        [GPIO_UART_RX] = { GPIOA, { GPIO_PIN_10, MODE_AF, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, GPIO_AF7_USART1 } }, // rx
        [GPIO_LED_BLUE] = { GPIOC, { GPIO_PIN_13, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0 } },

        [GPIO_LED_1] = { GPIOB, { GPIO_PIN_1, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0 } },
        [GPIO_LED_2] = { GPIOB, { GPIO_PIN_2, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0 } },
        [GPIO_LED_3] = { GPIOB, { GPIO_PIN_10, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0 } },
        [GPIO_LED_GND] = { GPIOB, { GPIO_PIN_0, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW, 0 } },
};

int BSP_GPIO_set(int pin, int val) {
    if (val)
        s_gpioCfg[pin].port->BSRR |= s_gpioCfg[pin].config.Pin;
    else
        s_gpioCfg[pin].port->BSRR |= s_gpioCfg[pin].config.Pin << 16;
    return 0;
}

int BSP_GPIO_get(int pin) {
    return -EIO;
}

static void initRcc(void) {

}

static void initGpio(void) {
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    for (int i = 0; i < ARRAY_SIZE(s_gpioCfg); ++i) {
        const GpioCfg_t* const itm = s_gpioCfg + i;
        HAL_GPIO_Init(itm->port, (GPIO_InitTypeDef*)&itm->config);
//            if (itm->config.Mode & 0x10010000)
//                HAL_NVIC_EnableIRQ(pin2Exti(itm->config.Pin));
    }
    BSP_GPIO_set(GPIO_LED_BLUE, 1);

}

static USART_HandleTypeDef s_tracerHandle;
static void initUsart1(void) {
    __HAL_RCC_USART1_CLK_ENABLE();
    static const USART_InitTypeDef ifaceParams = {
            115200,
            USART_WORDLENGTH_8B,
            USART_STOPBITS_1,
            USART_PARITY_NONE,
            USART_MODE_TX_RX,
            0,0,0
    };
    s_tracerHandle.Instance = USART1;
    s_tracerHandle.Init = ifaceParams;
    HAL_USART_Init(&s_tracerHandle);

    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

int BSP_trace(const void *data, size_t size) {
    int rv = HAL_USART_Transmit(&s_tracerHandle, (void*)data, size, 0xFFFF);
    if (rv)
        return -EIO;
    return size;
}
