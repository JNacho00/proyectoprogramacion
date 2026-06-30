#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#define spawn_personajex 'o'


void dibujo_personaje(personaje* p, float camara) {

    // Dibujamos al cuadrado
    al_draw_filled_rectangle(
        p->x,
        p->y - camara,
        p->x + p->ancho,
        p->y + p->alto - camara,
        al_map_rgb(255, 0, 0));

}

void spawn_personaje(personaje* p) {
    int f;
    int c;

    for (f = 0; f < mapa_filas; f++) {
        for (c = 0; c < mapa_col; c++) {
            if (mapa[f][c] == spawn_personajex) {
                p->x = c * ancho_v;
                p->y = f * largo_v;
                p->en_suelo = false;
                p->ancho = 40;  
                p->alto = 40;
                p->velocidadx = 5.0;
                p->velocidady = 5.0;
                p->vida = 100;

                mapa[f][c] = '.'; 

                return; 
            }
        }
    }
}

void movimiento(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado) {
    if (al_key_down(estado_teclado, ALLEGRO_KEY_A)) {
        p->x -= p->velocidadx;
    }
    if (al_key_down(estado_teclado, ALLEGRO_KEY_D)) {
        p->x += p->velocidadx;
    }

}
void saltar(personaje* p) {
    if (p->en_suelo == true) {
        p->velocidady = salto;
        p->en_suelo = false;
    }
}
void fisicas(personaje* p) {
    ALLEGRO_KEYBOARD_STATE key_state;

    int izq;
    int der;
    int fila;

    p->velocidady += gravedad; // cuando cambie velocidad y de p* va aumentando la gravedad
    p->y += p->velocidady; // va actualizando la poscicion y

    izq = fisicas_mapa((int)(p->x + 2), (int)(p->y + p->alto)); // se le agrega int para pasar un valor limpio a las fisicas
    der = fisicas_mapa((int)(p->x + p->ancho - 2), (int)(p->y + p->alto));

    if (izq || der) {
        p->velocidady = 0;
        p->en_suelo = true;

        fila = (int)(p->y + p->alto) / largo_v;
        p->y = fila * largo_v - p->alto;
    }

    al_get_keyboard_state(&key_state);
    movimiento(p, &key_state);
    direccion(p, &key_state);

    if (p->x > 800) p->x = -p->ancho; // Si sale por la derecha

    if (p->x + p->ancho < 0) p->x = 800;   // Si sale por la izquierda 
}

void direccion(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado) {
    int dx = 0;
    int dy = 0;

    if (al_key_down(estado_teclado, ALLEGRO_KEY_LEFT)) {
        dx = -1;
    }

    if (al_key_down(estado_teclado, ALLEGRO_KEY_RIGHT)) {
        dx = 1;
    }

    if (al_key_down(estado_teclado, ALLEGRO_KEY_UP)) {
        dy = -1;
    }

    if (al_key_down(estado_teclado, ALLEGRO_KEY_DOWN)) {
        dy = 1;
    }

    if (dx != 0 || dy != 0) {
        p->direccionx = dx;
        p->direcciony = dy;
    }
}

void disparar(personaje* p) {
 
    float vx;
    float vy;
    float posx, posy;
    
    vx = p->direccionx * velocidad_bala;
    vy = p->direcciony * velocidad_bala;
    
    posx = p->x + (p->ancho / 2);
    posy = p->y + (p->alto / 2);

    crear_bala(posx, posy, vx, vy);

}