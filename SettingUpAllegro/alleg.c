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


// -----------main--------------------
int main() {
    
    flujo_juego mijuego;

    
    if (!inicializar_sistema(&mijuego)) {
        fprintf(stderr, "Error");
        return -1;
    }

    if (!cargar_sprites_mapa()) {
        return -1;
    }

    if (!cargar_sprites_personaje()) {
        liberar_sprites_mapa();
        cerrar_sistema(&mijuego);
        return -1;
    }

    if (cargar_sprites_enemigos() == false) {
        return -1;
    }

    cargar_mapa("mapa.txt");

   

    personaje jugador;
    //enemigo enemigos[max_enemigos]; 
    spawn_personaje(&jugador);
    spawn_balas(jugador.balas);
    spawn_enemigos();
    float camara_x = 0.0f;
    float camara_y = 0.0f;

    //personaje jugador = { 375, (mapa_filas * largo_v) - 100.0f, 30, 5.0, 6.0 };

    bool corriendo = true;
    bool redibujar = true;

    while (corriendo) {
        ALLEGRO_EVENT event;
        al_wait_for_event(mijuego.queue, &event);

        // Detectar si cerramos la ventana
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            corriendo = false;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) {

            fisicas(&jugador);
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
            actualizar_ataques_enemigos(&jugador);
            fisicas_enemigos();
            fisicas_balas(jugador.balas);
            revisar_colisione_bala_enemigo();
            redibujar = true;
        }
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            event.keyboard.keycode;
            if (event.keyboard.keycode == ALLEGRO_KEY_LSHIFT) {
                disparar(&jugador);
            }
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                saltar(&jugador);
            }
        }

        if (redibujar && al_is_event_queue_empty(mijuego.queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));

            dibujar_fondo(camara_x);

            dibujar_mapa(camara_x, camara_y);
            dibujar_enemigos_mapa(camara_x, camara_y);   
            dibujar_balas_mapa(jugador.balas,camara_x, camara_y);
            dibujo_personaje(&jugador, camara_x, camara_y);
            dibujar_barra_vida(&jugador);

            
            al_flip_display();
            redibujar = false;
        }
    }
    liberar_sprites_mapa();
    liberar_sprites_personaje();
    liberar_sprites_enemigos();
    cerrar_sistema(&mijuego);

    return 0;
}



//funciones----------------------------

bool inicializar_sistema(flujo_juego* j) {
    if (!al_init()) return false;

    al_init_image_addon();
    al_init_font_addon();
    al_install_keyboard();
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

    al_start_timer(j->timer);
    return true;
}

void cerrar_sistema(flujo_juego* j) {
    al_destroy_timer(j->timer);
    al_destroy_event_queue(j->queue);
    al_destroy_display(j->display);
}



/////////////CAMBIAAR FUNCION/////////////////
int cargar_mapa(const char* ruta) {
    int cont;
    int i;
    char linea[columnas + 2];

    FILE* f = fopen(ruta, "r");

    if (!f) {
        fprintf(stderr, "Error: %s\n", ruta);
        return 0;
    }

    mapa_filas = 0;
    mapa_col = 0;

    while (fgets(linea, sizeof(linea), f) != NULL && mapa_filas < filas) { // lee todo el contenido de las filas
        cont = strlen(linea);
        if (cont > 0 && linea[cont - 1] == '\n') { // limpia '\n'
            linea[--cont] = '\0';
        }
        for (i = 0; i < cont && i < columnas; i++) {
            mapa[mapa_filas][i] = linea[i];
        }

        mapa[mapa_filas][columnas] = '\0';

        if (cont > mapa_col)
            mapa_col = cont;

        mapa_filas++;

    }

    fclose(f);
    return 1;
}
