#include <Arduino.h>
#include "ADS1115.h"
#include "SD_Card.h"

/**************************************任务句柄***************************************************/
TaskHandle_t ADS1115_Task_Handle;
TaskHandle_t SD_Card_Task_Handle;




void setup(void)
{
  Serial.begin(115200);
 

}

void loop(void)
{
  delay(1000);
}

