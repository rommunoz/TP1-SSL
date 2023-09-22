#ifndef scanner_h
#define scanner_h

enum Estado {INICIAL,
            IDENTIFICADOR = 100, ENTERO, HEXADECIMAL, FDT,
            ERROR_GENERAL = 200, ENTERO_MAL_FORMADO};

enum Estado estado = INICIAL;

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