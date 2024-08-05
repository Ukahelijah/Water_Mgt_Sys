
// PIC16F877A Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#define _XTAL_FREQ 20000000
#define RS RD2
#define EN RD3
#define D4 RD4
#define D5 RD5
#define D6 RD6 
#define D7 RD7
#define mode RB1
#define Start RB3
#define stop RB2
#define pumpLed RC2
#define Tankfull RC3
#define Automatic RC0
#define manual RC1
#define pump RC4
#define tank RB4
#define well RB5



int modetype=0;

void LCD_SetBit(char data_bit){
    if(data_bit& 1)
        D4=1;
    else
        D4=0;
     if(data_bit& 2)
        D5=1;
    else
        D5=0;
     if(data_bit& 4)
        D6=1;
    else
        D6=0;
     if(data_bit& 8)
        D7=1;
    else
        D7=0;
}
void LCD_Cmd (char a){
   RS=0;
   LCD_SetBit(a);
   EN=1;
   __delay_ms(4);
   EN=0;
   
}
void LCD_Clear(){
    LCD_Cmd (0);
    LCD_Cmd (1);
}
void LCD_Start(){
    LCD_SetBit(0x00);
    for(int i=1065244; i<=0; i--)NOP();
    LCD_Cmd(0x03);
    __delay_ms(5);
     LCD_Cmd(0x03);
    __delay_ms(11);
     LCD_Cmd(0x03);
      LCD_Cmd(0x02);
       LCD_Cmd(0x02);
        LCD_Cmd(0x08);
         LCD_Cmd(0x00);
          LCD_Cmd(0x0C);
           LCD_Cmd(0x00);
            LCD_Cmd(0x06);
            
    
    
}

void LCD_Set_Cursor(char a, char b){
    char temp,z,y;
    if(a==1){
        temp= 0x80 + b - 1;
        z= temp>>4;
        y= temp & 0x0F;
        LCD_Cmd (z); 
         LCD_Cmd (y);
    }
    else if(a==2){
        temp= 0xC0 + b - 1;
        z= temp>>4;
        y= temp & 0x0F;
        LCD_Cmd (z); 
         LCD_Cmd (y);
    }
    
}
void LCD_Print_Char(char data){
    char Lower_Nibble,Upper_Nibble;
    Lower_Nibble = data & 0x0F;
    Upper_Nibble = data & 0xF0;
    RS=1;
    LCD_SetBit(Upper_Nibble>>4);
    EN=1;
    for(int i=2130483; i<=0; i--)NOP();
    EN=0;
    LCD_SetBit(Lower_Nibble);
    EN=1;
    for(int i=2130483; i<=0; i--)NOP();
    EN=0;
    
}
   
void LCD_Print_String(char *a){
    int i;
    for(i=0; a[i]!='\0'; i++)
        LCD_Print_Char(a[i]);
}

void main(){
    unsigned int a;
    TRISD = 0x00;
    TRISB = 1;
    Start=1;
    TRISC = 0x00;
    Automatic=0;
    manual=0;
    Tankfull=0;
    pumpLed=0;
    pump=0;
    mode=1;
    LCD_Start();
    LCD_Clear();
    LCD_Set_Cursor(1,1);
    LCD_Print_String("    Welcome     ");
     LCD_Set_Cursor(2,1);
    LCD_Print_String("Water Management");
    __delay_ms(2000);
    LCD_Clear();
    LCD_Set_Cursor(1,1);
    LCD_Print_String("    System     ");
     LCD_Set_Cursor(2,1);
    LCD_Print_String("Starting..");
    __delay_ms(1000);
    LCD_Set_Cursor(2,11);
    LCD_Print_String(".");
    __delay_ms(1000);
    LCD_Set_Cursor(2,12);
    LCD_Print_String(".");
     __delay_ms(1000);
    LCD_Set_Cursor(2,13);
    LCD_Print_String(".");
  while(1){
      while(modetype==0){
          LCD_Set_Cursor(1,1);
    LCD_Print_String("Water MGT. SYS..");
     LCD_Set_Cursor(2,1);
    LCD_Print_String("Pls Select MODE.");
    
      if(mode==0){
          __delay_ms(100);
          modetype=modetype+1;
      }
      }
    


      if(mode==0){
          __delay_ms(100);
          modetype=modetype+1;
      }
      if(modetype>=3){
          modetype=1;
      }
       if(modetype==2 ){
          manual=0;
          Automatic=1;
          LCD_Set_Cursor(1,1);
           LCD_Print_String(" Automatic Mode");
     LCD_Set_Cursor(2,1);
          LCD_Print_String("    Activated   ");
           if(well==1 && tank==0){
              pump=1;
              pumpLed=1;
          }
           else if(well==0 || tank==1 || stop==0){
              pump=0;
              pumpLed=0;
              while(tank==1){
                  if(Start==0){
                    LCD_Set_Cursor(2,1);
                    LCD_Print_String("  Tank is Full  ");
                    __delay_ms(1000);
                    LCD_Set_Cursor(2,1);
          LCD_Print_String("Push StartButton");
                  }
              }
          }
          
          
      }
      if(modetype==1){
          LCD_Set_Cursor(1,1);
          LCD_Print_String("  Manual Mode.. ");
     LCD_Set_Cursor(2,1);
          LCD_Print_String("Push StartButton");
          manual=1;
          Automatic=0; 
          
          if(Start==0 && well==1 && tank==0){
              pump=1;
              pumpLed=1;
          }
          else if(well==0 || tank==1 || stop==0){
              pump=0;
              pumpLed=0;
              while(tank==1){
                  if(Start==0){
                    LCD_Set_Cursor(2,1);
                    LCD_Print_String("  Tank is Full  ");
                    __delay_ms(1000);
                    LCD_Set_Cursor(2,1);
          LCD_Print_String("Push StartButton");
                  }
              }
          }
        
              
          }
     

      }
         
  }
  
    
    
