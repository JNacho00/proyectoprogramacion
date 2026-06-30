#include "enemigos.h"
#include "mapa.h"
#include "balas.h"

#define ancho_enemigo 40
#define alto_enemigo 40
#define max_enemigos 8

enemigo enemigos[max_enemigos];

void dibujar_enemigo(enemigo* e, float camara) {
    float ancho_barra = e->ancho + 20.0f;
    float alto_barra = 7.0f;

    float vida_actual = (e->vida / (float)e->vida_max) * ancho_barra;

    float barra_x = e->x -10.0f;
    float barra_y = e->y - 20.0f - camara;

    al_draw_filled_rectangle(
        e->x,
        e->y - camara,
        e->x + e->ancho,
        e->y + e->alto - camara,
        al_map_rgb(0, 255, 0)
    );

    al_draw_filled_rectangle(
        barra_x,
        barra_y,
        barra_x + ancho_barra,
        barra_y + alto_barra,
        al_map_rgb(80, 0, 0)
    );

    al_draw_filled_rectangle(
        barra_x,
        barra_y,
        barra_x + vida_actual,
        barra_y + alto_barra,
        al_map_rgb(255, 0, 0)
    );
}

void crear_enemigo(float x, float y) {
    int i;

    for (i = 0; i < max_enemigos; i++) {
        if (enemigos[i].activo == false) {

            enemigos[i].x = x;
            enemigos[i].y = y;

            enemigos[i].ancho = 40;
            enemigos[i].alto = 40;

            enemigos[i].velocidadx = 0;
            enemigos[i].velocidady = 0;

            enemigos[i].vida = 3;
            enemigos[i].vida_max = 3;
            enemigos[i].dano = 10;

            enemigos[i].activo = true;
            return;
        }
    }
}

void dibujar_enemigos_mapa(float camara) {
    int i;

    for (i = 0; i < max_enemigos; i++) {
        if (enemigos[i].activo == true) {
            dibujar_enemigo(&enemigos[i], camara);
        }
    }
}

void spawn_enemigos() {
    int fila;
    int columna;

    for (fila = 0; fila < filas; fila++) {
        for (columna = 0; columna < columnas; columna++) {

            if (mapa[fila][columna] == 'e') {

                crear_enemigo(columna * ancho_v,fila * largo_v);

                mapa[fila][columna] = '.';
            }
        }
    }
}

bool colision_bala_enemigo(bala* b, enemigo* e) {

    if (b->x < e->x + e->ancho &&
        b->x + b->ancho > e->x &&
        b->y < e->y + e->alto &&
        b->y + b->alto > e->y) {

        e->vida -= 1;       
        b->activa = false;  

        if (e->vida <= 0) {
            e->vida = 0;
            e->activo = false;
        }

        return true;
    }

    return false;
}

void revisar_colisione_bala_enemigo() {
    int i;
    int j;

    for (i = 0; i < max_balas; i++) {
        if (balas[i].activa == true) {

            for (j = 0; j < max_enemigos; j++) {
                if (enemigos[j].activo == true) {

                    if (colision_bala_enemigo(&balas[i], &enemigos[j])) {
                        break;
                    }
                }
            }
        }
    }
}