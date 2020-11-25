#include "config.h"
#include "Timer.h"
#include "TempSetMode.h"
#include "ADC.h"
#include "EWHS.h"

unsigned char T0_ovfCounter;
unsigned char T1_ovfCounter;
unsigned int T2_ovfCounter;



void Timers_Init(void)
{
    /*Timer0*/
    
    //OPTION_REG register configurations
    OPTION_REGbits.T0CS = 0; // Timer0 uses internal clock source
    OPTION_REGbits.T0SE = 0; // Timer0 increments on 0 to 1 transition
 
    OPTION_REGbits.PSA = 0;  // Prescalar is assigned to timer0
    
    //prescalar value = 128
    OPTION_REGbits.PS0 = 0;
    OPTION_REGbits.PS1 = 1;
    OPTION_REGbits.PS2 = 1;
    
    //Clear the TMR0 register
    TMR0 = 0x00;
    
    //INTCON register configurations.
    INTCONbits.PEIE = 1;    
    INTCONbits.T0IE = 1;
    INTCONbits.T0IF = 0;
    INTCONbits.GIE = 1;
    
    /*Timer1*/
    T1CONbits.T1OSCEN = 0; // Disable timer1 oscillator
    T1CONbits.T1CKPS0 = 0; // prescaler bit1
    T1CONbits.T1CKPS1 = 1; // prescaler bit2
    T1CONbits.TMR1CS = 0;   // Select clock source (timer mode)
    T1CONbits.T1INSYNC = 0; // This bit is ignored anyway as long as timer1 is in timer mode
   
    INTCONbits.PEIE = 1; // Peripheral interrupt enable
    PIR1bits.TMR1IF = 0; // Interrupt flag bit clear
    PIE1bits.TMR1IE = 1;   // Enable timer1 interrupt
    INTCONbits.GIE = 1; // Enable global interrupt
    
    /*Timer2*/
    
    T2CON = 0x00;
    PIR1bits.TMR2IF = 0;
    PIE1bits.TMR2IE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.GIE = 1;
    
}

void Timer0_Restart(void)
{
    TMR0 = 0x00;
    T0_ovfCounter = 0; 
}

void Timer1_Start(void)
{
   TMR1 = 0x00;
   T1CONbits.TMR1ON = 1; // Enable timer 1
}
void Timer1_Stop(void)
{
    T1CONbits.TMR1ON = 0; // Disable timer 1
}
void Timer1_Restart(void)
{
    TMR1 = 0x00;
    T1_ovfCounter = 0;
    T1CONbits.TMR1ON = 1; // Enable timer 1
}

void Timer2_Start(void)
{
    TMR2 = 0x00;
    T2CONbits.TMR2ON = 1;
}


void Timer2_Stop(void)
{
    T2CONbits.TMR2ON = 0;
}

__interrupt() void ISR()
{
    if(INTCONbits.INTF)
    {
        state ^= 1;
        Timer1_Stop();
        Timer2_Stop();
        INTCONbits.INTF = 0;
    }
    else if(INTCONbits.T0IF)
    {
        T0_ovfCounter++;
        if(T0_ovfCounter == 3)
        {
            senseTempFlag = 1;          // This variable allows temperature measurement.
            T0_ovfCounter = 0;         // Reset the counter 
        }
        INTCONbits.T0IF = 0;    //clear interrupt flag
    }
    else if(PIR1bits.TMR1IF)
    {
        T1_ovfCounter++;
        if(T1_ovfCounter == 19)
        {
            T1_ovfCounter = 0;
            /*Write the action here*/
            Timer1_Stop();
            Timer2_Stop();
            E2PROM_flag = 1;
        }
        PIR1bits.TMR1IF = 0;
    }
    else if(PIR1bits.TMR2IF)
    {
        T2_ovfCounter++;
        if(T2_ovfCounter == 3806)
        {
            display_OFF ^= 1;
            T2_ovfCounter = 0;
        }
       PIR1bits.TMR2IF = 0; 
    }
        
    
}



