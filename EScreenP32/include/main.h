#pragma once
#include "WifiHandler.h"
#include "DisplayHandler.h"


//#define T_IRQ 
#define T_DO 19
#define T_DIN 23
#define T_CS 4
#define T_CLK 18
#define LCD_MISO 19
#define LCD_LED 16
#define LCD_SCK 18
#define LCD_MOSI 23
#define LCD_RS 13
#define LCD_RESET 0
#define LCD_CS 2

#define LCD_ON 26


//global parameters
typedef struct Parametros{
    bool WifiStatus;
}Tipo_Parametros;