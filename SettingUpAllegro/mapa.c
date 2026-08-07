#define _CRT_SECURE_NO_WARNINGS
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "mapa.h"
#include "items.h"

ALLEGRO_BITMAP* sprite_calle = NULL;
ALLEGRO_BITMAP* tierra1 = NULL;
ALLEGRO_BITMAP* tierra2 = NULL;
ALLEGRO_BITMAP* tierra3 = NULL;
ALLEGRO_BITMAP* tierram2 = NULL;
ALLEGRO_BITMAP* pasto = NULL;
ALLEGRO_BITMAP* fondo = NULL;
ALLEGRO_BITMAP* sprite_calle1 = NULL;
ALLEGRO_BITMAP* sprite_calle2 = NULL;
ALLEGRO_BITMAP* sprite_calle3 = NULL;
ALLEGRO_BITMAP* sprite_columna_calle = NULL;
ALLEGRO_BITMAP* sprite_pilar_columna = NULL;
ALLEGRO_BITMAP* mansion1 = NULL;
ALLEGRO_BITMAP* mansion2 = NULL;
ALLEGRO_BITMAP* mansion3 = NULL;
ALLEGRO_BITMAP* sprite_noria = NULL;
ALLEGRO_BITMAP* edificio1 = NULL;
ALLEGRO_BITMAP* edificio2 = NULL;
ALLEGRO_BITMAP* edificio3 = NULL;
ALLEGRO_BITMAP* edificio4 = NULL;
ALLEGRO_BITMAP* edificio5 = NULL;
ALLEGRO_BITMAP* sprite_edificio6 = NULL;
ALLEGRO_BITMAP* sprite_caseta = NULL;
ALLEGRO_BITMAP* sprite_barrera = NULL;
ALLEGRO_BITMAP* sprite_plat_movil = NULL;
ALLEGRO_BITMAP* sprite_trampa = NULL;
ALLEGRO_BITMAP* sprite_caja_rompible = NULL;
ALLEGRO_BITMAP* sprite_portal_cerrado = NULL;
ALLEGRO_BITMAP* sprite_portal_abierto = NULL;

char mapa[filas][columnas];

void inicializar_plat(plataforma_movil plataformas_moviles[]) {
    int i;

    for (i = 0; i < max_plat; i++) {
        plataformas_moviles[i].x = 0;
        plataformas_moviles[i].y = 0;
        plataformas_moviles[i].ancho = 0;
        plataformas_moviles[i].alto = 0;
        plataformas_moviles[i].y_inicio = 0;
        plataformas_moviles[i].x_inicio = 0;
        plataformas_moviles[i].velocidad_y = 0;
        plataformas_moviles[i].velocidad_x = 0;
        plataformas_moviles[i].distancia = 0;
        plataformas_moviles[i].direccion_y = 0;
        plataformas_moviles[i].direccion_x = 0;
        plataformas_moviles[i].dano = 0;
        plataformas_moviles[i].activa = false;
    }
}

void crear_plat_novil(plataforma_movil plataformas_moviles[], float x, float y, tipo_plataforma tipo) {
    int i;

    for (i = 0; i < max_plat; i++) {

        if (plataformas_moviles[i].activa == false) {
            plataformas_moviles[i].x = x;
            plataformas_moviles[i].y = y;
            plataformas_moviles[i].ancho = 100;
            plataformas_moviles[i].alto = 20;
            plataformas_moviles[i].tipo = tipo;

            if (tipo == PLAT_VERTICAL) {
                plataformas_moviles[i].y_inicio = y;
                plataformas_moviles[i].direccion_y = 1;
                plataformas_moviles[i].distancia = 250;
                plataformas_moviles[i].velocidad_y = 1.0f;
            }
            if (tipo == PLAT_HORIZ) {
                plataformas_moviles[i].x_inicio = x;
                plataformas_moviles[i].direccion_x = -1;
                plataformas_moviles[i].distancia = 200;
                plataformas_moviles[i].velocidad_x = 2.0f;
            }
            plataformas_moviles[i].activa = true;
            return;
        }
    }
}

void spawn_plat_moviles(plataforma_movil plataformas_moviles[]) {
    int f;
    int c;

    for (f = 0; f < filas; f++) {
        for (c = 0; c < columnas; c++) {
            
            if (mapa[f][c] == 'l') {
                crear_plat_novil(plataformas_moviles, c * ancho_v, f * largo_v, PLAT_VERTICAL);
          
                mapa[f][c] = '.';
            }
            if (mapa[f][c] == 'h') {
                crear_plat_novil(plataformas_moviles, c * ancho_v, f * largo_v, PLAT_HORIZ);

                mapa[f][c] = '.';
            }
        }
    }

}

void mov_plat_moviles(plataforma_movil plataformas_moviles[10]) {
    int i;

    for (i = 0; i < max_plat; i++) {

        if (plataformas_moviles[i].activa == true) {
            if (plataformas_moviles[i].tipo == PLAT_VERTICAL) {

                 plataformas_moviles[i].y += plataformas_moviles[i].velocidad_y * plataformas_moviles[i].direccion_y;

                if (plataformas_moviles[i].y >= plataformas_moviles[i].y_inicio + plataformas_moviles[i].distancia) {
                    plataformas_moviles[i].direccion_y = -1;
                }

                if (plataformas_moviles[i].y <= plataformas_moviles[i].y_inicio) {
                    plataformas_moviles[i].direccion_y = 1;
                }
            }
            if (plataformas_moviles[i].tipo == PLAT_HORIZ) {
                plataformas_moviles[i].x += plataformas_moviles[i].velocidad_x * plataformas_moviles[i].direccion_x;

                if (plataformas_moviles[i].x >= plataformas_moviles[i].x_inicio + plataformas_moviles[i].distancia) {
                    plataformas_moviles[i].direccion_x = -1;
                }

                if (plataformas_moviles[i].x <= plataformas_moviles[i].x_inicio) {
                    plataformas_moviles[i].direccion_x = 1;
                }
            }
        }
    }
}

void dibujo_plat_mov(plataforma_movil plataformas_moviles[], float camara_x, float camara_y) {
    int i;

    for (i = 0; i < max_plat; i++) {

        if (plataformas_moviles[i].activa == true) {

            if (sprite_plat_movil != NULL) {
                al_draw_scaled_bitmap(
                    sprite_plat_movil,
                    0,
                    0,
                    al_get_bitmap_width(sprite_plat_movil),
                    al_get_bitmap_height(sprite_plat_movil),
                    plataformas_moviles[i].x - camara_x,
                    plataformas_moviles[i].y - camara_y,
                    plataformas_moviles[i].ancho,
                    plataformas_moviles[i].alto,
                    0
                );
            }
            else {
                al_draw_filled_rectangle(
                    plataformas_moviles[i].x - camara_x,
                    plataformas_moviles[i].y - camara_y,
                    plataformas_moviles[i].x - camara_x + plataformas_moviles[i].ancho,
                    plataformas_moviles[i].y - camara_y + plataformas_moviles[i].alto,
                    al_map_rgb(150, 150, 150)
                );
            }
        }
    }
}

void colision_plat_personaje(personaje* p, plataforma_movil plataformas_moviles[]) {
    int i;
    float jugador_izq;
    float jugador_der;
    float jugador;
    float plat_izq;
    float plat_der;
    float plat_arriba;

    for (i = 0; i < max_plat; i++) {

        if (plataformas_moviles[i].activa == true) {

            jugador_izq = p->x;
            jugador_der = p->x + p->ancho;
            jugador = p->y + p->alto;

            plat_izq = plataformas_moviles[i].x;
            plat_der = plataformas_moviles[i].x + plataformas_moviles[i].ancho;
            plat_arriba = plataformas_moviles[i].y;

            if (p->velocidady >= 0) {
                if (jugador_der > plat_izq && jugador_izq < plat_der) {
                    if (jugador >= plat_arriba - 20 && jugador <= plat_arriba + 20) {

                        p->y = plat_arriba - p->alto;
                        p->velocidady = 0;
                        p->en_suelo = true;
                    }
                }
            }
        }
    }
}


int fisicas_mapa(int pos_x, int pos_y) {
    int col = pos_x / ancho_v;
    int fila = pos_y / largo_v;
    char bloque;

    if (col < 0 || col >= columnas) {
        return 1; // para solidos return 1 y no s. return 0
    }

    if (fila < 0) {
        return 1; //condicion para no subir mas de la fila "40"
    }

    if (fila >= filas) {
        return 1; // hacer suelo solido debajo de la fila 40
    }

    bloque = mapa[fila][col];

    switch (bloque) {
    case 'P':
        return 1;

    case 'r':
       romper_plataforma(fila);
        return 1;
    case '1':
        return 1;
    case '2':
        return 1;
    case '3':
        return 1;
    case '4':
        return 1;
    case '5':
        return 1;
    case '6':
        return 1;
    case 'A':
        return 1;
    case 'B':
        return 1;
    case 'C':
        return 1;
    case 'T':
        return 1;
    case 'y':
        return 1;
    case 'u':
        return 1;
    case 'i':
        return 1;
    case 'w':
        return 1;
    case 'Q':
        return 1;
    case 'H':
        return 1;
    case 'O':
        return 1;
    case 'N':
        return 1;
    case '?':
        return 1;
    case '{':
        return 1;
    }

    return 0;
}

void dibujar_mapa(float camara_x, float camara_y) {
    int fila;
    int col;
    char bloque;

    for (fila = 0; fila < filas; fila++) {
        for (col = 0; col < columnas; col++) {

            bloque = mapa[fila][col];

            float x = col * ancho_v - camara_x;
            float y = fila * largo_v - camara_y;

            switch (bloque) {
            case 'P':
                al_draw_filled_rectangle(x, y, x + ancho_v, y + largo_v,
                    al_map_rgb(139, 90, 43));
                break;
            case 'r':
                al_draw_filled_rectangle(x, y, x + ancho_v, y + largo_v,
                    al_map_rgb(250, 250, 250));
                break;
            case '1':
                dibujar_tile_calle(0, 0, x, y);
                break;

            case '2':
                dibujar_tile_calle(1, 0, x, y);
                break;

            case '3':
                dibujar_tile_calle(0, 1, x, y);
                break;

            case '4':
                dibujar_tile_calle(1, 1, x, y);
                break;

            case '5':
                dibujar_tile_calle(0, 2, x, y);
                break;

            case '6':
                dibujar_tile_calle(1, 2, x, y);
                break;
            case 'A':
                al_draw_bitmap(tierra1, x, y, 0);
                break;

            case 'B':
                al_draw_bitmap(tierra2, x, y, 0);
                break;

            case 'C':
                al_draw_bitmap(tierra3, x, y, 0);
                break;
            case 'T':
                al_draw_bitmap(tierram2, x, y, 0);
                break;
            case 'y':
                al_draw_bitmap(pasto, x, y, 0);
                break;
            case 'x':
                al_draw_scaled_bitmap(
                sprite_trampa,
                    0,
                    0,
                    al_get_bitmap_width(sprite_trampa),
                    al_get_bitmap_height(sprite_trampa),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                    );
                break;
            case 'u':
                al_draw_scaled_bitmap(
                    sprite_calle1,
                    0,
                    0,
                    al_get_bitmap_width(sprite_calle1),
                    al_get_bitmap_height(sprite_calle1),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
                break;
            case 'i':
                al_draw_scaled_bitmap(
                    sprite_calle2,
                    0,
                    0,
                    al_get_bitmap_width(sprite_calle2),
                    al_get_bitmap_height(sprite_calle2),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
                break;
            case 'w':
                al_draw_scaled_bitmap(
                    sprite_calle3,
                    0,
                    0,
                    al_get_bitmap_width(sprite_calle3),
                    al_get_bitmap_height(sprite_calle3),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
                break;
            case 'I':
                al_draw_scaled_bitmap(
                    sprite_columna_calle,
                    0,
                    0,
                    al_get_bitmap_width(sprite_columna_calle),
                    al_get_bitmap_height(sprite_columna_calle),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
                break;
            case 'g':
                al_draw_scaled_bitmap(
                    sprite_pilar_columna,
                    0,
                    0,
                    al_get_bitmap_width(sprite_pilar_columna),
                    al_get_bitmap_height(sprite_pilar_columna),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
                break;
            case 'Q':
                al_draw_filled_rectangle(
                    x,
                    y,
                    x + ancho_v,
                    y + largo_v,
                    al_map_rgb(0, 200, 255)
                );
            case 'H':
                al_draw_scaled_bitmap(
                    sprite_caja_rompible,
                    0,
                    0,
                    al_get_bitmap_width(sprite_caja_rompible),
                    al_get_bitmap_height(sprite_caja_rompible),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
            case 'O':
                al_draw_scaled_bitmap(
                    sprite_caja_rompible,
                    0,
                    0,
                    al_get_bitmap_width(sprite_caja_rompible),
                    al_get_bitmap_height(sprite_caja_rompible),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
            case 'N':
                al_draw_scaled_bitmap(
                    sprite_caja_rompible,
                    0,
                    0,
                    al_get_bitmap_width(sprite_caja_rompible),
                    al_get_bitmap_height(sprite_caja_rompible),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
            case '?':
                al_draw_scaled_bitmap(
                    sprite_caja_rompible,
                    0,
                    0,
                    al_get_bitmap_width(sprite_caja_rompible),
                    al_get_bitmap_height(sprite_caja_rompible),
                    x,
                    y,
                    ancho_v,
                    largo_v,
                    0
                );
            /*case '{':
                al_draw_filled_rectangle(
                    x,
                    y,
                    x + ancho_v,
                    y + largo_v,
                    al_map_rgb(0, 200, 255)
                );*/ 
            }
        }
    }
}

int dano_plat(personaje* p) {
    int col = p->x / ancho_v;
    int fila = p->y/ largo_v;
    char bloque;

    if (col < 0 || col >= columnas) {
        return 1; // para solidos return 1 y no s. return 0
    }

    if (fila < 0) {
        return 1; //condicion para no subir mas de la fila "40"
    }

    if (fila >= filas) {
        return 1; // hacer suelo solido debajo de la fila 40
    }

    bloque = mapa[fila][col];

    switch (bloque) {
    case 'x':
        recibir_dano_personaje(p, 1);
        return 1;
    }

    return 0;
}

void romper_plataforma(int fila) {
    int j;

    for (j = 0; j < columnas; j++) {

        if (mapa[fila][j] == 'r') {
            mapa[fila][j] = '.';
        }

    }

}

bool cargar_sprites_mapa() {
sprite_calle = al_load_bitmap("assets/sprite_calle1.png");

    if (sprite_calle == NULL) {
        printf("No se pudo cargar sprite_calle1.png\n");
        return false;
    }

    tierra1 = al_load_bitmap("assets/tierra1.png");
    tierra2 = al_load_bitmap("assets/tierra2.png");
    tierra3 = al_load_bitmap("assets/tierra3.png");
    tierram2 = al_load_bitmap("assets/tierram2.png");
    pasto = al_load_bitmap("assets/pasto2.png");

    if (tierra1 == NULL || tierra2 == NULL || tierra3 == NULL || tierram2 == NULL || pasto == NULL) {
        printf("No se pudo cargar una imagen de tierra\n");
        return false;
    }

    sprite_calle1 = al_load_bitmap("assets/mapa 2/calle1.png");
    sprite_calle2 = al_load_bitmap("assets/mapa 2/calle2.png");
    sprite_calle3 = al_load_bitmap("assets/mapa 2/calle3.png");
    sprite_columna_calle = al_load_bitmap("assets/mapa 2/columna_calle.png");
    sprite_pilar_columna = al_load_bitmap("assets/mapa 2/pilar_columna.png");
    
    if (sprite_calle1 == NULL) {
        printf("No se pudo cargar calle1.png\n");
        return false;
    }

    if (sprite_calle2 == NULL) {
        printf("No se pudo cargar calle2.png\n");
        return false;
    }

    if (sprite_calle3 == NULL) {
        printf("No se pudo cargar calle3.png\n");
        return false;
    }

    if (sprite_columna_calle == NULL) {
        printf("No se pudo cargar columna_calle.png\n");
        return false;
    }

    if (sprite_pilar_columna == NULL) {
        printf("No se pudo cargar pilar_columna.png\n");
        return false;
    }
    mansion1 = al_load_bitmap("assets/mapa 1/mansion1.png");
    mansion2 = al_load_bitmap("assets/mapa 1/mansion2.png");
    mansion3 = al_load_bitmap("assets/mapa 1/mansion3.png");

    if (mansion1 == NULL) {
        printf("No se pudo cargar mansion1.png\n");
        return false;
    }

    if (mansion2 == NULL) {
        printf("No se pudo cargar mansion2.png\n");
        return false;
    }

    if (mansion3 == NULL) {
        printf("No se pudo cargar mansion3.png\n");
        return false;
    }

    sprite_noria = al_load_bitmap("assets/mapa 1/noria.png");

    if (sprite_noria == NULL) {
        printf("No se pudo cargar noria.png\n");
        return false;
    }
    edificio1 = al_load_bitmap("assets/mapa4/edificio1.png");
    edificio2 = al_load_bitmap("assets/mapa4/edificio2.png");
    edificio3 = al_load_bitmap("assets/mapa4/edificio3.png");
    edificio4 = al_load_bitmap("assets/mapa4/edificio4.png");
    edificio5 = al_load_bitmap("assets/mapa4/edificio5.png");

    if (edificio1 == NULL) {
        printf("No se pudo cargar edificio1.png\n");
        return false;
    }

    if (edificio2 == NULL) {
        printf("No se pudo cargar edificio2.png\n");
        return false;
    }

    if (edificio3 == NULL) {
        printf("No se pudo cargar edificio3.png\n");
        return false;
    }

    if (edificio4 == NULL) {
        printf("No se pudo cargar edificio4.png\n");
        return false;
    }

    if (edificio5 == NULL) {
        printf("No se pudo cargar edificio5.png\n");
        return false;
    }

    sprite_edificio6 = al_load_bitmap("assets/mapa4/edificio6.png");

    if (sprite_edificio6 == NULL) {
        printf("No se pudo cargar edificio6.png\n");
        return false;
    }

    sprite_caseta = al_load_bitmap("assets/mapa4/caseta.png");
    sprite_barrera = al_load_bitmap("assets/mapa4/barrera.png");

    sprite_plat_movil = al_load_bitmap("assets/plat_movil.png");

    if (sprite_plat_movil == NULL) {
        printf("No se pudo cargar plat_movil.png\n");
        return false;
    }

    sprite_trampa = al_load_bitmap("assets/trampa.png");

    if (sprite_trampa == NULL) {
        printf("No se pudo cargar el sprite del bloque de trampa\n");
        return false;
    }

    sprite_caja_rompible = al_load_bitmap("assets/caja.png");
    if (sprite_caja_rompible == NULL) {
        printf("No se pudo cargar sprite_caja_rompible\n");
        return false;
    }

    sprite_portal_cerrado = al_load_bitmap("assets/puerta_cerrada.png");
    sprite_portal_abierto = al_load_bitmap("assets/puerta_abierta.png");

    if (sprite_portal_cerrado == NULL) {
        printf("No se pudo cargar portal_cerrado.png\n");
        return false;
    }

    if (sprite_portal_abierto == NULL) {
        printf("No se pudo cargar portal_abierto.png\n");
        return false;
    }


    return true;
}

void dibujar_tile_calle(int columna_sprite, int fila_sprite, float x, float y) {
   
    al_draw_bitmap_region(sprite_calle,columna_sprite * 20,fila_sprite * 20,20,20,x,y,0);

}

void liberar_sprites_mapa() {
    if (sprite_calle != NULL) {
        al_destroy_bitmap(sprite_calle);
        sprite_calle = NULL;
    }

    if (tierra1 != NULL) {
        al_destroy_bitmap(tierra1);
        tierra1 = NULL;
    }

    if (tierra2 != NULL) {
        al_destroy_bitmap(tierra2);
        tierra2 = NULL;
    }

    if (tierra3 != NULL) {
        al_destroy_bitmap(tierra3);
        tierra3 = NULL;
    }

    if (fondo != NULL) {
        al_destroy_bitmap(fondo);
        fondo = NULL;
    }

    if (sprite_noria != NULL) {
        al_destroy_bitmap(sprite_noria);
        sprite_noria = NULL;
    }

    if (edificio1 != NULL) {
        al_destroy_bitmap(edificio1);
        edificio1 = NULL;
    }

    if (edificio2 != NULL) {
        al_destroy_bitmap(edificio2);
        edificio2 = NULL;
    }

    if (edificio3 != NULL) {
        al_destroy_bitmap(edificio3);
        edificio3 = NULL;
    }

    if (edificio4 != NULL) {
        al_destroy_bitmap(edificio4);
        edificio4 = NULL;
    }

    if (edificio5 != NULL) {
        al_destroy_bitmap(edificio5);
        edificio5 = NULL;
    }
    if (sprite_edificio6 != NULL) {
        al_destroy_bitmap(sprite_edificio6);
        sprite_edificio6 = NULL;
    }

    if (sprite_caseta != NULL) {
        al_destroy_bitmap(sprite_caseta);
        sprite_caseta = NULL;
    }

    if (sprite_barrera != NULL) {
        al_destroy_bitmap(sprite_barrera);
        sprite_barrera = NULL;
    }

    if (sprite_plat_movil != NULL) {
        al_destroy_bitmap(sprite_plat_movil);
        sprite_plat_movil = NULL;
    }

    if (sprite_trampa != NULL) {
        al_destroy_bitmap(sprite_trampa);
        sprite_trampa = NULL;
    }

    if (sprite_caja_rompible != NULL) {
        al_destroy_bitmap(sprite_caja_rompible);
        sprite_caja_rompible = NULL;
    }

    if (sprite_portal_cerrado != NULL) {
        al_destroy_bitmap(sprite_portal_cerrado);
        sprite_portal_cerrado = NULL;
    }

    if (sprite_portal_abierto != NULL) {
        al_destroy_bitmap(sprite_portal_abierto);
        sprite_portal_abierto = NULL;
    }
}


void dibujar_fondo(float camara_x) {
    int ancho_fondo = 1455;
    int mov;
    int x;

    if (fondo == NULL) {
        return;
    }

    mov = (int)(camara_x * 0.06f);

    x = -(mov % ancho_fondo);

    while (x < 1455) {

        al_draw_scaled_bitmap(fondo,0,0,al_get_bitmap_width(fondo),al_get_bitmap_height(fondo),x,0,ancho_fondo,800,0);

        x += ancho_fondo;
    }
}

void mapas(char txt[], int nivel) {
    char numero[2];

    strcpy(txt, "mapa");

    numero[0] = '0' + nivel;
    numero[1] = '\0';

    strcat(txt, numero);
    strcat(txt, ".txt");
}

bool portal(personaje* p) {
    int col;
    int fila;

    int centro_x;
    int centro_y;

    centro_x = p->x + p->ancho / 2.0f;
    centro_y = p->y + p->alto / 2.0f;

    col = centro_x / ancho_v;
    fila = centro_y / largo_v;

    if (p->llave < p->llaves_nivel) {
        return false;
    }

    if (fila < 0 || fila >= filas || col < 0 || col >= columnas) {
        return false;
    }

    if (mapa[fila][col] == 'Z') {
        return true;
    }

    return false;
}

bool cargar_fondos(int nivel) {
   
    if (fondo != NULL) {
        al_destroy_bitmap(fondo);
        fondo = NULL;
    }

    if (nivel == 1) {
        fondo = al_load_bitmap("assets/fondo 5.png");
    }
    else if (nivel == 2) {
        fondo = al_load_bitmap("assets/fondo2.png");
    }
    else if (nivel == 3) {
        fondo = al_load_bitmap("assets/fondol3.png");
    }
    else if (nivel == 4) {
        fondo = al_load_bitmap("assets/mapa4/fondom4.png");
    }

    if (fondo == NULL) {
        printf("No se pudo cargar el fondo del nivel %d\n", nivel);
        return false;
    }

    return true;
}

int cargar_mapa(const char* txt, int* llaves_necesarias) {
    FILE* f;
    char linea[columnas + 1];
    int fila;
    int col;

    f = fopen(txt, "r");

    if (f == NULL) {
        printf("No se pudo abrir el archivo: %s\n", txt);
        return 0;
    }

    fscanf(f, "%d", llaves_necesarias);
    fgetc(f); 

    for (fila = 0; fila < filas; fila++) {

        if (fgets(linea, sizeof(linea), f) == NULL) {
            printf("Error: faltan filas en el mapa\n");
            fclose(f);
            return 0;
        }

        for (col = 0; col < columnas; col++) {
            mapa[fila][col] = linea[col];
        }
    }

    fclose(f);
    return 1;
}

int romper_caja(int x, int y, item items[]) {
    int col;
    int f;
    char caja;
    col = x / ancho_v;
    f = y / largo_v;

    if (f < 0 || f >= filas || col < 0 || col >= columnas) {
        return 0;
    }

    caja = mapa[f][col];

    if (mapa[f][col] == 'Q' ||
        mapa[f][col] == 'H' ||
        mapa[f][col] == 'N' ||
        mapa[f][col] == '?' ||
        mapa[f][col] == 'O') {

        mapa[f][col] = '.';

        if (caja == 'Q') {
            crear_item(items, col * ancho_v, (f - 1) * largo_v, ESCUDO, 5);
        }
        if (caja == 'H') {
            crear_item(items, col * ancho_v, (f - 1) * largo_v, MONEDA, 2000);
        }
        if (caja == 'N') {
            crear_item(items, col * ancho_v, (f - 1) * largo_v, GRANADA, 3);
        }
        if (caja == '?') {
            crear_item(items, col * ancho_v, (f - 1) * largo_v, LLAVE, 1);
        }


        return 1;
    }

    return 0;
}

void dibujar_mansion(ALLEGRO_BITMAP* mansion, float x, float y, float ancho, float alto, float camara_x, float camara_y) {

    if (mansion == NULL) {
        return;
    }

    al_draw_scaled_bitmap(
        mansion,
        0,
        0,
        al_get_bitmap_width(mansion),
        al_get_bitmap_height(mansion),
        x - camara_x,
        y - camara_y,
        ancho,
        alto,
        0
    );
}

void dibujar_mansiones_nivel1(float camara_x, float camara_y) {

    dibujar_mansion(
        mansion1,
        0,
        460,
        900,
        420,
        camara_x,
        camara_y
    );

    dibujar_mansion(
        mansion2,
        1300,
        445,
        850,
        400,
        camara_x,
        camara_y
    );

    dibujar_mansion(
        mansion3,
        2600,
        482,
        900,
        410,
        camara_x,
        camara_y
    );

    if (sprite_noria != NULL) {
        al_draw_scaled_bitmap(
            sprite_noria,
            0,
            0,
            al_get_bitmap_width(sprite_noria),
            al_get_bitmap_height(sprite_noria),
            3600 - camara_x,
            430 - camara_y,
            300,
            420,
            0
        );
    }
}

void dibujar_decoraciones_nivel4(float camara_x, float camara_y) {

    dibujar_mansion(
        edificio1,
        50,
        258,
        850,
        560,
        camara_x,
        camara_y
    );

    dibujar_mansion(
        edificio2,
        870,
        160,
        500,
        700,
        camara_x,
        camara_y
    );

    dibujar_mansion(
        edificio3,
        1400,
        117,
        560,
        720,
        camara_x,
        camara_y
    );

    dibujar_mansion(
        edificio4,
        2000,
        175,
        480,
        680,
        camara_x,
        camara_y
    );

    dibujar_mansion(
        edificio5,
        2470,
        130,
        430,
        700,
        camara_x,
        camara_y
    );

    if (sprite_edificio6 != NULL) {
        al_draw_scaled_bitmap(
            sprite_edificio6,
            0,
            0,
            al_get_bitmap_width(sprite_edificio6),
            al_get_bitmap_height(sprite_edificio6),
            3100 - camara_x,
            45 - camara_y,
            420,
            850,
            0
        );
    }

    al_draw_scaled_bitmap(
        sprite_caseta,
        0,
        0,
        al_get_bitmap_width(sprite_caseta),
        al_get_bitmap_height(sprite_caseta),
        3570 - camara_x,
        580 - camara_y,
        420,
        320,
        0
    );

    al_draw_scaled_bitmap(
        sprite_barrera,
        0,
        0,
        al_get_bitmap_width(sprite_barrera),
        al_get_bitmap_height(sprite_barrera),
        3520 - camara_x,
        395 - camara_y,
        90,
        520,
        0
    );
}

void dibujar_portal_mapa(float camara_x, float camara_y, personaje* p) {
    int fila;
    int col;
    ALLEGRO_BITMAP* sprite;
    float x;
    float y;
    float alto_dibujo;
    float ancho_dibujo;

    for (fila = 0; fila < filas; fila++) {
        for (col = 0; col < columnas; col++) {

            if (mapa[fila][col] == 'Z') {

                if (p->llave >= p->llaves_nivel) {
                    sprite = sprite_portal_abierto;
                }
                else {
                    sprite = sprite_portal_cerrado;
                }

                if (sprite == NULL) {
                    return;
                }

                alto_dibujo = 80.0f;

                ancho_dibujo = alto_dibujo *
                    al_get_bitmap_width(sprite) /
                    al_get_bitmap_height(sprite);

                x = col * ancho_v - camara_x;
                y = fila * largo_v - camara_y;

                al_draw_scaled_bitmap(
                    sprite,
                    0,
                    0,
                    al_get_bitmap_width(sprite),
                    al_get_bitmap_height(sprite),

                    x - (ancho_dibujo - ancho_v) / 2.0f,
                    y - (alto_dibujo - largo_v) +10,

                    ancho_dibujo,
                    alto_dibujo,
                    0
                );
            }
        }
    }
}

void romper_cajas_granada(float centro_x, float centro_y, int rango, item items[]) {
    int col_inicio;
    int col_fin;
    int fila_inicio;
    int fila_fin;

    int fila;
    int col;

    col_inicio = (int)(centro_x - rango) / ancho_v;
    col_fin = (int)(centro_x + rango) / ancho_v;

    fila_inicio = (int)(centro_y - rango) / largo_v;
    fila_fin = (int)(centro_y + rango) / largo_v;

    if (col_inicio < 0) {
        col_inicio = 0;
    }

    if (fila_inicio < 0) {
        fila_inicio = 0;
    }

    if (col_fin >= columnas) {
        col_fin = columnas - 1;
    }

    if (fila_fin >= filas) {
        fila_fin = filas - 1;
    }

    for (fila = fila_inicio; fila <= fila_fin; fila++) {
        for (col = col_inicio; col <= col_fin; col++) {

            if (mapa[fila][col] == 'Q' ||
                mapa[fila][col] == 'H' ||
                mapa[fila][col] == 'N' ||
                mapa[fila][col] == '?' ||
                mapa[fila][col] == 'O') {

                romper_caja(col * ancho_v, fila * largo_v, items);
            }
        }
    }
}