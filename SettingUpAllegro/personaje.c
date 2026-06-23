#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#include "mapa.h"


void dibujo_personaje(personaje* p, float camara) {

    // Dibujamos al cuadrado
    al_draw_filled_rectangle(p->x,
                             p->y - camara,
                             p->x + p->tamano,
                             p->y + p->tamano - camara,
                             al_map_rgb(255, 0, 0));

}

void movimiento(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado) {
    if (al_key_down(estado_teclado, ALLEGRO_KEY_A)) { 
        p->x -= p->velocidadx;
    }
    if (al_key_down(estado_teclado, ALLEGRO_KEY_D)) { 
        p->x += p->velocidadx;
    }
    if (al_key_down(estado_teclado, ALLEGRO_KEY_W)) {
        p->y -= p->velocidady;
    } 
    if (al_key_down(estado_teclado, ALLEGRO_KEY_S)) { 
        p->y += p->velocidady;
    }
}

void fisicas(personaje* p) {
    ALLEGRO_KEYBOARD_STATE key_state;

    int izq;
    int der;
    int fila;

    p->velocidady += gravedad; // cuando cambie velocidad y de p* va aumentando la gravedad
    p->y += p->velocidady; // va actualizando la poscicion y


    izq = fisicas_mapa((int)(p->x + 2), (int)(p->y + p->tamano)); // se le agrega int para pasar un valor limpio a las fisicas
    der = fisicas_mapa((int)(p->x + p->tamano - 2), (int)(p->y + p->tamano));

    if (izq || der) {
        p->velocidady = salto;
  
        fila = (int)(p->y + p->tamano) / largo_v;
        p->y = fila * largo_v - p->tamano;
    }

    al_get_keyboard_state(&key_state);
    movimiento(p, &key_state);

    if (p->x > 800) p->x = -p->tamano; // Si sale por la derecha
 
    if (p->x + p->tamano < 0) p->x = 780;   // Si sale por la izquierda 
}

