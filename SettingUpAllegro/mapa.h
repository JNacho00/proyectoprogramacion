#ifndef mapa_h
#define mapa_h
#include "personaje.h"
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <allegro5/allegro_image.h>
#include <stdio.h>
#include <stdbool.h>
#define ancho_v 20
#define largo_v 20
#define columnas 200
#define filas 40
#define max_plat 10

typedef enum {

	PLAT_VERTICAL,
	PLAT_HORIZ,

} tipo_plataforma;

typedef struct {
	float x, y;
	float ancho;
	float alto;

	float y_inicio;
	float distancia;
	float velocidad_y;
	float x_inicio;
	float velocidad_x;
	int dano;

	int direccion_y;
	int direccion_x;
	
	tipo_plataforma tipo;
	bool activa;

} plataforma_movil;

typedef struct personaje personaje;
typedef struct item item;
extern char mapa[filas][columnas];


void dibujar_mapa(float camara_x, float camara_y);
int fisicas_mapa(int pos_x, int pos_y);
void romper_plataforma(int fila);
bool cargar_sprites_mapa();
void dibujar_tile_calle(int columna_sprite, int fila_sprite, float x, float y);
void liberar_sprites_mapa();
bool cargar_fondos(int nivel);
bool portal(personaje* p);
void mapas(char ruta[], int nivel);
void dibujar_fondo(float camara_x);
void inicializar_plat(plataforma_movil plataformas_moviles[]);
void crear_plat_novil(plataforma_movil plataformas_moviles[], float x, float y, tipo_plataforma tipo);
void spawn_plat_moviles(plataforma_movil plataformas_moviles[]);
void mov_plat_moviles(plataforma_movil plataformas_moviles[10]);
void dibujo_plat_mov(plataforma_movil plataformas_moviles[], float camara_x, float camara_y);
void colision_plat_personaje(personaje* p, plataforma_movil plataformas_moviles[]);
int dano_plat(personaje* p);
int cargar_mapa(const char* txt, int* llaves);

int romper_caja(int x, int y, item items[]);

#endif