#define ALLEGRO_STATICLINK
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

// Enum seria uma maneira mais organizada de fazer: DEFINE MENU 0, DEFINE PLAYING 1 r DEFINE PLACAR 2.
enum GameState { // o estado atual em que o jogo está
    MENU,
    PLAYING,
    PLACAR
};

int main() {
    //Variáveis do menu.
    // jogo já inicia com o menu selecionado (status do jogo = MENU).
    enum GameState gameState = MENU;
    int menu_selection = 0;
    // para não registrar as setas como pressionadas continuamente.
    int key_released = 1;

    // --- Variáveis ---
    int largura_tela = 1280;
    int altura_tela = 720;

    float frame = 0.0f;
    int pos_x = 425, pos_y = 600 - 73;

    int current_frame_y = 73;   // linha do sprite de andar

    bool esta_pulando = false;
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
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* display = al_create_display(largura_tela, altura_tela);
    al_set_window_position(display, 200, 200);
    al_set_window_title(display, "Spiderman: run to the home!");

    ALLEGRO_FONT* fonte_titulo = al_load_font("arial.ttf", 120, 0);    // Título grande
    ALLEGRO_FONT* fonte_menu = al_load_font("arial.ttf", 36, 0);      // Menu médio
    ALLEGRO_FONT* fonte_placar = al_load_font("arial.ttf", 28, 0);    // Placar

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

    ALLEGRO_BITMAP* spriteSpiderman = al_load_bitmap("./img/spiderman.png");
    ALLEGRO_BITMAP* bg = al_load_bitmap("./img/bg.png");

    // sugestão do copilot do proprio vs: aqui ele sugeriu verificar se as fontes foram carregadas corretamente
    //caso não, retornar para a fonte padrão do allegro
    if (!fonte_titulo) fonte_titulo = al_create_builtin_font();
    if (!fonte_menu) fonte_menu = al_create_builtin_font();
    if (!fonte_placar) fonte_placar = al_create_builtin_font();

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_start_timer(timer);

    // --- Loop principal ---
    while (1) {
        ALLEGRO_EVENT event;
        al_wait_for_event(event_queue, &event);

        // menu
        if (gameState == MENU) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN)
            {
                if (event.keyboard.keycode == ALLEGRO_KEY_UP && key_released) {
                    menu_selection = (menu_selection - 1 + 3) % 3; // 3 opções
                    key_released = 0;
                }
                // matematica para permitir navegar circularmente no menu 
                else if (event.keyboard.keycode == ALLEGRO_KEY_DOWN && key_released) {
                    menu_selection = (menu_selection + 1) % 3; // 3 opções agora
                    key_released = 0;
                }
                else if (event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
                    if (menu_selection == 0) {
                        gameState = PLAYING; // iunicia o jogo
                    }
                    else if (menu_selection == 1) {
                        gameState = PLACAR;  // mostra o placar
                    }
                    else if (menu_selection == 2) {
                        break; // sai do jogo
                    }
                }
            }

            if (event.type == ALLEGRO_EVENT_KEY_UP) {
                key_released = 1;
            }

            // aqui vai o fundo do menu, substituir aqui por img de fundo se quiser
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // título no menu
            al_draw_text(fonte_titulo, al_map_rgb(255, 0, 0), largura_tela / 2, 80, ALLEGRO_ALIGN_CENTER, "SPIDERMAN: RUN TO THE HOME!");

            // as opçoes do menu estilizadas com >< quando selecionadas
            if (menu_selection == 0) {
                al_draw_text(fonte_menu, al_map_rgb(255, 0, 0), largura_tela / 2, 350, ALLEGRO_ALIGN_CENTER, "> JOGAR <");
            }
            else {
                al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), largura_tela / 2, 350, ALLEGRO_ALIGN_CENTER, "JOGAR");
            }

            if (menu_selection == 1) {
                al_draw_text(fonte_menu, al_map_rgb(255, 0, 0), largura_tela / 2, 400, ALLEGRO_ALIGN_CENTER, "> PLACAR <");
            }
            else {
                al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), largura_tela / 2, 400, ALLEGRO_ALIGN_CENTER, "PLACAR");
            }

            if (menu_selection == 2) {
                al_draw_text(fonte_menu, al_map_rgb(255, 0, 0), largura_tela / 2, 450, ALLEGRO_ALIGN_CENTER, "> SAIR <");
            }
            else {
                al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), largura_tela / 2, 450, ALLEGRO_ALIGN_CENTER, "SAIR");
            }

            al_flip_display();
            continue;
        }

        // placar
        if (gameState == PLACAR) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) { //ae apertar uma tecla
                if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_ENTER) { //e essa tecla foe esc ou enter
                    gameState = MENU; //voltar para o menu
                }
            }

            // fundo do placar
            al_clear_to_color(al_map_rgb(0, 0, 0));

            // titulo do placar
            al_draw_text(fonte_titulo, al_map_rgb(255, 215, 0), largura_tela / 2, 100, ALLEGRO_ALIGN_CENTER, "PLACAR");

			// aqui eu fiz um exemplo simples da pntuação, substituir por variaveis que armazenem as pontuações reais
			// add uma função para gerar arquivos com as pontuações depois
			// add outra função para ler esses arquivos aqui e mostrar as pontuações reais
            al_draw_text(fonte_placar, al_map_rgb(255, 255, 255), largura_tela / 2, 200, ALLEGRO_ALIGN_CENTER, "MELHOR PONTUAÇÃO: 1500");
            al_draw_text(fonte_placar, al_map_rgb(255, 255, 255), largura_tela / 2, 250, ALLEGRO_ALIGN_CENTER, "ÚLTIMA PONTUAÇÃO: 1200");
            al_draw_text(fonte_placar, al_map_rgb(255, 255, 255), largura_tela / 2, 300, ALLEGRO_ALIGN_CENTER, "RECORDE: 2000");

            // para volyar ao menu
            al_draw_text(fonte_placar, al_map_rgb(150, 150, 150), largura_tela / 2, 500, ALLEGRO_ALIGN_CENTER, "Pressione ESC ou ENTER para voltar");

            al_flip_display();
            continue;
        }

        // Fechar janela
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
            break;
        }

        // --- Entrada de teclado ---
        if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !esta_pulando) {
                // inicia o pulo
                esta_pulando = true;
                tempo_pulo = 0.0f;
                current_frame_y = 0;  // sprite do pulo
            }
            // ⭐ VOLTAR AO MENU DURANTE O JOGO
            else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
                gameState = MENU;
            }
        }

        // --- Atualização por timer ---
        if (event.type == ALLEGRO_EVENT_TIMER) {
            // movimento automático
            bg_x += bg_velocidade;

            if (bg_x <= -1280)
                bg_x = 0;

            // animação
            frame += anim_corrida_veleocidade;
            if (frame >= 3)
                frame = 0;

            // --- Controle de tempo do pulo ---
            if (esta_pulando) {
                tempo_pulo += 1.0f / 60.0f;

                if (tempo_pulo >= duracao_pulo) {
                    esta_pulando = false;
                    current_frame_y = 73; // volta ao sprite normal
                }
            }
        }

        // --- Renderização DO JOGO ---
        al_clear_to_color(al_map_rgb(255, 255, 255));

        al_draw_bitmap(bg, bg_x, 0, 0);
        al_draw_bitmap(bg, bg_x + 1280, 0, 0);

        al_draw_text(fonte_placar, al_map_rgb(0, 0, 0), 7, 7, 0, "SCORE: dragon");
        al_draw_text(fonte_placar, al_map_rgb(255, 255, 255), 5, 5, 0, "SCORE: dragon");

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
    al_destroy_font(fonte_titulo);
    al_destroy_font(fonte_menu);
    al_destroy_font(fonte_placar);
    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    return 0;
}