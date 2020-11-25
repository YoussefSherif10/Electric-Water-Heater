#include <pic16f877a.h>

#include "ADC.h"
#include "Timer.h"
#include "Cooler_Heater.h"
#include "SevenSegment.h"
#include "TempSetMode.h"
#include "I2C_EEPROM2.h"
#include "EWHS.h" 


unsigned char state = 0;
unsigned char OnState = 0;
unsigned char OffState = 0;


// Initialize software
void EWHS_Init(void)
{
    I2C_Master_Init(100000);
    Timers_Init();
    ADC_Init();
    buttonsInit();
    sevenSegmentInit();
    cooler_heaterInit();
    On_Off_SwitchInit();
}

void On_Off_SwitchInit(void)
{
    // Set pin 0 in portB as input
    TRISBbits.TRISB0 = 1;
    PORTBbits.RB0 = 0;
    
    // activate pullup resistor in portb
    OPTION_REGbits.nRBPU = 0;
    
    // interrupt sense on falling edge
    OPTION_REGbits.INTEDG = 0;
    //External interrupt enable bit.
    INTCONbits.INTE = 1;
    // External interrupt flag clear
    INTCONbits.INTF = 0;
    //Global interrupt enable bit.
    INTCONbits.GIE = 1;
}

// Turn off software
void EWHS_Off(void)
{
    if(OffState == 1)
    {
        /* DO NOTHING! */
    }
    else
    {
        OffState = 1;
        OnState = 0;
        ADC_Off();
        
        sevenSegmentOFF();
        PORTCbits.RC5 = 0;  // heater off
        PORTCbits.RC2 = 0;  // cooler off
    }
}

// Turn on software
void EWHS_On(void)
{
    if(OnState == 1)
    {
        temperatureSense();
        setDisplay(temp);
        tempSetMode();
        cooler_heaterControl();
    }
    else
    {
        OnState = 1;
        OffState = 0;
        retreiveTemp2();
        ADC_On();
        Timer0_Restart();
        
    }
}

void EWHS(void)
{
    if(state == 0)
    {
        EWHS_Off();
    }
    else if (state == 1)
    {
        EWHS_On();
    }
        
}

