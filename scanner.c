#include <stdio.h>
#include "scanner.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

int obtenerColumna(int car) {
    if (isspace(car))
        return ESPACIO;
    else if (car == EOF || car == 26) //haciendo debug 
        return FDT;
    else if (car == '0')
        return CERO;
    else if (isdigit(car))
        return DIGITO_NO_CERO;
    else if (car == 'x' || car == 'X')
        return LETRA_X;
    else if (isxdigit(car))
        return LETRAS_HEX;
    else if (isalpha(car))
        return RESTO_LETRAS; 
    else
        return OTROS;
}

int obtenerFila(enum Estado est){ //este enum Correccion surge de -separar en clases los estados- 
    if(est < 100){
        return est;
    } else {
        return esAceptor(est) ? 
            est - CORRECCION_ACEP : est - CORRECCION_RECH; 
    }
}

void cargarTabla(){
    TT[INICIAL][LETRAS_HEX] = IDENTIFICADOR;
    TT[INICIAL][LETRA_X] = IDENTIFICADOR;
    TT[INICIAL][RESTO_LETRAS] = IDENTIFICADOR;

    for(int j = 0; j<5; j++)
        TT[1][j] = IDENTIFICADOR;

    TT[INICIAL][CERO] = ENTERO;
    TT[INICIAL][DIGITO_NO_CERO] = ENTERO;
    TT[ENTERO - CORRECCION_ACEP][CERO] = ENTERO;
    TT[ENTERO - CORRECCION_ACEP][DIGITO_NO_CERO] = ENTERO;

    TT[HEXADECIMAL - CORRECCION_ACEP][CERO] = HEXADECIMAL;
    TT[HEXADECIMAL - CORRECCION_ACEP][DIGITO_NO_CERO] = HEXADECIMAL;
    TT[HEXADECIMAL - CORRECCION_ACEP][LETRAS_HEX] = HEXADECIMAL;
    TT[ENTERO - CORRECCION_ACEP][LETRA_X] = HEXADECIMAL;

    TT[ENTERO - CORRECCION_ACEP][LETRAS_HEX] = ENTERO_MAL_FORMADO;
    TT[ENTERO - CORRECCION_ACEP][RESTO_LETRAS] = ENTERO_MAL_FORMADO;
    TT[HEXADECIMAL - CORRECCION_ACEP][LETRAS_HEX] = ENTERO_MAL_FORMADO;
    TT[HEXADECIMAL - CORRECCION_ACEP][RESTO_LETRAS] = ENTERO_MAL_FORMADO;

    //utilizamos for para rellenar la tabla y acortar el codigo
    for(int j = CERO; j<ESPACIO; j++)
        TT[ENTERO_MAL_FORMADO - CORRECCION_RECH][j] = ENTERO_MAL_FORMADO;

    for(int i = CERO; i<OTROS; i++)
        TT[i][6] = ERROR_GENERAL;
    for(int j = CERO; j<ESPACIO; j++)
        TT[ERROR_GENERAL - CORRECCION_RECH][j] = ERROR_GENERAL;

    for(int i = CERO; i<OTROS; i++)
        TT[i][FDT] = FDA;

    for(int i = CERO; i<FDT; i++)
        TT[i][ESPACIO] = INICIAL; //podria ahorrarme esto, pero no se si es buena practica...
    for(int j = CERO; j<FDT+1; j++) //
        TT[FDA - CORRECCION_RECH][j] = INICIAL;
}

int esAceptor(int unEstado){ // no se pide informar, pero me sirve
    return 100 <= unEstado && unEstado < 200 ? 1 : 0; 
}

void emitirLexema(enum Estado est){ 
     // En realidad el token es un entero que tiene un significado, si no me equivoco
     switch(est){
        case ERROR_GENERAL:
            printf("Error general '%s'\n", lexem_buffer);
            break;
        case IDENTIFICADOR:
            printf("Identificador '%s'\n", lexem_buffer);
            break;
        case ENTERO:
            printf("Entero '%s'\n", lexem_buffer);
            break;
        case INICIAL: 
            //printf("Espacio '%s'\n", lexem_buffer); 
            break;
        case HEXADECIMAL: 
            printf("Hexadecimal '%s'\n", lexem_buffer); 
            break;
        case ENTERO_MAL_FORMADO: 
            printf("Entero Mal formado '%s'\n", lexem_buffer);  
            break;
        case FDA:
            printf("Se llego al fin de archivo.\n");  
            break;
     } 
} 

//bool estaLlenoElBuffer = false;

void guardarEnLexema(int unCaracter){
    if(lexbf_index < 256){ //dejo un lugar para el '\0'
        lexem_buffer[lexbf_index++] = unCaracter;
    } else {
        printf("El buffer del lexema se llenó ");
        //estaLlenoElBuffer = true;
    }
}

void manejoDeCentinela(void){
    guardarEnLexema('\0'); // indico fin de cadena
    emitirLexema(estado);
    lexbf_index = 0;
    token = estado;
    estado = realizarTransicion(columna);
}

enum Estado realizarTransicion(short col){
    fila = obtenerFila(estado);
    return TT[fila][col];
}

bool esPrimerLlamadoAScanner = true;

void scanner (void) {
    if(esPrimerLlamadoAScanner){
        cargarTabla();
        esPrimerLlamadoAScanner = false;
    }
    caracter = getchar();
    columna = obtenerColumna(caracter);
    
    if(columna == ESPACIO){
        manejoDeCentinela();
        return;
    }
    
    if(columna == FDT){
        manejoDeCentinela();
        ungetc(caracter, stdin);
        return;
    }

    estado = realizarTransicion(columna);
    //if(!estaLlenoElBuffer)
    guardarEnLexema(caracter);
    return;
}