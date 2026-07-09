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


bala balas[max_balas_p];

void dibujar_bala(bala* b, float camara_x, float camara_y) {
	al_draw_filled_rectangle(
		b->x - camara_x,
		b->y - camara_y,
		b->x + ancho_bala - camara_x,
		b->y + alto_bala - camara_y,
		al_map_rgb(0, 255, 0)
	);
}

bool crear_bala(int x, int y, float belocidadx, float belocidady) {
	
	static int i;


	for (i = 0; i < max_balas_p; i++) {
		if (balas[i].activa == false) {

			balas[i].x = x;
			balas[i].y = y;
			balas[i].velocidad_bx = belocidadx;
			balas[i].velocidad_by = belocidady;
			balas[i].alto = alto_bala;
			balas[i].ancho = ancho_bala;

			balas[i].activa = true;
			return true;
		}
	}
	return false;
}

void dibujar_balas_mapa(float camara_x, float camara_y) {
	int i;
	for (i = 0; i < max_balas_p; i++) {
		if (balas[i].activa == true) {
			dibujar_bala(&balas[i], camara_x, camara_y);
		}
	}
}

void fisicas_balas() {
	int i;

	for (i = 0; i < max_balas_p; i++) {
		if (balas[i].activa == true) {

			float nueva_x = balas[i].x + balas[i].velocidad_bx;
			float nueva_y = balas[i].y + balas[i].velocidad_by;

			if (fisicas_mapa((int)nueva_x, (int)nueva_y) || 
				fisicas_mapa((int)(nueva_x + balas[i].ancho), (int)nueva_y) || 
				fisicas_mapa((int)nueva_x, (int)(nueva_y + balas[i].alto)) ||
				fisicas_mapa((int)(nueva_x + balas[i].ancho), (int)(nueva_y + balas[i].alto))) {

				balas[i].activa = false;
			}
			else {
				balas[i].x = nueva_x;
				balas[i].y = nueva_y;
			}
		}
	}
}