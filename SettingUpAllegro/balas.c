#include "balas.h"
#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mapa.h"
#define max_balas_p 3


void dibujar_bala(bala* b, float camara_x, float camara_y) {
	al_draw_filled_rectangle(
		b->x - camara_x,
		b->y - camara_y,
		b->x + ancho_bala - camara_x,
		b->y + alto_bala - camara_y,
		al_map_rgb(0, 255, 0)
	);
}

void spawn_balas(bala balas[]) {
	int i;

	for (i = 0; i < max_balas_p; i++) {
		balas[i].x = 0;
		balas[i].y = 0;

		balas[i].ancho = ancho_bala;
		balas[i].alto = alto_bala;

		balas[i].velocidad_bx = 0;
		balas[i].velocidad_by = 0;

		balas[i].activa = false;
	}
}

bool crear_bala(personaje* p, bala balas[]) {
	
	int i;
	float vx;
	float vy;
	float posx;
	float posy;

	if (p->municion <= 0) {
		return;
	}

	if (p->direccionx == 0 && p->direcciony == 0) {
		return;
	}

	vx = p->direccionx * velocidad_bala;
	vy = p->direcciony * velocidad_bala;

	posx = p->x + (p->ancho - ancho_bala) / 2.0f;
	posy = p->y + (p->alto - alto_bala) / 2.0f;

	if (p->direccionx > 0) {
		posx = p->x + p->ancho + 1;
	}
	else if (p->direccionx < 0) {
		posx = p->x - ancho_bala - 1;
	}

	if (p->direcciony > 0) {
		posy = p->y + p->alto + 1;
	}
	else if (p->direcciony < 0) {
		posy = p->y - alto_bala - 1;
	}

	for (i = 0; i < max_balas_p; i++) {
		if (balas[i].activa == false) {

			balas[i].x = posx;
			balas[i].y = posy;
			balas[i].velocidad_bx = vx;
			balas[i].velocidad_by = vy;
			balas[i].alto = alto_bala;
			balas[i].ancho = ancho_bala;

			balas[i].activa = true;
			p->municion--;
			return true;
		}
	}
	return false;
}

void dibujar_balas_mapa(bala balas[], float camara_x, float camara_y) {
	int i;
	for (i = 0; i < max_balas_p; i++) {
		if (balas[i].activa == true) {
			dibujar_bala(&balas[i], camara_x, camara_y);
		}
	}
}

void fisicas_balas(bala balas[]) {
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