#ifndef sonidos_h
#define sonidos_h
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>

bool iniciar_sonidos(void);
bool cargar_sonidos(void);
void liberar_sonidos(void);

void reproducir_disparo(void);
void reproducir_explosion(void);
void reproducir_vida_escudo(void);
void reproducir_moneda(void);
void reproducir_pasar_nivel(void);
void reproducir_dano_personaje(void);
void reproducir_dano_zombie(void);
#endif 
