#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#define spawn_personajex 'o'
#define t_invulnerable 60

#define FRAMES_IDLE 4
#define FRAMES_CORRER 4

#define CAMBIO_FRAME_IDLE 12
#define CAMBIO_FRAME_CORRER 6

#define FRAMES_SALTAR 5
#define CAMBIO_FRAME_SALTAR 20

static ALLEGRO_BITMAP* frames_idle[FRAMES_IDLE] = {
    NULL, NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_correr[FRAMES_CORRER] = {
    NULL, NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_saltar[FRAMES_SALTAR] = {
    NULL, NULL, NULL, NULL, NULL
};

void dibujo_personaje(personaje* p, float camara_x, float camara_y) {
    ALLEGRO_BITMAP* sprite = NULL;
    int flags = 0;

    if (p->animacion == ANIM_CORRER) {
        sprite = frames_correr[p->frame_actual];
    }
    else if (p->animacion == ANIM_SALTAR) {
        sprite = frames_saltar[p->frame_actual];
    }
    else {
        sprite = frames_idle[p->frame_actual];
    }

    if (p->mira_derecha == false) {
        flags = ALLEGRO_FLIP_HORIZONTAL;
    }

    if (sprite != NULL) {
        al_draw_bitmap(
            sprite,
            p->x - camara_x,
            p->y - camara_y,
            flags
        );
    }
}

void dibujar_barra_vida(personaje* p) {
    float porcentaje;
    float ancho_vida;

    int x = 20;
    int y = 20;
    int ancho_barra = 220;
    int alto_barra = 22;

    if (p->vida_max <= 0) {
        return;
    }

    porcentaje = (float)p->vida / (float)p->vida_max;

    if (porcentaje < 0.0f) {
        porcentaje = 0.0f;
    }

    if (porcentaje > 1.0f) {
        porcentaje = 1.0f;
    }

    ancho_vida = ancho_barra * porcentaje;

    al_draw_filled_rectangle(x,y,x + ancho_barra,y + alto_barra,al_map_rgb(90, 90, 90));

    al_draw_filled_rectangle(x,y,x + ancho_vida,y + alto_barra,al_map_rgb(0, 200, 0));

    al_draw_rectangle(x,y,x + ancho_barra,y + alto_barra,al_map_rgb(255, 255, 255),2);
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
                p->velocidadx = 3.0;
                p->velocidady = 3.0;
                p->vida_max = 10;
                p->vida = p->vida_max;
                p->municion = 20;

                p->frame_actual = 0;
                p->contador_animacion = 0;

                p->animacion = ANIM_IDLE;
                p->frame_actual = 0;
                p->contador_animacion = 0;
                p->mira_derecha = true;

                mapa[f][c] = '.'; 

                return; 
            }
        }
    }
}

void recibir_dano_personaje(personaje* p, int dano_recibido) {
    if (dano_recibido <= 0) {
        return;
    }

    if (p->vida <= 0) {
        return;
    }

    if (p->invulnerable > 0) {
        return;
    }

    p->vida -= dano_recibido;

    if (p->vida < 0) {
        p->vida = 0;
    }

    p->invulnerable = t_invulnerable;
}

void movimiento(personaje* p, ALLEGRO_KEYBOARD_STATE* estado_teclado) {
    float nueva_x;
    int arriba;
    int abajo;
    int col;

    /* Mover a la izquierda */
    if (al_key_down(estado_teclado, ALLEGRO_KEY_A)) {
        p->mira_derecha = false;

        nueva_x = p->x - p->velocidadx;

        arriba = fisicas_mapa((int)nueva_x,(int)(p->y + 2));

        abajo = fisicas_mapa((int)nueva_x,(int)(p->y + p->alto - 2));

        if (!arriba && !abajo) {
            p->x = nueva_x;
        }
        else {
            col = (int)nueva_x / ancho_v;
            p->x = (col + 1) * ancho_v;
        }
    }

    /* Mover a la derecha */
    else if (al_key_down(estado_teclado, ALLEGRO_KEY_D)) {
        p->mira_derecha = true;

        nueva_x = p->x + p->velocidadx;

        arriba = fisicas_mapa((int)(nueva_x + p->ancho),(int)(p->y + 2));

        abajo = fisicas_mapa((int)(nueva_x + p->ancho),(int)(p->y + p->alto - 2));

        if (!arriba && !abajo) {
            p->x = nueva_x;
        }
        else {
            col = (int)(nueva_x + p->ancho) / ancho_v;
            p->x = col * ancho_v - p->ancho;
        }
    }
}

void saltar(personaje* p) {
    if (p->en_suelo == true) {
        p->velocidady = salto;
        p->en_suelo = false;
    }
}

void actualizar_idle_personaje(personaje* p, bool quieto) {
    if (quieto == false) {
        p->frame_actual = 0;
        p->contador_animacion = 0;
        return;
    }

    p->contador_animacion++;

    if (p->contador_animacion >= 12) {
        p->contador_animacion = 0;

        p->frame_actual++;

        if (p->frame_actual >= FRAMES_IDLE) {
            p->frame_actual = 0;
        }
    }
}

void actualizar_animacion_personaje(personaje* p, bool se_mueve) {
    tipo_animacion nueva_animacion;
    int total_frames;
    int cambio_frame;

    if (p->en_suelo == false) {
        nueva_animacion = ANIM_SALTAR;
    }
    else if (se_mueve == true) {
        nueva_animacion = ANIM_CORRER;
    }
    else {
        nueva_animacion = ANIM_IDLE;
    }

    if (p->animacion != nueva_animacion) {
        p->animacion = nueva_animacion;
        p->frame_actual = 0;
        p->contador_animacion = 0;
    }

    if (p->animacion == ANIM_SALTAR) {

        if (p->velocidady < -15.0f) {
            p->frame_actual = 0;  
        }
        else if (p->velocidady < -10.0f) {
            p->frame_actual = 1;   
        }
        else if (p->velocidady < 0.0f) {
            p->frame_actual = 2;   
        }
        else if (p->velocidady < 15.0f) {
            p->frame_actual = 3;   
        }
        else {
            p->frame_actual = 4;   
        }

        return;
    }

    if (p->animacion == ANIM_CORRER) {
        total_frames = FRAMES_CORRER;
        cambio_frame = CAMBIO_FRAME_CORRER;
    }
    else {
        total_frames = FRAMES_IDLE;
        cambio_frame = CAMBIO_FRAME_IDLE;
    }

    p->contador_animacion++;

    if (p->contador_animacion >= cambio_frame) {
        p->contador_animacion = 0;
        p->frame_actual++;

        if (p->frame_actual >= total_frames) {
            p->frame_actual = 0;
        }
    }
}

void fisicas(personaje* p) {
   ALLEGRO_KEYBOARD_STATE key_state;

   int izq;
   int der;
   int fila;

   float nueva_y;
   float x_antes;
   bool se_mueve;

   al_get_keyboard_state(&key_state);
   x_antes = p->x;
   movimiento(p, &key_state);
   direccion(p, &key_state);
   
   se_mueve = p->x != x_antes;
   actualizar_animacion_personaje(p, se_mueve);
   p->en_suelo = false;
   p->velocidady += gravedad;
   nueva_y = p->y + p->velocidady;

   if (p->invulnerable > 0) {
       p->invulnerable--;
   }

   if (p->velocidady > 0) {
       izq = fisicas_mapa((int)(p->x + 2), (int)(nueva_y + p->alto));
       der = fisicas_mapa((int)(p->x + p->ancho - 2), (int)(p->y + p->alto));

       if (izq || der) {
           fila = (int)(nueva_y + p->alto) / largo_v;

           p->y = fila * largo_v - p->alto;
           p->velocidady = 0;
           p->en_suelo = true;
       }
       else {
           p->y = nueva_y;        
       }
   }


   else if (p->velocidady < 0) {
       izq = fisicas_mapa((int)(p->x + 2),(int)nueva_y);
       der = fisicas_mapa((int)(p->x + p->ancho - 2),(int)nueva_y);

       if (izq || der) {
           fila = (int)nueva_y / largo_v;

           p->y = (fila + 1) * largo_v;
           p->velocidady = 0;
       }
       else {
           p->y = nueva_y;
       }
   }

   bool quieto;
   quieto = p->en_suelo == true &&! al_key_down(&key_state, ALLEGRO_KEY_A) &&! al_key_down(&key_state, ALLEGRO_KEY_D);

   se_mueve = p->x != x_antes;

   actualizar_animacion_personaje(p, se_mueve);

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

    crear_bala(p, p->balas);
}


void liberar_sprites_personaje(void) {
    int i;

    for (i = 0; i < FRAMES_IDLE; i++) {
        if (frames_idle[i] != NULL) {
            al_destroy_bitmap(frames_idle[i]);
            frames_idle[i] = NULL;
        }
    }

    for (i = 0; i < FRAMES_CORRER; i++) {
        if (frames_correr[i] != NULL) {
            al_destroy_bitmap(frames_correr[i]);
            frames_correr[i] = NULL;
        }
    }

    for (i = 0; i < FRAMES_SALTAR; i++) {
        if (frames_saltar[i] != NULL) {
            al_destroy_bitmap(frames_saltar[i]);
            frames_saltar[i] = NULL;
        }
    }
}


bool cargar_sprites_personaje(void) {
    const char* idle[FRAMES_IDLE] = {
        "assets/personaje/PersonajeIddle1.png",
        "assets/personaje/PersonajeIddle2.png",
        "assets/personaje/PersonajeIddle3.png",
        "assets/personaje/PersonajeIddle4.png"
    };

    const char* correr[FRAMES_CORRER] = {
        "assets/personaje/Personajecorriendo1.png",
        "assets/personaje/Personajecorriendo2.png",
        "assets/personaje/Personajecorriendo3.png",
        "assets/personaje/Personajecorriendo4.png"
    };

    const char* rutas_saltar[FRAMES_SALTAR] = {
        "assets/personaje/personajesalto1.png",
        "assets/personaje/personajesalto2.png",
        "assets/personaje/personajesalto3.png",
        "assets/personaje/personajesalto4.png",
        "assets/personaje/personajesalto5.png"
    };

    int i;

    for (i = 0; i < FRAMES_IDLE; i++) {
        frames_idle[i] = al_load_bitmap(idle[i]);

        if (frames_idle[i] == NULL) {
            printf("No se pudo cargar idle%d.png\n", i + 1);
            liberar_sprites_personaje();
            return false;
        }
    }

    for (i = 0; i < FRAMES_CORRER; i++) {
        frames_correr[i] = al_load_bitmap(correr[i]);

        if (frames_correr[i] == NULL) {
            printf("No se pudo cargar frame correr %d\n", i + 1);
            liberar_sprites_personaje();
            return false;
        }
    }

    for (i = 0; i < FRAMES_SALTAR; i++) {
        frames_saltar[i] = al_load_bitmap(rutas_saltar[i]);

        if (frames_saltar[i] == NULL) {
            printf("No se pudo cargar: %s\n", rutas_saltar[i]);
            liberar_sprites_personaje();
            return false;
        }
    }
    return true;
}