#include <stdio.h>
#include "scanner.h"
//#include "main.h"
#include "scanner.c"

int main(void){
    while(token != FDT){
        scanner();
    }    
    return 0;
}