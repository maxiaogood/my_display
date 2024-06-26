/**
 * @file gxhtc3.c
 * @author maxiao (maxiaogood@foxmail.com)
 * @brief gxhtc3温湿度传感器驱动
 * @version 0.1
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024, maxiao. All rights reserved.
 *
 */
#ifndef __GXHTC3_C
#define __GXHTC3_C

#include "bsp_iic.h"
#include "gxhtc3.h"
#include <math.h>

uint8_t tah_data[6];
uint16_t rawValueTemp, rawValueHumi;
float temp = 0, humi = 0;
uint8_t temp_int, humi_int;

#define POLYNOMIAL 0x31 // P(x) = x^8 + x^5 + x^4 + 1 = 00110001

/**
 * @brief CRC校验
 *
 * @param crcdata 需要校验的数据
 * @param len 校验数据data的长度
 * @return uint8_t 校验结果
 */
uint8_t gxhtc3_calc_crc(uint8_t *crcdata, uint8_t len)
{
    uint8_t crc = 0xFF;

    for (uint8_t i = 0; i < len; i++)
    {
        crc ^= (crcdata[i]);
        for (uint8_t j = 8; j > 0; --j)
        {
            if (crc & 0x80)
                crc = (crc << 1) ^ POLYNOMIAL;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

/**
 * @brief 读取传感器ID
 *
 * @return esp_err_t
 */
esp_err_t gxhtc3_read_id(void)
{
    esp_err_t ret;
    uint8_t data[3];

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x70 << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0xEF, true);
    i2c_master_write_byte(cmd, 0xC8, true);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);
    if (ret != ESP_OK)
    {
        goto end;
    }
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x70 << 1 | I2C_MASTER_READ, true);
    i2c_master_read(cmd, data, 3, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);

    if (data[2] != gxhtc3_calc_crc(data, 2))
    {
        ret = ESP_FAIL;
    }
end:
    i2c_cmd_link_delete(cmd);

    return ret;
}

/**
 * @brief 传感器唤醒
 *
 * @return esp_err_t
 */
esp_err_t gxhtc3_wake_up(void)
{
    int ret;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x70 << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x35, true);
    i2c_master_write_byte(cmd, 0x17, true);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);

    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief 传感器开启测量
 *
 * @return esp_err_t
 */
esp_err_t gxhtc3_measure(void)
{
    int ret;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x70 << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0x7c, true);
    i2c_master_write_byte(cmd, 0xa2, true);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);

    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief 从传感器读出温湿度数据
 *
 * @return esp_err_t
 */
esp_err_t gxhtc3_read_tah(void)
{
    int ret;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x70 << 1 | I2C_MASTER_READ, true);
    i2c_master_read(cmd, tah_data, 6, I2C_MASTER_LAST_NACK);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);

    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief 传感器休眠
 *
 * @return esp_err_t
 */
esp_err_t gxhtc3_sleep(void)
{
    int ret;

    i2c_cmd_handle_t cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, 0x70 << 1 | I2C_MASTER_WRITE, true);
    i2c_master_write_byte(cmd, 0xB0, true);
    i2c_master_write_byte(cmd, 0x98, true);
    i2c_master_stop(cmd);

    ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_PERIOD_MS);

    i2c_cmd_link_delete(cmd);
    return ret;
}

/**
 * @brief 获取并计算温湿度数据
 *
 * @return esp_err_t
 */
esp_err_t gxhtc3_get_tah(void)
{
    int ret;

    gxhtc3_wake_up();
    gxhtc3_measure();
    vTaskDelay(20 / portTICK_PERIOD_MS);
    gxhtc3_read_tah();
    gxhtc3_sleep();

    if ((tah_data[2] != gxhtc3_calc_crc(tah_data, 2) || (tah_data[5] != gxhtc3_calc_crc(&tah_data[3], 2))))
    {
        temp = 0;
        humi = 0;
        ret = ESP_FAIL;
    }
    else
    {
        rawValueTemp = (tah_data[0] << 8) | tah_data[1];
        rawValueHumi = (tah_data[3] << 8) | tah_data[4];
        temp = (175.0 * (float)rawValueTemp) / 65535.0 - 45.0;
        humi = (100.0 * (float)rawValueHumi) / 65535.0;
        ret = ESP_OK;
    }
    return ret;
}

#endif // __GXHTC3_C
