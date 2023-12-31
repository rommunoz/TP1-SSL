#include <stdio.h>
#include "scanner.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

//Tabla de Transicion
static int TT[FILAS][COLUMNAS] = {
//                 CERO           [1-9]          [A-Fa-f]       [xX]           [E-Ze-z]       espacios       EOF            otros    
/*INICIAL*/       {ENTERO_CERO,   ENTERO,        IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, INICIAL,       FDA,           ERROR_GENERAL   },
/*ENTERO_CERO*/   {ENTERO,        ENTERO,        ENT_MAL_FORM,  HEXADECIMAL,   ENT_MAL_FORM,  ENTERO_CERO,   ENTERO_CERO,   ENTERO_CERO     },
/*ENTERO*/        {ENTERO,        ENTERO,        ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENTERO,        ENTERO,        ENTERO          },
/*HEXADECIMAL*/   {HEXADECIMAL,   HEXADECIMAL,   HEXADECIMAL,   HEXADECIMAL ,  HEXADECIMAL ,  HEXADECIMAL,   HEXADECIMAL,   HEXADECIMAL     },
/*IDENTIFICADOR*/ {IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR, IDENTIFICADOR   },
/*ENT_MAL_FORM*/  {ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM,  ENT_MAL_FORM    },
/*ERROR_GENERAL*/ {ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL, ERROR_GENERAL   },
/*FDA*/           {FDA,           FDA,           FDA,           FDA,           FDA,           FDA,           FDA,           FDA             }
}; 

int columna = 0;
bool deboParar = false;
enum Token estado = INICIAL;
char lexem_buffer[512+1] = {0};
static int lexbf_index = 0; //indice del buffer
static int caracter;

int obtenerColumna(int car) {
    if (isspace(car)){
        return ESPACIO; //5
    } else if (car == EOF){ 
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

int esCentinela(int c){
    return c == ESPACIO || c == FDT || (c == RESTO_LETRAS && estado == HEXADECIMAL)
        || (c == OTROS && estado != INICIAL && estado != ERROR_GENERAL);
}

void guardarEnLexema(int unCaracter){
    if(lexbf_index < 512){ //dejo un lugar para el '\0'
        lexem_buffer[lexbf_index++] = unCaracter;
    } else {
        printf("El buffer del lexema se llenó ");
        //estaLlenoElBuffer = true;
    }
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
        deboParar = esCentinela(columna); 
        estado = realizarTransicion(columna);
    }

    manejoDeCentinela();
    if(columna != ESPACIO){
        ungetc(caracter, stdin); //devuelvo cualquier centinela que no sea espacio para la proxima vuelta
    }

    deboParar = false;
    return token;
}