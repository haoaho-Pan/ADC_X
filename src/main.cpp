#include <Arduino.h>
#include "ADS1115.h"
#include "SD_Card.h"

/**************************************任务句柄***************************************************/
TaskHandle_t ADS1115_Task_Handle;
TaskHandle_t SD_Card_Task_Handle;




void setup(void)
{
  Serial.begin(115200);

  // xTaskCreate(
  //               ADS1115_Task         , //任务函数
  //               "ADS1115_Task"       , //带任务名称的字符串
  //               2*1024               , //堆栈大小，单位为字节
  //               NULL                 , //任务输入传递的参数
  //               5                    , //任务的优先级
  //               &ADS1115_Task_Handle   //任务句柄
  //            );      

  // xTaskCreate(
  //               SD_Card_Task         , //任务函数
  //               "SD_Card_Task"       , //带任务名称的字符串
  //               2*1024               , //堆栈大小，单位为字节
  //               NULL                 , //任务输入传递的参数
  //               5                    , //任务的优先级
  //               &SD_Card_Task_Handle   //任务句柄
  //            );     

}

void loop(void)
{
  delay(1000);
}

