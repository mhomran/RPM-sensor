  /*
 * rpmSpeed.c
 *
 * Created: 9/3/2018 8:36:28 PM
 * Author : Mohamed Hassanin
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL


volatile int stateSpeed = 1;
volatile int printSpeed = 0;
volatile int rpmSpeed =0;
volatile int wheelSpeed=0;
volatile int counterEyeSpeedTemp =0;
volatile int counterEyeSpeed =0;
volatile int OF_Speed = 0;

volatile int stateRpmEngine = 1;
volatile int printRpmEngine = 0;
volatile int rpmEngine =0;
volatile int counterEyeRpmEngineTemp =0;
volatile int counterEyeRpmEngine =0;
volatile int OF_RpmEngine = 0;

//prototypes
void init_Timer4(void);
void init_Timer5(void);

//------------------------------------------------------------------------------------------------------------//    
//main
int main(void)
{
    Serial.begin(9600);

    init_Timer4();
    init_Timer5();
    sei();

    while (1) 
    {
        if(printSpeed == 1){
            rpmSpeed =  (1875000.0/(ICR4));
            counterEyeSpeed = rpmSpeed/240;  //60 >> prints every second 1Hz = 60 rpmSpeed
            if (counterEyeSpeedTemp >=counterEyeSpeed){
                counterEyeSpeedTemp=0; 
                wheelSpeed =  ( (2 * 3.14159 * (0.5664/2)) /(60.0/rpmSpeed ) ) * (18.0 /5);
            if(wheelSpeed != -2727){                 //Garbage 
                Serial.print('k');              //A flag for the app
                Serial.print(wheelSpeed);
            }
            printSpeed=0;
            }
        }
        if(printRpmEngine == 1){
            rpmEngine =  (1875000.0/(ICR5));
            counterEyeRpmEngine = rpmEngine/240;  //60 >> prints every second 1Hz = 60 rpmSpeed
            if (counterEyeRpmEngineTemp >=counterEyeRpmEngine){
                counterEyeRpmEngineTemp=0;
                Serial.print('r');                    //A flag for the app
                Serial.print(rpmEngine);
                printRpmEngine=0;
            }
        } 

        //overFlows handlers
        if(OF_Speed){
            Serial.print('k');
            Serial.print('0');
        }
        if(OF_RpmEngine){
            Serial.print('r');
            Serial.print('0'); 
        }
    }
}   //end of main

///////////////////////////////////////////////////////////////////////////////
//this timer is for engine RPM
void init_Timer4(void){
    TCCR4B |= (1 << ICES4); //triggers rising edge
    TIMSK4 |= 1 << ICIE4 | 1 << TOIE4;
    TCCR4B |= 1 << CS40 | 1 << CS42; // prescaler = 1024
}

ISR(TIMER4_OVF_vect){
    OF_Speed = 1;
}

ISR(TIMER4_CAPT_vect){
    if(stateSpeed == 1){
        TCNT4 = 0;
        stateSpeed =0;
    }
    else{
        stateSpeed=1;
        printSpeed= 1;
        counterEyeRpmEngineTemp++;
    }
}
//////////////////////////////////////////////////////////////////////////////
void init_Timer5(void){
    TCCR5B |= (1 << ICES5); //triggers rising edge
    TIMSK5 |= 1 << ICIE5 | 1 << TOIE5;
    TCCR5B |= 1 << CS50 | 1 << CS52; // prescaler = 1024
}

ISR(TIMER5_OVF_vect){
    OF_RpmEngine = 1;
}

ISR(TIMER5_CAPT_vect){
    if(stateRpmEngine == 1){
        TCNT5 = 0;
        stateRpmEngine =0;
    }
    else{
        stateRpmEngine=1;
        printRpmEngine= 1;
        counterEyeSpeedTemp++;
    }
}
//////////////////////////////////////////////////////////////////////////////





