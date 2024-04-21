/**
 * @file bsp_iic.c
 * @author maxiao (maxiaogood@foxmail.com)
 * @brief IIC驱动
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) 2024, maxiao. All rights reserved.
 * 
 */
#ifndef __BSP_IIC_C
#define __BSP_IIC_C

#include "bsp_iic.h"

/**
 * @brief i2c master initialization
 */
esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };

    i2c_param_config(i2c_master_port, &conf);

    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}
#endif // __BSP_IIC_C
