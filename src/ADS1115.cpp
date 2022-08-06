#include "ADS1115.h"

#include <Adafruit_ADS1X15.h>
#include <Wire.h>

Adafruit_ADS1115 ADS1115[4];  /* Use this for the 16-bit version */



void ADS1115_Task( void * parameter)
{

    //                                                                       ADS1115
    //                                                                       -------
    // ads.setGain(GAIN_TWOTHIRDS);     // 2/3x gain +/- 6.144V  1 bit =     0.1875mV (default)
    // ads.setGain(GAIN_ONE);           // 1x gain   +/- 4.096V  1 bit =     0.125mV
    // ads.setGain(GAIN_TWO);           // 2x gain   +/- 2.048V  1 bit =     0.0625mV
    // ADS1115[0].setGain(GAIN_FOUR);   // 4x gain   +/- 1.024V  1 bit =     0.03125mV
    // ads.setGain(GAIN_EIGHT);         // 8x gain   +/- 0.512V  1 bit =     0.015625mV
    // ads.setGain(GAIN_SIXTEEN);       // 16x gain  +/- 0.256V  1 bit =     0.0078125mV

    Wire.begin(21,22);    //初始化I2C总线

    ADS1115[0].setGain(GAIN_FOUR);    // 4x gain   +/- 1.024V     0.03125mV
    if (!ADS1115[0].begin(0x48)) 
    {
        Serial.println("Failed to initialize ADS1115[0]!");
        while (1)
        {
            Serial.println("Failed to initialize ADS1115[0]!");
            delay(500);
        }
    }

    //最后一次唤醒的tick count，第一次使用需要赋值
    //以后此变量会由vTaskDelayUntil自动更新
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = 1000; // 间隔 ticks

    

    while(1)
    {
        int16_t adc0;
        int16_t volts0;

        adc0 = ADS1115[0].readADC_SingleEnded(0);
        volts0 = 1000*(ADS1115[0].computeVolts(adc0));

        Serial.println("-----------------------------------------------------------");
        Serial.print("AIN0: "); Serial.print(adc0); Serial.print("  "); Serial.print(volts0); Serial.println("mV");

        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }

    Serial.println("Ending ADS1115_Task");   //不会运行到这里，除非任务删除
    vTaskDelete( NULL );  //用于删除自身。
}