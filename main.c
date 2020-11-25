/*
 * File:   main.c
 * Author: Mohab
 *
 * Created on July 7, 2020, 12:16 PM
 */


#include "config.h"
#include "Timer.h"
#include "TempSetMode.h"
#include "SevenSegment.h"
#include "ADC.h"
#include "Cooler_Heater.h"
#include "EWHS.h"
#include "I2C_EEPROM2.h"

/*void main(void)
{
    EWHS_Init();
    saveTemp2();
    retreiveTemp2();
    while(1)
    {
        temperatureSense();
        setDisplay(temp);
        tempSetMode();
        cooler_heaterControl();
    }
    return;
}*/

void main(void)
{
    EWHS_Init();
    while(1)
    {
        EWHS();
    }
    return;
}
