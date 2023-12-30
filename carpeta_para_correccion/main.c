#include <stdio.h>
#include "scanner.h"

int main(void){
    while(token != FDA){
        token = scanner();
        switch(token){
            case ERROR_GENERAL:
                printf("Error general '%s'\n", lexem_buffer);
                break;
            case IDENTIFICADOR:
                printf("Identificador '%s'\n", lexem_buffer);
                break;
            case (ENTERO_CERO):
                printf("Entero '%s'\n", lexem_buffer);
                break;
            case (ENTERO):
                printf("Entero '%s'\n", lexem_buffer);
                break;
            case HEXADECIMAL: 
                printf("Hexadecimal '%s'\n", lexem_buffer); 
                break;
            case ENT_MAL_FORM: 
                printf("Entero Mal formado '%s'\n", lexem_buffer);  
                break;
            case FDA:
                printf("Se llego al fin de archivo.\n");  
                break;
        }
    }
    return 0;
}