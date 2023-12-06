#ifndef scanner_h
#define scanner_h

enum Token {INICIAL,
            ENTERO_CERO, ENTERO, HEXADECIMAL,IDENTIFICADOR,
            ENT_MAL_FORM, ERROR_GENERAL, FDA}; //FDA por fin de archivo y que no haya conflicto con columna FDT

enum Dimensiones {FILAS = 8, COLUMNAS = 8};

static int caracter;
extern char lexem_buffer[512+1]; //+1 para el '\0'
static int lexbf_index = 0; //indice del buffer
static int token = INICIAL;
static int TT[FILAS][COLUMNAS]; //Tabla de Transicion

int scanner(void);

enum Columna {CERO, DIGITO_NO_CERO, LETRAS_HEX, LETRA_X, RESTO_LETRAS, ESPACIO, FDT, OTROS}; //el EOF vale -1 pero su columna es la 6

int esAceptor(int unEstado);
void guardarEnLexema(int unCaracter);
void manejoDeCentinela(void);
enum Token realizarTransicion(int car);

#endif