#include <stdio.h>
#include "scanner.h"
//#include "main.h"
#include "scanner.c"

int main(void){
    while(token != EOF){
        scanner();
    }    
    return 0;
}