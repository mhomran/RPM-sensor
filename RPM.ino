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
volatile int printLCD = 0;
volatile float rpm =0;
volatile int overflows =0;
volatile int overflowsDet =0;
volatile int counterR =0;
volatile int counterEYE =0;

void init_Timer1(void){
  TCCR1B |= (1 << ICES1); //triggers rising edge
  TIMSK1 |= 1 << ICIE1 | 1 << TOIE1;
  TCCR1B |= 1 << CS10 | 1 << CS12; // prescaler = 1024
}

ISR(TIMER1_OVF_vect){
  lcd.setCursor(0,1);
  lcd.print("                    ");
}

ISR(TIMER1_CAPT_vect){
  if(state == 1){
  TCNT1 = 0;
  overflows =0;
  state =0;
  }
  else{
  state=1;
  printLCD= 1;
  overflowsDet=overflows;
  counterR++;
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
  PORTB &= ~(1 << PINB0); //pull down
  init_Timer1();
  sei();
  
while (1) 
{
  if(printLCD == 1){
  rpm =  (937500.0/(ICR1 + (overflowsDet * 65536)));
  counterEYE = rpm/60;  //60 >> prints every second 1Hz = 60 RPM
  if (counterR >=counterEYE){
  counterR=0; 
  lcd.setCursor(0,1);
  lcd.print(rpm * 2);  //*2 because of LCD halves the value
  lcd.print(" RPM               ");
  printLCD=0;
}
}
}
}





