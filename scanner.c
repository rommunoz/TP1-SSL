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
    TT[0][2] = IDENTIFICADOR;
    TT[0][3] = IDENTIFICADOR;
    TT[0][4] = IDENTIFICADOR;

    for(int j = 0; j<5; j++)
        TT[1][j] = IDENTIFICADOR;

    TT[0][0] = ENTERO;
    TT[0][1] = ENTERO;
    TT[2][0] = ENTERO;
    TT[2][1] = ENTERO;

    TT[3][0] = HEXADECIMAL;
    TT[3][1] = HEXADECIMAL;
    TT[3][2] = HEXADECIMAL;
    TT[2][3] = HEXADECIMAL;

    TT[2][2] = ENTERO_MAL_FORMADO;
    TT[2][4] = ENTERO_MAL_FORMADO;
    TT[3][3] = ENTERO_MAL_FORMADO;
    TT[3][4] = ENTERO_MAL_FORMADO;

    for(int j = 0; j<5; j++)
        TT[4][j] = ENTERO_MAL_FORMADO;

    for(int i = 0; i<6; i++)
        TT[i][6] = ERROR_GENERAL;
    for(int j = 0; j<5; j++)
        TT[5][j] = ERROR_GENERAL;

    for(int i = 0; i<6; i++)
        TT[i][7] = FDA;

    for(int i = 0; i<7; i++)
        TT[i][5] = INICIAL; //podria ahorrarme esto, pero no se si es buena practica...
    for(int j = 0; j<8; j++)
        TT[6][j] = INICIAL;
}

int esAceptor(int unEstado){ // despues me di cuenta que no es
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
            printf("Espacio '%s'\n", lexem_buffer); 
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


void guardarEnLexema(int unCaracter){
    if(lexbf_index < 256+1){
        lexem_buffer[lexbf_index++] = unCaracter;
    } else {
        printf("El buffer del lexema se llenó ");
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

bool primerLlamadoAScanner = true;
void scanner (void) {
    if(primerLlamadoAScanner){
        cargarTabla();
        primerLlamadoAScanner = false;
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
    guardarEnLexema(caracter);
    return;
}