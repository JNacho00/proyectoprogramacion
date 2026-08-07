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
#include "ranking.h"
#include "personaje.h"
#include "mapa.h"
#include "balas.h"
#include "enemigos.h"
#include "items.h"
#include "sonidos.h"
#define ANCHO_PANTALLA 1200
#define ALTO_PANTALLA 800

// estructuras---------------------------

typedef enum {

    MENU,
    CONTROLES,
    INFORMACION,
    INGRESAR_NOMBRE,
    JUGANDO,
    GAME_OVER,
    VICTORIA,
    RANKING,
    SALIR,

} menu;

typedef struct {
    ALLEGRO_DISPLAY* display;
    ALLEGRO_TIMER* timer;
    ALLEGRO_EVENT_QUEUE* queue;
} flujo_juego;

//incializar funciones-----------------------------------

bool inicializar_sistema(flujo_juego* j);
void cerrar_sistema(flujo_juego* j);
void cargar_nivel(int nivel, personaje* p, enemigo enemigos[], item items[], plataforma_movil plataformas_moviles[]);
void dibujar_menu(ALLEGRO_FONT* fuente_menu, int opcion_menu, ALLEGRO_BITMAP* fondo_men);
void dibujar_controles(ALLEGRO_FONT* fuente_titulo, ALLEGRO_FONT* fuente_menu);
void dibujar_boton(ALLEGRO_FONT* fuente, float x1, float y1, float x2, float y2, const char* texto, bool seleccionado);
void dibujar_game_over(ALLEGRO_BITMAP* fondo_game_over, ALLEGRO_FONT* fuente_menu, personaje* p);
void dibujar_ranking(registros_ranking ranking[], int cantidad, int limite, ALLEGRO_FONT* fuente, float x, float y);
void dibujar_ingresar_nombre(ALLEGRO_FONT* fuente_menu, ALLEGRO_BITMAP* fondo_menu, char nombre_jugador[]);
void dibujar_municion(personaje* p, ALLEGRO_FONT* fuente);
void dibujar_introduccion(ALLEGRO_FONT* fuente_menu, ALLEGRO_BITMAP* fondo_menu, ALLEGRO_BITMAP* imagen_introduccion);
void dibujar_victoria(ALLEGRO_BITMAP* fondo_victoria, ALLEGRO_FONT* fuente_menu, personaje* p);
// -----------main--------------------
int main() {
    
    flujo_juego mijuego;
    personaje jugador;
    item items[max_items];
    enemigo enemigos[max_enemigos]; 
    plataforma_movil plataformas_moviles[max_plat];

    menu estado = MENU;
    int opcion_menu = 0;


    float camara_x = 0.0f;
    float camara_y = 0.0f;
    float zoom = 1.2f;

    bool corriendo = true;
    bool redibujar = true;
    bool puntaje_guardado = false;

    ALLEGRO_FONT* fuente_titulo = NULL;
    ALLEGRO_FONT* fuente_menu = NULL;
    ALLEGRO_FONT* fuente_hud = NULL;
    ALLEGRO_BITMAP* fondo_menu = NULL;
    ALLEGRO_BITMAP* fondo_game_over = NULL;
    ALLEGRO_BITMAP* fondo_ranking = NULL;
    ALLEGRO_BITMAP* imagen_introduccion = NULL;
    ALLEGRO_BITMAP* fondo_victoria = NULL;

    if (inicializar_sistema(&mijuego) == false) {
        fprintf(stderr, "Error");
        return -1;
    }

    fuente_titulo = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 70, 0);
    fuente_menu = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 34, 0);
    fuente_hud = al_load_ttf_font("assets/PressStart2P-Regular.ttf", 22, 0);

    if (fuente_titulo == NULL || fuente_menu == NULL) {
        printf("No se pudo cargar la fuente del menu\n");
        cerrar_sistema(&mijuego);
        return -1;
    }

    fondo_menu = al_load_bitmap("assets/fondo_menu.png");

    if (fondo_menu == NULL) {
        printf("No se pudo cargar el fondo del menu\n");
        al_destroy_font(fuente_titulo);
        al_destroy_font(fuente_menu);
        cerrar_sistema(&mijuego);
        return -1;
    }

    fondo_game_over = al_load_bitmap("assets/fondo game_over.png");

    if (fondo_game_over == NULL) {
        printf("No se pudo cargar fondo_game_over.png\n");
        return -1;
    }

    fondo_ranking = al_load_bitmap("assets/fondo_ranking.png");

    if (fondo_ranking == NULL) {
        printf("No se pudo cargar fondo_ranking.png\n");
        return -1;
    }

    imagen_introduccion = al_load_bitmap("assets/intro1.png");

    if (imagen_introduccion == NULL) {
        printf("No se pudo cargar guia_rapida.png\n");
        return -1;
    }

    fondo_victoria = al_load_bitmap("assets/fondo_victoria.png");

    if (fondo_victoria == NULL) {
        printf("No se pudo cargar fondo_victoria.png\n");
        return -1;
    }

    if (cargar_sprites_mapa() == false) {
        return -1;
    }

    if (cargar_sprites_personaje() == false) {
        liberar_sprites_mapa();
        cerrar_sistema(&mijuego);
        return -1;
    }

    if (cargar_sprites_enemigos() == false) {
        return -1;
    }

    if (cargar_sprites_balas() == false) {
        return -1;
    }

    if (cargar_sprites_items() == false) {
        return -1;
    }

    if (iniciar_sonidos() == false) {
        cerrar_sistema(&mijuego);
        return -1;
    }

    if (cargar_sonidos() == false) {
        cerrar_sistema(&mijuego);
        return -1;
    }

    if (cargar_sprites_granadas() == false) {
        return -1;
    }
 
    int nivel_actual = 4;
    char nombre_jugador[max_nombre] = "";
    int letras_nombre = 0;

    cargar_nivel(nivel_actual, &jugador, enemigos, items, plataformas_moviles);
    reproducir_pasar_nivel();
    jugador.puntaje = 0;

    for (int i = 0; i < max_enemigos; i++) {
        spawn_balas(enemigos[i].balas_enemigo);
    }

    while (corriendo) {
        ALLEGRO_EVENT event;
        al_wait_for_event(mijuego.queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            corriendo = false;
        }
//////////////////////////////////////////////////////////timer/////////////////////////////////
        else if (event.type == ALLEGRO_EVENT_TIMER) {

            if (estado == JUGANDO) {

            fisicas(&jugador);
            dano_plat(&jugador);
            mov_plat_moviles(plataformas_moviles);
            colision_plat_personaje(&jugador, plataformas_moviles);
            actualizar_animacion_final_personaje(&jugador);

            if (portal(&jugador)) {
                nivel_actual++;
                jugador.puntaje += 1000;
                if (nivel_actual == 5) {
                    if (puntaje_guardado == false) {
                        guardar_registro_ranking(nombre_jugador, jugador.puntaje);
                        puntaje_guardado = true;
                    }
                    estado = VICTORIA;
                }
                else {
                    cargar_nivel(nivel_actual, &jugador, enemigos, items, plataformas_moviles);
                    reproducir_pasar_nivel();
                    for (int i = 0; i < max_enemigos; i++) {
                        spawn_balas(enemigos[i].balas_enemigo);
                    }

                    camara_x = 0.0f;
                    camara_y = 0.0f;
                }
            }
            float ancho_visible;
            float alto_visible;

            float centro_jugador_x;
            float centro_jugador_y;

            float objetivo_x;
            float objetivo_y;

            float max_camara_x;
            float max_camara_y;

            ancho_visible = ANCHO_PANTALLA / zoom;
            alto_visible = ALTO_PANTALLA / zoom;

            centro_jugador_x = jugador.x + jugador.ancho / 2.0f;
            centro_jugador_y = jugador.y + jugador.alto / 2.0f;

            objetivo_x = centro_jugador_x - ancho_visible / 2.0f;

            /* deja al jugador un poco más abajo de la mitad de pantalla */
            objetivo_y = centro_jugador_y - alto_visible * 0.60f;

            camara_x += (objetivo_x - camara_x) * 0.06f;
            camara_y += (objetivo_y - camara_y) * 0.06f;

            max_camara_x = columnas * ancho_v - ancho_visible;
            max_camara_y = filas * largo_v - alto_visible;

            if (max_camara_x < 0) {
                max_camara_x = 0;
            }

            if (max_camara_y < 0) {
                max_camara_y = 0;
            }

            if (camara_x < 0) {
                camara_x = 0;
            }

            if (camara_y < 0) {
                camara_y = 0;
            }

            if (camara_x > max_camara_x) {
                camara_x = max_camara_x;
            }

            if (camara_y > max_camara_y) {
                camara_y = max_camara_y;
            }
            actualizar_items(items, &jugador);
            actualizar_ataques_enemigos(&jugador, enemigos);
            fisicas_enemigos(enemigos, &jugador);
            disparo_zombie_d(enemigos, &jugador);
            disparo_zombie_v(enemigos, &jugador);
            fisicas_balas(jugador.balas, items);
            fisicas_granadas(jugador.granadas, items);
            for (int i = 0; i < max_enemigos; i++) {
                fisicas_balas(enemigos[i].balas_enemigo, items);
            }
            colision_bala_barril(items, jugador.balas, enemigos, &jugador);
            revisar_colisiones_bala_enemigo(enemigos, jugador.balas, &jugador);
            revisar_colisiones_granada_enemigo(enemigos, jugador.granadas, &jugador);
            for (int i = 0; i < max_enemigos; i++) {
                revisar_colison_bala_personaje(enemigos[i].balas_enemigo, &jugador, enemigos[i].dano);
            }


            if (jugador.vida <= 0 && puntaje_guardado == false) {
                guardar_registro_ranking(nombre_jugador, jugador.puntaje);
                puntaje_guardado = true;

                estado = GAME_OVER;
            }
        }
            if (estado == SALIR) {
                corriendo = false;
            }
            redibujar = true;
        }
///////////////////////////////////////////////////////////key_down/////////////////////////////////////////////////////////////////////
        else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {

            if (estado == MENU) {

                if (event.keyboard.keycode == ALLEGRO_KEY_W ||
                    event.keyboard.keycode == ALLEGRO_KEY_UP) {

                    opcion_menu--;

                    if (opcion_menu < 0) {
                        opcion_menu = 4;
                    }
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_S ||
                    event.keyboard.keycode == ALLEGRO_KEY_DOWN) {

                    opcion_menu++;

                    if (opcion_menu > 4) {
                        opcion_menu = 0;
                    }
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {

                    if (opcion_menu == 0) {
                        nombre_jugador[0];
                        letras_nombre = 0;
                        estado = INGRESAR_NOMBRE;
                    }
                    else if (opcion_menu == 1) {
                        estado = INFORMACION;
                        dibujar_introduccion(fuente_menu, fondo_menu, imagen_introduccion);
                    }
                    else if (opcion_menu == 2) {
                        estado = CONTROLES;
                    }
                    else if (opcion_menu == 3) {
                        estado = RANKING;
                    }
                    else if (opcion_menu == 4) {
                        estado = SALIR;
                    }
                }
            }
            else if (estado == CONTROLES) {

                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = MENU;
                }
        
            }
            else if (estado == INFORMACION) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = MENU;
                }
            }
////////////////////////////////////////////////////////////////////////nombre///////////////////////////////////////////////////////
            else if (estado == INGRESAR_NOMBRE) {

                if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {

                    if (letras_nombre == 0) {
                        strcpy(nombre_jugador, "Jugador");
                    }

                    nivel_actual = 1;
                    puntaje_guardado = false;

                    cargar_nivel(nivel_actual, &jugador, enemigos, items, plataformas_moviles);
                    jugador.puntaje = 0;

                    for (int i = 0; i < max_enemigos; i++) {
                        spawn_balas(enemigos[i].balas_enemigo);
                    }

                    camara_x = 0.0f;
                    camara_y = 0.0f;

                    estado = JUGANDO;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_BACKSPACE) {

                    if (letras_nombre > 0) {
                        letras_nombre--;
                        nombre_jugador[letras_nombre] = '\0';
                    }
                }

                else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = MENU;
                }
            }
            else if (estado == JUGANDO) {

                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                    saltar(&jugador);
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = MENU;
                }
            }
            else if (estado == GAME_OVER) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    corriendo = false;
                }

                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = MENU;
                }
            }
            else if (estado == RANKING) {

                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = MENU;
                }
            }
            else if (estado == VICTORIA) {
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                    estado = MENU;
                }
}
        }
////////////////////////////////////////////////////////////////////////////////mouse////////////////////////////////////////////////
        else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            if (estado == JUGANDO) {
                if (event.mouse.button == 1) {
                    if (disparo_mouse(&jugador, event.mouse.x, event.mouse.y, camara_x, camara_y, zoom) == true) {
                        reproducir_disparo();
                    }
                }

                if (event.mouse.button == 2) {
                    disparo_granada_mouse(&jugador, event.mouse.x, event.mouse.y, camara_x, camara_y, zoom);
                }
            }
        }
//////////////////////////////////////////////////////////////////key_char/////////////////////////////////////////////////////
        else if (event.type == ALLEGRO_EVENT_KEY_CHAR) {

            if (estado == INGRESAR_NOMBRE) {

                int tecla = event.keyboard.unichar;

                if (letras_nombre < max_nombre - 1) {

                    if ((tecla >= 'A' && tecla <= 'Z') ||
                        (tecla >= 'a' && tecla <= 'z') ||
                        (tecla >= '0' && tecla <= '9') ||
                        tecla == ' ') {

                        nombre_jugador[letras_nombre] = tecla;
                        letras_nombre++;
                        nombre_jugador[letras_nombre] = '\0';
                    }
                }
            }
        }
 ////////////////////////////////////////////redibujar///////////////////////////////
        if (redibujar && al_is_event_queue_empty(mijuego.queue)) {

            if (estado == MENU) {
                dibujar_menu(fuente_menu, opcion_menu, fondo_menu);
            }
            else if (estado == CONTROLES) {
                dibujar_controles(fuente_titulo, fuente_menu);
            }
            else if (estado == INGRESAR_NOMBRE) {
                dibujar_ingresar_nombre(fuente_menu, fondo_menu, nombre_jugador);
            }
            else if (estado == JUGANDO) {
                al_clear_to_color(al_map_rgb(0, 0, 0));
                float zoom = 1.2f;
                ALLEGRO_TRANSFORM transform;
                ALLEGRO_TRANSFORM normal;
                al_identity_transform(&transform);
                al_scale_transform(&transform, zoom, zoom);
                al_use_transform(&transform);

                dibujar_fondo(camara_x);
                if (nivel_actual == 1) {
                    dibujar_mansiones_nivel1(camara_x, camara_y);
                }
                if (nivel_actual == 4) {
                    dibujar_decoraciones_nivel4(camara_x, camara_y);
                }
                dibujar_mapa(camara_x, camara_y);
                dibujar_portal_mapa(camara_x, camara_y, &jugador);
                dibujar_items(items, camara_x, camara_y);
                dibujo_plat_mov(plataformas_moviles, camara_x, camara_y);
                dibujar_enemigos_mapa(enemigos, camara_x, camara_y);
                dibujar_balas_mapa(jugador.balas, camara_x, camara_y);
                dibujar_granadas_mapa(jugador.granadas, camara_x, camara_y);
                for (int i = 0;i < max_enemigos;i++) {
                    dibujar_balas_mapa(enemigos[i].balas_enemigo, camara_x, camara_y);
                }
                sombra_personaje(&jugador, camara_x, camara_y);
                dibujo_personaje(&jugador, camara_x, camara_y);

                al_identity_transform(&normal);
                al_use_transform(&normal);
                dibujar_barra_vida(&jugador);
                dibujar_puntaje(&jugador, fuente_menu);
                dibujar_municion(&jugador, fuente_hud);
            }
            else if (estado == GAME_OVER) {
                dibujar_game_over(fondo_game_over, fuente_menu, &jugador);
                registros_ranking ranking[max_registros_leidos];
                int cont;

                cont = leer_ranking(ranking, max_registros_leidos);
                ordenar_ranking(ranking, cont);

                dibujar_ranking(ranking, cont, 5, fuente_menu, 600, 430);
            }
            else if (estado == VICTORIA) {
                dibujar_victoria(fondo_victoria, fuente_menu, &jugador);

                registros_ranking ranking[max_registros_leidos];
                int cont;

                cont = leer_ranking(ranking, max_registros_leidos);
                ordenar_ranking(ranking, cont);

                dibujar_ranking(ranking, cont, 5, fuente_menu, 600, 430);
            }
            else if (estado == RANKING) {
                registros_ranking ranking[max_registros_leidos];
                int cont;

                al_draw_scaled_bitmap(
                    fondo_ranking,
                    0,
                    0,
                    al_get_bitmap_width(fondo_ranking),
                    al_get_bitmap_height(fondo_ranking),
                    0,
                    0,
                    1200,
                    800,
                    0
                );

                cont = leer_ranking(ranking, max_registros_leidos);
                ordenar_ranking(ranking, cont);

                dibujar_ranking(ranking, cont, 10, fuente_menu, 600, 230);

                al_draw_text(
                    fuente_menu,
                    al_map_rgb(255, 255, 255),
                    600,
                    730,
                    ALLEGRO_ALIGN_CENTER,
                    "ESC - VOLVER"
                );
            }

            al_flip_display();

            redibujar = false;
        }
    }
    liberar_sprites_mapa();
    liberar_sprites_personaje();
    liberar_sprites_enemigos();
    liberar_sprites_balas();
    liberar_sprites_items();
    liberar_sonidos();
    liberar_sprites_granadas();
    al_destroy_bitmap(fondo_menu);
    al_destroy_bitmap(fondo_game_over);
    al_destroy_font(fuente_titulo);
    al_destroy_font(fuente_menu);
    al_destroy_bitmap(fondo_ranking);
    al_destroy_bitmap(fondo_victoria);
    cerrar_sistema(&mijuego);

    return 0;
}

//funciones----------------------------

bool inicializar_sistema(flujo_juego* j) {
    if (!al_init()) return false;

    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_install_keyboard();
    al_install_mouse();
    al_init_primitives_addon();

    j->display = al_create_display(1200, 800);
    if (!j->display) return false;

    j->timer = al_create_timer(1.0 / 60.0);
    j->queue = al_create_event_queue();
    if (!j->queue) {
        al_destroy_display(j->display);
        return false;
    }

    al_register_event_source(j->queue, al_get_display_event_source(j->display));
    al_register_event_source(j->queue, al_get_timer_event_source(j->timer));
    al_register_event_source(j->queue, al_get_keyboard_event_source());
    al_register_event_source(j->queue, al_get_mouse_event_source());

    al_start_timer(j->timer);
    return true;
}

void cerrar_sistema(flujo_juego* j) {
        al_destroy_timer(j->timer);
        al_destroy_event_queue(j->queue);
        al_destroy_display(j->display);
}

void cargar_nivel(int nivel, personaje* p, enemigo enemigos[], item items[], plataforma_movil plataformas_moviles[]) {
    char txt[100];
    int llaves_necesarias;

    if (nivel == 1) {
        strcpy(txt, "mapa1.txt");
    }
    else if (nivel == 2) {
        strcpy(txt, "mapa2.txt");
    }
    else if (nivel == 3) {
        strcpy(txt, "mapa3.txt");
    }
    else if (nivel == 4) {
        strcpy(txt, "mapa4.txt");
    }

    cargar_mapa(txt, &llaves_necesarias);

    cargar_fondos(nivel);

    spawn_personaje(p);

    p->llave = 0;
    p->llaves_nivel = llaves_necesarias;

    spawn_balas(p->balas);
    spawn_granadas(p->granadas);

    inicializar_enemigos(enemigos);
    spawn_enemigos(enemigos);

    inicializar_items(items);
    spawn_items(items);

    inicializar_plat(plataformas_moviles);
    spawn_plat_moviles(plataformas_moviles);

}

void dibujar_menu(ALLEGRO_FONT* fuente_menu, int opcion_menu, ALLEGRO_BITMAP* fondo_menu) {

    al_draw_scaled_bitmap(
        fondo_menu,
        0,
        0,
        al_get_bitmap_width(fondo_menu),
        al_get_bitmap_height(fondo_menu),
        0,
        0,
        ANCHO_PANTALLA,
        ALTO_PANTALLA,
        0
    );

    dibujar_boton(fuente_menu, 400, 390, 800, 455, "JUGAR", opcion_menu == 0);    
    dibujar_boton(fuente_menu, 400, 470, 800, 535, "INFO", opcion_menu == 1);
    dibujar_boton(fuente_menu, 400, 550, 800, 615, "CONTROLES", opcion_menu == 2);
    dibujar_boton(fuente_menu, 400, 630, 800, 695, "RANKING", opcion_menu == 3);
    dibujar_boton(fuente_menu, 400, 710, 800, 775, "SALIR", opcion_menu == 4);


}

void dibujar_controles(ALLEGRO_FONT* fuente_titulo, ALLEGRO_FONT* fuente_menu) {

    al_draw_filled_rectangle(220,170,980,700,al_map_rgba(0, 0, 0, 190));

    al_draw_rectangle(220,170,980,700,al_map_rgb(255, 200, 0),5);

    al_draw_text(
        fuente_titulo,
        al_map_rgb(255, 200, 0),
        600,
        200,
        ALLEGRO_ALIGN_CENTER,
        "CONTROLES"
    );

    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 300, 330, ALLEGRO_ALIGN_LEFT, "A / D");
    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 620, 330, ALLEGRO_ALIGN_LEFT, "Mover");

    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 300, 390, ALLEGRO_ALIGN_LEFT, "ESPACIO");
    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 620, 390, ALLEGRO_ALIGN_LEFT, "Saltar");

    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 300, 450, ALLEGRO_ALIGN_LEFT, "MOUSE");
    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 620, 450, ALLEGRO_ALIGN_LEFT, "Apuntar");

    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 300, 510, ALLEGRO_ALIGN_LEFT, "CLICK IZQ");
    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 620, 510, ALLEGRO_ALIGN_LEFT, "Balas");

    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 300, 570, ALLEGRO_ALIGN_LEFT, "CLICK DER");
    al_draw_text(fuente_menu, al_map_rgb(255, 255, 255), 620, 570, ALLEGRO_ALIGN_LEFT, "Granada");

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 200, 0),
        600,
        630,
        ALLEGRO_ALIGN_CENTER,
        "ESC - Volver"
    );
}

void dibujar_boton(ALLEGRO_FONT* fuente_menu, float x1, float y1, float x2, float y2, const char* texto, bool seleccionado) {

    ALLEGRO_COLOR color_fondo;
    ALLEGRO_COLOR color_borde;
    ALLEGRO_COLOR color_texto;

    if (seleccionado == true) {
        color_fondo = al_map_rgb(255, 200, 0);
        color_borde = al_map_rgb(255, 255, 255);
        color_texto = al_map_rgb(0, 0, 0);
    }
    else {
        color_fondo = al_map_rgb(25, 25, 25);
        color_borde = al_map_rgb(120, 120, 120);
        color_texto = al_map_rgb(255, 255, 255);
    }

    al_draw_filled_rectangle(x1, y1, x2, y2, color_fondo);
    al_draw_rectangle(x1, y1, x2, y2, color_borde, 5);

    al_draw_text(
        fuente_menu,
        color_texto,
        (x1 + x2) / 2.0f,
        y1 + 14,
        ALLEGRO_ALIGN_CENTER,
        texto
    );
}

void dibujar_game_over(ALLEGRO_BITMAP* fondo_game_over, ALLEGRO_FONT* fuente_menu, personaje* p) {
    char texto_puntaje[50];

    al_draw_scaled_bitmap(
        fondo_game_over,
        0,
        0,
        al_get_bitmap_width(fondo_game_over),
        al_get_bitmap_height(fondo_game_over),
        0,
        0,
        1200,
        800,
        0
    );

    sprintf(texto_puntaje, "Puntaje final: %d", p->puntaje);

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 255, 255),
        600,
        360,
        ALLEGRO_ALIGN_CENTER,
        texto_puntaje
    );

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 220, 0),
        600,
        700,
        ALLEGRO_ALIGN_CENTER,
        "ESCAPE - Salir"
    );
}

void dibujar_ranking(registros_ranking ranking[], int cont, int limite, ALLEGRO_FONT* fuente_menu, float x, float y) {
    int i;
    int separacion;
    float y_linea;

    ALLEGRO_COLOR blanco = al_map_rgb(255, 255, 255);  //texto
    ALLEGRO_COLOR amarillo = al_map_rgb(255, 190, 0);  //top
    ALLEGRO_COLOR sombra = al_map_rgb(0, 0, 0);       

    if (cont <= 0) {
        al_draw_text(fuente_menu, sombra, x + 3, y + 3, ALLEGRO_ALIGN_CENTER, "NO HAY PUNTAJES");
        al_draw_text(fuente_menu, blanco, x, y, ALLEGRO_ALIGN_CENTER, "NO HAY PUNTAJES");
        return;
    }

    if (cont < limite) {
        limite = cont;
    }

    separacion = al_get_font_line_height(fuente_menu) + 10;

    al_draw_textf(fuente_menu, sombra, x + 3, y + 3, ALLEGRO_ALIGN_CENTER, "TOP %d", limite);
    al_draw_textf(fuente_menu, amarillo, x, y, ALLEGRO_ALIGN_CENTER, "TOP %d", limite);

    for (i = 0; i < limite; i++) {
        y_linea = y + 50 + i * separacion;

        al_draw_textf(
            fuente_menu,
            sombra,
            x + 3,
            y_linea + 3,
            ALLEGRO_ALIGN_CENTER,
            "%d. %s - %d",
            i + 1,
            ranking[i].nombre,
            ranking[i].puntaje
        );

        al_draw_textf(
            fuente_menu,
            blanco,
            x,
            y_linea,
            ALLEGRO_ALIGN_CENTER,
            "%d. %s - %d",
            i + 1,
            ranking[i].nombre,
            ranking[i].puntaje
        );
    }
}


void dibujar_ingresar_nombre(ALLEGRO_FONT* fuente_menu, ALLEGRO_BITMAP* fondo_menu, char nombre_jugador[]) {

    al_draw_scaled_bitmap(
        fondo_menu,
        0,
        0,
        al_get_bitmap_width(fondo_menu),
        al_get_bitmap_height(fondo_menu),
        0,
        0,
        ANCHO_PANTALLA,
        ALTO_PANTALLA,
        0
    );

    al_draw_filled_rectangle(
        230,
        250,
        970,
        600,
        al_map_rgba(0, 0, 0, 190)
    );

    al_draw_rectangle(
        230,
        250,
        970,
        600,
        al_map_rgb(255, 200, 0),
        4
    );

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 200, 0),
        600,
        310,
        ALLEGRO_ALIGN_CENTER,
        "INGRESA TU NOMBRE"
    );

    if (strlen(nombre_jugador) == 0) {
        al_draw_text(
            fuente_menu,
            al_map_rgb(150, 150, 150),
            600,
            410,
            ALLEGRO_ALIGN_CENTER,
            "..."
        );
    }
    else {
        al_draw_text(
            fuente_menu,
            al_map_rgb(255, 255, 255),
            600,
            410,
            ALLEGRO_ALIGN_CENTER,
            nombre_jugador
        );
    }

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 255, 255),
        600,
        500,
        ALLEGRO_ALIGN_CENTER,
        "ENTER - JUGAR"
    );

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 200, 0),
        600,
        550,
        ALLEGRO_ALIGN_CENTER,
        "ESC - VOLVER"
    );
}

void dibujar_municion(personaje* p, ALLEGRO_FONT* fuente) {

    al_draw_textf(
        fuente,
        al_map_rgb(255, 255, 255),
        30,
        175,
        ALLEGRO_ALIGN_LEFT,
        "Balas: %d",
        p->municion
    );

    al_draw_textf(
        fuente,
        al_map_rgb(255, 200, 0),
        30,
        135,
        ALLEGRO_ALIGN_LEFT,
        "Granadas: %d",
        p->municion_granadas
    );
}

void dibujar_introduccion(ALLEGRO_FONT* fuente_menu, ALLEGRO_BITMAP* fondo_menu, ALLEGRO_BITMAP* imagen_introduccion) {

    al_draw_scaled_bitmap(
        fondo_menu,
        0,
        0,
        al_get_bitmap_width(fondo_menu),
        al_get_bitmap_height(fondo_menu),
        0,
        0,
        ANCHO_PANTALLA,
        ALTO_PANTALLA,
        0
    );

    al_draw_filled_rectangle(
        120,
        40,
        1080,
        740,
        al_map_rgba(0, 0, 0, 180)
    );

    al_draw_rectangle(
        120,
        40,
        1080,
        740,
        al_map_rgb(255, 200, 0),
        4
    );

    if (imagen_introduccion != NULL) {
        al_draw_scaled_bitmap(
            imagen_introduccion,
            0,
            0,
            al_get_bitmap_width(imagen_introduccion),
            al_get_bitmap_height(imagen_introduccion),
            220,
            60,
            760,
            620,
            0
        );
    }

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 200, 0),
        600,
        705,
        ALLEGRO_ALIGN_CENTER,
        "ESC - VOLVER"
    );
}

void dibujar_victoria(ALLEGRO_BITMAP* fondo_victoria, ALLEGRO_FONT* fuente_menu, personaje* p) {
    char texto_puntaje[50];

    al_draw_scaled_bitmap(
        fondo_victoria,
        0,
        0,
        al_get_bitmap_width(fondo_victoria),
        al_get_bitmap_height(fondo_victoria),
        0,
        0,
        1200,
        800,
        0
    );

    sprintf(texto_puntaje, "Puntaje final: %d", p->puntaje);

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 255, 255),
        600,
        320,
        ALLEGRO_ALIGN_CENTER,
        texto_puntaje
    );

    al_draw_text(
        fuente_menu,
        al_map_rgb(255, 220, 0),
        600,
        700,
        ALLEGRO_ALIGN_CENTER,
        "ESC - VOLVER AL MENU"
    );
}