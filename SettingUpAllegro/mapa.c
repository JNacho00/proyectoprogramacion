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

ALLEGRO_BITMAP* sprite_calle = NULL;
ALLEGRO_BITMAP* tierra1 = NULL;
ALLEGRO_BITMAP* tierra2 = NULL;
ALLEGRO_BITMAP* tierra3 = NULL;
ALLEGRO_BITMAP* tierram2 = NULL;
ALLEGRO_BITMAP* pasto = NULL;
ALLEGRO_BITMAP* fondo = NULL;

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
                plataformas_moviles[i].distancia = 150;
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
    float x, y;

    for (i = 0; i < max_plat; i++) {

        if (plataformas_moviles[i].activa == true) {
            if (plataformas_moviles[i].tipo == PLAT_VERTICAL) {

                x = plataformas_moviles[i].x - camara_x;
                y = plataformas_moviles[i].y - camara_y;

                al_draw_filled_rectangle(
                    x,
                    y,
                    x + plataformas_moviles[i].ancho,
                    y + plataformas_moviles[i].alto,
                    al_map_rgb(255, 255, 255)
                );
            }
        }
        if (plataformas_moviles[i].tipo == PLAT_HORIZ) {

            x = plataformas_moviles[i].x - camara_x;
            y = plataformas_moviles[i].y - camara_y;

            al_draw_filled_rectangle(
                x,
                y,
                x + plataformas_moviles[i].ancho,
                y + plataformas_moviles[i].alto,
                al_map_rgb(255, 255, 255)
            );
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
            case 'Z':
                al_draw_filled_rectangle(
                    x,
                    y,
                    x + ancho_v,
                    y + largo_v,
                    al_map_rgb(0, 200, 255)
                );
                break;
            case 'T':
                al_draw_bitmap(tierram2, x, y, 0);
                break;
            case 'y':
                al_draw_bitmap(pasto, x, y, 0);
                break;
            case 'x':
                al_draw_filled_rectangle(
                    x,
                    y,
                    x + ancho_v,
                    y + largo_v,
                    al_map_rgb(0, 200, 255));
                break;
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

    if (p->agarro_llave == false) {
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
        fondo = al_load_bitmap("assets/imagen_fondo2.png");
    }
    else if (nivel == 2) {
        fondo = al_load_bitmap("assets/fondo2.png");
    }

    if (fondo == NULL) {
        printf("No se pudo cargar el fondo del nivel %d\n", nivel);
        return false;
    }

    return true;
}

int cargar_mapa(const char* txt) {
    FILE* f;
    int i;
    int c;

    f = fopen(txt, "r");

    if (f == NULL) {
        printf("No se pudo abrir el archivo: %s\n", txt);
        return 0;
    }

    for (i = 0; i < filas; i++) {
        for (c = 0; c < columnas; c++) {
            fscanf(f, "%c", &mapa[i][c]);
        }
    }

    

    fclose(f);
    return 1;
}