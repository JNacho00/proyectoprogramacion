#include "sonidos.h"
#include <stdio.h>

ALLEGRO_SAMPLE* sonido_disparo = NULL;
ALLEGRO_SAMPLE* sonido_explosion = NULL;
ALLEGRO_SAMPLE* sonido_items_curativos = NULL;
ALLEGRO_SAMPLE* sonido_moneda = NULL;
ALLEGRO_SAMPLE* sonido_pasar_nivel = NULL;
ALLEGRO_SAMPLE* sonido_dano_personaje = NULL;
ALLEGRO_SAMPLE* sonido_dano_zombie = NULL;



bool iniciar_sonidos(void) {
    if (al_install_audio() == false) {
        printf("No se pudo iniciar el audio\n");
        return false;
    }

    if (al_init_acodec_addon() == false) {
        printf("No se pudo iniciar el acodec\n");
        return false;
    }

    if (al_reserve_samples(20) == false) {
        printf("No se pudo reservar sonidos\n");
        return false;
    }

    return true;
}

bool cargar_sonidos(void) {

    sonido_disparo = al_load_sample("assets/sonidos/disparo.mp3");
    sonido_explosion = al_load_sample("assets/sonidos/explosion.wav");
    sonido_items_curativos = al_load_sample("assets/sonidos/vida_y_escudo.wav");
    sonido_moneda = al_load_sample("assets/sonidos/moneda.wav");
    sonido_pasar_nivel = al_load_sample("assets/sonidos/pasar_nivel.wav");
    sonido_dano_personaje = al_load_sample("assets/sonidos/dano_personaje.wav");
    sonido_dano_zombie = al_load_sample("assets/sonidos/dano_zombie.wav");


    if (sonido_disparo == NULL) {
        printf("No se pudo cargar disparo.mp3\n");
        return false;
    }

    if (sonido_explosion == NULL) {
        printf("No se pudo cargar explosion.wav\n");
        return false;
    }

    if (sonido_items_curativos == NULL) {
        printf("No se pudo cargar vida_y_escudo.wav\n");
        return false;
    }

    if (sonido_moneda == NULL) {
        printf("No se pudo cargar moneda.wav\n");
        return false;
    }

    if (sonido_pasar_nivel == NULL) {
        printf("No se pudo cargar pasar_nivel.wav\n");
        return false;
    }

    if (sonido_dano_personaje == NULL) {
        printf("No se pudo cargar dano_personaje.wav\n");
        return false;
    }

    if (sonido_dano_zombie == NULL) {
        printf("No se pudo cargar dano_zombie.wav\n");
        return false;
    }
    return true;
}

void liberar_sonidos(void) {

    if (sonido_disparo != NULL) {
        al_destroy_sample(sonido_disparo);
        sonido_disparo = NULL;
    }

    if (sonido_explosion != NULL) {
        al_destroy_sample(sonido_explosion);
        sonido_explosion = NULL;
    }

    if (sonido_items_curativos != NULL) {
        al_destroy_sample(sonido_items_curativos);
        sonido_items_curativos = NULL;
    }

    if (sonido_moneda != NULL) {
        al_destroy_sample(sonido_moneda);
        sonido_moneda = NULL;
    }

    if (sonido_pasar_nivel != NULL) {
        al_destroy_sample(sonido_pasar_nivel);
        sonido_pasar_nivel = NULL;
    }

    if (sonido_dano_personaje != NULL) {
        al_destroy_sample(sonido_dano_personaje);
        sonido_dano_personaje = NULL;
    }

    if (sonido_dano_zombie != NULL) {
        al_destroy_sample(sonido_dano_zombie);
        sonido_dano_zombie = NULL;
    }
}

void reproducir_disparo(void) {

    if (sonido_disparo != NULL) {
        al_play_sample(
            sonido_disparo,
            1.0,
            0.0,
            1.0,
            ALLEGRO_PLAYMODE_ONCE,
            NULL
        );
    }
}

void reproducir_explosion(void) {

    if (sonido_explosion != NULL) {
        al_play_sample(
            sonido_explosion,
            1.0,
            0.0,
            1.0,
            ALLEGRO_PLAYMODE_ONCE,
            NULL
        );
    }
}

void reproducir_vida_escudo(void) {

    if (sonido_items_curativos != NULL) {
        al_play_sample(
            sonido_items_curativos,
            1.0,
            0.0,
            1.0,
            ALLEGRO_PLAYMODE_ONCE,
            NULL
        );
    }
}

void reproducir_moneda(void) {

    if (sonido_moneda != NULL) {
        al_play_sample(
            sonido_moneda,
            1.0,
            0.0,
            1.0,
            ALLEGRO_PLAYMODE_ONCE,
            NULL
        );
    }
}

void reproducir_pasar_nivel(void) {

    if (sonido_pasar_nivel != NULL) {
        al_play_sample(
            sonido_pasar_nivel,
            1.0,
            0.0,
            1.0,
            ALLEGRO_PLAYMODE_ONCE,
            NULL
        );
    }
}

void reproducir_dano_personaje(void) {

    if (sonido_dano_personaje != NULL) {
        al_play_sample(
            sonido_dano_personaje,
            1.0,
            0.0,
            1.0,
            ALLEGRO_PLAYMODE_ONCE,
            NULL
        );
    }
}

void reproducir_dano_zombie(void) {

    if (sonido_dano_zombie != NULL) {
        al_play_sample(
            sonido_dano_zombie,
            1.0,
            0.0,
            1.0,
            ALLEGRO_PLAYMODE_ONCE,
            NULL
        );
    }
}
