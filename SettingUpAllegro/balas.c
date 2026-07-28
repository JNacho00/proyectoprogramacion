#include "balas.h"
#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mapa.h"
#define max_balas_p 3

#define FRAMES_BALA 4
#define CAMBIO_FRAME_BALA 4

ALLEGRO_BITMAP* frames_bala[FRAMES_BALA];

void dibujar_bala(bala* b, float camara_x, float camara_y) {
	ALLEGRO_BITMAP* sprite;

	if (b->activa == false) {
		return;
	}

	sprite = frames_bala[b->frame_actual];

	if (sprite == NULL) {
		return;
	}

	al_draw_scaled_bitmap(
		sprite,

		0,
		0,
		al_get_bitmap_width(sprite),
		al_get_bitmap_height(sprite),

		b->x - camara_x,
		b->y - camara_y,
		b->ancho,
		b->alto,

		0
	);
}

void spawn_balas(bala balas[]) { //inicializa balas
	int i;

	for (i = 0; i < max_balas_p; i++) {
		balas[i].x = 0;
		balas[i].y = 0;

		balas[i].ancho = ancho_bala;
		balas[i].alto = alto_bala;

		balas[i].velocidad_bx = 0;
		balas[i].velocidad_by = 0;

		balas[i].frame_actual = 0;
		balas[i].contador_animacion = 0;

		balas[i].activa = false;
	}
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

			actualizar_animacion_bala(&balas[i]);
			float nueva_x = balas[i].x + balas[i].velocidad_bx;
			float nueva_y = balas[i].y + balas[i].velocidad_by;

			if (fisicas_mapa((int)nueva_x, (int)nueva_y) ||										  //A
				fisicas_mapa((int)(nueva_x + balas[i].ancho), (int)nueva_y) ||					  //B	
				fisicas_mapa((int)nueva_x, (int)(nueva_y + balas[i].alto)) ||					  //C
				fisicas_mapa((int)(nueva_x + balas[i].ancho), (int)(nueva_y + balas[i].alto))) {  //C

				balas[i].activa = false;
			}
			else {
				balas[i].x = nueva_x;
				balas[i].y = nueva_y;
			}
		}
	}
}

bool cargar_sprites_balas(void) {
	frames_bala[0] = al_load_bitmap("assets/armas&balas/balas1.png");
	frames_bala[1] = al_load_bitmap("assets/armas&balas/balas2.png");
	frames_bala[2] = al_load_bitmap("assets/armas&balas/balas3.png");
	frames_bala[3] = al_load_bitmap("assets/armas&balas/balas4.png");

	for (int i = 0; i < FRAMES_BALA; i++) {
		if (frames_bala[i] == NULL) {
			printf("No se pudo cargar un sprite de bala: frame %d\n", i);
			return false;
		}
	}

	return true;
}

void liberar_sprites_balas(void) {
	int i;

	for (i = 0; i < FRAMES_BALA; i++) {
		if (frames_bala[i] != NULL) {
			al_destroy_bitmap(frames_bala[i]);
			frames_bala[i] = NULL;
		}
	}
}

void actualizar_animacion_bala(bala* b) {
	if (b->activa == false) {
		return;
	}

	b->contador_animacion++;

	if (b->contador_animacion >= CAMBIO_FRAME_BALA) {
		b->contador_animacion = 0;
		b->frame_actual++;

		if (b->frame_actual >= FRAMES_BALA) {
			b->frame_actual = 0;
		}
	}
}