#include <allegro5\allegro5.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<stdio.h>

int main() {
    // 1. Inicialización básica
    if (!al_init()) {
        fprintf(stderr, "Error: No se pudo inicializar Allegro.\n");
        return -1;
    }

    // 2. Crear la ventana
    ALLEGRO_DISPLAY* display = al_create_display(800, 400);
    if (!display) {
        fprintf(stderr, "Error: No se pudo crear la ventana.\n");
        return -1;
    }

    al_set_window_title(display, "Mi Ventana ");

    // 3. Crear la cola de eventos
    ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
    if (!queue) {
        fprintf(stderr, "Error: No se pudo crear la cola de eventos.\n");
        al_destroy_display(display);
        return -1;
    }

    // Registrar la ventana en la cola
    al_register_event_source(queue, al_get_display_event_source(display));

    // 4. Pintar el fondo negro y mostrarlo
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    // 5. Bucle Principal
    bool ejecutando = true;
    while (ejecutando) {
        ALLEGRO_EVENT event;
        al_wait_for_event(queue, &event);

        // Detectar clic en la 'X'
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            ejecutando = false;
        }
    }

    // 6. Limpiar memoria al salir
    al_destroy_event_queue(queue);
    al_destroy_display(display);

    return 0;
}