/*
 * main.c
 *
 *  Created on: May 4, 2021
 *      Author: shapa
 */

#include "bsp.h"


#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>

static void busy_wait(volatile int ms) {
    while (ms--) {
        volatile int cnt = 0x7FF;
        while (cnt--);
    }
}

int main() {

    BSP_Init();
    printf("starting\r\n");
    while (1) {
        BSP_GPIO_set(GPIO_LED_BLUE, 0);
        busy_wait(1000);
        BSP_GPIO_set(GPIO_LED_BLUE, 1);
        busy_wait(1000);
    }

    vTaskStartScheduler();
    return 0;
}
