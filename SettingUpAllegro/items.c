#include "items.h"
#include "mapa.h"
#include "enemigos.h"
#include "balas.h"
#include <stdio.h>
#include <allegro5/allegro_primitives.h>


void inicializar_items(item items[]) {
    int i;

    for (i = 0; i < max_items; i++) {

        items[i].x = 0;
        items[i].y = 0;

        items[i].ancho = 20;
        items[i].alto = 20;

        items[i].tipo = 0;
        items[i].valor = 0;

        items[i].activo = false;
    }
}

void crear_item(item items[], float x, float y, tipo_item tipo, int valor) {
    int i;

    for (i = 0; i < max_items; i++) {
        if (items[i].activo == false) {
         
            items[i].x = x;
            items[i].y = y;

            items[i].ancho = 20;
            items[i].alto = 20;

            items[i].tipo = tipo;
            items[i].valor = valor;

            items[i].activo = true;
            return;
        }
    }
}

void spawn_items(item items[]) {
    int fila;
    int col;

    for (fila = 0; fila < mapa_filas; fila++) {
        for (col = 0; col < mapa_col; col++) {

            if (mapa[fila][col] == 'v') {

                crear_item(items, col * ancho_v, fila * largo_v, VIDA, 5);

                mapa[fila][col] = '.';
            }
            if (mapa[fila][col] == 'm') {

                crear_item(items, col * ancho_v, fila * largo_v, MUNICION, 20);

                mapa[fila][col] = '.';
            }
            if (mapa[fila][col] == 'b') {

                crear_item(items, col * ancho_v, fila * largo_v, BARRIL, 3);

                mapa[fila][col] = '.';
            }
        }
    }
}

bool interaccion_item(item* i, personaje* p) {
    return 
        i->x < p->x + p->ancho &&
        i->x + i->ancho > p->x &&
        i->y < p->y + p->alto &&
        i->y + i->alto > p->y;
}

void actualizar_items(item items[], personaje* p) {
    int i;

    for (i = 0; i < max_items; i++) {

        if (items[i].activo == true) {
            if (interaccion_item(&items[i], p)) {
                if (items[i].tipo == VIDA) {
                    p->vida += items[i].valor;

                    items[i].activo = false;
                }
                if (items[i].tipo == MUNICION) {
                    p->municion += items[i].valor;

                    items[i].activo = false;
                }
            }
        }
    }
}

void dibujar_item(item* i, float camara_x, float camara_y) {
    
    if (i->activo == false) {
        return;
    }

    if (i->tipo == VIDA) {
        al_draw_filled_rectangle(
            i->x - camara_x,
            i->y - camara_y,
            i->x + i->ancho - camara_x,
            i->y + i->alto - camara_y,
            al_map_rgb(0, 255, 0)
        );
    }
    if (i->tipo == MUNICION) {
        al_draw_filled_rectangle(
            i->x - camara_x,
            i->y - camara_y,
            i->x + i->ancho - camara_x,
            i->y + i->alto - camara_y,
            al_map_rgb(255, 180, 0)
        );
    }
    if (i->tipo == BARRIL) {
        al_draw_filled_rectangle(
            i->x - camara_x,
            i->y - camara_y,
            i->x + i->ancho - camara_x,
            i->y + i->alto - camara_y,
            al_map_rgb(150, 80, 20)
        );
    }
}

void dibujar_items(item items[], float camara_x, float camara_y) {
	int i;

	for (i = 0; i < max_items; i++) {

		if (items[i].activo == true) {
			dibujar_item(&items[i], camara_x, camara_y);
		}
	}
}

bool colision_bala_item(bala* b, item* i) {
    return 
        b->x < i->x + i->ancho &&
        b->x + b->ancho > i->x &&
        b->y < i->y + i->alto &&
        b->y + b->alto > i->y;
}

void explosion_barril(item* barril, enemigo enemigos[]) {
    int i;

    float rango = 300.0f;

    float explosion_izq = barril->x - rango;
    float explosion_der = barril->x + barril->ancho + rango;
    float explosion_arriba = barril->y - rango;
    float explosion_abajo = barril->y + barril->alto;

    for (i = 0; i < max_enemigos; i++) {

        if (enemigos[i].activo == true) {
            if (enemigos[i].x < explosion_der &&
                enemigos[i].x + enemigos[i].ancho > explosion_izq &&
                enemigos[i].y < explosion_abajo &&
                enemigos[i].y + enemigos[i].alto > explosion_arriba) {

                enemigos[i].vida -= barril->valor;

                if (enemigos[i].vida <= 0) {
                    enemigos[i].vida = 0;
                    enemigos[i].activo = false;
                }
            }
        }
    }
    barril->activo = false;
}

void colision_bala_barril(item items[], bala balas[], enemigo enemigos[]) {
    int i;
    int j;

    for (i = 0; i < max_items; i++) {

        if (items[i].activo == true && items[i].tipo == BARRIL) {

            for (j = 0; j < max_balas_p; j++) {

                if (balas[j].activa == true) {

                    if (colision_bala_item(&balas[j], &items[i])) {
                        balas[j].activa = false;
                        explosion_barril(&items[i], enemigos);
                        break;
                    }
                }
            }
        }
    }
}