/*
 * File:   re4s.c
 * Author: Akagi
 *
 * Created on June 21, 2023, 9:27 AM
 */


// PIC16F1508 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTOSC    // Oscillator Selection Bits (INTOSC oscillator: I/O function on CLKIN pin)
#pragma config WDTE = OFF        // Watchdog Timer Enable (WDT enabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
#pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable (Brown-out Reset enabled)
#pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
#pragma config IESO = OFF       // Internal/External Switchover Mode (Internal/External Switchover Mode is disabled)
#pragma config FCMEN = ON       // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is enabled)

// CONFIG2
#pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
#pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
#pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
#pragma config LPBOR = OFF      // Low-Power Brown Out Reset (Low-Power BOR is disabled)
#pragma config LVP = ON         // Low-Voltage Programming Enable (Low-voltage programming enabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic.h>
#include <stdint.h>             //define uint_8
#define _XTAL_FREQ 4000000UL

unsigned char rd_data[2] = 0;
unsigned char temp;
int i = 0;
//int akagi = 0422;

void main()
{
    OSCCONbits.IRCF3 = 1;
    OSCCONbits.IRCF2 = 1;
    OSCCONbits.IRCF1 = 0;
    OSCCONbits.IRCF0 = 1;
    OSCCONbits.SCS = 10;
    
    TRISAbits.TRISA4 = 0;//a4 output  LED
    TRISAbits.TRISA5 = 0;//a5 output  LED
    
    TRISCbits.TRISC6 = 0;//c6 output  motor
    TRISCbits.TRISC7 = 0;//c7 output  motor
    
    TRISCbits.TRISC3 = 1;//c3 input  SW
    TRISCbits.TRISC4 = 1;//c4 input  SW
    TRISCbits.TRISC5 = 1;//c5 input  SW
    
    TRISCbits.TRISC0 = 1;//c4 output  compare
    TRISCbits.TRISC1 = 1;//c5 output  compare
    
    ANSELCbits.ANSC0 = 1;//c0 analog input
    ANSELCbits.ANSC1 = 1;//c1 analog input
    
    CM2CON0bits.C2ON = 1;
    CM2CON0bits.C2OE = 0;
    CM2CON0bits.C2POL = 1;//1
    CM2CON0bits.C2SP = 1;
    CM2CON0bits.C2HYS = 0;
    CM2CON0bits.C2SYNC = 1;
    CM2CON1bits.C2INTP = 0;
    CM2CON1bits.C2INTN = 0;
    CM2CON1bits.C2PCH1 = 0;
    CM2CON1bits.C2PCH0 = 0;
    CM2CON1bits.C2NCH2 = 0;
    CM2CON1bits.C2NCH1 = 0;
    CM2CON1bits.C2NCH0 = 1;
    
    ANSELAbits.ANSA4 = 0;
    ANSELCbits.ANSC3 = 0;
    ANSELCbits.ANSC6 = 0;
    ANSELCbits.ANSC7 = 0;
    
    TRISBbits.TRISB4 = 1;//i2c
    TRISBbits.TRISB6 = 1;
    ANSELBbits.ANSB4 = 0;
    WPUBbits.WPUB4 = 1;
    WPUBbits.WPUB6 = 1;
    
    SSP1BUF = 0x00;
    SSP1CON1 = 0x36;
    SSP1CON2 = 0x81;//10000001
    SSP1CON3 = 0x10;//00010000
    SSP1STAT = 0x00;
    SSP1ADD = 0xA6;//52
    SSP1CON1bits.SSPM3=0;
    SSP1CON1bits.SSPM2=1;
    SSP1CON1bits.SSPM1=1; 
    SSP1CON1bits.SSPM0=0;
    PIR1bits.SSP1IF = 0;
    i = 0;
    SSP1STATbits.S = 0;
    
    LATAbits.LATA5=1;//light on
    LATAbits.LATA4=0;//light on
    //__delay_ms(100);
    LATAbits.LATA5=0;//light on
    LATAbits.LATA4=1;//light on
    //__delay_ms(100);
    LATAbits.LATA5=1;
    LATAbits.LATA4=1;
    LATCbits.LATC6=0;
    LATCbits.LATC7=0;
    
    while(i < 2)
    {
        if (PORTCbits.RC5 == 0)
        {
            __delay_ms(2000);
            if(PORTCbits.RC5 == 0)
            {
                if(PORTCbits.RC3 == 0 && PORTCbits.RC4 == 1)
                {
                   while(PORTCbits.RC4 != 0)
                    {
                        LATAbits.LATA5=0;//light on
                        LATAbits.LATA4=0;//light on
                        LATCbits.LATC6=1;
                        LATCbits.LATC7=0;
                    }
                    i = 9;
                }
                else if(PORTCbits.RC3 == 1 && PORTCbits.RC4 == 0)
                {
                    while(PORTCbits.RC3 != 0)
                    {
                        LATAbits.LATA5=0;//light on
                        LATAbits.LATA4=0;//light on
                        LATCbits.LATC6=1;
                        LATCbits.LATC7=0;
                    }
                    i = 9;
                }
               else if(PORTCbits.RC3 == 1 && PORTCbits.RC4 == 1)
                {
                    while(PORTCbits.RC3 == 1 && PORTCbits.RC4 == 1)
                    {
                        LATAbits.LATA5=0;//light on
                        LATAbits.LATA4=0;//light on
                        LATCbits.LATC6=1;
                        LATCbits.LATC7=0;
                    }
                    i = 9;
                }
                else
                {
                    LATAbits.LATA5=1;//light off
                    LATAbits.LATA4=1;//light off
                    LATCbits.LATC6=0;
                    LATCbits.LATC7=0;
                    i = 9;
                }
            }
            else
                i = 9;
        }
        
        else if ((SSP1STATbits.S == 1) && (SSP1STATbits.D_nA == 0) && (SSP1STATbits.R_nW == 0) && (SSP1STATbits.BF == 1))          //MASTER ADD WRITE
        {
            SSP1CON2bits.ACKDT = 0;
            while(SSP1IF == 1)
            SSP1IF = 0;
            temp = SSP1BUF;
            while(SSP1STATbits.BF == 0)
            {
                i = 0;
                SSP1CON1bits.CKP = 1; 
            }
        }
        
        else if ((SSP1STATbits.S == 1) && (SSP1STATbits.D_nA == 1) && (SSP1STATbits.R_nW == 0) && (SSP1STATbits.BF == 1))    // MASTER DATA WRITE can't in
        {
            SSP1CON2bits.ACKDT = 0;
            while(SSP1IF == 1)
            SSP1IF = 0; 
            rd_data[i] = SSP1BUF;
            
            
            if(rd_data[0] == 0x52 || rd_data[0] == 0x18)
            {
                while(SSP1STATbits.BF == 0)
                {
                    i++;
                    SSP1CON1bits.CKP = 1; 
                }
                SSP1CON2bits.ACKDT = 0;
                if(SSP1IF == 1)
                SSP1IF = 0; 
                rd_data[1] = SSP1BUF;
                if(SSP1STATbits.BF == 0)
                {
                    i = 2;
                    SSP1CON1bits.CKP = 1; 
                }
            }
            else if(rd_data[0] == 0x8E)
            {
                //__delay_ms(10);
                SSPCON=0x36;//DO NOT TOUCH THIS LINE
                SSP1IF=0;
                i = 0;
            }
        }
    
        else if ((1 == SSP1STATbits.S) && (0 == SSP1STATbits.D_nA) && (1 == SSP1STATbits.R_nW) && (SSP1STATbits.BF == 1))        // MASTER ADDRESS READ
        {
            
            SSP1CON1bits.CKP = 0;
            
            if(PORTCbits.RC3 == 0 && PORTCbits.RC4 == 1)
            {
                if(PIR1bits.SSP1IF == 1)
                PIR1bits.SSP1IF = 0;
                temp = SSP1BUF;
                if(SSP1STATbits.BF == 0)
                SSP1BUF = 0x8A;
                if(SSP1STATbits.BF == 1)
                SSP1CON1bits.CKP = 1;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x8A;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x8A;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x8A;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
            }
            else if(PORTCbits.RC3 == 1 && PORTCbits.RC4 == 0)
            {
                if(PIR1bits.SSP1IF == 1)
                PIR1bits.SSP1IF = 0;
                temp = SSP1BUF;
                if(SSP1STATbits.BF == 0)
                SSP1BUF = 0x89;
                if(SSP1STATbits.BF == 1)
                SSP1CON1bits.CKP = 1;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x89;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x89;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x89;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
            }
            else if(PORTCbits.RC3 == 1 && PORTCbits.RC4 == 1)
            {
                if(PIR1bits.SSP1IF == 1)
                PIR1bits.SSP1IF = 0;
                temp = SSP1BUF;
                if(SSP1STATbits.BF == 0)
                SSP1BUF = 0x8B;
                if(SSP1STATbits.BF == 1)
                SSP1CON1bits.CKP = 1;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x8B;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x8B;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
                
                __delay_us(70);
                
                if(ACKSTAT == 0)
                {
                    if(PIR1bits.SSP1IF == 1)
                    PIR1bits.SSP1IF = 0;
                    SSP1BUF = 0x8B;
                    if(SSP1STATbits.BF == 1)
                    SSP1CON1bits.CKP = 1;
                }
                else
                    i = 0;
            }
        }
    }
    
    while(i == 2)
    {
        if(PORTCbits.RC3 == 0 && PORTCbits.RC4 == 1) //3close 4open
        {
            if(rd_data[0] == 0x52 && rd_data[1] == 0xAD)
            {
                while(PORTCbits.RC4 != 0)
                {
                    LATAbits.LATA5=0;//light on
                    LATAbits.LATA4=0;//light on
                    LATCbits.LATC6=1;
                    LATCbits.LATC7=0;
                    
                    if ((SSP1STATbits.S == 1) && (SSP1STATbits.D_nA == 0) && (SSP1STATbits.R_nW == 0) && (SSP1STATbits.BF == 1))          //MASTER ADD WRITE
                   {
                        SSP1CON2bits.ACKDT = 0;
                        if(SSP1IF == 1)
                        SSP1IF = 0;
                        temp = SSP1BUF;
                        if(SSP1STATbits.BF == 0)
                        {
                            i = 0;
                            SSP1CON1bits.CKP = 1; 
                        }
                    }
                   else if ((SSP1STATbits.S == 1) && (SSP1STATbits.D_nA == 1) && (SSP1STATbits.R_nW == 0) && (SSP1STATbits.BF == 1))    // MASTER DATA WRITE can't in
                    {
                        SSP1CON2bits.ACKDT = 0;
                        if(SSP1IF == 1)
                        SSP1IF = 0; 
                        rd_data[i] = SSP1BUF;
                        SSPCON=0x36;//DO NOT TOUCH THIS LINE
                        SSP1IF=0;
                        i = 0;
                    }
                    else if ((1 == SSP1STATbits.S) && (0 == SSP1STATbits.D_nA) && (1 == SSP1STATbits.R_nW) && (SSP1STATbits.BF == 1))        // MASTER ADDRESS READ
                    {
            
                        //SSP1CON1bits.CKP = 0;
            
                        if(CM2CON0bits.C2OUT == 0)
                        {
                            if(PIR1bits.SSP1IF == 1)
                            PIR1bits.SSP1IF = 0;
                            temp = SSP1BUF;
                            if(SSP1STATbits.BF == 0)
                            SSP1BUF = 0x8D;
                            if(SSP1STATbits.BF == 1)
                            SSP1CON1bits.CKP = 1;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                        }
                        else if(CM2CON0bits.C2OUT == 1)
                        {
                            if(PIR1bits.SSP1IF == 1)
                            PIR1bits.SSP1IF = 0;
                            temp = SSP1BUF;
                            if(SSP1STATbits.BF == 0)
                            SSP1BUF = 0x84;
                            if(SSP1STATbits.BF == 1)
                            SSP1CON1bits.CKP = 1;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                        }
                    }
                    rd_data[0] == 0x00;
                }
                i = 0;
            }
            else
            {
                LATAbits.LATA5=1;//light off
                LATAbits.LATA4=1;//light off
                LATCbits.LATC6=0;
                LATCbits.LATC7=0;
                rd_data[0] == 0x00;
                i = 0;
            }
        }
        else if(PORTCbits.RC3 == 1 && PORTCbits.RC4 == 0)
        {
            if(rd_data[0] == 0x18 && rd_data[1] == 0xE7)
            {
                 while(PORTCbits.RC3 != 0)
                {
                    LATAbits.LATA5=0;//light on
                    LATAbits.LATA4=0;//light on
                    LATCbits.LATC6=1;
                    LATCbits.LATC7=0;
                    
                    if ((SSP1STATbits.S == 1) && (SSP1STATbits.D_nA == 0) && (SSP1STATbits.R_nW == 0) && (SSP1STATbits.BF == 1))          //MASTER ADD WRITE
                   {
                        SSP1CON2bits.ACKDT = 0;
                        if(SSP1IF == 1)
                        SSP1IF = 0;
                        temp = SSP1BUF;
                        if(SSP1STATbits.BF == 0)
                        {
                            i = 0;
                            SSP1CON1bits.CKP = 1; 
                        }
                    }
                   else if ((SSP1STATbits.S == 1) && (SSP1STATbits.D_nA == 1) && (SSP1STATbits.R_nW == 0) && (SSP1STATbits.BF == 1))    // MASTER DATA WRITE can't in
                    {
                        SSP1CON2bits.ACKDT = 0;
                        if(SSP1IF == 1)
                        SSP1IF = 0; 
                        rd_data[i] = SSP1BUF;
                        SSPCON=0x36;//DO NOT TOUCH THIS LINE
                        SSP1IF=0;
                        i = 0;
                    }
                    else if ((1 == SSP1STATbits.S) && (0 == SSP1STATbits.D_nA) && (1 == SSP1STATbits.R_nW) && (SSP1STATbits.BF == 1))        // MASTER ADDRESS READ
                    {
            
                        //SSP1CON1bits.CKP = 0;
            
                        if(CM2CON0bits.C2OUT == 0)
                        {
                            if(PIR1bits.SSP1IF == 1)
                            PIR1bits.SSP1IF = 0;
                            temp = SSP1BUF;
                            if(SSP1STATbits.BF == 0)
                            SSP1BUF = 0x8D;
                            if(SSP1STATbits.BF == 1)
                            SSP1CON1bits.CKP = 1;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                        }
                        else if(CM2CON0bits.C2OUT == 1)
                        {
                            if(PIR1bits.SSP1IF == 1)
                            PIR1bits.SSP1IF = 0;
                            temp = SSP1BUF;
                            if(SSP1STATbits.BF == 0)
                            SSP1BUF = 0x84;
                            if(SSP1STATbits.BF == 1)
                            SSP1CON1bits.CKP = 1;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                        }
                    }
                    
                    rd_data[0] == 0x00;
                }
                i = 0;
            }
            else
            {
                LATAbits.LATA5=1;//light off
                LATAbits.LATA4=1;//light off
                LATCbits.LATC6=0;
                LATCbits.LATC7=0;
                rd_data[0] == 0x00;
                i = 0;
            }
        }
        else if(PORTCbits.RC3 == 1 && PORTCbits.RC4 == 1)
        {
            if((rd_data[0] == 0x18 && rd_data[1] == 0xE7)||(rd_data[0] == 0x52 && rd_data[1] == 0xAD))
            {
                if(PORTCbits.RC3 != 0 && PORTCbits.RC4 != 0)
                {
                    LATAbits.LATA5=0;//light on
                    LATAbits.LATA4=0;//light on
                    LATCbits.LATC6=1;
                    LATCbits.LATC7=0;
                    
                    if ((SSP1STATbits.S == 1) && (SSP1STATbits.D_nA == 0) && (SSP1STATbits.R_nW == 0) && (SSP1STATbits.BF == 1))          //MASTER ADD WRITE
                   {
                        SSP1CON2bits.ACKDT = 0;
                        if(SSP1IF == 1)
                        SSP1IF = 0;
                        temp = SSP1BUF;
                        if(SSP1STATbits.BF == 0)
                        {
                            i = 0;
                            SSP1CON1bits.CKP = 1; 
                        }
                    }
                   else if ((SSP1STATbits.S == 1) && (SSP1STATbits.D_nA == 1) && (SSP1STATbits.R_nW == 0) && (SSP1STATbits.BF == 1))    // MASTER DATA WRITE can't in
                    {
                        SSP1CON2bits.ACKDT = 0;
                        if(SSP1IF == 1)
                        SSP1IF = 0; 
                        rd_data[i] = SSP1BUF;
                        SSPCON=0x36;//DO NOT TOUCH THIS LINE
                        SSP1IF=0;
                        i = 0;
                    }
                    else if ((1 == SSP1STATbits.S) && (0 == SSP1STATbits.D_nA) && (1 == SSP1STATbits.R_nW) && (SSP1STATbits.BF == 1))        // MASTER ADDRESS READ
                    {
            
                        //SSP1CON1bits.CKP = 0;
            
                        if(CM2CON0bits.C2OUT == 0)
                        {
                            if(PIR1bits.SSP1IF == 1)
                            PIR1bits.SSP1IF = 0;
                            temp = SSP1BUF;
                            if(SSP1STATbits.BF == 0)
                            SSP1BUF = 0x8D;
                            if(SSP1STATbits.BF == 1)
                            SSP1CON1bits.CKP = 1;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x8D;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                        }
                        else if(CM2CON0bits.C2OUT == 1)
                        {
                            if(PIR1bits.SSP1IF == 1)
                            PIR1bits.SSP1IF = 0;
                            temp = SSP1BUF;
                            if(SSP1STATbits.BF == 0)
                            SSP1BUF = 0x84;
                            if(SSP1STATbits.BF == 1)
                            SSP1CON1bits.CKP = 1;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                            
                            __delay_us(70);
                
                            if(ACKSTAT == 0)
                            {
                                if(PIR1bits.SSP1IF == 1)
                                PIR1bits.SSP1IF = 0;
                                SSP1BUF = 0x84;
                                if(SSP1STATbits.BF == 1)
                                SSP1CON1bits.CKP = 1;
                            }
                            else
                                i = 0;
                        }
                    }
                    
                    rd_data[0] == 0x00;
                }
            }
            else
            {
                LATAbits.LATA5=1;//light off
                LATAbits.LATA4=1;//light off
                LATCbits.LATC6=0;
                LATCbits.LATC7=0;
                rd_data[0] == 0x00;
                i = 0;
            }
        }
        else
        {
            LATAbits.LATA5=1;//light off
            LATAbits.LATA4=1;//light off
            LATCbits.LATC6=0;
            LATCbits.LATC7=0;
            rd_data[0] == 0x00;
            i = 0;
        }
    }
}
