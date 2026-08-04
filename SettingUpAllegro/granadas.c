#include "granadas.h"
#include "mapa.h"

void dibujar_granada(granada* g, float camara_x, float camara_y) {

    if (g->activa == false) {
        return;
    }

    al_draw_filled_circle(
        g->x - camara_x + g->ancho / 2.0f,
        g->y - camara_y + g->alto / 2.0f,
        g->ancho / 2.0f,
        al_map_rgb(80, 80, 80)
    );

    al_draw_circle(
        g->x - camara_x + g->ancho / 2.0f,
        g->y - camara_y + g->alto / 2.0f,
        g->ancho / 2.0f,
        al_map_rgb(255, 255, 255),
        2
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
        granadas[i].rango_explosion = 40;

        granadas[i].frame_actual = 0;
        granadas[i].contador_animacion = 0;
    }
}

void dibujar_granadas_mapa(granada granadas[], float camara_x, float camara_y) {
    int i;

    for (i = 0; i < max_granadas_p; i++) {
        if (granadas[i].activa == true) {
            dibujar_granada(&granadas[i], camara_x, camara_y);
        }
    }
}

void fisicas_granadas(granada granadas[]) {
    int i;

    for (i = 0; i < max_granadas_p; i++) {
        if (granadas[i].activa == true) {

            float nueva_x;
            float nueva_y;

            granadas[i].velocidad_gy += gravedad;

            nueva_x = granadas[i].x + granadas[i].velocidad_gx;
            nueva_y = granadas[i].y + granadas[i].velocidad_gy;

            if (fisicas_mapa((int)nueva_x, (int)nueva_y) ||
                fisicas_mapa((int)(nueva_x + granadas[i].ancho), (int)nueva_y) ||
                fisicas_mapa((int)nueva_x, (int)(nueva_y + granadas[i].alto)) ||
                fisicas_mapa((int)(nueva_x + granadas[i].ancho), (int)(nueva_y + granadas[i].alto))) {

                granadas[i].x = nueva_x;
                granadas[i].y = nueva_y;
                granadas[i].explotar = true;

            }
            else {
                granadas[i].x = nueva_x;
                granadas[i].y = nueva_y;
            }
        }
    }
}