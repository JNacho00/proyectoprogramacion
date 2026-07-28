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
#define CAMBIO_FRAME_ENEMIGO_ATACAR 10

#define FRAMES_ZOMBIE_R_CAMINAR 3
#define CAMBIO_FRAME_ZOMBIE_R_CAMINAR 7

#define FRAMES_ZOMBIE_R_ATACAR 3
#define CAMBIO_FRAME_ZOMBIE_R_ATACAR 10

#define FRAMES_ZOMBIE_D 2
#define CAMBIO_FRAME_ZOMBIE_D 15

static ALLEGRO_BITMAP * frames_enemigo_caminar[FRAMES_ENEMIGO_CAMINAR] = {
    NULL, NULL, NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_enemigo_atacar[FRAMES_ENEMIGO_ATACAR] = {
    NULL, NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_zombie_r_caminar[FRAMES_ZOMBIE_R_CAMINAR] = {
    NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_zombie_r_atacar[FRAMES_ZOMBIE_R_ATACAR] = {
    NULL, NULL, NULL
};

static ALLEGRO_BITMAP* frames_zombie_d[FRAMES_ZOMBIE_D] = {
    NULL, NULL
};

static ALLEGRO_BITMAP* sprite_volador_abajo = NULL;
static ALLEGRO_BITMAP* sprite_volador_diag_izq = NULL;
static ALLEGRO_BITMAP* sprite_volador_diag_der = NULL;

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

        if (e->animacion == CAMINAR) {
            sprite = frames_zombie_r_caminar[e->frame_actual];
        }
        else if (e->animacion == ATACAR) {
            sprite = frames_zombie_r_atacar[e->frame_actual];
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
    else if (e->tipo == zombie_d) {
        sprite = frames_zombie_d[e->frame_actual];

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
    else if (e->tipo == volador) {
        if (e->direcciony_disp == 1 && e->direccionx_disp < 0) {
            sprite = sprite_volador_diag_izq;
        }
        else if (e->direcciony_disp == 1 && e->direccionx_disp > 0) {
            sprite = sprite_volador_diag_der;
        }
        else {
            sprite = sprite_volador_abajo;
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
                0
            );
        }
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
        enemigos[i].y_inicio = 0;
        enemigos[i].x_inicio = enemigos[i].x;
        enemigos[i].dist = 0;


        enemigos[i].vida = 0;
        enemigos[i].vida_max = 0;
        enemigos[i].dano = 0;
        enemigos[i].puntos = 0;

        enemigos[i].direccionx = 1;
        enemigos[i].direcciony = 0;
        enemigos[i].direccionx_disp = 0;
        enemigos[i].direcciony_disp = 1;

        enemigos[i].atacando = false;
        enemigos[i].cooldown_ataque = 0;
        enemigos[i].cooldown_disparo = 0;

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
                enemigos[i].vida_max = 4;
                enemigos[i].dano = 3;
                enemigos[i].rango_vision = 550.0f;
                enemigos[i].puntos = 250;

            }
            if (tipo == zombie_n) {
                enemigos[i].direccionx = 1;
                enemigos[i].velocidadx = 1.0f;
                enemigos[i].vida_max = 3;
                enemigos[i].dano = 2;
                enemigos[i].puntos = 50;

            }
            if (tipo == zombie_d) {
                enemigos[i].direccionx = 1;
                enemigos[i].velocidadx = 2.0f;
                enemigos[i].vida_max = 2;
                enemigos[i].dano = 10;
                enemigos[i].rango_vision = 450.0f;
                enemigos[i].cooldown_disparo = 0;
                enemigos[i].puntos = 300;

            }
            if (tipo == volador) {
                enemigos[i].direccionx = 1;
                enemigos[i].direcciony = 0;
                enemigos[i].direccionx_disp = 0;
                enemigos[i].direcciony_disp = 1;
                enemigos[i].velocidadx = 2.0f;
                enemigos[i].velocidady = 2.0f;
                enemigos[i].vida_max = 2;
                enemigos[i].dano = 5;
                enemigos[i].rango_vision = 220.0f;
                enemigos[i].cooldown_disparo = 0;
                enemigos[i].y_inicio = y;
                enemigos[i].x_inicio = enemigos[i].x;
                enemigos[i].dist = 200;
                enemigos[i].puntos = 500;


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
            if (mapa[fila][columna] == 'D') {

                crear_enemigo(enemigos, columna * ancho_v, fila * largo_v, zombie_d);

                mapa[fila][columna] = '.';
            }
            if (mapa[fila][columna] == 'V') {

                crear_enemigo(enemigos, columna * ancho_v, fila * largo_v, volador);

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

    if (e->tipo == volador) {
        float centro_enemigo;
        float centro_personaje;
        float distancia;

        // define la direccion de p
        centro_enemigo = e->x + e->ancho / 2.0f;
        centro_personaje = p->x + p->ancho / 2.0f;

        distancia = centro_personaje - centro_enemigo;

        e->x += e->velocidadx * e->direccionx;

        if (e->x >= e->x_inicio + e->dist) {
            e->direccionx = -1;
        }

        if (e->x <= e->x_inicio) {
            e->direccionx = 1;
        }

        return;
    }
   
    if (e->tipo == zombie_d) {
        float distanciax;
        float distanciay;
        bool jugador_delante = false;

        distanciay = (p->y + p->alto / 2.0f) - (e->y + e->alto / 2.0f);
        distanciax = (p->x + p->ancho / 2.0f) - (e->x + e->ancho / 2.0f);

        if (distanciax < 0) {
            distanciax = distanciax * -1;
        }
        if (distanciay < 0) {
            distanciay = distanciay * -1;
        }
        if (e->direccionx == 1 && p->x + p->ancho / 2.0f > e->x + e->ancho / 2.0f) {
            jugador_delante = true;
        }

        if (e->direccionx == -1 && p->x + p->ancho / 2.0f < e->x + e->ancho / 2.0f) {
            jugador_delante = true;
        }

        if (distanciax <= e->rango_vision && distanciay <= 40.0f && jugador_delante == true) {
            return;
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

bool colision_bala_enemigo(bala* b, enemigo* e, personaje* p) {

    if (b->x + b->ancho < e->x) {
        return false;
    }

    if (b->x > e->x + e->ancho) {
        return false;
    }

    if (b->y + b->alto < e->y) {
        return false;
    }

    if (b->y > e->y + e->alto ) {
        return false;
    }

    e->vida -= 1;
    b->activa = false;

    if (e->vida <= 0) {
        e->vida = 0;
        p->puntaje += e->puntos;
        e->activo = false;
    }
    return true;
}



void revisar_colisiones_bala_enemigo(enemigo enemigos[], bala balas[], personaje* p) {
    int i;
    int j;

    for (i = 0; i < max_balas_p; i++) {
        if (balas[i].activa == true) {

            for (j = 0; j < max_enemigos; j++) {
                if (enemigos[j].activo == true) {

                    if (colision_bala_enemigo(&balas[i], &enemigos[j], p)) {
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

    const char* rutas_zombie_r[FRAMES_ZOMBIE_R_CAMINAR] = {
    "assets/enemigos/enemigo2corriendo1.png",
    "assets/enemigos/enemigo2corriendo2.png",
    "assets/enemigos/enemigo2corriendo3.png"
    };

    const char* rutas_zombie_r_atacar[FRAMES_ZOMBIE_R_ATACAR] = {
    "assets/enemigos/enemigo2atacando1.png",
    "assets/enemigos/enemigo2atacando2.png",
    "assets/enemigos/enemigo2atacando3.png"
    };

    const char* rutas_zombie_d[FRAMES_ZOMBIE_D] = {
    "assets/enemigos/enemigo3.png",
    "assets/enemigos/enemigo4.png"
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

    for (i = 0; i < FRAMES_ZOMBIE_R_CAMINAR; i++) {
        frames_zombie_r_caminar[i] = al_load_bitmap(rutas_zombie_r[i]);

        if (frames_zombie_r_caminar[i] == NULL) {
            printf("No se pudo cargar: %s\n", rutas_zombie_r[i]);
            liberar_sprites_enemigos();
            return false;
        }
    }

    for (i = 0; i < FRAMES_ZOMBIE_R_ATACAR; i++) {
        frames_zombie_r_atacar[i] = al_load_bitmap(rutas_zombie_r_atacar[i]);

        if (frames_zombie_r_atacar[i] == NULL) {
            printf("No se pudo cargar: %s\n", rutas_zombie_r_atacar[i]);
            liberar_sprites_enemigos();
            return false;
        }
    }

    for (i = 0; i < FRAMES_ZOMBIE_D; i++) {
        frames_zombie_d[i] = al_load_bitmap(rutas_zombie_d[i]);

        if (frames_zombie_d[i] == NULL) {
            printf("No se pudo cargar: %s\n", rutas_zombie_d[i]);
            liberar_sprites_enemigos();
            return false;
        }
    }

    sprite_volador_abajo = al_load_bitmap("assets/enemigos/enemigovolador1.png");

    if (sprite_volador_abajo == NULL) {
        printf("No se pudo cargar enemigovolador1.png\n");
        liberar_sprites_enemigos();
        return false;
    }

    sprite_volador_diag_izq = al_load_bitmap("assets/enemigos/enemigovolador3.png");

    if (sprite_volador_diag_izq == NULL) {
        printf("No se pudo cargar enemigovolador3.png\n");
        liberar_sprites_enemigos();
        return false;
    }

    sprite_volador_diag_der = al_load_bitmap("assets/enemigos/enemigovolador2.png");

    if (sprite_volador_diag_der == NULL) {
        printf("No se pudo cargar enemigovolador2.png\n");
        liberar_sprites_enemigos();
        return false;
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

    for (i = 0; i < FRAMES_ZOMBIE_R_CAMINAR; i++) {
        if (frames_zombie_r_caminar[i] != NULL) {
            al_destroy_bitmap(frames_zombie_r_caminar[i]);
            frames_zombie_r_caminar[i] = NULL;
        }
    }

    for (i = 0; i < FRAMES_ZOMBIE_R_ATACAR; i++) {
        if (frames_zombie_r_atacar[i] != NULL) {
            al_destroy_bitmap(frames_zombie_r_atacar[i]);
            frames_zombie_r_atacar[i] = NULL;
        }
    }

    for (i = 0; i < FRAMES_ZOMBIE_D; i++) {
        if (frames_zombie_d[i] != NULL) {
            al_destroy_bitmap(frames_zombie_d[i]);
            frames_zombie_d[i] = NULL;
        }
    }

    if (sprite_volador_abajo != NULL) {
        al_destroy_bitmap(sprite_volador_abajo);
        sprite_volador_abajo = NULL;
    }

    if (sprite_volador_diag_izq != NULL) {
        al_destroy_bitmap(sprite_volador_diag_izq);
        sprite_volador_diag_izq = NULL;
    }

    if (sprite_volador_diag_der != NULL) {
        al_destroy_bitmap(sprite_volador_diag_der);
        sprite_volador_diag_der = NULL;
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

    if (e->tipo == zombie_d) {
        total_frames = FRAMES_ZOMBIE_D;
        cambio_frame = CAMBIO_FRAME_ZOMBIE_D;
    }
    else if (e->tipo == zombie_r) {

        switch (e->animacion) {
        case CAMINAR:
            total_frames = FRAMES_ZOMBIE_R_CAMINAR;
            cambio_frame = CAMBIO_FRAME_ZOMBIE_R_CAMINAR;
            break;

        case ATACAR:
            total_frames = FRAMES_ZOMBIE_R_ATACAR;
            cambio_frame = CAMBIO_FRAME_ZOMBIE_R_ATACAR;
            break;

        default:
            return;
        }
    }

    else {

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

bool colision_enemigo(enemigo* e, personaje* p) {

    if (e->x + e->ancho < p->x) {
        return false;
    }

    if (e->x > p->x + p->ancho) {
        return false;
    }

    if (e->y + e->alto < p->y) {
        return false;
    }

    if (e->y > p->y + p->alto) {
        return false;
    }
    return true;
}

void actualizar_ataque_enemigo(enemigo* e, personaje* p) {

    if (e->tipo == zombie_d) {
        return;
    }

    if (e->activo == false) {
        return;
    }

    if (e->cooldown_ataque > 0) {
        e->cooldown_ataque--;
    }

    if (colision_enemigo(e, p)) {
        e->atacando = true;


        if (e->x + e->ancho / 2.0f < p->x + p->ancho) {
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

void disparo_enemgio(enemigo* e, bala balas_enemigo[], personaje* p) {
    int i;

    int direccion_bala_x = 0; 

    float centro_enemigo_x;
    float centro_enemigo_y;

    float centro_jugador_x;

    if (e->activo == false) {
        return;
    }

    centro_enemigo_x = e->x + e->ancho / 2.0f;
    centro_enemigo_y = e->y + e->alto / 2.0f;

    centro_jugador_x = p->x + p->ancho / 2.0f;

    direccion_bala_x = e->direccionx;


    for (i = 0; i < max_balas_p; i++) {
        if (balas_enemigo[i].activa == false) {

            balas_enemigo[i].x = centro_enemigo_x - ancho_bala/ 2.0f;
            balas_enemigo[i].y = centro_enemigo_y + alto_bala/ 2.0f;

            balas_enemigo[i].ancho = ancho_bala;
            balas_enemigo[i].alto = alto_bala;

            balas_enemigo[i].velocidad_bx = direccion_bala_x * velocidad_bala;
            balas_enemigo[i].velocidad_by = 0;
            balas_enemigo[i].frame_actual = 0;
            balas_enemigo[i].contador_animacion = 0;
            balas_enemigo[i].activa = true;

            return;
        }
    }
}

void disparo_enemigo_v(enemigo* e, bala balas_enemigo[], personaje* p) {
    int i;

    int direccion_bala_x = 0;
    int direccion_bala_y = 0;

    float centro_enemigo_x;
    float centro_enemigo_y;

    float centro_jugador_x;
    float centro_jugador_y;

    float margen_x = 10.0f;

    if (e->activo == false) {
        return;
    }

    centro_enemigo_x = e->x + e->ancho / 2.0f;
    centro_enemigo_y = e->y + e->alto / 2.0f;

    centro_jugador_x = p->x + p->ancho / 2.0f;
    centro_jugador_y = p->y + p->alto / 2.0f;

    if (centro_jugador_x > centro_enemigo_x + margen_x) {
        direccion_bala_x = 1;
    }
    else if (centro_jugador_x < centro_enemigo_x - margen_x) {
        direccion_bala_x = -1;
    }
    else {
        direccion_bala_x = 0;
    }

    if (centro_jugador_y > centro_enemigo_y) {
        direccion_bala_y = 1;
    }
    else {
        direccion_bala_y = 0;
    }

    if (direccion_bala_x == 0 && direccion_bala_y == 0) {
        return;
    }

    for (i = 0; i < max_balas_p; i++) {
        if (balas_enemigo[i].activa == false) {

            balas_enemigo[i].x = centro_enemigo_x - ancho_bala / 2.0f;
            balas_enemigo[i].y = centro_enemigo_y - alto_bala / 2.0f;

            balas_enemigo[i].ancho = ancho_bala;
            balas_enemigo[i].alto = alto_bala;

            balas_enemigo[i].velocidad_bx = direccion_bala_x * velocidad_bala;
            balas_enemigo[i].velocidad_by = direccion_bala_y * velocidad_bala;

            e->direccionx_disp = direccion_bala_x;
            e->direcciony_disp = direccion_bala_y;

            balas_enemigo[i].frame_actual = 0;
            balas_enemigo[i].contador_animacion = 0;
            balas_enemigo[i].activa = true;

            return;
        }
    }
}

void disparo_zombie_v(enemigo enemigos[], personaje* p) {
    int i;
    float distancia;

    for (i = 0; i < max_enemigos; i++) {

        if (enemigos[i].activo == true) {
            if (enemigos[i].tipo == volador) {

                distancia = (enemigos[i].x + enemigos[i].ancho / 2) - (p->x - p->ancho / 2);

                if (distancia < 0) {
                    distancia = distancia * -1;
                }
                if (distancia > enemigos[i].rango_vision) {
                    continue;
                }

                if (enemigos[i].cooldown_disparo > 0) {
                    enemigos[i].cooldown_disparo--;
                }
                if (enemigos[i].cooldown_disparo == 0) {
                    disparo_enemigo_v(&enemigos[i], enemigos[i].balas_enemigo, p);
                    enemigos[i].cooldown_disparo = 30;
                }
            }
        }
    }
}

void disparo_zombie_d(enemigo enemigos[], personaje* p) {
    int i;
    float distanciax;
    float distanciay;
    bool jugador_delante;

    for (i = 0; i < max_enemigos; i++) {

        if (enemigos[i].activo == true) {
            if (enemigos[i].tipo == zombie_d) {
                jugador_delante = false;
                distanciax = (p->x - p->ancho / 2.0f) - (enemigos[i].x + enemigos[i].ancho / 2.0f);
                distanciay = (p->y - p->alto / 2.0f) - (enemigos[i].y + enemigos[i].alto / 2.0f);

                if (distanciax < 0) {
                    distanciax = distanciax * -1;
                }
                if (distanciay < 0) {
                    distanciay = distanciay * -1;
                }
                if (enemigos[i].direccionx == 1 && (p->x - p->ancho / 2.0f) > (enemigos[i].x + enemigos[i].ancho / 2.0f)) {
                    jugador_delante = true;
                }

                if (enemigos[i].direccionx == -1 && (p->x - p->ancho / 2.0f) < (enemigos[i].x + enemigos[i].ancho / 2.0f)) {
                    jugador_delante = true;
                }

                if (distanciax > enemigos[i].rango_vision || distanciay > 40.0f || jugador_delante == false) {
                    continue;
                }

                if (enemigos[i].cooldown_disparo > 0) {
                    enemigos[i].cooldown_disparo--;
                }
                if (enemigos[i].cooldown_disparo == 0) {
                    disparo_enemgio(&enemigos[i], enemigos[i].balas_enemigo, p);
                    enemigos[i].cooldown_disparo = 80;
                }
            }
        }
    }
}

bool colision_bala_personaje(bala* b, personaje* p) {
    
    if (b->x > p->x + p->ancho) {
        return false;
    }

    if (b->x + b->ancho < p->x) {
        return false;
    }

    if (b->y + b->alto < p->y) {
        return false;
    }

    if (b->y > p->y + p->alto) {
        return false;
    }
    return true;
}

void revisar_colison_bala_personaje(bala balas_enemigo[], personaje* p, int dano) {
    int i;

    for (i = 0; i < max_balas_p; i++) {
        
        if (balas_enemigo[i].activa == true) {
            if (colision_bala_personaje(&balas_enemigo[i], p) == true) {
                recibir_dano_personaje(p, dano);
                balas_enemigo[i].activa = false;
            }
        }
    }
}