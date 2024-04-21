/**
 * @file th_task.c
 * @author maxiao (maxiaogood@foxmail.com)
 * @brief 温度传感器任务
 * @version 0.1
 * @date 2024-04-21
 *
 * @copyright Copyright (c) 2024, maxiao. All rights reserved.
 *
 */
#ifndef __TH_TASK_H
#define __TH_TASK_H
#include "app_main.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "gxhtc3.h"

int temp_value, humi_value; // 室内实时温湿度值

// 获取温湿度的任务函数
void get_th_task(void *args)
{
    esp_err_t ret;
    int time_cnt = 0, date_cnt = 0;
    float temp_sum = 0.0, humi_sum = 0.0;
    // 检查温湿度芯片
    ret = gxhtc3_read_id();
    while(ret != ESP_OK)
    {
         ret = gxhtc3_read_id();
         ESP_LOGI(__FILE__,"GXHTC3 READ ID");
         vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    gxhtc3_get_tah();
    temp_value = round(temp);
    humi_value = round(humi);
    while (1)
    {
        ret = gxhtc3_get_tah(); // 获取一次温湿度
        if (ret != ESP_OK)
        {
            ESP_LOGE(__FILE__, "GXHTC3 READ TAH ERROR.");
        }
        else
        {                               // 如果成功获取数据
            temp_sum = temp_sum + temp; // 温度累计和
            humi_sum = humi_sum + humi; // 湿度累计和
            date_cnt++;                 // 记录累计次数
        }
        vTaskDelay(100 / portTICK_PERIOD_MS); // 延时100毫秒
        time_cnt++;                           // 每100毫秒+1
        if (time_cnt > 10)                    // 1秒钟到
        {
            // 取平均数 且把结果四舍五入为整数
            temp_value = round(temp_sum / date_cnt);
            humi_value = round(humi_sum / date_cnt);
            // 各标志位清零
            time_cnt = 0;
            date_cnt = 0;
            temp_sum = 0;
            humi_sum = 0;
            // 标记温湿度有新数值
            // th_update_flag = 1;
            ESP_LOGI(__FILE__, "TEMP:%d HUMI:%d", temp_value, humi_value);
        }
        if (icon_flag == 0)
        {
            break;
        }
    }
    vTaskDelete(NULL);
}

#endif // __TH_TASK_H
