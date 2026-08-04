#ifndef items_h
#define items_h
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_physfs.h>
#include <stdio.h>
#include <stdbool.h>
#include "balas.h"
#include "enemigos.h"

#define max_items 10

typedef enum {

	VIDA,
	MUNICION,
	ESCUDO,
	BARRIL,
	LLAVE,
	MONEDA,
	GRANADA,

} tipo_item;

typedef struct item{
	float x, y;
	int ancho;
	int alto;
	int valor;

	float y_inicio;
	float velocidad_y;
	float distancia;
	int direccion_y;

	int frame_actual;
	int contador_animacion;

	//para el barril
	bool explotando;
	int frame_explosion;
	int contador_explosion;

	tipo_item tipo;
	bool activo;
} item;

typedef struct personaje personaje;

void dibujar_item(item* it, float camara_x, float camara_y);
void inicializar_items(item items[]);
void dibujar_items(item items[], float camara_x, float camara_y);
void crear_item(item items[], float x, float y, tipo_item tipo, int valor);
void spawn_items(item items[]);
bool interaccion_item(item* it, personaje* p);
void actualizar_items(item items[], personaje* p);

bool colision_bala_item(bala* b, item* i);
void explosion_barril(item* barril, enemigo enemigos[], personaje* p);
void colision_bala_barril(item items[], bala balas[], enemigo enemigos[], personaje* p);


bool cargar_sprites_items(void);
void liberar_sprites_items(void);
void actualizar_animacion_item(item* i);
void mover_item(item* i);
void actualizar_animacion_explosion_barril(item* i);
void dibujar_explosion_barril(item* i, float camara_x, float camara_y);


#endif 
