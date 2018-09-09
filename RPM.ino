  /*
 * RPM.c
 *
 * Created: 9/3/2018 8:36:28 PM
 * Author : Mohamed Hassanin
 */ 
#include <LiquidCrystal.h>

LiquidCrystal lcd(A1, A0, 2, 3, 4, 5);

#include <avr/io.h>
#include <avr/interrupt.h>
#define F_CPU 16000000UL


volatile int state = 1;
volatile float rpm;
volatile int overflows =0;
volatile int overflowsDet =0;
void init_Timer1(void){
  TCCR1B &=~(1 << ICES1);              //falling
  TIMSK1 |= 1 << ICIE1 | 1 << TOIE1;
  TCCR1B |= 1 << CS10 | 1 << CS12; //1024
}

ISR(TIMER1_OVF_vect){
  overflows++;
}



ISR(TIMER1_CAPT_vect){
  if(state == 1){
  TCNT1 = 0;
  overflows =0;
  //TCCR1B |= (1 << ICES1); //rising
  state =0;
  }
  else{
  state=1;
  //TCCR1B &=~(1 << ICES1);
  overflowsDet=overflows;
  }
}

int main(void)
{
  DDRD = 0b00111100;
  DDRC |= 1 << PINC0 | 1 << PINC1;
  
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("SHELL E MARATHON");
  // Turn off the blinking cursor:
  lcd.noBlink();
  
  PORTB |=(1 << PINB0); //pull up
  init_Timer1();
  
  sei();
  
    while (1) 
    {
      if(state == 1)
  rpm =  (937500.0/(ICR1 + (overflowsDet * 65536)));
      lcd.setCursor(0,1);
  lcd.print(rpm);
  lcd.print(" RPM               ");
    }
}





