#ifndef LIB_V01_H
#define LIB_V01_H

#define _XTAL_FREQ   20000000

#define saida   PORTB
#define rs      PORTBbits.RB4
#define en      PORTBbits.RB5

#define C1      PORTCbits.RC2
#define C2      PORTCbits.RC6
#define C3      PORTCbits.RC1
#define C4      PORTCbits.RC7
#define L1      PORTCbits.RC5
#define L2      PORTCbits.RC4
#define L3      PORTCbits.RC0
#define L4      PORTCbits.RC3

#define saida1  PORTDbits.RD0
#define saida2  PORTDbits.RD1
#define saida3  PORTDbits.RD2

void lcd4Bits(unsigned char valor, unsigned char pino)
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
    lcd4Bits(0x02,0);
    lcd4Bits(0x28,0);
    lcd4Bits(0x0E,0);
    lcd4Bits(0x01,0);
}

void lcdPos(unsigned char lin,unsigned char col)
{ 
    if(lin==1)lcd4Bits(127+col,0);
    if(lin==2)lcd4Bits(191+col,0);
}

void lcdTexto(char *palavra)
{
    while(*palavra)
    {
        lcd4Bits(*palavra,1);
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
	 * Não é possivel escolher somente: 2, 3, 4 canais ou 7 canais sequenciais.
	 */
    #define bit1(valor,bit) valor |= (1<<bit)
	unsigned char adTmpLac=0;
	TRISA=0;
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
    unsigned int vlrTmp;						// Variável local temporária.
	ADCON0 = (vlrCanal<<2);                      // Posiciona o valor do canal no registro.
    ADCON0bits.ADON = 1;						// Habilita módulo.
    __delay_ms(1);								// Aguarda...
    ADCON0bits.GO = 1;							// Inicia conversão.
    while(ADCON0bits.GO);						// Aguarda terminar a conversão.
    vlrTmp = (ADRESH<<8) | ADRESL;				// Unindo as partes para compor o valor.
    return (vlrTmp);							// Retorna com o valor bruto.
}

void contador(unsigned char lin, unsigned char col, unsigned int valor)
{
    unsigned char unidade, dezena, centena, milhar;
    
    lcdPos(lin,col);
    
    milhar  = (valor / 1000)     + 0x30;
    centena = (valor / 100) % 10 + 0x30;
    dezena  = (valor / 10)  % 10 + 0x30;
    unidade = (valor % 10)       + 0x30;
    
    if(milhar =='0') milhar=' ';
    if(centena=='0' && milhar==' ') centena=' ';
    if(dezena =='0' && centena==' ') dezena=' ';
    
    lcd4Bits(milhar ,1);
    lcd4Bits(centena,1);
    lcd4Bits(dezena ,1);
    lcd4Bits(unidade,1);
}

unsigned char tecAntiga, tecNova;

unsigned char teclado(void)
{
    unsigned char tecla;
    C1=1,C2=0,C3=0,C4=0;
    if(L1) tecla='7';
    if(L2) tecla='4';
    if(L3) tecla='1';
    if(L4) tecla='*';
    
    C1=0,C2=1,C3=0,C4=0;
    if(L1) tecla='8';
    if(L2) tecla='5';
    if(L3) tecla='2';
    if(L4) tecla='0';
    
    C1=0,C2=0,C3=1,C4=0;
    if(L1) tecla='9';
    if(L2) tecla='6';
    if(L3) tecla='3';
    if(L4) tecla='#';
    
    C1=0,C2=0,C3=0,C4=1;
    if(L1) tecla='A';
    if(L2) tecla='B';
    if(L3) tecla='C';
    if(L4) tecla='D';
    
    tecNova = tecla;
    if(tecNova != tecAntiga)
    {
        tecAntiga = tecNova;
        return(tecNova);
    }
    else
    {
        tecNova = 0;
        return(tecNova);
    }
    __delay_ms(50);
}

char celsius(unsigned int valorBin)
{
    unsigned char temp;
    temp = valorBin * 0.4878;
    return (temp);
}

unsigned char matEsq[]={0x20,0x10,0x01,0x08};
unsigned char matDir[]={0x08,0x01,0x10,0x20};

void motorPasso(unsigned int ang, char sentido)
{
    unsigned int contar;
    ang = ang * 5.6889;
    for(contar=0;contar<ang;contar++)
    {
        if(sentido)PORTC=matDir[contar%4];
        else       PORTC=matEsq[contar%4];
        __delay_ms(10);
    }
}

#endif