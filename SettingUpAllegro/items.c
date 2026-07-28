#include "items.h"
#include "mapa.h"
#include "enemigos.h"
#include "balas.h"
#include <stdio.h>
#include <allegro5/allegro_image.h>
#include <stdbool.h>
#include <allegro5/allegro_primitives.h>
#define FRAMES_VIDA 6
#define CAMBIO_FRAME_VIDA 10
#define FRAMES_LLAVE 6
#define CAMBIO_FRAME_LLAVE 10

ALLEGRO_BITMAP* frames_vida[FRAMES_VIDA];
ALLEGRO_BITMAP* sprite_municion = NULL;
ALLEGRO_BITMAP* sprite_escudo = NULL;
ALLEGRO_BITMAP* sprite_barril = NULL;
ALLEGRO_BITMAP* frames_llave[FRAMES_LLAVE];

void inicializar_items(item items[]) {
    int i;

    for (i = 0; i < max_items; i++) {

        items[i].x = 0;
        items[i].y = 0;

        items[i].ancho = 20;
        items[i].alto = 20;

        items[i].y_inicio = 0;
        items[i].velocidad_y = 0;
        items[i].distancia = 0;
        items[i].direccion_y = 0;

        items[i].frame_actual = 0;
        items[i].contador_animacion = 0;

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

            items[i].y_inicio = y;

            items[i].frame_actual = 0;
            items[i].contador_animacion = 0;

            items[i].tipo = tipo;
            items[i].valor = valor;

            if (tipo != BARRIL) {
                items[i].velocidad_y = 0.4f;
                items[i].distancia = 20.0f;
                items[i].direccion_y = 1;
            }
            else {
                items[i].velocidad_y = 0.0f;
                items[i].distancia = 0.0f;
                items[i].direccion_y = 0;
                items[i].ancho = 20;
                items[i].alto = 40;

            }

            items[i].activo = true;
            return;
        }
    }
}

void spawn_items(item items[]) {
    int fila;
    int col;

    for (fila = 0; fila < filas; fila++) {
        for (col = 0; col < columnas; col++) {

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
            if (mapa[fila][col] == 'k') {

                crear_item(items, col * ancho_v, fila * largo_v, LLAVE, 1);

                mapa[fila][col] = '.';
            }
            if (mapa[fila][col] == 's') {

                crear_item(items, col * ancho_v, fila * largo_v, ESCUDO, 5);

                mapa[fila][col] = '.';
            }
            if (mapa[fila][col] == '$') {

                crear_item(items, col * ancho_v, fila * largo_v, MONEDA, 2000);

                mapa[fila][col] = '.';
            }
        }
    }
}

bool interaccion_item(item* i, personaje* p) {

    if (i->x + i->ancho < p->x) {
        return false;
    }

    if (i->x > p->x + p->ancho) {
        return false;
    }

    if (i->y + i->alto < p->y) {
        return false;
    }

    if (i->y > p->y + p->alto) {
        return false;
    }
    return true;
}

void actualizar_items(item items[], personaje* p) {
    int i;

    for (i = 0; i < max_items; i++) {

        if (items[i].activo == true) {
            actualizar_animacion_item(&items[i]);
            mover_item(&items[i]);
            if (interaccion_item(&items[i], p)) {

                if (items[i].tipo == VIDA) {
                    p->vida += items[i].valor;

                    if (p->vida > p->vida_max) {
                        p->vida = p->vida_max;
                    }

                    items[i].activo = false;
                }

                if (items[i].tipo == MUNICION) {
                    p->municion += items[i].valor;

                    items[i].activo = false;
                }

                if (items[i].tipo == LLAVE) {
                    p->agarro_llave = true;
                    items[i].activo = false;
                }

                if (items[i].tipo == ESCUDO) {
                    p->escudo += items[i].valor;

                    if (p->escudo > p->escudo_max) {
                        p->escudo = p->escudo_max;
                    }

                    items[i].activo = false;
                }
                if (items[i].tipo == MONEDA) {
                    p->puntaje += items[i].valor;

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
        ALLEGRO_BITMAP* sprite = frames_vida[i->frame_actual];

        if (sprite != NULL) {
            al_draw_scaled_bitmap(
                sprite,
                0,
                0,
                al_get_bitmap_width(sprite),
                al_get_bitmap_height(sprite),
                i->x - camara_x,
                i->y - camara_y,
                i->ancho,
                i->alto,
                0
            );
        }
    }
    if (i->tipo == MUNICION) {
        if (sprite_municion != NULL) {
            al_draw_scaled_bitmap(
                sprite_municion,
                0,
                0,
                al_get_bitmap_width(sprite_municion),
                al_get_bitmap_height(sprite_municion),
                i->x - camara_x,
                i->y - camara_y,
                i->ancho,
                i->alto,
                0
            );
        }
    }
    if (i->tipo == BARRIL) {
        if (sprite_barril != NULL) {
            al_draw_scaled_bitmap(
                sprite_barril,
                0,
                0,
                al_get_bitmap_width(sprite_barril),
                al_get_bitmap_height(sprite_barril),
                i->x - camara_x,
                i->y - camara_y,
                i->ancho,
                i->alto,
                0
            );
        }

    }
    if (i->tipo == LLAVE) {
        ALLEGRO_BITMAP* sprite = frames_llave[i->frame_actual];

        if (sprite != NULL) {
            al_draw_scaled_bitmap(
                sprite,
                0,
                0,
                al_get_bitmap_width(sprite),
                al_get_bitmap_height(sprite),
                i->x - camara_x,
                i->y - camara_y,
                i->ancho,
                i->alto,
                0
            );
        }
    }
    if (i->tipo == ESCUDO) {
        if (sprite_escudo != NULL) {
            al_draw_scaled_bitmap(
                sprite_escudo,
                0,
                0,
                al_get_bitmap_width(sprite_escudo),
                al_get_bitmap_height(sprite_escudo),
                i->x - camara_x,
                i->y - camara_y,
                i->ancho,
                i->alto,
                0
            );
        }
    }
    if (i->tipo == MONEDA) {
        al_draw_filled_rectangle(
            i->x - camara_x,
            i->y - camara_y,
            i->x - camara_x + i->ancho,
            i->y - camara_y + i->alto,
            al_map_rgb(255, 220, 0)
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

    if (b->x + b->ancho < i->x) {
        return false;
    }

    if (b->x > i->x + i->ancho) {
        return false;
    }

    if (b->y + b->alto < i->y) {
        return false;
    }

    if (b->y > i->y + i->alto) {
        return false;
    }

    return true;

}

void explosion_barril(item* barril, enemigo enemigos[], personaje* p) {
    int i;

    float rango_explosion = 300.0f;

    float explosion_izq = barril->x - rango_explosion;
    float explosion_der = barril->x + barril->ancho + rango_explosion;
    float explosion_arriba = barril->y - rango_explosion;
    float barril_abajo = barril->y + barril->alto;

    for (i = 0; i < max_enemigos; i++) {

        if (enemigos[i].activo == true) {
            if (enemigos[i].x < explosion_der &&
                enemigos[i].x + enemigos[i].ancho > explosion_izq &&
                enemigos[i].y < barril_abajo &&
                enemigos[i].y + enemigos[i].alto > explosion_arriba) {

                enemigos[i].vida -= barril->valor;

                if (enemigos[i].vida <= 0) {
                    enemigos[i].vida = 0;
                    p->puntaje += enemigos[i].puntos;
                    enemigos[i].activo = false;
                }
            }
        }
    }
    barril->activo = false;
}

void colision_bala_barril(item items[], bala balas[], enemigo enemigos[], personaje* p) {
    int i;
    int j;

    for (i = 0; i < max_items; i++) {

        if (items[i].activo == true && items[i].tipo == BARRIL) {

            for (j = 0; j < max_balas_p; j++) {

                if (balas[j].activa == true) {

                    if (colision_bala_item(&balas[j], &items[i])) {
                        balas[j].activa = false;
                        explosion_barril(&items[i], enemigos, p);
                        break;
                    }
                }
            }
        }
    }
}

bool cargar_sprites_items(void) {
    frames_vida[0] = al_load_bitmap("assets/items/item_vida1.png");
    frames_vida[1] = al_load_bitmap("assets/items/item_vida2.png");
    frames_vida[2] = al_load_bitmap("assets/items/item_vida3.png");
    frames_vida[3] = al_load_bitmap("assets/items/item_vida4.png");
    frames_vida[4] = al_load_bitmap("assets/items/item_vida5.png");
    frames_vida[5] = al_load_bitmap("assets/items/item_vida6.png");

    for (int i = 0; i < FRAMES_VIDA; i++) {
        if (frames_vida[i] == NULL) {
            printf("No se pudo cargar un sprite de vida\n");
            return false;
        }
    }

    sprite_municion = al_load_bitmap("assets/items/municion.png");

    if (sprite_municion == NULL) {
        printf("No se pudo cargar el sprite de municion\n");
        return false;
    }

    sprite_escudo = al_load_bitmap("assets/items/escudo.png");

    if (sprite_escudo == NULL) {
        printf("No se pudo cargar el sprite de escudo\n");
        return false;
    }

    sprite_barril = al_load_bitmap("assets/items/barril.png");

    if (sprite_barril == NULL) {
        printf("No se pudo cargar el sprite de barril\n");
        return false;
    }

    frames_llave[0] = al_load_bitmap("assets/items/llave1.png");
    frames_llave[1] = al_load_bitmap("assets/items/llave2.png");
    frames_llave[2] = al_load_bitmap("assets/items/llave3.png");
    frames_llave[3] = al_load_bitmap("assets/items/llave4.png");
    frames_llave[4] = al_load_bitmap("assets/items/llave5.png");
    frames_llave[5] = al_load_bitmap("assets/items/llave6.png");

    for (int i = 0; i < FRAMES_LLAVE; i++) {
        if (frames_llave[i] == NULL) {
            printf("No se pudo cargar un sprite de llave\n");
            return false;
        }
    }
    return true;
}

void liberar_sprites_items(void) {
    for (int i = 0; i < FRAMES_VIDA; i++) {
        if (frames_vida[i] != NULL) {
            al_destroy_bitmap(frames_vida[i]);
            frames_vida[i] = NULL;
        }
    }

    if (sprite_municion != NULL) {
        al_destroy_bitmap(sprite_municion);
        sprite_municion = NULL;
    }

    if (sprite_escudo != NULL) {
        al_destroy_bitmap(sprite_escudo);
        sprite_escudo = NULL;
    }
    if (sprite_barril != NULL) {
        al_destroy_bitmap(sprite_barril);
        sprite_barril = NULL;
    }
    for (int i = 0; i < FRAMES_LLAVE; i++) {
        if (frames_llave[i] != NULL) {
            al_destroy_bitmap(frames_llave[i]);
            frames_llave[i] = NULL;
        }
    }
}

void actualizar_animacion_item(item* i) {
    if (i->tipo == VIDA) {
        i->contador_animacion++;

        if (i->contador_animacion >= CAMBIO_FRAME_VIDA) {
            i->contador_animacion = 0;
            i->frame_actual++;

            if (i->frame_actual >= FRAMES_VIDA) {
                i->frame_actual = 0;
            }
        }
    }

    else if (i->tipo == LLAVE) {
        i->contador_animacion++;

        if (i->contador_animacion >= CAMBIO_FRAME_LLAVE) {
            i->contador_animacion = 0;
            i->frame_actual++;

            if (i->frame_actual >= FRAMES_LLAVE) {
                i->frame_actual = 0;
            }
        }
    }
}

void mover_item(item* i) {

    if (i->activo == false) {
        return;
    }

    if (i->tipo == BARRIL) {
        return;
    }

    i->y += i->velocidad_y * i->direccion_y;

    if (i->y >= i->y_inicio + i->distancia) {
        i->direccion_y = -1;
    }

    if (i->y <= i->y_inicio) {
        i->direccion_y = 1;
    }
}