#define _CRT_SECURE_NO_WARNINGS
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "personaje.h"
#include "mapa.h"
#include "balas.h"
#include "enemigos.h"
#include "items.h"

// estructuras---------------------------

typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
} flujo_juego;

//incializar funciones-----------------------------------

bool inicializar_sistema(flujo_juego* j);
void cerrar_sistema(flujo_juego* j);
int cargar_mapa(const char* ruta);
void cargar_nivel(int nivel, personaje* p, enemigo enemigos[], item items[]);


// -----------main--------------------
int main() {
    
    flujo_juego mijuego;

    
    if (inicializar_sistema(&mijuego) == false) {
        fprintf(stderr, "Error");
        return -1;
    }

    if (cargar_sprites_mapa() == false) {
        return -1;
    }

    if (cargar_sprites_personaje() == false) {
        liberar_sprites_mapa();
        cerrar_sistema(&mijuego);
        return -1;
    }

    if (cargar_sprites_enemigos() == false) {
        return -1;
    }

    if (cargar_sprites_balas() == false) {
        return -1;
    }


    personaje jugador;
    item items[max_items];
    enemigo enemigos[max_enemigos]; 
 

    int nivel_actual = 1;

    cargar_nivel(nivel_actual, &jugador, enemigos, items);

    float camara_x = 0.0f;
    float camara_y = 0.0f;

    bool corriendo = true;
    bool redibujar = true;

    while (corriendo) {
        ALLEGRO_EVENT event;
        al_wait_for_event(mijuego.queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            corriendo = false;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {

            fisicas(&jugador);

            if (portal(&jugador)) {
                nivel_actual++;

                cargar_nivel(nivel_actual, &jugador, enemigos, items);

                camara_x = 0.0f;
                camara_y = 0.0f;
            }

            float objetivo_x = jugador.x - 500.0f;

            if (objetivo_x > camara_x) {
                camara_x = objetivo_x;
            }

            camara_y = jugador.y - 520.0f;

            if (camara_x < 0) {
                camara_x = 0;
            }

            if (camara_y < 0) {
                camara_y = 0;
            }
            actualizar_items(items, &jugador);
            actualizar_ataques_enemigos(&jugador, enemigos);
            fisicas_enemigos(enemigos, &jugador);
            fisicas_balas(jugador.balas);
            colision_bala_barril(items, jugador.balas, enemigos);
            revisar_colisiones_bala_enemigo(enemigos, jugador.balas);
            redibujar = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            event.keyboard.keycode;
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                saltar(&jugador);
            }
        }
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (event.mouse.button == 1) {
                disparo_mouse(&jugador, event.mouse.x, event.mouse.y, camara_x, camara_y);
            }
        }

        if (redibujar && al_is_event_queue_empty(mijuego.queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            dibujar_fondo(camara_x);

            dibujar_mapa(camara_x, camara_y);
            dibujar_items(items, camara_x, camara_y);
            dibujar_enemigos_mapa(enemigos, camara_x, camara_y);   
            dibujar_balas_mapa(jugador.balas, camara_x, camara_y);
            dibujo_personaje(&jugador, camara_x, camara_y);
            dibujar_barra_vida(&jugador);

            
            al_flip_display();
            redibujar = false;
        }
    }
    liberar_sprites_mapa();
    liberar_sprites_personaje();
    liberar_sprites_enemigos();
    liberar_sprites_balas();
    cerrar_sistema(&mijuego);

    return 0;
}



//funciones----------------------------

bool inicializar_sistema(flujo_juego* j) {
    if (!al_init()) return false;

    al_init_image_addon();
    al_init_font_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();

    j->display = al_create_display(1200, 800);
    if (!j->display) return false;

    j->timer = al_create_timer(1.0 / 60.0);
    j->queue = al_create_event_queue();
    if (!j->queue) {
        al_destroy_display(j->display);
        return false;
    }

    al_register_event_source(j->queue, al_get_display_event_source(j->display));
    al_register_event_source(j->queue, al_get_timer_event_source(j->timer));
    al_register_event_source(j->queue, al_get_keyboard_event_source());
    al_register_event_source(j->queue, al_get_mouse_event_source());

    al_start_timer(j->timer);
    return true;
}

void cerrar_sistema(flujo_juego* j) {
    al_destroy_timer(j->timer);
    al_destroy_event_queue(j->queue);
    al_destroy_display(j->display);
}

int cargar_mapa(const char* ruta) {
    FILE* f;
    int fila;
    int col;

    f = fopen(ruta, "r");

    if (f == NULL) {
        printf("No se pudo abrir el archivo: %s\n", ruta);
        return 0;
    }

    for (fila = 0; fila < filas; fila++) {
        for (col = 0; col < columnas; col++) {
            fscanf(f, " %c", &mapa[fila][col]);
        }

        mapa[fila][columnas] = '\0';
    }

    mapa_filas = filas;
    mapa_col = columnas;

    fclose(f);
    return 1;
}

void cargar_nivel(int nivel, personaje* p, enemigo enemigos[], item items[]) {
    char ruta[15];

    mapas(ruta, nivel);

    cargar_mapa(ruta);

    spawn_personaje(p);

    spawn_balas(p->balas);

    inicializar_enemigos(enemigos);
    spawn_enemigos(enemigos);

    inicializar_items(items);
    spawn_items(items);

}
