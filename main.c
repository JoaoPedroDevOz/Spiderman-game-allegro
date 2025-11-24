#define ALLEGRO_STATICLINK
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

int main() {

    // --- Variáveis ---
    int largura_tela = 1280;
    int altura_tela = 720;

    float frame = 0.0f;
    int pos_x = 425, pos_y = 527;

    int current_frame_y = 73;   // linha do sprite de andar

    int mapa[20][30] = {
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    };

    int TILE = 32;

    bool esta_pulando = false; // variavel de pulo
    float tempo_pulo = 0.0f;
    float duracao_pulo = 0.25f; // segundos no ar
    float anim_corrida_veleocidade = 0.15;

    float bg_x = 0;
    float bg_velocidade = -5; // velocidade do fundo (negativo = anda para a esquerda)

    // --- Inicialização ---
    al_init();
    al_init_font_addon();
    al_init_image_addon();
    al_install_keyboard();

    ALLEGRO_DISPLAY* display = al_create_display(largura_tela, altura_tela);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Spiderman: run to the home!");

    ALLEGRO_FONT* font = al_create_builtin_font();
    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    ALLEGRO_BITMAP* spriteSpiderman = al_load_bitmap("./img/spiderman.png");
    ALLEGRO_BITMAP* bg = al_load_bitmap("./img/Background.png");

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    // --- Loop principal ---
    bool running = true;

    while (running) {

        ALLEGRO_EVENT event;

        // processa TODOS os eventos disponíveis
        while (al_get_next_event(event_queue, &event)) {

            // fechar janela
            if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
                running = false;
            }

            // teclado
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !esta_pulando) {
                    esta_pulando = true;
                    tempo_pulo = 0.0f;
                    current_frame_y = 0;
                    pos_y -= 45;
                }
            }

            // timer = lógica do jogo
            if (event.type == ALLEGRO_EVENT_TIMER) {

                bg_x += bg_velocidade;

                if (bg_x <= -1280)
                    bg_x = 0;

                frame += anim_corrida_veleocidade;
                if (frame >= 3)
                    frame = 0;

                if (esta_pulando) {
                    tempo_pulo += al_get_timer_speed(timer);

                    if (tempo_pulo >= duracao_pulo) {
                        esta_pulando = false;
                        current_frame_y = 73;
                        pos_y = 527;
                    }
                }
            }
        }

        // Renderização
        al_clear_to_color(al_map_rgb(255, 255, 255));

        al_draw_bitmap(bg, bg_x, 0, 0);
        al_draw_bitmap(bg, bg_x + 1280, 0, 0);

        for (int linha = 0; linha < 20; linha++) {
            for (int coluna = 0; coluna < 30; coluna++) {

                if (mapa[linha][coluna] == 1) {
                    al_draw_bitmap(
                        al_load_bitmap("./img/bloco.png"),
                        coluna * TILE,
                        linha * TILE,
                        0
                    );
                }
                if (mapa[linha][coluna] == 2) {
                    al_draw_bitmap(
                        al_load_bitmap("./img/plataforma.png"),
                        coluna * TILE,
                        linha * TILE,
                        0
                    );
                }
            }
        }

        

        al_draw_text(font, al_map_rgb(0, 0, 0), 7, 7, 0, "SCORE: dragon");
        al_draw_text(font, al_map_rgb(255, 255, 255), 5, 5, 0, "SCORE: dragon");

        al_draw_bitmap_region(
            spriteSpiderman,
            50 * (int)frame,
            current_frame_y,
            42, 73,
            pos_x, pos_y,
            0
        );

        al_flip_display();
    }


    // --- Finalização ---
    al_destroy_bitmap(bg);
    al_destroy_bitmap(spriteSpiderman);
    al_destroy_font(font);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);

    return 0;
}
