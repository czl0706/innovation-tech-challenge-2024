/*
  mpu6050.c

  InvenSense MPU6050 driver.
*/

#include <stdint.h>
#include <stdlib.h>
#include "mxc.h"
#include "mpu6050.h"

// I2C address with AD0 pin pulled low
#define MPU6050_ADDR 0x68

// MPU6050 Register Map
#define MPU6050_REG_WHO_AM_I        0x75
#define MPU6050_REG_PWR_MGMT_1      0x6B
#define MPU6050_REG_ACCEL_XOUT_H    0x3B
#define MPU6050_REG_ACCEL_YOUT_H    0x3D
#define MPU6050_REG_ACCEL_ZOUT_H    0x3F
#define MPU6050_REG_GYRO_XOUT_H     0x43
#define MPU6050_REG_GYRO_YOUT_H     0x45
#define MPU6050_REG_GYRO_ZOUT_H     0x47
#define MPU6050_REG_SMPLRT_DIV      0x19
#define MPU6050_REG_CONFIG          0x1A
#define MPU6050_REG_GYRO_CONFIG     0x1B
#define MPU6050_REG_ACCEL_CONFIG    0x1C

// #define MPU6050_FIFO_EN_TEMP_OUT    0x80
// #define MPU6050_FIFO_EN_XG_FIFO     0x40
// #define MPU6050_FIFO_EN_YG_FIFO     0x20
// #define MPU6050_FIFO_EN_ZG_FIFO     0x10
// #define MPU6050_FIFO_EN_ACCEL       0x08
// #define MPU6050_FIFO_EN_SLV2_FIFO   0x04
// #define MPU6050_FIFO_EN_SLV1_FIFO   0x02
// #define MPU6050_FIFO_EN_SLV0_FIFO   0x01

static mxc_i2c_req_t i2c_req;

static inline int reg_write(uint8_t reg, uint8_t val)
{
    uint8_t buf[2] = { reg, val };

    i2c_req.tx_buf = buf;
    i2c_req.tx_len = sizeof(buf);
    i2c_req.rx_len = 0;

    return MXC_I2C_MasterTransaction(&i2c_req);
}

static inline int reg_read(uint8_t reg, uint8_t *dat)
{
    uint8_t buf[1] = { reg };

    i2c_req.tx_buf = buf;
    i2c_req.tx_len = sizeof(buf);
    i2c_req.rx_buf = dat;
    i2c_req.rx_len = 1;

    return MXC_I2C_MasterTransaction(&i2c_req);
}

int mpu6050_read_accel_x(int16_t *accel_x)
{
    uint8_t buf[2];
    int result;

    buf[0] = MPU6050_REG_ACCEL_XOUT_H;

    i2c_req.tx_buf = buf;
    i2c_req.tx_len = 1;
    i2c_req.rx_buf = buf;
    i2c_req.rx_len = 2;

    result = MXC_I2C_MasterTransaction(&i2c_req);
    if (result != E_NO_ERROR) {
        return result;
    }

    *accel_x = (buf[0] << 8) | buf[1]; // X-axis acceleration

    return E_NO_ERROR;
}

int mpu6050_read_accel_y(int16_t *accel_y)
{
    uint8_t buf[2];
    int result;

    buf[0] = MPU6050_REG_ACCEL_YOUT_H;

    i2c_req.tx_buf = buf;
    i2c_req.tx_len = 1;
    i2c_req.rx_buf = buf;
    i2c_req.rx_len = 2;

    result = MXC_I2C_MasterTransaction(&i2c_req);
    if (result != E_NO_ERROR) {
        return result;
    }

    *accel_y = (buf[0] << 8) | buf[1]; // Y-axis acceleration

    return E_NO_ERROR;
}

int mpu6050_read_accel_z(int16_t *accel_z)
{
    uint8_t buf[2];
    int result;

    buf[0] = MPU6050_REG_ACCEL_ZOUT_H;

    i2c_req.tx_buf = buf;
    i2c_req.tx_len = 1;
    i2c_req.rx_buf = buf;
    i2c_req.rx_len = 2;

    result = MXC_I2C_MasterTransaction(&i2c_req);
    if (result != E_NO_ERROR) {
        return result;
    }

    *accel_z = (buf[0] << 8) | buf[1]; // Z-axis acceleration

    return E_NO_ERROR;
}

int mpu6050_read_gyro_x(int16_t *gyro_x)
{
    uint8_t buf[2];
    int result;

    result = reg_read(MPU6050_REG_GYRO_XOUT_H, buf);
    if (result != E_NO_ERROR) {
        return result;
    }

    *gyro_x = (buf[0] << 8) | buf[1]; // X-axis

    return E_NO_ERROR;
}

int mpu6050_read_gyro_y(int16_t *gyro_y)
{
    uint8_t buf[2];
    int result;

    result = reg_read(MPU6050_REG_GYRO_YOUT_H, buf);
    if (result != E_NO_ERROR) {
        return result;
    }

    *gyro_y = (buf[0] << 8) | buf[1]; // Y-axis

    return E_NO_ERROR;
}

int mpu6050_read_gyro_z(int16_t *gyro_z)
{
    uint8_t buf[2];
    int result;

    result = reg_read(MPU6050_REG_GYRO_ZOUT_H, buf);
    if (result != E_NO_ERROR) {
        return result;
    }

    *gyro_z = (buf[0] << 8) | buf[1]; // Z-axis

    return E_NO_ERROR;
}

int mpu6050_set_sample_rate(uint8_t rate)
{
    return reg_write(MPU6050_REG_SMPLRT_DIV, rate);
}

int mpu6050_set_accel_range(uint8_t range)
{
    return reg_write(MPU6050_REG_ACCEL_CONFIG, range << 3);
}

int mpu6050_set_gyro_range(uint8_t range)
{
    return reg_write(MPU6050_REG_GYRO_CONFIG, range << 3);
}

int mpu6050_init(mxc_i2c_regs_t *i2c_inst)
{
    int result;
    uint8_t id;

    if (!i2c_inst)
        return E_NULL_PTR;

    i2c_req.i2c = i2c_inst;
    i2c_req.addr = MPU6050_ADDR;
    i2c_req.restart = 0;
    i2c_req.callback = NULL;

    if ((result = reg_read(MPU6050_REG_WHO_AM_I, &id)) != E_NO_ERROR)
        return result;
    if (id != MPU6050_ADDR)
        return E_NOT_SUPPORTED;

    // Wake up the MPU6050
    return reg_write(MPU6050_REG_PWR_MGMT_1, 0x00);
}

// int mpu6050_set_power_management(uint8_t setting, bool enable)
// {
//     uint8_t reg;

//     // 讀取當前的電源管理寄存器值
//     int result = reg_read(MPU6050_REG_PWR_MGMT_1, &reg);
//     if (result != E_NO_ERROR) {
//         return result;
//     }

//     // 根據enable標誌設置或清除對應的位
//     if (enable) {
//         reg |= setting;
//     } else {
//         reg &= ~setting;
//     }

//     // 寫回電源管理寄存器
//     return reg_write(MPU6050_REG_PWR_MGMT_1, reg);
// }

int mpu6050_config(void)
{
    int result;

    // 设置加速度计和陀螺仪范围
    result = mpu6050_set_accel_range(MPU6050_ACCEL_RANGE_2G);
    if (result != E_NO_ERROR) return result;

    result = mpu6050_set_gyro_range(MPU6050_GYRO_RANGE_250DPS);
    if (result != E_NO_ERROR) return result;

    // 设置采样率为1kHz
    result = mpu6050_set_sample_rate(0x07); // Sample rate divisor = 7, 1kHz/(1+7) = 125Hz
    if (result != E_NO_ERROR) return result;

    // 禁用数字低通滤波器（设置为0）
    result = reg_write(MPU6050_REG_CONFIG, 0x00); // Assuming register address for digital low pass filter
    if (result != E_NO_ERROR) return result;

    // 唤醒 MPU6050
    result = reg_write(MPU6050_REG_PWR_MGMT_1, 0x00); // Clear sleep bit to wake up
    if (result != E_NO_ERROR) return result;

    // 延时等待 MPU6050 稳定
    MXC_Delay(MXC_DELAY_MSEC(100)); // Adjust delay as necessary

    // int16_t tmp;
    // const float accel_scale = 2.0 / 32768.0; // For ±2g
    // const float gyro_scale = 250.0 / 32768.0; // For ±250°/s
    
    // while (1) {
    //     mpu6050_read_accel_x(&tmp);
    //     printf("%f ", tmp * accel_scale);

    //     mpu6050_read_accel_y(&tmp);
    //     printf("%f ", tmp * accel_scale);

    //     mpu6050_read_accel_z(&tmp);
    //     printf("%f ", tmp * accel_scale);

    //     mpu6050_read_accel_z(&tmp);
    //     printf("%f ", tmp * accel_scale);

    //     mpu6050_read_gyro_x(&tmp);
    //     printf("%f ", tmp * gyro_scale);

    //     mpu6050_read_gyro_y(&tmp);
    //     printf("%f ", tmp * gyro_scale);

    //     mpu6050_read_gyro_z(&tmp);
    //     printf("%f\n", tmp * gyro_scale);
    // }

    // // 读取初始数据以清除任何残留值
    // int16_t tmp[3];
    // result = mpu6050_get_accel_data(tmp);
    // if (result != E_NO_ERROR) return result;

    // result = mpu6050_get_gyro_data(tmp);
    // if (result != E_NO_ERROR) return result;

    return E_NO_ERROR;
}

void mpu6050_setup() 
{ 
    if (MXC_I2C_Init(MPU_I2C_INST, 1, 0) != E_NO_ERROR) {
        printf("Trouble initializing I2C instance.\n");
        while (1);
    }

    printf("I2C init successful\n");

    MXC_I2C_SetFrequency(MPU_I2C_INST, MPU_I2C_FREQ);

    while (mpu6050_init(MPU_I2C_INST) != E_NO_ERROR) {
        printf("Trouble initializing MPU6050.\n");
        // while (1);
        MXC_Delay(MXC_DELAY_MSEC(100));
    }

    while (mpu6050_config() != E_NO_ERROR) {
        printf("Trouble configuring MPU6050.\n");
        // while (1);
        MXC_Delay(MXC_DELAY_MSEC(100));
    }

    printf("MPU6050 init successful\n");
}