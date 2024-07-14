#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc.h"
#include "mxc_device.h"
#include "led.h"
#include "board.h"
#include "mxc_delay.h"

#include "wifi.h"
#include "mpu6050.h"
#include "callback.h"

#define TICK_TIME_MS 10
volatile uint64_t g_ticks = 0;
void delay_ms(uint32_t ms);

int dataIndex = 0;

int16_t bias_x, bias_y, bias_z;

void MPU6050_DataCallback() {
    static int16_t tmp_x, tmp_y, tmp_z;
    static const float accel_scale =   2.0 / 32768.0; // For ±2g
    static const float gyro_scale  = 250.0 / 32768.0; // For ±250°/s

    static double acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;
    
    mpu6050_read_accel_x(&tmp_x);
    mpu6050_read_accel_y(&tmp_y);
    mpu6050_read_accel_z(&tmp_z);

    tmp_x -= bias_x;
    tmp_y -= bias_y;
    tmp_z -= bias_z;

    acc_x = tmp_x * accel_scale;
    acc_y = -tmp_z * accel_scale;
    acc_z = tmp_y * accel_scale;

    mpu6050_read_gyro_x(&tmp_x);
    mpu6050_read_gyro_y(&tmp_y);
    mpu6050_read_gyro_z(&tmp_z);

    gyro_x = tmp_x * gyro_scale;
    gyro_y = -tmp_z * gyro_scale;
    gyro_z = tmp_y * gyro_scale;

    // printf("%5lld, %+8f, %+8f, %+8f\n", g_ticks * TICK_TIME_MS, acc_x, acc_y, acc_z);

    printf("%5lld, %+8f, %+8f, %+8f, %+11f, %+11f, %+11f\n", 
            g_ticks * TICK_TIME_MS, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z);
}

void System_Init(void) {
    const uint32_t ticks = (SystemCoreClock / 1000) * TICK_TIME_MS;
    
    SysTick_Config(ticks);  

    setbuf(stdout, NULL); // Disable stdout buffering

    mpu6050_setup();

    g_ticks = 0;
    
    // Register callbacks
    register_callback(MPU6050_DataCallback, 1);
}

int main(void) {
    LED_On(LED_RED);

    System_Init();

    int16_t tmp_x, tmp_y, tmp_z;
    int32_t x = 0, y = 0, z = 0;
    const int samp = 500;
    
    for(int i = 0; i < samp; i++) {
        mpu6050_read_accel_x(&tmp_x);
        mpu6050_read_accel_y(&tmp_y);
        mpu6050_read_accel_z(&tmp_z);
        x += tmp_x;
        y += tmp_y;
        z += tmp_z;
        delay_ms(TICK_TIME_MS);
    }
    bias_x = x / samp;
    bias_y = y / samp;
    bias_z = z / samp;

    printf("Start\n");
    LED_Off(LED_RED);

    while (true) { 
        for (uint32_t i = 0; i < num_callbacks; i++) {
            if (callbacks[i].callback == NULL) {
                break; 
            }
            
            if ((g_ticks - callbacks[i].last_called) >= callbacks[i].period_ticks) {
                callbacks[i].callback();
                callbacks[i].last_called = g_ticks;
            }
        }
    }    
}

void SysTick_Handler(void) {
    g_ticks++;
}

void delay_ms(uint32_t ms) {
    uint64_t start = g_ticks;
    while ((g_ticks - start) * TICK_TIME_MS < ms);
}
