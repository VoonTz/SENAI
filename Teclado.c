
#include <xc.h>
#include "config.h"
#include "LIB_V01.h"

void main(void) 
{

    picIniciar();
    lcdIniciar();
    adcIniciarCanal(1); 
   
    lcdPos(1,1);
    lcdTexto("Luan");
    
    while(1)
    {
        botoes();
    }
}
