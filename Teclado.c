/*
 * File:   Teclado.c
 * Author: 37306869841
 *
 * Created on 13 de Setembro de 2022, 08:12
 */

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
