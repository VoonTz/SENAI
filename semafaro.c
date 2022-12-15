#include <xc.h>
#include "lib_v01.h"
#include "config.h"

#define R0 PORTDbits.RD0
#define R1 PORTDbits.RD1
#define R2 PORTDbits.RD2
#define R3 PORTDbits.RD3

#define s0 PORTDbits.RD4
#define s1 PORTDbits.RD5
#define s2 PORTDbits.RD6

void main(void) 
{   
    picIniciar();
    lcdIniciar();
    
    lcdPos(1,1);
    lcdTexto("Luan");
    
    lcdPos(1,8);
    lcdTexto("Pedestre");
    
    while(1)
    {
        lcdPos(2,1);
        lcdTexto("VM");
        
        lcdPos(2,8);
        lcdTexto("VD");
        R0 = 1;
        
        R3 = 1;
        __delay_ms(3000);
        
        lcdPos(2,1);
        lcdTexto("AM");
        
        lcdPos(2,8);
        lcdTexto("VM");
        
        R0 = 0;
        R1 = 1;
        
        R3 = 0;
        __delay_ms(1000);
        
        lcdPos(2,1);
        lcdTexto("VD");
        
        lcdPos(2,8);
        lcdTexto("VM");
        R1 = 0;
        R2 = 1;
        
        R3 = 0;
        __delay_ms(2000);
        R2 = 0;
    }
}
