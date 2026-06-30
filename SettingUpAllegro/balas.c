#include "balas.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mapa.h"


bala balas[max_balas];

void dibujar_bala(bala* b, float camara) {
	al_draw_filled_rectangle(
		b->x,
		b->y - camara,
		b->x + ancho_bala,
		b->y + alto_bala - camara,
		al_map_rgb(0, 255, 0)
	);
}

void crear_bala(int x, int y, float belocidadx, float belocidady) {
	int i;

	bool activa = true;

	for (i = 0; i < max_balas; i++) {
		if (balas[i].activa == false) {

			balas[i].x = x;
			balas[i].y = y;
			balas[i].velocidad_bx = belocidadx;
			balas[i].velocidad_by = belocidady;
			balas[i].alto = alto_bala;
			balas[i].ancho = ancho_bala;

			balas[i].activa = true;
			return;
		}
	}
}

/*
void actualizar_bala() {
	int i;

	bool activa = true;

	for (i = 0; i < max_balas; i++) {
		if (balas[i].activa == true) {
			balas[i].x += balas[i].velocidad_bx;
			balas[i].y += balas[i].velocidad_by;
		}
	}
}
*/
void dibujar_balas_mapa(float camara) {
	int i;
	for (i = 0; i < max_balas; i++) {
		if (balas[i].activa == true) {
			dibujar_bala(&balas[i], camara);
		}
	}
}

void fisicas_balas() {
	int i;

	for (i = 0; i < max_balas; i++) {
		if (balas[i].activa == true) {

			float nueva_x = balas[i].x + balas[i].velocidad_bx;
			float nueva_y = balas[i].y + balas[i].velocidad_by;

			if (fisicas_mapa((int)nueva_x, (int)nueva_y) || // arriba izquierda
				fisicas_mapa((int)(nueva_x + balas[i].ancho), (int)nueva_y) || // arriba derecha
				fisicas_mapa((int)nueva_x, (int)(nueva_y + balas[i].alto)) || // abajo izquierda
				fisicas_mapa((int)(nueva_x + balas[i].ancho), // abajo derecha
					(int)(nueva_y + balas[i].alto))) {

				balas[i].activa = false;
			}
			else {
				balas[i].x = nueva_x;
				balas[i].y = nueva_y;
			}
		}
	}
}