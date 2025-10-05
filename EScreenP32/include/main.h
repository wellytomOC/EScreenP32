#pragma once
#include "WifiHandler.h"
#include "DisplayHandler.h"



#define LCD_CS 1
#define LCD_RESET 2
#define LCD_RS 3
#define LCD_MOSI 4
#define LCD_MISO 5
#define LCD_SCK 6
#define LCD_LED -1
#define T_CS 7
#define T_IRQ 8 
#define T_DIN 4
#define T_DO 5
#define T_CLK 6



//global parameters
typedef struct Parametros{
    bool WifiStatus;
}Tipo_Parametros;