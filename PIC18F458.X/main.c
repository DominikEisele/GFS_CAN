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
int i = 0;
int btn2;
int btn3;
unsigned long timems = 0;
unsigned long ms = 0;
int pause = 0;



void portinit();
void settimer();
void delay_ms (unsigned long ms);



#pragma interrupt high_isr
void high_isr(void)
{
  int tmr2ie, tmr2if;
  tmr2ie = (PIE1 >> 1) & 1;
  tmr2if = (PIR1 >> 1) & 1;

  if (tmr2ie && tmr2if) {
    timems++;
    PIR1 &= ~(1 << 1);  //Clear Flag
  }
}

 #pragma code HIGH_INTERRUPT_VECTOR = 0x08
 void high_interrupt(void)
{
 	_asm GOTO high_isr _endasm
}
#pragma code /* return to the default code section */

void portinit() 
{
    TRISD = 0;
    TRISB = 0xFF;
}

void settimer()
{
    T2CON =0x4D; 
    INTCON  |= (1 << 7); // GIEH=1
    INTCON  |= (1 << 6); // GIEL=1
    IPR2 |= (1 << 1); // TMR2IP=1
    PIE1 |= (1 << 1); // TMR2IE=1
    //PR2 = 0xF9;
}

void delay_ms (unsigned long ms)
{
    int tmp;
    tmp = timems;
    while(timems - tmp < ms) {
        _asm NOP _endasm
    }
        
}

void main (void) 
{
    portinit();
    settimer();
    while (1) {
        /*int tmp;
        int i;
        tmp = 0;
        i = 0;
        btn2 = (PORTB >> 4) & 1;
        btn3 = (PORTB >> 5) & 1;
        if(btn2 == 0) {
            pause = 300;
        } else if (btn3 == 0) {
            pause = 200;
        } else {
            pause = 400;
        }
        LATD = lauflicht;
        if (timems - tmp > pause) {
             lauflicht = lauflicht >> 1;
             i++;
             tmp = timems;
             LATD = lauflicht;
            }
        if (i > 6) {
            lauflicht = 0x01;
        }
        if (timems - tmp > pause && i>0) {
             lauflicht = lauflicht >> 1;
             i--;
             tmp = timems;
            }*/
        LATD = 0xFF ;
        delay_ms(2000);
        LATD = 0x00;
            
    }
}
