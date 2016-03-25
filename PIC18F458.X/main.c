#pragma config OSC = ECIO       // Oscillator Selection bits (EC oscillator w/ OSC2 configured as RA6)
#pragma config OSCS = OFF       // Oscillator System Clock Switch Enable bit (Oscillator system clock switch option is disabled (main oscillator is source))
#pragma config PWRT = ON        // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOR = OFF        // Brown-out Reset Enable bit (Brown-out Reset disabled)
#pragma config BORV = 25        // Brown-out Reset Voltage bits (VBOR set to 2.5V)
#pragma config WDT = OFF        // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config WDTPS = 128      // Watchdog Timer Postscale Select bits (1:128)
#pragma config STVR = ON        // Stack Full/Underflow Reset Enable bit (Stack Full/Underflow will cause Reset)
#pragma config LVP = OFF        // Low-Voltage ICSP Enable bit (Low-Voltage ICSP enabled)
#pragma config CP0 = OFF        // Code Protection bit (Block 0 (000200-001FFFh) not code protected)
#pragma config CP1 = OFF        // Code Protection bit (Block 1 (002000-003FFFh) not code protected)
#pragma config CP2 = OFF        // Code Protection bit (Block 2 (004000-005FFFh) not code protected)
#pragma config CP3 = OFF        // Code Protection bit (Block 3 (006000-007FFFh) not code protected)
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0001FFh) not code protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code protected)
#pragma config WRT0 = OFF       // Write Protection bit (Block 0 (000200-001FFFh) not write protected)
#pragma config WRT1 = OFF       // Write Protection bit (Block 1 (002000-003FFFh) not write protected)
#pragma config WRT2 = OFF       // Write Protection bit (Block 2 (004000-005FFFh) not write protected)
#pragma config WRT3 = OFF       // Write Protection bit (Block 3 (006000-007FFFh) not write protected)
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0001FFh) not write protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write protected)
#pragma config EBTR0 = OFF      // Table Read Protection bit (Block 0 (000200-001FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit (Block 1 (002000-003FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit (Block 2 (004000-005FFFh) not protected from Table Reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit (Block 3 (006000-007FFFh) not protected from Table Reads executed in other blocks)
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
long tmp = 0;
int leuchtbalken = 0;


void portinit();
void settimer();
void setad();
void setcan();
void delay_ms (unsigned long ms);


#pragma interrupt high_isr
void high_isr(void) {
  int tmr2ie, tmr2if;
  tmr2ie = (PIE1 >> 1) & 1;
  tmr2if = (PIR1 >> 1) & 1;

  if (tmr2ie && tmr2if) {
    timems++;
    PIR1 &= ~(1 << 1);  //Clear Flag
  }
}
 
#pragma code HIGH_INTERRUPT_VECTOR = 0x08
 void high_interrupt(void) {
 	_asm GOTO high_isr _endasm
}
#pragma code /* return to the default code section */

 
void portinit() {
    TRISA = 0xFF;
    
    TRISB &= ~(1 << 2);
    TRISB |= (1 << 3);
    TRISB |= (1 << 4);
    TRISB |= (1 << 5);
    
    TRISC &= ~(1 << 2);
    
    TRISD = 0x00;
}

void settimer() {
    T2CON =0x4D; 
    INTCON  |= (1 << 7); // GIEH   = 1
    INTCON  |= (1 << 6); // GIEL   = 1
    IPR2 |= (1 << 1);    // TMR2IP = 1
    PIE1 |= (1 << 1);    // TMR2IE = 1
}

void setad(){ 
    ADCON0 = 0;
    ADCON1 = 0;
    
    ADCON0 |= (1 << 0);  // ADON  = 1
    ADCON0 |= (1 << 7);  // ADCS1 = 1
    ADCON0 |= (1 << 5);  // CHS2  = 1  
    ADCON0 |= (1 << 2);  // DONE  = 1 
    ADCON1 |= (1 << 6);  // ADCS2 = 1
}

void setcan() {
    CANCON |= (1 << 7);  // Set Config-Mode

    CANCON &= ~(1 << 1);
    CANCON &= ~(1 << 2);
    CANCON |= (1 << 3);
    CANCON &= ~(1 << 4);
    CANCON &= ~(1 << 5);
    CANCON &= ~(1 << 6);
    
    BRGCON1 |= (1 << 0);
    BRGCON1 &= ~(1 << 1);
    BRGCON1 &= ~(1 << 2);
    BRGCON1 |= (1 << 3);
    BRGCON1 &= ~(1 << 4);
    BRGCON1 &= ~(1 << 5);
    BRGCON1 &= ~(1 << 6);
    BRGCON1 &= ~(1 << 7);
    
    BRGCON2 |= (1 << 0);    
    BRGCON2 |= (1 << 1);
    BRGCON2 &= ~(1 << 2);
    BRGCON2 |= (1 << 3);
    BRGCON2 &= ~(1 << 4);
    BRGCON2 |= (1 << 5);
    BRGCON2 &= ~(1 << 6);
    BRGCON2 |= (1 << 7);
    
    BRGCON3 &= ~(1 << 0);
    BRGCON3 |= (1 << 1);
    BRGCON3 &= ~(1 << 2);
    BRGCON3 &= ~(1 << 6);
    
    CANCON &= ~(1 << 7); // Set Normal Mode
}

void delay_ms (unsigned long ms) {
    long tmp;
    tmp = timems;
    while(timems - tmp < ms) {
        _asm NOP _endasm
    }
}

void main (void) {
    portinit();
    settimer();
    setad();
    setcan();
    while (1) {
        ADCON0 |= (1 << 2);
        btn2 = (PORTB >> 4) & 1;
        if (btn2 == 1) {  
            if (ADRESH == 0) {
                leuchtbalken = 0x00;
            } else if (ADRESH <= 11) {
                leuchtbalken = 0x01;
            } else if (ADRESH <= 21) {
                leuchtbalken = 0x03;
            } else if (ADRESH <= 31) {
                leuchtbalken = 0x07;
            } else if (ADRESH <= 41) {
                leuchtbalken = 0x0F;
            } else if (ADRESH <= 51) {
                leuchtbalken = 0x1F;
            } else if (ADRESH <= 61) {
                leuchtbalken = 0x3F;
            } else if (ADRESH <= 71) {
                leuchtbalken = 0x7F;
            } else {
                leuchtbalken = 0xFF;
            }
            LATD = leuchtbalken;
            if (ADRESH > 81) {
                LATC |= (1 << 2);
            }
            else {
                LATC &= ~(1 << 2);
            }
        } else {
            LATC &= ~(1 << 2);
            LATD = ADRESH;
        }
    }
}
    
        
//        int tmp;
//        tmp = (ADCON0 >> 2) & 1;
//        if (tmp == 0){
//            LATD = ADRESH;
//            ADCON0 |= (1 << 2);
//        }    
//         ADCON0 |= (1 << 2);
//         delay_ms(10);
//         LATD = ADRESH;
//         delay_ms(10);
//    }
//        btn2 = (PORTB >> 4) & 1;
//        btn3 = (PORTB >> 5) & 1;
//        if ((btn2 == 0) && (btn3 == 0)) {
//            pause = 31;
//        } else if (btn3 == 0) {
//            pause = 200;
//        } else if (btn2 == 0) {
//            pause = 500;
//        } else {
//            pause = 1000;
//        }
//        LATD = lauflicht;
//        if ((timems - tmp) > pause) {
//             lauflicht = lauflicht << 1;
//             i++;
//             tmp = timems;
//            }
//         if (i > 7) {
//            lauflicht = 0x01;
//            i = 0;
//        }
//       
//    }
//}
