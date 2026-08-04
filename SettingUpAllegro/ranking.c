#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include "ranking.h"
#include "personaje.h"

void guardar_registro_ranking(char nombre[], int puntaje) {
    FILE* f;
    registros_ranking ranking[max_ingresos];
    int cantidad = leer_ranking(ranking, 10);
    int i;

    if (cantidad < max_ingresos) {
        strcpy(ranking[cantidad].nombre, nombre);
        ranking[cantidad].puntaje = puntaje;
        cantidad++;
    }

    ordenar_ranking(ranking, cantidad);

    if (cantidad > 10) {
        cantidad = 10;
    }

    f = fopen("ranking.txt", "w");

    if (f == NULL) {
        printf("No se pudo abrir ranking.txt\n");
        return;
    }
    for (i = 0; i < cantidad; i++) {
        fprintf(f, "%s;%d\n", ranking[i].nombre, ranking[i].puntaje);
    }
    fclose(f);
}

int leer_ranking(registros_ranking ranking[], int max_registros) {
    FILE* f;
    int cont = 0;

    f = fopen("ranking.txt", "r");

    if (f == NULL) {
        printf("error\n");
        return 0;
    }

    while (cont < max_registros && fscanf(f, " %19[^;];%d", ranking[cont].nombre, &ranking[cont].puntaje) == 2) {
        cont++;
    }

    fclose(f);

    return cont;
}

void ordenar_ranking(registros_ranking ranking[], int cont) {
    int i;
    int j;
    int pos_mayor;
    registros_ranking aux;

    for (i = 0; i < cont; i++) {
        pos_mayor = i;
        for (j = 0; j < cont; j++) {
            if (ranking[j].puntaje < ranking[pos_mayor].puntaje) {
                aux = ranking[j];
                ranking[j] = ranking[pos_mayor];
                ranking[pos_mayor] = aux;
            }
        }
    }
}