sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D4 at RB0_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D4_Direction at TRISB0_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D7_Direction at TRISB3_bit;


char credit[] = "by oKaue";
char fas1[] = "FASE 1:ABASTECIMENTO";
char abast[] = "Abastecendo Agua";
char nvl[] = "Nivel:";
char extb[] = "VAZIO";
char baixo[] = "BAIXO";
char medio[] = "MEDIO";
char alto[] = " ALTO";
char aguades[] = "Agua Desligada";
char toinit[] = "Para Iniciar";
char pressOn[] = "Press. LIGAR";
char fas2[] = "FASE 2: AQUECIMENTO";
unsigned char setp[10];
char textsetpoint[] = "setp.=";
char celsius[] =  "C";
int iZ1, iZ2, iZ3, iZ4, imed, isetp;
unsigned char Z1[10];
unsigned char Z2[10];
unsigned char Z3[10];
unsigned char Z4[10];
unsigned char smed[10];
char tZ1[] = "Z1=";
char tZ2[] = "Z2=";
char tZ3[] = "Z3=";
char tZ4[] = "Z4=";
char tmed[] = "M.=";
char fas3[] = "FASE 3: MISTURA";
char corante[] = "Aplicando Corante";
char temp[] = "TEMPO=";
unsigned char scontagem[10];
int icontagem;
char mistura[] = "MISTURA DE SOLUCOES";
char cura[] = "TEMPO DE CURA";
char fas4[] = "FASE 4: ESVAZIAR";
char esv[] = "ESVAZIANDO RESERV.";
char aguaoff[] = "AGUA DESLIGADA";
char fim[] = "FIM DO PROCESSO";
char i=0;

int setpoint=100;
int fase=0;

void main() {
TRISA=0b00000011;
TRISB=0b00000000;
TRISC=0b00000000;
TRISD=0b11110000;

//LCD
Lcd_Init();
Lcd_Cmd(_LCD_CLEAR);
Lcd_Cmd(_LCD_CURSOR_OFF);
Lcd_Out(1,3,credit);
Delay_ms(2000);
Lcd_Cmd(_LCD_CLEAR);

//CONVERSOR A/D
ADCON1=0b1010;

while (1)
{
    //FASE 1
    if (PORTD.RD7==1&&fase==0)
    {
        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Out(1,1, fas1);
        PORTC.RC0=1;
        Lcd_Out(2,1,abast);
        while (PORTD.RD4==0 && PORTD.RD5==0 && PORTD.RD6==0)
        {
            Lcd_Out(3,1,nvl);
            Lcd_Out(3,8, extb);
        }
        while (PORTD.RD4==1 && PORTD.RD5==0 && PORTD.RD6==0)
        {
            Lcd_Out(3,1,nvl);
            Lcd_Out(3,8, baixo);
        }
        while (PORTD.RD4==1 && PORTD.RD5==1 && PORTD.RD6==0)
        {
            Lcd_Out(3,1,nvl);
            Lcd_Out(3,8, medio);
        }
        if (PORTD.RD4==1 && PORTD.RD5==1 && PORTD.RD6==1)
        {
            Lcd_Out(3,1,nvl);
            Lcd_Out(3,8, alto);
            PORTC.RC0=0;
            Delay_ms(5000);
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1,1, fas1);
            Lcd_Out(3,1, aguades);
            Delay_ms(3000);
            Lcd_Cmd(_LCD_CLEAR);
            fase = 1;
        }
    }
    else
    {
        Lcd_Out(2,5, toinit);
        Lcd_Out(3,5, pressOn);
    }

    //FASE 2
    while (fase==1)
    {
        Lcd_Out(1,1, fas2);
        PORTC.RC1=1;
        PORTC.RC2=1;
        //A/D TEMPS
        LCD_Out(2,1,tZ1);
        iZ1 = (ADC_Read(0)*4.88)/10.00;
        IntToStr(iZ1, Z1);
        LCD_Out(2,4, Z1);
        LCD_Out(2,12,tZ2);
        iZ2 = (ADC_Read(1)*4.88)/10.00;
        IntToStr(iZ2, Z2);
        LCD_Out(2,15, Z2);
        LCD_Out(3,1,tZ3);
        iZ3 = (ADC_Read(2)*4.88)/10.00;
        IntToStr(iZ3, Z3);
        LCD_Out(3,4, Z3);
        LCD_Out(3,12,tZ4);
        iZ4 = (ADC_Read(3)*4.88)/10.00;
        IntToStr(iZ4, Z4);
        LCD_Out(3,15, Z4);
        LCD_Out(4,1,"SP=");
        isetp = (ADC_Read(4)*4.88)/10.00;
        IntToStr(isetp, setp);
        LCD_Out(4,4, setp);
        //MEDIA
        imed = (iZ1+iZ2+iZ3+iZ4)/4;
        IntToStr(imed, smed);
        Lcd_Out(4,12, tmed);
        Lcd_Out(4,15, smed);
        Delay_ms(1000);
        Lcd_Cmd(_LCD_CLEAR);
        //FIM FASE 2
        if (imed > isetp)
        {
            fase = 2;
            PORTC.RC1=0;
            Lcd_Cmd(_LCD_CLEAR);
            Delay_ms(1000);
            break;
        }
    }

    //FASE 3
    while (fase==2)
    {
        Lcd_Out(1,1, fas3);
        PORTC.RC3 = 1;
        PORTC.RC4 = 1;
        Lcd_Out(2,1, corante);
        Lcd_Out(3,1, temp);
        Delay_ms(1000);
        while (icontagem<10)
        {
            icontagem++;
            IntToStr(icontagem, scontagem);
            Lcd_Out(3,7, scontagem);
            Delay_ms(1000);
        }
        PORTC.RC3 = 0;
        PORTC.RC4 = 0;
        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Out(1,1, fas3);
        Lcd_Out(2,1, mistura);
        Lcd_Out(3,1, temp);
        Delay_ms(1000);
        icontagem = 0;
        while (icontagem<10)
        {
            icontagem++;
            IntToStr(icontagem, scontagem);
            Lcd_Out(3,7, scontagem);
            Delay_ms(1000);
        }
        PORTC.RC2=0;
        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Out(1,1, fas3);
        Lcd_Out(2,1, cura);
        Lcd_Out(3,1, temp);
        icontagem = 0;
        Delay_ms(1000);
        while (icontagem<15)
        {
            icontagem++;
            IntToStr(icontagem, scontagem);
            Lcd_Out(3,7, scontagem);
            Delay_ms(1000);
        }
        fase = 3;
        Lcd_Cmd(_LCD_CLEAR);
    }

    //FASE 4
    while (fase==3)
    {
        Lcd_Cmd(_LCD_CLEAR);
        Lcd_Out(1,1, fas4);
        Lcd_Out(2,1, esv);
        PORTC.RC5=1;
        while (PORTD.RD4==1 && PORTD.RD5==1 && PORTD.RD6==1)
        {
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1,1, fas4);
            Lcd_Out(2,1, esv);
            Lcd_Out(3,1,nvl);
            Lcd_Out(3,8, alto);
            Delay_ms(1000);
        }
        while (PORTD.RD4==1 && PORTD.RD5==1 && PORTD.RD6==0)
        {
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1,1, fas4);
            Lcd_Out(2,1, esv);
            Lcd_Out(3,1,nvl);
            Lcd_Out(3,8, medio);
            Delay_ms(1000);
        }
        while (PORTD.RD4==1 && PORTD.RD5==0 && PORTD.RD6==0)
        {
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1,1, fas4);
            Lcd_Out(2,1, esv);
            Lcd_Out(3,1,nvl);
            Lcd_Out(3,8, baixo);
            Delay_ms(1000);
        }
        while (PORTD.RD4==0 && PORTD.RD5==0 && PORTD.RD6==0 && fase==3)
        {
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1,1, fas4);
            Lcd_Out(2,1, esv);
            Lcd_Out(3,1,nvl);
            Lcd_Out(3,8, extb);
            PORTC.RC5=0;
            Delay_ms(2000);
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(1,1, fas4);
            Lcd_Out(3,4, aguaoff);
            Delay_ms(2000);
            Lcd_Cmd(_LCD_CLEAR);
            Lcd_Out(3,4, fim);
            Delay_ms(2000);
            Lcd_Cmd(_LCD_CLEAR);
            fase=0;
        }
    }

}
}