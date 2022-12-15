
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
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)


#define _XTAL_FREQ 20000000

#define saida  PORTB
#define rs     PORTBbits.RB4
#define en     PORTBbits.RB5

#define C1     PORTCbits.RC2
#define C2     PORTCbits.RC6
#define C3     PORTCbits.RC1
#define C4     PORTCbits.RC7

#define L1     PORTCbits.RC5
#define L2     PORTCbits.RC4
#define L3     PORTCbits.RC0
#define L4     PORTCbits.RC3

#define saida1 PORTDbits.RD0
#define saida2 PORTDbits.RD1
#define saida3 PORTDbits.RD2

#include <xc.h>

void lcd4bits(unsigned char valor, unsigned char pino)
{
    saida = (valor >> 4);
    rs = pino;
    en = 1, en = 0;
    
    __delay_ms(5);
    
    saida = valor;
    rs = pino;
    en = 1, en = 0;
    
    __delay_ms(5);
}

void lcdIniciar(void)
{
    lcd4bits(0x02,0);
    lcd4bits(0x28,0);
    lcd4bits(0x0E,0);
    lcd4bits(0x01,0);
}

void lcdPos(unsigned char lin, unsigned char col)
{
    if(lin==1)lcd4bits(127+col,0);
    if(lin==2)lcd4bits(191+col,0);
}

void lcdTexto( char *palavra)
{
    while(*palavra)
    {
       lcd4bits(*palavra,1);
       palavra++;
    }
}

void picIniciar(void)
{
    TRISB = 0x00;
    PORTB = 0x00;
    TRISC = 0x39;
    PORTC = 0x00;
    TRISD = 0xF0;
    PORTD = 0x00;
    ANSELH= 0x00;
}

void adcIniciarCanal(unsigned char numCanal)	// Inicializa o ADC com numero de canal(ais) (0 a 15). Opcao em '0' entradas de TRISA modo digital.
{
	/* Atencao:
	 * Se 'numCanal = 0' entradas de TRISA modo digital
	 * N�o � possivel escolher somente: 2, 3, 4 canais ou 7 canais sequenciais.
	 */
    #define bit1(valor,bit) valor |= (1<<bit)
	unsigned char adTmpLac=0;
	TRISA=255;
	if(numCanal > 13)numCanal = 13;
    PIR1bits.ADIF = 0;	// Limpa Flag da interrupcao do modulo.
    PIE1bits.ADIE = 0;	// Desliga a interrupcao do ADC.
    if(numCanal > 7)numCanal = 7;
		// canal em uso:     0    1    5    5    5    5    6    8    8
	unsigned char adcTab[]={0x86,0x8E,0x82,0x82,0x82,0x82,0x89,0x80,0x80};
	do
	{
		bit1(TRISA,adTmpLac);
		adTmpLac++;
	}while(adTmpLac<numCanal);
	ADCON1 = adcTab[numCanal];
	PORTA=0x00;                                 // Limpa PORT.
    __delay_ms(35);                             // Aguarda estabilizar...
}

unsigned int adcCanal(unsigned char vlrCanal)	// Seleciona canal (0 a 15) e retorna com valor (INT) (0 a 1023).
{
    unsigned int vlrTmp;						// Vari�vel local tempor�ria.
	ADCON0 = (vlrCanal<<2) | ADCON0;            // Posiciona o valor do canal no registro.
    ADCON0bits.ADON = 1;						// Habilita m�dulo.
    __delay_ms(1);								// Aguarda...
    ADCON0bits.GO = 1;							// Inicia convers�o.
    while(ADCON0bits.GO);						// Aguarda terminar a convers�o.
    vlrTmp = (ADRESH<<8) | ADRESL;				// Unindo as partes para compor o valor.
    return (vlrTmp);							// Retorna com o valor bruto.
}

void contador(unsigned char lin, unsigned char col,unsigned int valor)
{
    unsigned char dezmilhar, milhar, centena, dezena, unidade;
    
       lcdPos(lin,col);
       
       dezmilhar = (valor / 10000)+0x30;
       milhar =    (valor / 1000)%10  +0x30;
       
       centena=    (valor / 100) %10  +0x30;
       dezena =    (valor / 10)  %10  +0x30;
       unidade=    (valor % 10)       +0x30;


       lcd4bits(dezmilhar,1);
       lcd4bits(milhar,1);
       lcd4bits(centena,1);
       lcd4bits(dezena,1);
       lcd4bits(unidade,1);
}

//char pesos(void)
//{
//    unsigned int valorBin;
//    unsigned int gramas;
//    unsigned int TARA;
//  
//    valorBin = adcCanal(0);
//    contador(2,1,valorBin);
//    
//    TARA=adcCanal(0)
//    
//    
//    
//    gramas = valorBin * 9.775 ;
//    contador(2,1,gramas);
//    lcdTexto("g");
// 
//    __delay_ms(2000);
//    
//    
//}

void main(void) 
{
    unsigned int valorBin;
    unsigned int gramas;
    unsigned int TARA;
    unsigned int converteMassa;
    
    picIniciar();
    lcdIniciar();
    adcIniciarCanal(1); 
    __delay_ms(500);
   
    TARA = adcCanal(0);
    contador(1,10,TARA);
    
    while(1)
    {
        valorBin = adcCanal(0);
        contador(2,10,valorBin);

        converteMassa = valorBin - TARA;
//        converteMassa = valorBin;
        
        gramas = converteMassa * 9.77517 ;
//         gramas = converteMassa * 1 ;
        contador(2,1,gramas);
        lcdTexto("g ");

        lcdPos(1,1);
        lcdTexto("N.L,L");
        
        __delay_ms(1000);
       
    }
   
}
