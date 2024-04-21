/**
 * @file gxhtc3.h
 * @author maxiao (maxiaogood@foxmail.com)
 * @brief gxhtc3温湿度传感器驱动
 * @version 0.1
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024, maxiao. All rights reserved.
 *
 */
#ifndef __GXHTC3_H
#define __GXHTC3_H
#include "esp_err.h"

extern esp_err_t gxhtc3_read_id(void);
extern esp_err_t gxhtc3_get_tah(void);
#endif // __GXHTC3_H
