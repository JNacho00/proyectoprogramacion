#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <stdbool.h>


// estructuras---------------------------

typedef struct {
    float x, y;
    float tamano;
    float velocidad;
} personaje;

typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
} flujo_juego;

//incializar funciones-----------------------------------

bool inicializar_sistema(flujo_juego* j);
void cerrar_sistema(flujo_juego* j);
void movimiento(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado);
void fisicas(personaje* p);
void dibujo_personaje(personaje* p);

//funciones----------------------------

bool inicializar_sistema(flujo_juego* j) {
    if (!al_init()) return false;

    al_init_image_addon();
    al_init_font_addon();
    al_install_keyboard();
    al_init_primitives_addon();

    j->display = al_create_display(800,600);
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

void movimiento(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado) {
    if (al_key_down(estado_teclado, ALLEGRO_KEY_A)) { // [cite: 186]
        p->x -= p->velocidad;
    }
    if (al_key_down(estado_teclado, ALLEGRO_KEY_D)) { // [cite: 186]
        p->x += p->velocidad;
    }
    if (al_key_down(estado_teclado, ALLEGRO_KEY_W)) { // [cite: 186]
        p->y -= p->velocidad;
    }
    if (al_key_down(estado_teclado, ALLEGRO_KEY_S)) { // [cite: 186]
        p->y += p->velocidad;
    }
}
//----------aca ira la gravedad y las colisiones-----
void fisicas(personaje* p) {
    ALLEGRO_KEYBOARD_STATE key_state;
    al_get_keyboard_state(&key_state); 
        movimiento(p, &key_state);
}


void dibujo_personaje(personaje *p) {
    al_clear_to_color(al_map_rgb(0, 0, 0)); 

        // Dibujamos al cuadrado
     al_draw_filled_rectangle(p->x, p->y,
        p->x + p->tamano, p->y + p->tamano,
        al_map_rgb(255, 0, 0)); 

     al_flip_display(); 
}

// -----------main--------------------
int main() {
    flujo_juego mijuego;

    if (!inicializar_sistema(&mijuego)) {
        fprintf(stderr, "Error");
        return -1;
    }

    // Inicializamos a nuestro protagonista
    personaje jugador = { 375, 275, 50, 5.0 }; // x, y, tamano, velocidad

    bool corriendo = true;
    bool redibujar = true;

    while (corriendo) {
        ALLEGRO_EVENT event;
        al_wait_for_event(mijuego.queue, &event); // [cite: 91]

        // Detectar si cerramos la ventana
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) { // [cite: 84]
            corriendo = false;
        }
        else if (event.type == ALLEGRO_EVENT_TIMER) { // [cite: 83]

            fisicas(&jugador);
            redibujar = true;
        }

        if (redibujar && al_is_event_queue_empty(mijuego.queue)) {
            dibujo_personaje(&jugador);
            redibujar = false;
        }
    }
    cerrar_sistema(&mijuego);


    return 0;
}