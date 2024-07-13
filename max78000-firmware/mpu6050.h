/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. All Rights Reserved.
 * (now owned by Analog Devices, Inc.),
 * Copyright (C) 2023 Analog Devices, Inc. All Rights Reserved. This software
 * is proprietary to Analog Devices, Inc. and its licensors.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/*
  mpu6050.h

  InvenSense MPU6050 driver header file.
*/

#ifndef _MPU6050_H_
#define _MPU6050_H_

#include <stdint.h>
#include "mxc_device.h"

// The I2C peripheral the MPU6050 is connected to and the bus speed.
#define MPU_I2C_INST MXC_I2C1
#define MPU_I2C_FREQ 400000

// MPU6050 address
#define MPU6050_ADDR 0x68

// Accelerometer ranges
#define MPU6050_ACCEL_RANGE_2G  0x00
#define MPU6050_ACCEL_RANGE_4G  0x01
#define MPU6050_ACCEL_RANGE_8G  0x02
#define MPU6050_ACCEL_RANGE_16G 0x03

// Gyroscope ranges
#define MPU6050_GYRO_RANGE_250DPS  0x00
#define MPU6050_GYRO_RANGE_500DPS  0x01
#define MPU6050_GYRO_RANGE_1000DPS 0x02
#define MPU6050_GYRO_RANGE_2000DPS 0x03

#ifdef __cplusplus
extern "C" {
#endif

int mpu6050_init(mxc_i2c_regs_t *i2c_inst);
// int mpu6050_get_accel_data(int16_t *accel_data);
// int mpu6050_get_gyro_data(int16_t *gyro_data);
int mpu6050_set_sample_rate(uint8_t rate);
int mpu6050_set_accel_range(uint8_t range);
int mpu6050_set_gyro_range(uint8_t range);
// int mpu6050_set_power_management(uint8_t setting, bool enable);

int mpu6050_config(void);

int mpu6050_read_accel_x(int16_t *accel_x);
int mpu6050_read_accel_y(int16_t *accel_y);
int mpu6050_read_accel_z(int16_t *accel_z);

int mpu6050_read_gyro_x(int16_t *gyro_x);
int mpu6050_read_gyro_y(int16_t *gyro_y);
int mpu6050_read_gyro_z(int16_t *gyro_z);

void mpu6050_setup();

#ifdef __cplusplus
}
#endif

#endif // _MPU6050_H_
