#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "personaje.h"
#include "mapa.h"


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

    cargar_mapa("mapa.txt");
  float camara = (mapa_filas * largo_v) - 600.0f;

    // Inicializamos a nuestro protagonista
    //personaje jugador = { 375, 800, 50, 5.0, 6.0 }; // x, y, tamano, velocidad
   
  personaje jugador = { 375, (mapa_filas * largo_v) - 100.0f, 30, 5.0, 6.0 };

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
            if (jugador.y < camara + 100.0f) {  
                camara = jugador.y - 100.0f;
            }

            redibujar = true;
        }

        if (redibujar && al_is_event_queue_empty(mijuego.queue)) {
            al_clear_to_color(al_map_rgb(0, 0, 0));
            dibujar_mapa(camara);
            dibujo_personaje(&jugador, camara);
            al_flip_display();
            redibujar = false;
        }
    }
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

    j->display = al_create_display(800, 600);
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

int cargar_mapa(const char* ruta) {
    int cont;
    int i;
    char linea[columnas + 2];

    FILE* f = fopen(ruta, "r");

    if (!f) {
        fprintf(stderr, "Error: %s\n", ruta);
        return 0;
    }

    while (fgets(linea, sizeof(linea), f) != NULL && mapa_filas < filas) { // lee todo el contenido de las filas
        cont = strlen(linea);
        if (cont > 0 && linea[cont - 1] == '\n') { // limpia '\n'
            linea[--cont] = '\0';
        }
        for (i = 0; i <= cont && i < columnas; i++) {
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
