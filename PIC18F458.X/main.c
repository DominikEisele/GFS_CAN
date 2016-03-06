// CONFIG1H
#pragma config OSC = ECIO       // Oscillator Selection bits (EC oscillator w/ OSC2 configured as RA6)
//#pragma config OSC = RC
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))

// CONFIG2L
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config BORV = 25        // Brown-out Reset Voltage bits (VBOR set to 2.5V)
   
// CONFIG2H
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)

// CONFIG4L
#pragma config STVR = ON         // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause Reset)
#pragma config LVP = OFF         // Low-Voltage ICSP Enable bit (Low-Voltage ICSP enabled)
   
// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)
 
// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)
   
// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)
 
// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)
   
// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)
   
// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0001FFh) not protected from Table Reads executed in other blocks)
 




#include <p18F458.h>

int lauflicht = 0x01;
unsigned long counter = 0;
unsigned long cmax = 0;
int i = 0;
int btn2;
int btn3;
unsigned long timems = 0;
unsigned int interruptcounter = 5;


void portinit();
//void interrupt ISR();
void settimer();
void delay();
void delay_ms (unsigned long ms);




/*#pragma code low_isr=0x18
#pragma interruptlow low_isr
void low_isr (void)
{
    int tmr0ie, tmr0if;
    tmr0ie = (INTCON >> 5) & 1;
    tmr0if = (INTCON >> 2) & 1;

   if(tmr0ie && tmr0if)
   {
      //TMR0 Overflow ISR
      interruptcounter++;  //Increment Over Flow Counter


      //Clear Flag
      tmr0if=0;
   }
} */ /* low_isr */

//#pragma interrupt high_isr save=section(".tmpdata"),PROD,section("MATH_DATA")
#pragma interrupt high_isr
void high_isr(void)
{
  int tmr2ie, tmr2if;
  tmr2ie = (PIE1 >> 1) & 1;
  tmr2if = (PIR1 >> 1) & 1;

  if (tmr2ie && tmr2if) {
    //TMR0 Overflow ISR
    interruptcounter++;  //Increment Over Flow Counter
    //Clear Flag
    PIR1 &= ~(1 << 1);
  }
}
#pragma code HIGH_INTERRUPT_VECTOR = 0x08
 
void high_interrupt(void)
{
 	_asm GOTO high_isr _endasm
}
#pragma code /* return to the default code section */

 
 //---------------------------------------------------------------------
 // Low priority interrupt routines
 //---------------------------------------------------------------------
/*#pragma interrupt low_isr
void low_isr(void)
 {
         int tmr0ie, tmr0if;
    tmr0ie = (INTCON >> 5) & 1;
    tmr0if = (INTCON >> 2) & 1;

   if(tmr0ie && tmr0if)
   {
      //TMR0 Overflow ISR
      interruptcounter++;  //Increment Over Flow Counter


      //Clear Flag
      //INTCON &= ~(1 << 2);
   }

 }*/



 
 //---------------------------------------------------------------------
 // Low priority interrupt vector
 //---------------------------------------------------------------------
//#pragma code low_vector=0x18
/*#pragma code LOW_INTERRUPT_VECTOR = 0x18
void low_interrupt (void)
{
 	_asm GOTO low_isr _endasm
}
#pragma code*/




void delay() 
{    
    btn2 = (PORTB >> 4) & 1; /* 0=gedrueckt */
    btn3 = (PORTB >> 5) & 1; /* 0=gedrueckt */
    
    
    if (btn3 == 0) { /* ganz schnell */
      cmax = 10000;  
    }
    else if (btn2 == 0) { /* mittel*/
      cmax = 100000;
    }
    else { /* langsam */
      cmax = 500000;
    }
    for (counter = 0; counter < cmax; counter++) {
        ;
    }
}

    
void delay_ms (unsigned long ms)
{
    for (counter = 0; counter < ms; counter++) {
        ;
    }
}


void portinit() 
{
    TRISD = 0;
    TRISB = 0xFF;
}

void settimer()
{
    T2CON =0xFF; 
    INTCON  |= (1 << 7); // GIEH=1
    INTCON  |= (1 << 6); // GIEL=1
    IPR2 |= (1 << 1); // TMR2IP=1
    PIE1 |= (1 << 1); // TMR2IE=1
    
    /*RCON |= (1 << 7); // IPEN=1
   //T0CON = 0xCF;
   T0CON = 0xC7;
   //INTCON = 0xE0;
   //TRISA = 0xFF;
   
   INTCON = 0x00;
   
   INTCON2 |= (1 << 2); // TMR0IP=1
   //INTCON2 &= ~(1 << 2); // TMR0IP=0
   INTCON  |= (1 << 5); // TMR0IE=1*/
   
   
   
   //T0PS0=1; //Prescaler is divide by 256
   //T0PS1=1;
   //T0PS2=1;
   //PSA=0;      //Timer Clock Source is from Prescaler
   //T0CS=0;     //Prescaler gets clock from FCPU (5MHz)
   //T08BIT=1;   //8 BIT MODE
   //TMR0IE=1;   //Enable TIMER0 Interrupt
   //PEIE=1;     //Enable Peripheral Interrupt
   //GIE=1;      //Enable INTs globally
   //TMR0ON=1;      //Now start the timer!
   //Set RB1 as output because we have LED on it
   //while(1);   //Sit Idle Timer will do every thing!
}

//Main Interrupt Service Routine (ISR)
/*void interrupt ISR()
{
    TMR0IE = (INTCON >> 5) & 1;
    TMR0IF = (INTCON >> 2) & 1;

   if(TMR0IE && TMR0IF)
   {
      //TMR0 Overflow ISR
      interruptcounter++;  //Increment Over Flow Counter

      if(interruptcounter==76)
      {
         //Toggle RB1 (LED)

         if(PORTD==0)
            PORTD=1;
         else
            PORTD=0;

         interruptcounter=0;  //Reset Counter

      }

      //Clear Flag
      TMR0IF=0;
   }
}*/


void main (void) 
{
    portinit();
    settimer();
    while (1) {
      unsigned char tmp;// = TMR2;
      tmp = interruptcounter >> 3;
      LATD = tmp;
      /*delay ();
       LATD = lauflicht;
        for(i = 0; i<7; i++)
        {
            LATD = lauflicht;
            delay();
            lauflicht = lauflicht<<1;
            LATD = lauflicht;
            delay();
        }
        for(;i>0; i--)
        {
            LATD = lauflicht;
            delay();
            lauflicht >>= 1;
            LATD = lauflicht;
            delay();
        }    */
    }
}




/*#pragma code high_isr=0x08
#pragma interrupt high_isr
void high_isr (void)
{
    int tmr0ie, tmr0if;
    tmr0ie = (INTCON >> 5) & 1;
    tmr0if = (INTCON >> 2) & 1;

   if(tmr0ie && tmr0if)
   {
      //TMR0 Overflow ISR
      interruptcounter++;  //Increment Over Flow Counter


      //Clear Flag
      tmr0if=0;
   }
}*/ /* high_isr */





//test2