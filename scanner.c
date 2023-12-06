#include <stdio.h>
#include "scanner.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

static int TT[FILAS][COLUMNAS] = {
/*INICIAL*/       {ENTERO_CERO,   ENTERO,        IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, INICIAL,       FDA,           ERROR_GENERAL},
/*ENTERO_CERO*/   {ENTERO,        ENTERO,        ENT_MAL_FORM,  HEXADECIMAL,   ENT_MAL_FORM,  ENTERO_CERO,   ENTERO_CERO,   ERROR_GENERAL},
/*ENTERO*/        {ENTERO,        ENTERO,        ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENTERO,        ENTERO,        ERROR_GENERAL},
/*HEXADECIMAL*/   {HEXADECIMAL,   HEXADECIMAL,   HEXADECIMAL,   ENT_MAL_FORM,  ENT_MAL_FORM,  HEXADECIMAL,   HEXADECIMAL,   ERROR_GENERAL},
/*IDENTIFICADOR*/ {IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, ERROR_GENERAL},
/*ENT_MAL_FORM*/  {ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ERROR_GENERAL},
/*ERROR_GENERAL*/ {ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL},
/*FDA*/           {FDA,           FDA,           FDA,           FDA,           FDA,           FDA,           FDA,           FDA}
};

static int columna = 0;
bool deboParar = false;
enum Token estado = INICIAL;
char lexem_buffer[512+1] = {0};

int obtenerColumna(int car) {
    if (isspace(car)){
        deboParar = true;
        return ESPACIO; //5
    } else if (car == EOF){ 
        deboParar = true;
        return FDT; //6
    } else if (car == '0')
        return CERO;//0
    else if (isdigit(car))
        return DIGITO_NO_CERO;//1
    else if (car == 'x' || car == 'X')
        return LETRA_X;//3
    else if (isxdigit(car))
        return LETRAS_HEX;//2
    else if (isalpha(car))
        return RESTO_LETRAS;//4
    else
        return OTROS;//7
}

int esAceptor(int unEstado){ // queda magico pero despues se puede arreglar o sacar
    return ENTERO_CERO <= unEstado && unEstado < IDENTIFICADOR ? 1 : 0; 
}

void guardarEnLexema(int unCaracter){
    if(lexbf_index < 512){ //dejo un lugar para el '\0'
        lexem_buffer[lexbf_index++] = unCaracter;
    } else {
        printf("El buffer del lexema se llenó ");
        manejoDeCentinela();
        //estaLlenoElBuffer = true;
    }
}

int esCentinela(int c){
    return c == ESPACIO || c == FDT;
}


void manejoDeCentinela(){
    guardarEnLexema('\0'); // indico fin de cadena
    lexbf_index = 0;
    token = estado;
    estado = INICIAL;
}

enum Token realizarTransicion(int car){
    return TT[estado][car];
}

int scanner (void) {
    while (!deboParar)
    {
        caracter = getchar();
        columna = obtenerColumna(caracter);
        if (!esCentinela(columna)){
            guardarEnLexema(caracter);
        }
        estado = realizarTransicion(columna);
    }

    if(columna == ESPACIO){
        manejoDeCentinela();
        estado = INICIAL;
    }

    if(columna == FDT){
        manejoDeCentinela();
        ungetc(caracter, stdin);
    }

    deboParar = false;
    return token;
}