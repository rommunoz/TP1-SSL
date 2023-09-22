#include <stdio.h>
#include "scanner.h"
#include <ctype.h>
#include <string.h>
#include <stdbool.h>


int obtenerColumna(int car) {
    if (isspace(car))
        return ESPACIO;
    else if (car == EOF)
        return FDA;
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

int obtenerFila(enum Estado est){ //usar enum Corrccion
    switch (est) {
        case INICIAL:
            return 0;
        case IDENTIFICADOR:
            return 1;
        case ENTERO:
            return 2;
        case HEXADECIMAL:
            return 3;
        case ENTERO_MAL_FORMADO:
            return 4;
        case ERROR_GENERAL:
            return 5;
        case FDT:
            return 6;
    }
}

void cargarTabla(){
    tabla_transicion[0][2] = IDENTIFICADOR;
    tabla_transicion[0][3] = IDENTIFICADOR;
    tabla_transicion[0][4] = IDENTIFICADOR;

    for(int j = 0; j<5; j++)
        tabla_transicion[1][j] = IDENTIFICADOR;

    tabla_transicion[0][0] = ENTERO;
    tabla_transicion[0][1] = ENTERO;
    tabla_transicion[2][0] = ENTERO;
    tabla_transicion[2][1] = ENTERO;

    tabla_transicion[3][0] = HEXADECIMAL;
    tabla_transicion[3][1] = HEXADECIMAL;
    tabla_transicion[3][2] = HEXADECIMAL;
    tabla_transicion[2][3] = HEXADECIMAL;

    tabla_transicion[2][2] = ENTERO_MAL_FORMADO;
    tabla_transicion[2][4] = ENTERO_MAL_FORMADO;
    tabla_transicion[3][3] = ENTERO_MAL_FORMADO;
    tabla_transicion[3][4] = ENTERO_MAL_FORMADO;

    for(int j = 0; j<5; j++)
        tabla_transicion[4][j] = ENTERO_MAL_FORMADO;

    for(int i = 0; i<6; i++)
        tabla_transicion[i][6] = ERROR_GENERAL;
    for(int j = 0; j<5; j++)
        tabla_transicion[5][j] = ERROR_GENERAL;

    for(int i = 0; i<6; i++)
        tabla_transicion[i][7] = FDT;

    for(int i = 0; i<7; i++)
        tabla_transicion[i][5] = INICIAL; //esto por ser una variable estatico podria no inicializar en 0
    for(int j = 0; j<8; j++)
        tabla_transicion[6][j] = INICIAL;
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
     } 
} 

enum Estado realizarTransicion(int col){
    fila = obtenerFila(estado);
    return tabla_transicion[fila][col];
}

void manejoDeCentinela(void){
    if(esAceptor(estado)){
        guardarEnLexema('\0'); // indico fin de cadena
        printf("Cadena aceptada:\n\t");
        emitirLexema(estado);
    } else {
        guardarEnLexema('\0');
        printf("Cadena \"rechazada\" (en realidad aceptada como error):\n\t");
        emitirLexema(estado);
    }
    lexbf_index = 0;
    token = estado;
    estado = realizarTransicion(columna);
}

void guardarEnLexema(int unCaracter){
    if(lexbf_index < 256+1){
        lexem_buffer[lexbf_index++] = unCaracter;
    } else {
        printf("El buffer del lexema se llenó ");
    }
}

//obtenerToken(estado);
int caracter;
bool flagPrimeraVez = true;

void scanner (void) {
    if(flagPrimeraVez){
        cargarTabla();
        flagPrimeraVez = false;
    }
    caracter = getchar();
    columna = obtenerColumna(caracter);
    if(caracter == EOF || columna == ESPACIO){
        manejoDeCentinela();
        printf("Token leido: %i\n\n", token);
        //return token;
        return;
    }
    switch (estado){
        case INICIAL:
            estado = realizarTransicion(columna);
            guardarEnLexema(caracter);
            break;

        case IDENTIFICADOR:
            estado = realizarTransicion(columna);
            guardarEnLexema(caracter);
            break;
        
        case ENTERO:
            estado = realizarTransicion(columna);
            guardarEnLexema(caracter);
            break;

        case HEXADECIMAL: 
            estado = realizarTransicion(columna);
            guardarEnLexema(caracter);
            break;

        case ENTERO_MAL_FORMADO: 
            estado = realizarTransicion(columna);
            guardarEnLexema(caracter);  
            break;  
        
        case ERROR_GENERAL:
            estado = realizarTransicion(columna);
            guardarEnLexema(caracter);
            break;
    } 
}

