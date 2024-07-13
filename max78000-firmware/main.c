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

#define BUFFER_LENGTH 3000
#define INPUT_LENGTH  60
#define PRED_LENGTH   24
float send_buffer [BUFFER_LENGTH][2];

float input_buffer[6][INPUT_LENGTH];
int dataIndex = 0, inputIndex = 0;

float output[6];
int pred_buffer[PRED_LENGTH][2];
int predIndex = -1;

char message[2048];
char temp[64];

// #include "tensor_data.h"
#include "classifier/model.h"

int16_t bias_x, bias_y, bias_z;

#define MAX_RETRIES 6
#define TIMEOUT_MS 5000

bool send_message_with_retry(const char* message) {
    int retries = 0;
    bool success = false;

    clear_uart();

    while (retries < MAX_RETRIES && !success) {
        send_msg(message);

        uint32_t start_time = g_ticks;
        
        while ((g_ticks - start_time) * TICK_TIME_MS < TIMEOUT_MS) {
            if (recv_msg(g_serial_buffer)) {
                if (g_serial_buffer[0] != '\r') {
                    printf("Received: %s", g_serial_buffer);
                }
                if (strncmp(g_serial_buffer, "OK", strlen("OK")) == 0) {
                    clear_buffer();
                    success = true;
                    break;
                } else if (strncmp(g_serial_buffer, "+NWMQM", strlen("+NWMQM")) == 0) {
                    clear_buffer();
                    success = true;
                    break;
                }
                clear_buffer();
            }
            // 可以在這裡加入一個短暫的延遲，以避免過度佔用CPU
            delay_ms(100);
        }

        if (!success) {
            printf("Attempt %d failed. Retrying...\n", retries + 1);
            retries++;
            // 在重試之前可以加入一個短暫的延遲
            delay_ms(3000);
        }
    }

    if (success) {
        // printf("Message sent successfully.\n");
    } else {
        printf("Failed to send message after %d attempts.\n", MAX_RETRIES);
        printf("Message: %s\n", message);
    }

    return success;
}

// void MPU6050_DataCallback() {
//     static int16_t tmp_x, tmp_y, tmp_z;
//     static const float accel_scale =   2.0 / 32768.0; // For ±2g
//     static const float gyro_scale  = 250.0 / 32768.0; // For ±250°/s
//     static float32_t output[6];

//     static const int DS_FACTOR = 4;
//     static int ds_counter = 0;

//     static double acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z;
    
//     mpu6050_read_accel_x(&tmp_x);
//     mpu6050_read_accel_y(&tmp_y);
//     mpu6050_read_accel_z(&tmp_z);

//     tmp_x -= bias_x;
//     tmp_y -= bias_y;
//     tmp_z -= bias_z;

//     acc_x = tmp_x * accel_scale;
//     acc_y = -tmp_z * accel_scale;
//     acc_z = tmp_y * accel_scale;

//     mpu6050_read_gyro_x(&tmp_x);
//     mpu6050_read_gyro_y(&tmp_y);
//     mpu6050_read_gyro_z(&tmp_z);

//     gyro_x = tmp_x * gyro_scale;
//     gyro_y = -tmp_z * gyro_scale;
//     gyro_z = tmp_y * gyro_scale;

//     send_buffer[dataIndex][0] = acc_x;
//     send_buffer[dataIndex][1] = acc_y;

//     // printf("%5lld, %+8f, %+8f, %+8f, %+11f, %+11f, %+11f\n", 
//     //         g_ticks * TICK_TIME_MS, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z);

//     // printf("%5lld, %+8f, %+8f, %+8f\n", g_ticks * TICK_TIME_MS, acc_x, acc_y, acc_z);

//     ds_counter += 1;
//     if (ds_counter == DS_FACTOR) { 
//         input_buffer[0][inputIndex + INPUT_LENGTH / 2] = acc_x;
//         input_buffer[1][inputIndex + INPUT_LENGTH / 2] = acc_y;
//         input_buffer[2][inputIndex + INPUT_LENGTH / 2] = acc_z;
//         input_buffer[3][inputIndex + INPUT_LENGTH / 2] = gyro_x;
//         input_buffer[4][inputIndex + INPUT_LENGTH / 2] = gyro_y;
//         input_buffer[5][inputIndex + INPUT_LENGTH / 2] = gyro_z;

//         inputIndex += 1;
//         if (inputIndex == (INPUT_LENGTH / 2)) {       
//             forward_bw_classifier(input_buffer, output);
//             for (int i = 0; i < 6; i++) {
//                 memmove(input_buffer[i], input_buffer[i] + INPUT_LENGTH / 2, (INPUT_LENGTH / 2) * sizeof(float));
//             }

//             float32_t *output_1 = output, *output_2 = output + 4;

//             int max_index1 = 0, max_index2 = 0;
//             for (int i = 0; i < 4; i++) {
//                 if (output_1[i] > output_1[max_index1]) {
//                     max_index1 = i;
//                 }
//             }
//             for (int i = 0; i < 2; i++) {
//                 if (output_2[i] > output_2[max_index2]) {
//                     max_index2 = i;
//                 }
//             }
        
//             if (predIndex >= 0) {
//                 pred_buffer[predIndex][0] = max_index1;
//                 pred_buffer[predIndex][1] = max_index2;
//                 printf("%d: ", predIndex);
//                 printf("%d, %d\n", max_index1, max_index2);
//                 // printf("%d, %d\n", pred_buffer[predIndex][0], pred_buffer[predIndex][1]);
//             }
//             predIndex += 1;

//             inputIndex = 0;
//         }
//         ds_counter = 0;
//     }

//     dataIndex += 1;
// }

void MPU6050_DataCallback() {
    static int16_t tmp_x, tmp_y, tmp_z;
    static const float accel_scale =   2.0 / 32768.0; // For ±2g
    static const float gyro_scale  = 250.0 / 32768.0; // For ±250°/s

    static const int DS_FACTOR = 4;
    static int ds_counter = 0;

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

    send_buffer[dataIndex][0] = acc_x;
    send_buffer[dataIndex][1] = acc_y;

    // printf("%5lld, %+8f, %+8f, %+8f, %+11f, %+11f, %+11f\n", 
    //         g_ticks * TICK_TIME_MS, acc_x, acc_y, acc_z, gyro_x, gyro_y, gyro_z);

    // printf("%5lld, %+8f, %+8f, %+8f\n", g_ticks * TICK_TIME_MS, acc_x, acc_y, acc_z);
    
    if (forward_bw_classifier_pipeline(NULL, output) == 2) {
        float32_t *output_1 = output, *output_2 = output + 4;

        int max_index1 = 0, max_index2 = 0;
        for (int i = 0; i < 4; i++) {
            if (output_1[i] > output_1[max_index1]) {
                max_index1 = i;
            }
        }
        for (int i = 0; i < 2; i++) {
            if (output_2[i] > output_2[max_index2]) {
                max_index2 = i;
            }
        }
    
        if (predIndex >= 0) {
            pred_buffer[predIndex][0] = max_index1;
            pred_buffer[predIndex][1] = max_index2;
            printf("%d: ", predIndex);
            printf("%d, %d\n", max_index1, max_index2);
        }

        if (predIndex >= 1 
            && pred_buffer[predIndex    ][0] == pred_buffer[predIndex - 1][0] 
            // && pred_buffer[predIndex - 1][0] == pred_buffer[predIndex - 2][0]
            ) {

            LED_Off(LED_RED);
            LED_Off(LED_GREEN);
            LED_Off(LED_BLUE);

            switch (pred_buffer[predIndex][0])
            {
            case 0: // GREEN
                LED_On(LED_GREEN);
                break;
            case 1: // YELLOW
                LED_On(LED_GREEN);
                LED_On(LED_RED);
                break;
            case 2: // RED
                LED_On(LED_RED);
                break;
            default:
                break;
            }

            // printf("%d, %d\n", pred_buffer[predIndex][0], pred_buffer[predIndex][1]);
        }
        predIndex += 1;
    }

    ds_counter += 1;
    if (ds_counter == DS_FACTOR) { 
        input_buffer[0][inputIndex + INPUT_LENGTH / 2] = acc_x;
        input_buffer[1][inputIndex + INPUT_LENGTH / 2] = acc_y;
        input_buffer[2][inputIndex + INPUT_LENGTH / 2] = acc_z;
        input_buffer[3][inputIndex + INPUT_LENGTH / 2] = gyro_x;
        input_buffer[4][inputIndex + INPUT_LENGTH / 2] = gyro_y;
        input_buffer[5][inputIndex + INPUT_LENGTH / 2] = gyro_z;

        inputIndex += 1;
        if (inputIndex == (INPUT_LENGTH / 2)) {      
            forward_bw_classifier_pipeline(input_buffer, output);
            
            for (int i = 0; i < 6; i++) {
                memmove(input_buffer[i], input_buffer[i] + INPUT_LENGTH / 2, (INPUT_LENGTH / 2) * sizeof(float));
            } 
            inputIndex = 0;
        }
        ds_counter = 0;
    }

    dataIndex += 1;
}

void System_Init(void) {
    const uint32_t ticks = (SystemCoreClock / 1000) * TICK_TIME_MS;
    
    SysTick_Config(ticks);  

    setbuf(stdout, NULL); // Disable stdout buffering

    mpu6050_setup();
    wifi_setup();

    init_bw_classifier();

    g_ticks = 0;
    
    // Register callbacks
    register_callback(MPU6050_DataCallback, 1);
}

// void test_model() {
//     uint64_t start_ticks = g_ticks;
    
//     float32_t output[6];
    
//     for (int i = 0; i < 3; i++) {
//         start_ticks = g_ticks;
//         forward_bw_classifier_pipeline(tensor_data, output);   
//         printf("%lld\n", (g_ticks - start_ticks) * TICK_TIME_MS); 
//     }

//     // forward_bw_classifier(tensor_data, output);
//     // printf("%lld\n", (g_ticks - start_ticks) * TICK_TIME_MS); 

//     for (int i = 0; i < 6; i++) {
//         printf("%f ", output[i]);
//     }
//     printf("\n");

//     while (true) { }
// }

int main(void) {
    // int ret = E_NO_ERROR;
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

    LED_On(LED_GREEN);
    while (true) { 

        if (dataIndex == BUFFER_LENGTH) { break; }

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

    // printf("%d", );

    LED_Off(LED_RED);
    LED_Off(LED_GREEN);
    LED_Off(LED_BLUE);

    int return_status = forward_bw_classifier_pipeline(NULL, output);
    if (return_status != -1) {
        while (return_status != 2) {
            return_status = forward_bw_classifier_pipeline(NULL, output);
        }
        float32_t *output_1 = output, *output_2 = output + 4;

        int max_index1 = 0, max_index2 = 0;
        for (int i = 0; i < 4; i++) {
            if (output_1[i] > output_1[max_index1]) {
                max_index1 = i;
            }
        }
        for (int i = 0; i < 2; i++) {
            if (output_2[i] > output_2[max_index2]) {
                max_index2 = i;
            }
        }
    
        pred_buffer[predIndex][0] = max_index1;
        pred_buffer[predIndex][1] = max_index2;
        printf("%d: ", predIndex);
        printf("%d, %d\n", max_index1, max_index2);
        predIndex += 1;
    }

    clear_uart();

    LED_On(LED_BLUE);

    printf("Sending prediction\n");
    for (int i = 0; i < 2; i++) {
        memset(message, 0, sizeof(message));
        snprintf(message, sizeof(message), "AT+NWMQMSG='");
        for (int j = 0; j < PRED_LENGTH; j++) {
            snprintf(temp, sizeof(temp), "%d", pred_buffer[j][i]); 
            strcat(message, temp);

            if (j < PRED_LENGTH - 1) {
                strcat(message, ",");
            }
        }

        sprintf(temp, "','pred_%d'\r\n", i + 1);
        strcat(message, temp);

        printf("Sending pred_%d\n", i + 1);    
        send_message_with_retry(message);
        delay_ms(1000);
    }

    printf("Sending data\n");
    const int parts = 10;
    for (int channel = 0; channel < 2; channel++) {
        for (int part = 0; part < parts; part++) {
            memset(message, 0, sizeof(message));
            snprintf(message, sizeof(message), "AT+NWMQMSG='");

            int start = part * (BUFFER_LENGTH / parts);
            int end = (part + 1) * (BUFFER_LENGTH / parts);
            
            for (int i = start; i < end; i++) {
                snprintf(temp, sizeof(temp), "%.2f", send_buffer[i][channel]);
                strcat(message, temp);

                if (i < end - 1) {
                    strcat(message, ",");
                }
            }

            sprintf(temp, "','raw_ch%d_part%d'\r\n", channel + 1, part + 1);
            strcat(message, temp);

            printf("Sending data of channel%d part%d\n", channel + 1, part + 1);
            send_message_with_retry(message);
            delay_ms(1000);
        }
    }
    LED_Off(LED_BLUE);

    printf("Message sent to MQTT Broker\n");
}

void SysTick_Handler(void) {
    g_ticks++;
}

void delay_ms(uint32_t ms) {
    uint64_t start = g_ticks;
    while ((g_ticks - start) * TICK_TIME_MS < ms);
}
