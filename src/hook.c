/*
 * hook.c
 *
 *  Created on: May 6, 2021
 *      Author: shapa
 */

#include "FreeRTOS.h"
#include "task.h"
#include "bsp.h"
#include <stdio.h>

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize) {
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationStackOverflowHook(TaskHandle_t task, char *name) {
    printf("%s\r\n", __FUNCTION__);
    while(1);
}

void vApplicationMallocFailedHook(void) {
    printf("%s\r\n", __FUNCTION__);
    while(1);
}
