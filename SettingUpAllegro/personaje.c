#define _CRT_SECURE_NO_WARNINGS
#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#define spawn_personajex 'o'
#define t_invulnerable 60

#define FRAMES_IDLE 4
#define FRAMES_CORRER 4
#define FRAMES_SALTAR 5

#define CAMBIO_FRAME_IDLE 15
#define CAMBIO_FRAME_CORRER 10
#define CAMBIO_FRAME_SALTAR 20

#define FRAMES_DISPARAR 3
#define TIEMPO_DISPARO 12
#define DISPARO_FRENTE 0
#define DISPARO_ABAJO 1
#define DISPARO_ARRIBA 2

static ALLEGRO_BITMAP* frames_idle[FRAMES_IDLE] = {
    NULL, NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_correr[FRAMES_CORRER] = {
    NULL, NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_saltar[FRAMES_SALTAR] = {
    NULL, NULL, NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_disparar[FRAMES_DISPARAR] = {
    NULL, NULL, NULL
};

void dibujo_personaje(personaje* p, float camara_x, float camara_y) {
    ALLEGRO_BITMAP* sprite = NULL;
    int flags = 0;

    if (p->mira_derecha == false) {
        flags = ALLEGRO_FLIP_HORIZONTAL;
    }

    if (p->tiempo_disparo > 0) {
        sprite = frames_disparar[p->frame_disparo];
    }
    else if (p->animacion == ANIM_CORRER) {
        sprite = frames_correr[p->frame_actual];
    }
    else if (p->animacion == ANIM_SALTAR) {
        sprite = frames_saltar[p->frame_actual];
    }
    else {
        sprite = frames_idle[p->frame_actual];
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
    float porcentaje_vida;
    float porcentaje_escudo;

    float ancho_vida;
    float ancho_escudo;

    int x = 20;
    int y = 20;

    int ancho_barra = 220;
    int alto_barra = 22;

    int separacion = 8;

    if (p->vida_max <= 0) {
        return;
    }

    porcentaje_vida = (float)p->vida / (float)p->vida_max;

    if (porcentaje_vida < 0.0f) {
        porcentaje_vida = 0.0f;
    }

    if (porcentaje_vida > 1.0f) {
        porcentaje_vida = 1.0f;
    }

    ancho_vida = ancho_barra * porcentaje_vida;

    al_draw_filled_rectangle(x,y,x + ancho_barra,y + alto_barra,al_map_rgb(90, 90, 90));

    al_draw_filled_rectangle(x,y,x + ancho_vida,y + alto_barra,al_map_rgb(0, 200, 0));

    al_draw_rectangle(x,y,x + ancho_barra,y + alto_barra,al_map_rgb(255, 255, 255),2);

    if (p->escudo_max <= 0) {
        return;
    }

    porcentaje_escudo = (float)p->escudo / (float)p->escudo_max;

    if (porcentaje_escudo < 0.0f) {
        porcentaje_escudo = 0.0f;
    }

    if (porcentaje_escudo > 1.0f) {
        porcentaje_escudo = 1.0f;
    }

    ancho_escudo = ancho_barra * porcentaje_escudo;

    al_draw_filled_rectangle(x,y + alto_barra + separacion,x + ancho_barra,y + alto_barra + separacion + alto_barra,al_map_rgb(70, 70, 90));

    al_draw_filled_rectangle(x,y + alto_barra + separacion,x + ancho_escudo,y + alto_barra + separacion + alto_barra,al_map_rgb(0, 150, 255));

    al_draw_rectangle(x,y + alto_barra + separacion,x + ancho_barra,y + alto_barra + separacion + alto_barra,al_map_rgb(255, 255, 255),2);
}

void dibujar_puntaje(personaje* p, ALLEGRO_FONT* fuente) {
    char texto[50];

    sprintf(texto, "Puntos:%d", p->puntaje);

    al_draw_text(
        fuente,
        al_map_rgb(255, 255, 255),
        20,
        85,
        ALLEGRO_ALIGN_LEFT,
        texto
    );
}

void spawn_personaje(personaje* p) {
   
    int f;
    int c;

    for (f = 0; f < filas; f++) {
        for (c = 0; c < columnas; c++) {
            if (mapa[f][c] == spawn_personajex) {
             
                p->x = c * ancho_v;
                p->y = f * largo_v;

                p->en_suelo = false;
                p->ancho = 40;
                p->alto = 40;

                p->velocidadx = 3.0;
                p->velocidady = 3.0;

                p->invulnerable = 0;
                p->vida_max = 10;
                p->vida = p->vida_max;
                p->escudo_max = 10;
                p->escudo = p->escudo_max;
                p->municion = 10;
                p->municion_granadas = 1;
                p->llave = 0;
                p->llaves_nivel = 0;
                p->agarro_llave = false;

                p->animacion = ANIM_IDLE;
                p->frame_actual = 0;
                p->contador_animacion = 0;
                p->frame_disparo = DISPARO_FRENTE;
                p->tiempo_disparo = 0;
                p->mira_derecha = true;

                mapa[f][c] = '.'; 

                return; 
            }
        }
    }
}

void recibir_dano_personaje(personaje* p, int dano_recibido) {
    if (dano_recibido < 0) {
        return;
    }

    if (p->vida <= 0) {
        return;
    }

    if (p->invulnerable > 0) {
        return;
    }

    if (p->escudo > 0) {
        p->escudo -= dano_recibido;

        if (p->escudo < 0) {
            p->vida += p->escudo;
            p->escudo = 0;
        }
    }

    else if (p->x + p->ancho / 2.0f) {
        p->vida -= dano_recibido;
    }

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

        // revisar colisiones 
        arriba = fisicas_mapa((int)nueva_x,(int)(p->y + 2));

        abajo = fisicas_mapa((int)nueva_x,(int)(p->y + p->alto - 2));

        if (arriba == 0 && abajo == 0) { // si ! hay, se mueve, 0 si no hay por fiscas_mapa
            p->x = nueva_x;
        }
        else { // deja al p en esa columna
            col = (int)nueva_x / ancho_v;
            p->x = (col + 1) * ancho_v;
        }
    }

    /* Mover a la derecha */
    else if (al_key_down(estado_teclado, ALLEGRO_KEY_D)) {
        p->mira_derecha = true;

        nueva_x = p->x + p->velocidadx;

        // revisar colisiones 
        arriba = fisicas_mapa((int)(nueva_x + p->ancho),(int)(p->y + 2));

        abajo = fisicas_mapa((int)(nueva_x + p->ancho),(int)(p->y + p->alto - 2));

        if (arriba == 0 && abajo == 0) {
            p->x = nueva_x;
        }
        else { // deja al p en esa columna
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

void actualizar_animacion_personaje(personaje* p, bool se_mueve) {
    tipo_animacion nueva_animacion;
    int total_frames;
    int cambio_frame;

    if (p->tiempo_disparo > 0) {
        p->tiempo_disparo--;
    }


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
   
   p->en_suelo = false;
   p->velocidady += gravedad;
   nueva_y = p->y + p->velocidady;

   if (p->invulnerable > 0) {
       p->invulnerable--;
   }

   // si cae
   if (p->velocidady > 0) {
       izq = fisicas_mapa((int)(p->x + 2), (int)(nueva_y + p->alto));
       der = fisicas_mapa((int)(p->x + p->ancho - 2), (int)(nueva_y + p->alto));

       if (izq || der) { // si toco bloque solido abajo
           fila = (int)(nueva_y + p->alto) / largo_v;

           p->y = fila * largo_v - p->alto;
           p->velocidady = 0;
           p->en_suelo = true;
       }
       else {
           p->y = nueva_y;        
       }
   }


   else if (p->velocidady < 0) { // si sube
       izq = fisicas_mapa((int)(p->x + 2),(int)nueva_y);
       der = fisicas_mapa((int)(p->x + p->ancho - 2),(int)nueva_y);

       if (izq || der) { // si toco bloque solido arriba
           fila = (int)nueva_y / largo_v;

           p->y = (fila + 1) * largo_v;
           p->velocidady = 0;
       }
       else {
           p->y = nueva_y;
       }
   }

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

    for (i = 0; i < FRAMES_DISPARAR; i++) {
        if (frames_disparar[i] != NULL) {
            al_destroy_bitmap(frames_disparar[i]);
            frames_disparar[i] = NULL;
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

    const char* disparar[FRAMES_DISPARAR] = {
    "assets/personaje/personaje_disparando_frente.png",
    "assets/personaje/personaje_disparando_abajo.png",
    "assets/personaje/personaje_disparando_arriba.png"
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

    for (i = 0; i < FRAMES_DISPARAR; i++) {
        frames_disparar[i] = al_load_bitmap(disparar[i]);

        if (frames_disparar[i] == NULL) {
            printf("No se pudo cargar frame disparar %d\n", i + 1);
            liberar_sprites_personaje();
            return false;
        }
    }

    return true;
}

bool disparo_mouse(personaje* p, float mouse_x, float mouse_y, float camara_x, float camara_y, float zoom) {

    int i;

    float mouse_mapa_x;
    float mouse_mapa_y;

    float centro_jugador_x;
    float centro_jugador_y;

    float dx;
    float dy;
    float distancia;

    if (p->municion <= 0) {
        return false;
    }

    mouse_mapa_x = mouse_x / zoom + camara_x;
    mouse_mapa_y = mouse_y / zoom + camara_y;

    centro_jugador_x = p->x + p->ancho / 2.0f;
    centro_jugador_y = p->y + p->alto / 2.0f;

    dx = mouse_mapa_x - centro_jugador_x;
    dy = mouse_mapa_y - centro_jugador_y;

    distancia = sqrt(dx * dx + dy * dy);

    if (distancia == 0) {
        return false;
    }

    for (i = 0; i < max_balas_p; i++) {
        if (p->balas[i].activa == false) {

            p->balas[i].x = centro_jugador_x;
            p->balas[i].y = centro_jugador_y;
            p->balas[i].x_inicio = centro_jugador_x;
            p->balas[i].y_inicio = centro_jugador_y;
            p->balas[i].rango = 500.0f;
            p->balas[i].ancho = ancho_bala;
            p->balas[i].alto = alto_bala;

            p->balas[i].velocidad_bx = (dx / distancia) * velocidad_bala;
            p->balas[i].velocidad_by = (dy / distancia) * velocidad_bala;

            p->balas[i].frame_actual = 0;
            p->balas[i].contador_animacion = 0;
            p->balas[i].activa = true;

            if (dx < 0) {
                p->mira_derecha = false;
            }
            else if (dx > 0) {
                p->mira_derecha = true;
            }

            if (dy > 25.0f) {
                p->frame_disparo = DISPARO_ABAJO;
            }
            else if (dy < -25.0f) {
                p->frame_disparo = DISPARO_ARRIBA;
            }
            else {
                p->frame_disparo = DISPARO_FRENTE;
            }

            p->tiempo_disparo = TIEMPO_DISPARO;

            p->municion--;

            return true;
        }
    }

    return false;
}

bool disparo_granada_mouse(personaje* p, float mouse_x, float mouse_y, float camara_x, float camara_y, float zoom) {
    int i;

    float mouse_mapa_x;
    float mouse_mapa_y;

    float centro_jugador_x;
    float centro_jugador_y;

    float dx;
    float dy;
    float distancia;

    if (p->municion_granadas <= 0) {
        return false;
    }

    mouse_mapa_x = mouse_x / zoom + camara_x;
    mouse_mapa_y = mouse_y / zoom + camara_y;

    centro_jugador_x = p->x + p->ancho / 2.0f;
    centro_jugador_y = p->y + p->alto / 2.0f;

    dx = mouse_mapa_x - centro_jugador_x;
    dy = mouse_mapa_y - centro_jugador_y;

    distancia = sqrt(dx * dx + dy * dy);

    if (distancia == 0) {
        return false;
    }

    for (i = 0; i < max_granadas_p; i++) {
        if (p->granadas[i].activa == false) {

            p->granadas[i].x = centro_jugador_x;
            p->granadas[i].y = centro_jugador_y;

            p->granadas[i].ancho = 20;
            p->granadas[i].alto = 20;

            p->granadas[i].velocidad_gx = (dx / distancia) * 8.0f;
            p->granadas[i].velocidad_gy = (dy / distancia) * 8.0f - 4.0f;

            p->granadas[i].frame_actual = 0;
            p->granadas[i].contador_animacion = 0;

            p->granadas[i].activa = true;
            p->granadas[i].explotar = false;

            p->municion_granadas--;

            return true;
        }
    }
    return false;
}

void sombra_personaje(personaje* p, float camara_x, float camara_y) {
    float centro_jugador_X;
    float centro_jugador_y;

    centro_jugador_X = p->x + p->ancho / 2.0f - camara_x;
    centro_jugador_y = p->y + p->alto / 2.0f - camara_y;

    al_draw_filled_ellipse(centro_jugador_X, centro_jugador_y,
                           p->ancho * 1.0f, p->alto * 1.0f,
                           al_map_rgba(60, 60, 60, 80));

    al_draw_ellipse(centro_jugador_X, centro_jugador_y,
        p->ancho * 1.0f, p->alto * 1.0f,
        al_map_rgba(20, 80, 80, 80), 10);
}

void actualizar_animacion_final_personaje(personaje* p) {
    ALLEGRO_KEYBOARD_STATE key_state;
    bool se_mueve = false;

    al_get_keyboard_state(&key_state);

    if (al_key_down(&key_state, ALLEGRO_KEY_A) ||
        al_key_down(&key_state, ALLEGRO_KEY_D)) {
        se_mueve = true;
    }

    actualizar_animacion_personaje(p, se_mueve);
}