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


static void routine (void *arg) {
    int led = (int)arg;
    const int del = 200 + 300 * (led - GPIO_LED_BLUE);
    int val = 0;
    while (1) {
        BSP_GPIO_set(led, val);
        val = !val;
        vTaskDelay(del/2);
    }
}

int main() {

    BSP_Init();
    printf("starting\r\n");

//    xTaskCreate(routine, "", configMINIMAL_STACK_SIZE, (void*)GPIO_LED_BLUE, tskIDLE_PRIORITY, NULL);
    xTaskCreate(routine, "", configMINIMAL_STACK_SIZE, (void*)GPIO_LED_1, tskIDLE_PRIORITY, NULL);
    xTaskCreate(routine, "", configMINIMAL_STACK_SIZE, (void*)GPIO_LED_2, tskIDLE_PRIORITY, NULL);
    xTaskCreate(routine, "", configMINIMAL_STACK_SIZE, (void*)GPIO_LED_3, tskIDLE_PRIORITY, NULL);

    vTaskStartScheduler();
    return 0;
}
