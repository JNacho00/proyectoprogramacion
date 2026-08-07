#include "granadas.h"
#include "mapa.h"
#define FRAMES_EXPLOSION_GRANADA 3
#define CAMBIO_FRAME_EXPLOSION_GRANADA 5

ALLEGRO_BITMAP* sprites_explosion_granada[FRAMES_EXPLOSION_GRANADA];
ALLEGRO_BITMAP* sprite_granada = NULL;

void dibujar_granada(granada* g, float camara_x, float camara_y) {
    if (g->activa == false) {
        return;
    }

    if (sprite_granada == NULL) {
        return;
    }

    al_draw_scaled_bitmap(
        sprite_granada,
        0,
        0,
        al_get_bitmap_width(sprite_granada),
        al_get_bitmap_height(sprite_granada),
        g->x - camara_x,
        g->y - camara_y,
        g->ancho,
        g->alto,
        0
    );
}

void spawn_granadas(granada granadas[]) {
    int i;

    for (i = 0; i < max_granadas_p; i++) {
        granadas[i].x = 0;
        granadas[i].y = 0;

        granadas[i].ancho = 20;
        granadas[i].alto = 20;

        granadas[i].velocidad_gx = 0;
        granadas[i].velocidad_gy = 0;

        granadas[i].activa = false;
        granadas[i].explotar = false;
        granadas[i].frame_explosion = 0;
        granadas[i].contador_explosion = 0;
        granadas[i].rango_explosion = 40;

        granadas[i].frame_actual = 0;
        granadas[i].contador_animacion = 0;
    }
}

void dibujar_granadas_mapa(granada granadas[], float camara_x, float camara_y) {
    int i;

    for (i = 0; i < max_granadas_p; i++) {
        if (granadas[i].activa == true) {

            if (granadas[i].explotar == true) {
                dibujar_explosion_granada(&granadas[i], camara_x, camara_y);
            }
            else {
                dibujar_granada(&granadas[i], camara_x, camara_y);
            }
        }
    }
}

void fisicas_granadas(granada granadas[], item items[]) {
    int i;

    for (i = 0; i < max_granadas_p; i++) {

        if (granadas[i].activa == true) {

            if (granadas[i].explotar == true) {
                actualizar_explosion_granada(&granadas[i]);
                continue;
            }

            float nueva_x;
            float nueva_y;
            float centro_x;
            float centro_y;

            granadas[i].velocidad_gy += gravedad;

            nueva_x = granadas[i].x + granadas[i].velocidad_gx;
            nueva_y = granadas[i].y + granadas[i].velocidad_gy;

            if (fisicas_mapa((int)nueva_x, (int)nueva_y) ||
                fisicas_mapa((int)(nueva_x + granadas[i].ancho), (int)nueva_y) ||
                fisicas_mapa((int)nueva_x, (int)(nueva_y + granadas[i].alto)) ||
                fisicas_mapa((int)(nueva_x + granadas[i].ancho), (int)(nueva_y + granadas[i].alto))) {

                centro_x = granadas[i].x + granadas[i].ancho / 2.0f;
                centro_y = granadas[i].y + granadas[i].alto / 2.0f;

                romper_cajas_granada(
                    centro_x,
                    centro_y,
                    granadas[i].rango_explosion,
                    items
                );

                granadas[i].explotar = true;
                granadas[i].frame_explosion = 0;
                granadas[i].contador_explosion = 0;

                granadas[i].velocidad_gx = 0;
                granadas[i].velocidad_gy = 0;
            }
            else {
                granadas[i].x = nueva_x;
                granadas[i].y = nueva_y;
            }
        }
    }
}

bool cargar_sprites_granadas(void) {
    sprites_explosion_granada[0] = al_load_bitmap("assets/explosiones/explosion_granada1.png");
    sprites_explosion_granada[1] = al_load_bitmap("assets/explosiones/explosion_granada2.png");
    sprites_explosion_granada[2] = al_load_bitmap("assets/explosiones/explosion_granada3.png");

    for (int i = 0; i < FRAMES_EXPLOSION_GRANADA; i++) {
        if (sprites_explosion_granada[i] == NULL) {
            printf("No se pudo cargar un sprite de explosion de granada\n");
            return false;
        }
    }

    sprite_granada = al_load_bitmap("assets/armas&balas/granada.png");

    if (sprite_granada == NULL) {
        printf("No se pudo cargar el sprite de la granada\n");
        return false;
    }

    return true;
}

void dibujar_explosion_granada(granada* g, float camara_x, float camara_y) {
    ALLEGRO_BITMAP* sprite;
    float centro_x;
    float centro_y;
    float ancho;
    float alto;

    if (g->explotar == false) {
        return;
    }

    sprite = sprites_explosion_granada[g->frame_explosion];

    if (sprite == NULL) {
        return;
    }

    ancho = al_get_bitmap_width(sprite);
    alto = al_get_bitmap_height(sprite);

    centro_x = g->x + g->ancho / 2.0f;
    centro_y = g->y + g->alto / 2.0f;

    al_draw_bitmap(
        sprite,
        centro_x - ancho / 2.0f - camara_x,
        centro_y - alto / 2.0f - camara_y - 25,
        0
    );
}

void actualizar_explosion_granada(granada* g) {
    if (g->explotar == false) {
        return;
    }

    g->contador_explosion++;

    if (g->contador_explosion >= CAMBIO_FRAME_EXPLOSION_GRANADA) {
        g->contador_explosion = 0;
        g->frame_explosion++;

        if (g->frame_explosion >= FRAMES_EXPLOSION_GRANADA) {
            g->explotar = false;
            g->activa = false;
            g->frame_explosion = 0;
        }
    }
}

void liberar_sprites_granadas(void) {
    int i;

    for (i = 0; i < FRAMES_EXPLOSION_GRANADA; i++) {
        if (sprites_explosion_granada[i] != NULL) {
            al_destroy_bitmap(sprites_explosion_granada[i]);
            sprites_explosion_granada[i] = NULL;
        }
    }

    if (sprite_granada != NULL) {
        al_destroy_bitmap(sprite_granada);
        sprite_granada = NULL;
    }
}