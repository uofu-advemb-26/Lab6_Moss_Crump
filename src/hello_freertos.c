/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>

#include "FreeRTOS.h"
#include "task.h"

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/cyw43_arch.h"

int count = 0;
bool on = false;
xSemaphore_t semaphore;

#define MAIN_TASK_PRIORITY      ( tskIDLE_PRIORITY + 2UL ) 
#define BLINK_TASK_PRIORITY     ( tskIDLE_PRIORITY + 1UL )
#define MAIN_TASK_STACK_SIZE configMINIMAL_STACK_SIZE
#define BLINK_TASK_STACK_SIZE configMINIMAL_STACK_SIZE

void blink_task(__unused void *params) {
    hard_assert(cyw43_arch_init() == PICO_OK);

    xSemaphoreTake(semaphore, portMAX_DELAY);
    cout << "Blink task started" << endl;
    xSemaphoreGive(semaphore);
    // while (true) {
    //     cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, on);
    //     if (count++ % 11) on = !on;
    //     vTaskDelay(500);
    // }
}

void main_task(__unused void *params) {
    //xTaskCreate(blink_task, "BlinkThread",
                //BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, NULL);
    // char c;
    // while(c = getchar()) {
    //     if (c <= 'z' && c >= 'a') putchar(c - 32);
    //     else if (c >= 'A' && c <= 'Z') putchar(c + 32);
    //     else putchar(c);
    // }
    //vTaskDelay(1000);
    xSemaphoreTake(semaphore, portMAX_DELAY);
    cout << "Main task started" << endl;
    xSemaphoreGive(semaphore);
}

int main( void )
{
    stdio_init_all();
    const char *rtos_name;
    rtos_name = "FreeRTOS";
    TaskHandle_t task, blink1, blink2, blink3;
    semaphore = xSemaphoreCreateBinary();
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, &blink1);
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, &blink2);
    xTaskCreate(blink_task, "BlinkThread",
                BLINK_TASK_STACK_SIZE, NULL, BLINK_TASK_PRIORITY, &blink3);
    vtaskDelay(100);
    xTaskCreate(main_task, "MainThread",
                MAIN_TASK_STACK_SIZE, NULL, MAIN_TASK_PRIORITY, &task);
    
    vTaskStartScheduler();
    return 0;
}
