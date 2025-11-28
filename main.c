#define _CRT_SECURE_NO_WARNINGS
#define ALLEGRO_STATICLINK
#define SPIDERMAN_W 42
#define SPIDERMAN_H 73

#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <stdlib.h>
#include <math.h>
#include "personagem.h"
#include "mapa.h"
#include "menu.h"

enum GameState {
	MENU,
	PLAYING,
	PLACAR
};

// --- Mapa do jogo ---
int mapa[MAPA_LINHAS][MAPA_COLUNAS] = {
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,13,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,11,11,11,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,0,0,13,0,0,0,0,0,0,0,0,0},
	{0,0,4,0,0,0,0,0,0,0,0,0,13,13,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,0,0,0,0,0,0,0,0},
	{0,0,4,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,0,0,0},
	{0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0},
	{0,7,4,0,0,0,0,12,0,0,0,13,13,13,13,0,0,0,0,0,0,0,0,0,13,0,8,9,9,9,9,10,0,13,0,5,5,5,5,5},
	{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
};

int main() {
	// --- Variáveis ---
	const int largura_tela = 1280;
	const int altura_tela = 720;
	const int TILE = 32;

	// Estado do jogo
	enum GameState gameState = MENU;
	MenuState menu;
	inicializarMenu(&menu);

	// Variáveis do jogo
	float tile_offset_x = 0;
	float tile_scroll_speed = -5.0f;
	char score_text[30];
	int score = 0;
	float bg_x = 0;
	float bg_velocidade = -5;

	// Personagem
	PersonagemBase personagemBase = {
		.esta_pulando = false,
		.tempo_pulo = 0.0f,
		.duracao_pulo = 0.5f,
		.velocidade = 5.0f
	};

	Spiderman spiderman = {
		.base = personagemBase,
		.frame = 0.0f,
		.pos_x = 425,
		.pos_y = 527,
		.current_frame_y = 65,
		.vel_y = 0.0f
	};

	// Física
	const float GRAVIDADE = 0.5f;
	const float IMPULSO_PULO = -11.0f;

	// --- Inicialização Allegro ---
	if (!al_init()) return -1;
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();

	ALLEGRO_DISPLAY* display = al_create_display(largura_tela, altura_tela);
	al_set_window_position(display, 200, 200);
	al_set_window_title(display, "Spiderman: run to the home!");

	ALLEGRO_FONT* fonte_titulo = al_load_font("arial.ttf", 120, 0);
	ALLEGRO_FONT* fonte_menu = al_load_font("arial.ttf", 36, 0);
	ALLEGRO_FONT* fonte_placar = al_load_font("arial.ttf", 28, 0);

	// Se qualquer fonte falhar, usar a fonte interna como fallback
	if (!fonte_titulo) fonte_titulo = al_create_builtin_font();
	if (!fonte_menu)   fonte_menu = al_create_builtin_font();
	if (!fonte_placar) fonte_placar = al_create_builtin_font();

	// Para textos pequenos internos
	ALLEGRO_FONT* builtin_font = al_create_builtin_font();

	// Carrega bitmaps
	ALLEGRO_BITMAP* spriteSpiderman = al_load_bitmap("./img/Sprites/spiderman.png");
	ALLEGRO_BITMAP* bg = al_load_bitmap("./img/Background.png");
	ALLEGRO_BITMAP* bg_menu = al_load_bitmap("./img/Menu.png");

	ALLEGRO_BITMAP* tiles[14] = { NULL };
	tiles[1] = al_load_bitmap("./img/Tiles/piso1.png");
	tiles[2] = al_load_bitmap("./img/Tiles/plataforma3.png");
	tiles[3] = al_load_bitmap("./img/Tiles/piso5.png");
	tiles[4] = al_load_bitmap("./img/Objects/escada.png");
	tiles[5] = al_load_bitmap("./img/Tiles/bloco.png");
	tiles[6] = al_load_bitmap("./img/Tiles/plataforma1.png");
	tiles[7] = al_load_bitmap("./img/Objects/placa.png");
	tiles[8] = al_load_bitmap("./img/Objects/barra1.png");
	tiles[9] = al_load_bitmap("./img/Objects/barra2.png");
	tiles[10] = al_load_bitmap("./img/Objects/barra3.png");
	tiles[11] = al_load_bitmap("./img/Tiles/plataforma2.png");
	tiles[12] = al_load_bitmap("./img/Objects/caixa.png");
	tiles[13] = al_load_bitmap("./img/Objects/coin3.png");

	// Verificação de recursos
	if (!spriteSpiderman || !bg) {
		fprintf(stderr, "Erro ao carregar bitmaps principais\n");
		return -1;
	}

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);
	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_start_timer(timer);

	// --- Loop principal ---
	bool running = true;
	bool redraw = true;

	while (running) {
		ALLEGRO_EVENT event;
		al_wait_for_event(event_queue, &event);

		bool sair = false;
		bool iniciarJogo = false;
		bool abrirRanking = false;

		switch (event.type) {
		case ALLEGRO_EVENT_DISPLAY_CLOSE:
			running = false;
			break;

		case ALLEGRO_EVENT_KEY_DOWN:
			if (gameState == PLAYING) {
				if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !spiderman.base.esta_pulando) {
					aplicarPulo(&spiderman, IMPULSO_PULO);
				}
				// ESC volta para o menu
				if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					gameState = MENU;
				}
			}
			break;

		case ALLEGRO_EVENT_TIMER:
			redraw = true;

			if (gameState == PLAYING) {
				// Atualizações do jogo
				bg_x += bg_velocidade;
				if (bg_x <= -largura_tela) bg_x = 0;

				tile_offset_x += tile_scroll_speed;

				spiderman.frame += spiderman.base.velocidade * 0.03f;
				if (spiderman.frame >= 3.0f) spiderman.frame = 0.0f;

				atualizarFisicaSpiderman(&spiderman, GRAVIDADE, 15.0f);
				checarColisaoComTiles(&spiderman, mapa, TILE, tile_offset_x);
				score = coletarMoedas(&spiderman, mapa, TILE, tile_offset_x);
				sprintf(score_text, "Moedas: %d", score);
			}
			break;
		}

		// Atualiza menu se estiver no estado MENU
		if (gameState == MENU) {
			atualizarMenu(&menu, &sair, &iniciarJogo, &abrirRanking);

			if (sair) running = false;
			if (iniciarJogo) {
				gameState = PLAYING;
				// Reinicia estado do jogo se necessário
				spiderman.pos_x = 425;
				spiderman.pos_y = 527;
				score = 0;
				tile_offset_x = 0;
				bg_x = 0;
			}
			if (abrirRanking) gameState = PLACAR;
		}

		if (gameState == PLACAR)
		{
			// ESC volta para o menu
			if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				gameState = MENU;
			}
		}

		// Renderização
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));

			if (gameState == MENU) {
				// Desenha menu
				if (bg_menu) {
					al_draw_scaled_bitmap(bg_menu, 0, 0,
						al_get_bitmap_width(bg_menu),
						al_get_bitmap_height(bg_menu),
						0, 0, largura_tela, altura_tela, 0);
				}
				else {
					al_clear_to_color(al_map_rgb(0, 0, 0)); // fallback
				}

				// Desenha título
				al_draw_text(fonte_titulo, al_map_rgb(255, 0, 0), largura_tela / 2, 80, ALLEGRO_ALIGN_CENTER,
					"SPIDERMAN: EM BUSCA DE DINHEIRO PARA CHEGAR EM CASA");


				// Opções do menu
				al_draw_text(fonte_menu,
					(menu.opcao_selecionada == MENU_START) ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255),
					largura_tela / 2, 350, ALLEGRO_ALIGN_CENTER,
					(menu.opcao_selecionada == MENU_START) ? "> JOGAR <" : "JOGAR");

				al_draw_text(fonte_menu,
					(menu.opcao_selecionada == MENU_RANKING) ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255),
					largura_tela / 2, 400, ALLEGRO_ALIGN_CENTER,
					(menu.opcao_selecionada == MENU_RANKING) ? "> PLACAR <" : "PLACAR");

				al_draw_text(fonte_menu,
					(menu.opcao_selecionada == MENU_SAIR) ? al_map_rgb(255, 0, 0) : al_map_rgb(255, 255, 255),
					largura_tela / 2, 450, ALLEGRO_ALIGN_CENTER,
					(menu.opcao_selecionada == MENU_SAIR) ? "> SAIR <" : "SAIR");

			}
			else if (gameState == PLAYING) {
				// Desenha jogo
				al_draw_bitmap(bg, bg_x, 0, 0);
				al_draw_bitmap(bg, bg_x + largura_tela, 0, 0);

				renderizarMapaRepetindo(
					mapa, tiles, tile_offset_x,
					MAPA_LINHAS, MAPA_COLUNAS, largura_tela, altura_tela, TILE, bg_x, spiderman.pos_x
				);

				al_draw_text(builtin_font, al_map_rgb(255, 255, 255), 20, 20, 0, score_text);
				desenharSpiderman(&spiderman, spriteSpiderman);

			}
			else if (gameState == PLACAR) {
				// Tela de placar
				al_draw_text(fonte_titulo, al_map_rgb(255, 0, 0), largura_tela / 2, 80, ALLEGRO_ALIGN_CENTER, "PLACAR");
				al_draw_text(fonte_menu, al_map_rgb(255, 255, 255), largura_tela / 2, 300, ALLEGRO_ALIGN_CENTER, "Em desenvolvimento...");
				al_draw_text(fonte_placar, al_map_rgb(200, 200, 200), largura_tela / 2, 500, ALLEGRO_ALIGN_CENTER, "Pressione ESC para voltar");
			}

			al_flip_display();
		}
	}

	// --- Limpeza ---
	for (int i = 1; i <= 13; i++) {
		if (tiles[i]) al_destroy_bitmap(tiles[i]);
	}
	al_destroy_bitmap(bg);
	al_destroy_bitmap(spriteSpiderman);
	al_destroy_font(fonte_titulo);
	al_destroy_font(fonte_menu);
	al_destroy_font(fonte_placar);
	al_destroy_font(builtin_font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);

	return 0;
}