
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)
// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF  

#define _XTAL_FREQ 20000000

#define in1 PORTCbits.RC5
#define in2 PORTCbits.RC4
#define in3 PORTCbits.RC0
#define in4 PORTCbits.RC3

#include <xc.h>

unsigned char matEsq[]={0x20,0x10,0x01,0x08};
unsigned char matDir[]={0x08,0x01,0x10,0x20};

void mPassoEsq(unsigned int valor)
{
    unsigned int contar;
    valor = valor * 5.6889;  // 0.1334
    for(contar=0;contar<valor;contar++)
    {
        PORTC = matEsq[contar%4];
        __delay_ms(10);
    }
}
void mPassoDir(unsigned int valor)
{
    unsigned int contar;
    valor = valor * 5.6889;  // 0.1334
    for(contar=0;contar<valor;contar++)
    {
        PORTC = matDir[contar%4];
        __delay_ms(10);
    }
}

void main(void)
{
    TRISC = 0x00;
    PORTC = 0x00;
    
    while(1)
    {
        mPassoEsq(180);    // andar até 180°
        
        mPassoDir(180);    // andar até 180°
        __delay_ms(2000); // 2s
        
    }
}
