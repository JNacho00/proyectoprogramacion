#ifndef ranking_h
#define ranking_h
#include "personaje.h"
#define max_registros_leidos 50
#define max_nombre 20
#define max_ingresos 11


typedef struct {
	
	char nombre[max_nombre];
	int puntaje;

} registros_ranking;

void guardar_registro_ranking(char nombre[], int puntaje);
int leer_ranking(registros_ranking ranking[], int max_registros);
void ordenar_ranking(registros_ranking ranking[], int cont);
#endif 