/**
 * @file sensor.h
 * @author maxiao (maxiaogood@foxmail.com)
 * @brief 传感器通用抽象层
 * @version 0.1
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024, maxiao. All rights reserved.
 *
 */
#ifndef __SENSOR_H
#define __SENSOR_H
#include "esp_err.h"

typedef struct __SENSOR_STRUCT SENSOR_STRUCT;

// 传感器保留接口,用于不要求传感器返回值的情况
static inline void *sensor_res(SENSOR_STRUCT *const handle);
// 传感器保留接口,用于需要传感器返回值的情况
static inline esp_err_t *sensor_return_res(SENSOR_STRUCT *const handle);


typedef esp_err_t *sensor_check(SENSOR_STRUCT *const handle);
typedef void *sensor_init(SENSOR_STRUCT *const handle);
typedef void *sensor_read(SENSOR_STRUCT *const handle);

struct __SENSOR_STRUCT
{
    const sensor_check check; // 传感器检查函数, 检查传感器是否存在
    const sensor_init init;   // 传感器初始化函数
    const sensor_read read;   // 传感器读取函数
};
#endif // __SENSOR_H
