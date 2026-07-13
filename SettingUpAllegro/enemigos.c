#include "enemigos.h"
#include "personaje.h"
#include "mapa.h"
#include "balas.h"

#define ancho_enemigo 40
#define alto_enemigo 40

#define GRAVEDAD_ENEMIGO 0.3f
#define coold_at 60

#define FRAMES_ENEMIGO_CAMINAR 5
#define CAMBIO_FRAME_ENEMIGO_CAMINAR 7

#define FRAMES_ENEMIGO_ATACAR 4
#define CAMBIO_FRAME_ENEMIGO_ATACAR 8


static ALLEGRO_BITMAP * frames_enemigo_caminar[FRAMES_ENEMIGO_CAMINAR] = {
    NULL, NULL, NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_enemigo_atacar[FRAMES_ENEMIGO_ATACAR] = {
    NULL, NULL, NULL, NULL
};


void dibujar_enemigo(enemigo* e, float camara_x, float camara_y) {
    float x;
    float y;

    float ancho_barra;
    float alto_barra;
    float vida_actual;
    float barra_x;
    float barra_y;

    ALLEGRO_BITMAP* sprite = NULL;
    int flags = 0;

    if (e->activo == false) {
        return;
    }

    x = e->x - camara_x;
    y = e->y - camara_y;


    if (e->tipo == zombie_n) {

        if (e->animacion == CAMINAR) {
            sprite = frames_enemigo_caminar[e->frame_actual];
        }
        else if (e->animacion == ATACAR) {
            sprite = frames_enemigo_atacar[e->frame_actual];
        }

        if (e->direccionx < 0) {
            flags = ALLEGRO_FLIP_HORIZONTAL;
        }

        if (sprite != NULL) {
            al_draw_scaled_bitmap(
                sprite,
                0,
                0,
                al_get_bitmap_width(sprite),
                al_get_bitmap_height(sprite),
                x,
                y,
                e->ancho,
                e->alto,
                flags
            );
        }
    }
    else if (e->tipo == zombie_r) {
        al_draw_filled_rectangle(
            x,
            y,
            x + e->ancho,
            y + e->alto,
            al_map_rgb(255, 0, 0)
        );
    }


    if (e->vida_max <= 0) {
        return;
    }

    ancho_barra = e->ancho + 20.0f;
    alto_barra = 7.0f;

    vida_actual = (e->vida / (float)e->vida_max) * ancho_barra;

    if (vida_actual < 0) {
        vida_actual = 0;
    }

    if (vida_actual > ancho_barra) {
        vida_actual = ancho_barra;
    }

    barra_x = x - 10.0f;
    barra_y = y - 20.0f;

    al_draw_filled_rectangle(barra_x,barra_y,barra_x + ancho_barra,barra_y + alto_barra,al_map_rgb(80, 0, 0));

    al_draw_filled_rectangle(barra_x,barra_y,barra_x + vida_actual,barra_y + alto_barra,al_map_rgb(255, 0, 0));
}


void inicializar_enemigos(enemigo enemigos[]) {
    int i;

    for (i = 0; i < max_enemigos; i++) {
        enemigos[i].x = 0;
        enemigos[i].y = 0;

        enemigos[i].ancho = 40;
        enemigos[i].alto = 40;

        enemigos[i].velocidadx = 0;
        enemigos[i].velocidady = 0;

        enemigos[i].vida = 0;
        enemigos[i].vida_max = 0;
        enemigos[i].dano = 0;

        enemigos[i].direccionx = 1;

        enemigos[i].atacando = false;
        enemigos[i].cooldown_ataque = 0;

        enemigos[i].animacion = CAMINAR;
        enemigos[i].frame_actual = 0;
        enemigos[i].contador_animacion = 0;

        enemigos[i].activo = false;
    }
}

void crear_enemigo(enemigo enemigos[], float x, float y, tipo_enemigo tipo) {
    int i;

    for (i = 0; i < max_enemigos; i++) {
        if (enemigos[i].activo == false) {

            enemigos[i].x = x;
            enemigos[i].y = y;

            enemigos[i].ancho = 40;
            enemigos[i].alto = 40;


            enemigos[i].tipo = tipo;
            enemigos[i].cooldown_ataque = 0;

            enemigos[i].velocidady = 0.0f;

            enemigos[i].animacion = CAMINAR;
            enemigos[i].frame_actual = 0;
            enemigos[i].contador_animacion = 0;

            enemigos[i].atacando = false;
            enemigos[i].en_suelo = false;
            enemigos[i].persiguiendo = false;

            if (tipo == zombie_r) {
                enemigos[i].direccionx = -1;
                enemigos[i].velocidadx = 2.3f;
                enemigos[i].vida_max = 1;
                enemigos[i].dano = 3;
                enemigos[i].rango_vision = 550.0f;

            }
            if (tipo == zombie_n) {
                enemigos[i].direccionx = 1;
                enemigos[i].velocidadx = 1.0f;
                enemigos[i].vida_max = 3;
                enemigos[i].dano = 2;
                enemigos[i].rango_vision = 0.0f;
            }
            enemigos[i].vida = enemigos[i].vida_max;
            enemigos[i].activo = true;
            return;
        }
    }
}

void dibujar_enemigos_mapa(enemigo enemigos[], float camara_x, float camara_y) {
    int i;

    for (i = 0; i < max_enemigos; i++) {
        if (enemigos[i].activo == true) {
            dibujar_enemigo(&enemigos[i], camara_x, camara_y);
        }
    }
}

void spawn_enemigos(enemigo enemigos[]) {
    int fila;
    int columna;

    for (fila = 0; fila < filas; fila++) {
        for (columna = 0; columna < columnas; columna++) {

            if (mapa[fila][columna] == 'e') {

                crear_enemigo(enemigos, columna * ancho_v,fila * largo_v, zombie_n);

                mapa[fila][columna] = '.';
            }
            if (mapa[fila][columna] == 'R') {

                crear_enemigo(enemigos, columna * ancho_v, fila * largo_v, zombie_r);

                mapa[fila][columna] = '.';
            }
        }
    }
}

void fisicas_enemigo(enemigo* e, personaje* p) {
    float nueva_pos_x;
    int pared;
    int suelo;
    int col;

    if (e->activo == false) {
        return;
    }

    if (e->tipo == zombie_r) {
        float centro_enemigo;
        float centro_personaje;
        float distancia;

        // define la direccion de p
        centro_enemigo = e->x + e->ancho / 2.0f;
        centro_personaje = p->x + p->ancho / 2.0f;

        distancia = centro_personaje - centro_enemigo;

        // valor absoluto para medir solo la distancia
        if (distancia < 0) {
            distancia = distancia * -1;
        }

        if (distancia <= e->rango_vision) {
            e->persiguiendo = true;
        }

        if (e->persiguiendo == true) {
            if (centro_personaje < centro_enemigo) {
                e->direccionx = -1;
            }
            else {
                e->direccionx = 1;
            }
        }
    }

    nueva_pos_x = e->x + e->velocidadx * e->direccionx;

    if (e->direccionx == 1) { // derecha

        // revisa colison
        pared = fisicas_mapa((int)(nueva_pos_x + e->ancho),(int)(e->y + 1)) || fisicas_mapa((int)(nueva_pos_x + e->ancho),(int)(e->y + e->alto - 1));

        suelo = fisicas_mapa((int)(nueva_pos_x + e->ancho + 1),(int)(e->y + e->alto + 1));

        if (pared || suelo == 0) { // si hay pared y no suelo
            e->direccionx = -1;

            if (pared) {
                col = (int)(nueva_pos_x + e->ancho) / ancho_v;
                e->x = col * ancho_v - e->ancho;
            }

            return;
        }
    }

    else { // izquierda

        // revisa colison
        pared = fisicas_mapa((int)nueva_pos_x,(int)(e->y + 1)) || fisicas_mapa((int)nueva_pos_x,(int)(e->y + e->alto - 1));

        suelo = fisicas_mapa((int)(nueva_pos_x - 1), (int)(e->y + e->alto + 1));

        if (pared || suelo == 0) { // si hay pared y no suelo
            e->direccionx = 1;

            if (pared) {
                col = (int)nueva_pos_x / ancho_v;
                e->x = (col + 1) * ancho_v;
            }

            return;
        }
    }

    e->x = nueva_pos_x;
}


void fisicas_enemigos(enemigo enemigos[], personaje* p) {
    int i;

    for (i = 0; i < max_enemigos; i++) {
        actualizar_animacion_enemigo(&enemigos[i]);

        fisicas_enemigo(&enemigos[i], p);
    }

}


bool colision_bala_enemigo(bala* b, enemigo* e) {

    if (b->x < e->x + e->ancho &&
        b->x + b->ancho > e->x &&
        b->y < e->y + e->alto &&
        b->y + b->alto > e->y) {

        e->vida -= 1;       
        b->activa = false;  

        if (e->vida <= 0) {
            e->vida = 0;
            e->activo = false;
        }

        return true;
    }

    return false;
}

void revisar_colisiones_bala_enemigo(enemigo enemigos[], bala balas[]) {
    int i;
    int j;

    for (i = 0; i < max_balas_p; i++) {
        if (balas[i].activa == true) {

            for (j = 0; j < max_enemigos; j++) {
                if (enemigos[j].activo == true) {

                    if (colision_bala_enemigo(&balas[i], &enemigos[j])) {
                        break;
                    }
                }
            }
        }
    }
}

bool cargar_sprites_enemigos(void) {
    const char* rutas[FRAMES_ENEMIGO_CAMINAR] = {
        "assets/enemigos/enemigocorriendo1.png",
        "assets/enemigos/enemigocorriendo2.png",
        "assets/enemigos/enemigocorriendo3.png",
        "assets/enemigos/enemigocorriendo4.png",
        "assets/enemigos/enemigocorriendo5.png"
    };

    const char* rutas_atacar[FRAMES_ENEMIGO_ATACAR] = {
        "assets/enemigos/enemigoatacandox1.png",
        "assets/enemigos/enemigoatacandox2.png",
        "assets/enemigos/enemigoatacandox3.png",
        "assets/enemigos/enemigoatacandox4.png"
    };

    int i;

    for (i = 0; i < FRAMES_ENEMIGO_CAMINAR; i++) {
        frames_enemigo_caminar[i] = al_load_bitmap(rutas[i]);

        if (frames_enemigo_caminar[i] == NULL) {
            printf("No se pudo cargar: %s\n", rutas[i]);

            liberar_sprites_enemigos();
            return false;
        }
    }

    for (i = 0; i < FRAMES_ENEMIGO_ATACAR; i++) {
        frames_enemigo_atacar[i] = al_load_bitmap(rutas_atacar[i]);

        if (frames_enemigo_atacar[i] == NULL) {
            printf("No se pudo cargar: %s\n", rutas_atacar[i]);
            liberar_sprites_enemigos();
            return false;
        }
    }

    return true;
}

void liberar_sprites_enemigos(void) {
    int i;

    for (i = 0; i < FRAMES_ENEMIGO_CAMINAR; i++) {
        if (frames_enemigo_caminar[i] != NULL) {
            al_destroy_bitmap(frames_enemigo_caminar[i]);
            frames_enemigo_caminar[i] = NULL;
        }
    }

    for (i = 0; i < FRAMES_ENEMIGO_ATACAR; i++) {
        if (frames_enemigo_atacar[i] != NULL) {
            al_destroy_bitmap(frames_enemigo_atacar[i]);
            frames_enemigo_atacar[i] = NULL;
        }
    }
}

void cambiar_animacion_enemigo(enemigo* e,tipo_animacion_enemigo nueva_animacion) 
{
    if (e->animacion != nueva_animacion) {
        e->animacion = nueva_animacion;
        e->frame_actual = 0;
        e->contador_animacion = 0;
    }
}

void actualizar_animacion_enemigo(enemigo* e) {
    int total_frames;
    int cambio_frame;

    if (e->activo == false) {
        return;
    }

    switch (e->animacion) {
    case CAMINAR:
        total_frames = FRAMES_ENEMIGO_CAMINAR;
        cambio_frame = CAMBIO_FRAME_ENEMIGO_CAMINAR;
        break;

    case ATACAR:
        total_frames = FRAMES_ENEMIGO_ATACAR;
        cambio_frame = CAMBIO_FRAME_ENEMIGO_ATACAR;
        break;

    default:
        return;
    }

    e->contador_animacion++;

    if (e->contador_animacion >= cambio_frame) {
        e->contador_animacion = 0;
        e->frame_actual++;

        if (e->frame_actual >= total_frames) {
            e->frame_actual = 0;
        }
    }
}

bool colison_enemigo(enemigo* e, personaje* p) {
    return
        e->x < p->x + p->ancho &&
        e->x + e->ancho > p->x &&
        e->y < p->y + p->alto &&
        e->y + e->alto > p->y;
}

void actualizar_ataque_enemigo(enemigo* e, personaje* p) {
    float p_medio;
    float e_medio;

    if (e->activo == false) {
        return;
    }

    if (e->cooldown_ataque > 0) {
        e->cooldown_ataque--;
    }

    if (colison_enemigo(e, p)) {
        e->atacando = true;

        e_medio = e->x + e->ancho / 2.0f;
        p_medio = p->x + p->ancho / 2.0f;

        if (p_medio < e_medio) {
            e->direccionx = -1;
        }
        else {
            e->direccionx = 1;
        }

        cambiar_animacion_enemigo(e, ATACAR);

        if (e->cooldown_ataque == 0) {
            recibir_dano_personaje(p, e->dano);
            e->cooldown_ataque = coold_at;
        }
    }
    else {
        e->atacando = false;
        cambiar_animacion_enemigo(e, CAMINAR);
    }
}

void actualizar_ataques_enemigos(personaje* p, enemigo enemigos[]) {
    int i;

    for (i = 0; i < max_enemigos; i++) {
        actualizar_ataque_enemigo(&enemigos[i], p);
    }
}