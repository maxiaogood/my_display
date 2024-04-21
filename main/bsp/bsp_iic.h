/**
 * @file bsp_iic.h
 * @author maxiao (maxiaogood@foxmail.com)
 * @brief IIC驱动
 * @version 0.1
 * @date 2024-04-21
 * 
 * @copyright Copyright (c) 2024, maxiao. All rights reserved.
 * 
 */
#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "driver/i2c.h"
#include "esp_err.h"

#define I2C_MASTER_SCL_IO           GPIO_NUM_1      /*!< GPIO number used for I2C master clock */
#define I2C_MASTER_SDA_IO           GPIO_NUM_0      /*!< GPIO number used for I2C master data  */
#define I2C_MASTER_NUM              0               /*!< I2C master i2c port number, the number of i2c peripheral interfaces available will depend on the chip */
#define I2C_MASTER_FREQ_HZ          400000          /*!< I2C master clock frequency */
#define I2C_MASTER_TX_BUF_DISABLE   0               /*!< I2C master doesn't need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0               /*!< I2C master doesn't need buffer */
#define I2C_MASTER_TIMEOUT_MS       1000

extern esp_err_t i2c_master_init(void);
#endif // __BSP_IIC_H
