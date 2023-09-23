#ifndef scanner_h
#define scanner_h
#define CORRECCION_ACEP 99  //desfase de la fila segun estado aceptor
#define CORRECCION_RECH 196 //desfase de la fila segun estado no aceptor

enum Estado {INICIAL,
            IDENTIFICADOR = 100, ENTERO, HEXADECIMAL,
            ENTERO_MAL_FORMADO = 200, ERROR_GENERAL, FDA}; //FDA por fin de archivo y que no haya conflicto con columna FDT

enum Estado estado = INICIAL;

enum Dimensiones {FILAS = 7, COLUMNAS = 8};

int caracter;
char lexem_buffer[256+1]; //+1 para el '\0'
int lexbf_index = 0; //indice del buffer
int token = INICIAL;
static int TT[FILAS][COLUMNAS]; //Tabla de Transicion


void cargarTabla(void);
void scanner(void);

enum Columna {CERO, DIGITO_NO_CERO, LETRAS_HEX, LETRA_X, RESTO_LETRAS, ESPACIO, OTROS, FDT}; //el EOF vale -1 pero su columna es la 7

short int columna = 0;
short int fila = 0;

int obtenerFila(enum Estado);
int obtenerColumna(int);
int esAceptor(int unEstado);
void guardarEnLexema(int unCaracter);
void emitirLexema(enum Estado est);
void manejoDeCentinela(void);
enum Estado realizarTransicion(short col);

#endif