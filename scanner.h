#ifndef scanner_h
#define scanner_h
#define CORRECCION_ACEP 99  //desfase de la fila segun estado aceptor
#define CORRECCION_RECH 196 //desfase de la fila segun estado no aceptor

enum Estado {INICIAL,
            IDENTIFICADOR = 100, ENTERO, HEXADECIMAL,
            ENTERO_MAL_FORMADO = 200, ERROR_GENERAL, FDT};

enum Estado estado = INICIAL;

int caracter;
char lexem_buffer[256+1];
int lexbf_index = 0;
int token = INICIAL;
static int tabla_transicion[7][8];

void cargarTabla(void);
void scanner(void);

enum Columna {CERO, DIGITO_NO_CERO, LETRAS_HEX,LETRA_X, RESTO_LETRAS, ESPACIO, OTROS, FDA}; //el EOF vale -1 pero su columna es la 7

short int columna = 0;
short int fila = 0;

int obtenerFila(enum Estado);
int obtenerColumna(int);
enum Estado realizarTransicion(int col);
int esAceptor(int unEstado);
void emitirLexema(enum Estado est);
void manejoDeCentinela(void);
void guardarEnLexema(int unCaracter);

#endif