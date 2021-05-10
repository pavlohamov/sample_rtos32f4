/*
 * main.c
 *
 *  Created on: May 4, 2021
 *      Author: shapa
 */

#include "bsp.h"

#include <stdio.h>
#include <string.h>

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#include "stm32f4xx_hal_cortex.h"
#include "stm32f4xx_hal_dma.h"
#include "stm32f4xx_hal_uart.h"

static struct {
    int curr;
    SemaphoreHandle_t rxSem;
    char buff[1024];
} s_dev;

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(BSP_getUartHandle());
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *husart) {
    xSemaphoreGiveFromISR(s_dev.rxSem, NULL);
}

static void shell (void *arg) {

    UART_HandleTypeDef *uart = BSP_getUartHandle();

    while (1) {
        HAL_UART_Receive_IT(uart, s_dev.buff + s_dev.curr, 1);
        s_dev.curr = (s_dev.curr + 1) % sizeof(s_dev.buff);
        if (!xSemaphoreTake(s_dev.rxSem, 10000))
            continue;
        char last = s_dev.buff[s_dev.curr - 1];
        BSP_trace("\r", 1);
        if (last == 0x7F) { // backspace
            for (int i = 0; i < s_dev.curr; ++i)
                BSP_trace(" ", 1);
            s_dev.curr -= 2;
            if (s_dev.curr < 0)
                s_dev.curr = 0;
            BSP_trace("\r", 1);
        }
        BSP_trace(s_dev.buff, s_dev.curr);
        if (last == '\r') {
            BSP_trace("\r\n", 2);
            s_dev.buff[s_dev.curr - 1] = 0;
            s_dev.curr = 0;
            if (strstr(s_dev.buff, "led on"))
                BSP_GPIO_set(GPIO_LED_BLUE, 0);
            else if (strstr( s_dev.buff, "led off"))
                BSP_GPIO_set(GPIO_LED_BLUE, 1);
            else if (*s_dev.buff) {
                printf("unknown '%s' cmd\r\n", s_dev.buff);
            }
        }

    }
}

int main() {
    vSemaphoreCreateBinary(s_dev.rxSem);
    xSemaphoreTake(s_dev.rxSem, 0); // freertos is ugly. semaphore must be inited with specific value

    BSP_Init();
    printf("starting\r\n");

    xTaskCreate(shell, "", configMINIMAL_STACK_SIZE, NULL, tskIDLE_PRIORITY, NULL);

    vTaskStartScheduler();
    return 0;
}
