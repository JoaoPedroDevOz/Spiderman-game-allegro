#define _CRT_SECURE_NO_WARNINGS
#define ALLEGRO_STATICLINK
#include <stdio.h>
#include <stdbool.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>

typedef struct {
	bool esta_pulando;
	float tempo_pulo;
	float duracao_pulo;
	float velocidade; // velocidade horizontal / de movimento (já usada p/ animação)
} PersonagemBase;

typedef struct {
	PersonagemBase base;
	float frame;
	int pos_x;
	int pos_y;
	int current_frame_y;
	float vel_y; // velocidade vertical (positiva = descendo)
} Spiderman;

int main() {
	// --- Variáveis ---
	const int TILE = 32;
	const int largura_tela = 1280;
	const int altura_tela = 720;

	float tile_offset_x = 0;
	float tile_scroll_speed = -5.0f;

	float score = 0;
	//char score_text[20];

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

	// guarda altura do chão (posição Y onde o personagem fica "no chão")
	const int altura_chao = spiderman.pos_y;

	// Pulo / Física
	const float GRAVIDADE = 0.5f;         // aceleração para baixo (ajuste para sentir melhor)
	const float IMPULSO_PULO = -11.0f;    // velocidade inicial para cima (negativa sobe)

	// --- mapa (mantive como estava) ---
	int mapa[23][80] = {
		{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
		{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,13,0,0,0,0,0,0,0,0,0,2,2,2,2},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,13,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,0,13,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,0,0,0,0,0,0},
		{0,0,0,0,0,0,11,11,11,11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,0,0,0,13,0,0,13,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,11,11,11,11,11,11,0,0,0,0,0,0,0,13,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,0,0,0,0,0,0,0,0,0,13,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,6,6,6,6,6,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,13,0,0},
		{1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,0,0,13,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,13,13,13,0,0,0,0,0,0,13,0,0,0,0,0,0,13,13,13,0,0,5,5,13,0},
		{0,0,4,0,0,0,0,0,0,0,0,0,13,13,0,0,0,0,0,0,0,13,13,0,0,0,6,6,6,6,6,6,0,0,0,13,13,0,0,0,0,0,0,0,0,0,13,13,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,5,0,0,0,0,6,6,6,6,6,0,0,5,5,5,13},
		{0,0,4,0,0,0,0,0,0,0,2,2,2,2,2,2,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,0,0,0,0,6,6,6,6,6,6,0,0,0,0,0,0,0,0,0,4,0,0,0,0,0,5,5,5,0,0,0,0,0,0,0,0,0,0,5,5,5,5},
		{0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,0,5,5,5,5,5,0,0,0,0,0,0,0,0,0,5,5,5,5},
		{0,7,4,0,0,0,0,12,0,0,0,13,13,13,13,0,0,0,5,5,5,5,5,0,13,0,8,9,9,9,9,10,0,13,0,5,5,5,5,5,0,0,13,13,13,0,0,12,12,0,8,9,9,9,9,10,0,4,7,13,13,5,5,5,5,5,5,5,0,13,13,0,0,0,0,0,5,5,5,5},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
		{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
		{3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3},
	};

	float bg_x = 0;
	float bg_velocidade = -3; // velocidade do fundo (negativo = anda para a esquerda)

	// --- Inicialização Allegro ---
	if (!al_init()) return -1;
	al_init_font_addon();
	al_init_image_addon();
	al_install_keyboard();

	ALLEGRO_DISPLAY* display = al_create_display(largura_tela, altura_tela);
	al_set_window_position(display, 200, 200);
	al_set_window_title(display, "Spiderman: run to the home!");

	ALLEGRO_FONT* font = al_create_builtin_font();
	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

	// Carrega bitmaps (caminhos mantidos)
	ALLEGRO_BITMAP* spriteSpiderman = al_load_bitmap("./img/Sprites/spiderman.png");
	ALLEGRO_BITMAP* bg = al_load_bitmap("./img/Background.png");

	ALLEGRO_BITMAP* tiles[14] = { NULL };
	tiles[1] = al_load_bitmap("./img/Tiles/piso1.png");
	tiles[3] = al_load_bitmap("./img/Tiles/piso5.png");
	tiles[5] = al_load_bitmap("./img/Tiles/bloco.png");
	tiles[6] = al_load_bitmap("./img/Tiles/plataforma1.png");
	tiles[2] = al_load_bitmap("./img/Tiles/plataforma3.png");
	tiles[11] = al_load_bitmap("./img/Tiles/plataforma2.png");
	tiles[4] = al_load_bitmap("./img/Objects/escada.png");
	tiles[7] = al_load_bitmap("./img/Objects/placa.png");
	tiles[8] = al_load_bitmap("./img/Objects/barra1.png");
	tiles[9] = al_load_bitmap("./img/Objects/barra2.png");
	tiles[10] = al_load_bitmap("./img/Objects/barra3.png");
	tiles[12] = al_load_bitmap("./img/Objects/caixa.png");
	tiles[13] = al_load_bitmap("./img/Objects/coin3.png");

	// Verificação simples
	if (!spriteSpiderman || !bg) {
		fprintf(stderr, "Erro ao carregar bitmaps principais\n");
		return -1;
	}
	for (int i = 1; i <= 13; i++) {
		if (!tiles[i]) {
			fprintf(stderr, "Erro ao carregar tile %d\n", i);
			return -1;
		}
	}

	ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_start_timer(timer);

	// --- Loop principal ---
	bool running = true;

	while (running) {
		ALLEGRO_EVENT event;

		while (al_get_next_event(event_queue, &event)) {
			// fechar janela
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				running = false;
			}

			// teclado (aperta tecla)
			if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
				// só inicia o pulo se NÃO estiver pulando
				if (event.keyboard.keycode == ALLEGRO_KEY_SPACE && !spiderman.base.esta_pulando) {
					spiderman.base.esta_pulando = true;
					spiderman.base.tempo_pulo = 0.0f;
					spiderman.current_frame_y = 0;
					spiderman.vel_y = IMPULSO_PULO; // aplica impulso vertical para cima
				}
			}

			// timer = lógica do jogo
			if (event.type == ALLEGRO_EVENT_TIMER) {

				// BG scroll
				bg_x += bg_velocidade;
				if (bg_x <= -1280) bg_x = 0;

				// Tiles scroll
				tile_offset_x += tile_scroll_speed;
				if (tile_offset_x <= -1280) tile_offset_x = 0;

				// animação (usa velocidade para modular animação)
				spiderman.frame += spiderman.base.velocidade * 0.03f;
				if (spiderman.frame >= 3.0f) spiderman.frame = 0.0f;

				// --- PULO: aplicar física simples ---
				if (spiderman.base.esta_pulando) {
					spiderman.base.tempo_pulo += 1.0f / 60.0f;

					// aplica velocidade vertical e gravidade
					spiderman.pos_y += (int)spiderman.vel_y;
					spiderman.vel_y += GRAVIDADE;

					// se tocou o chão -> volta ao estado normal
					if (spiderman.pos_y >= altura_chao) {
						spiderman.pos_y = altura_chao;
						spiderman.vel_y = 0.0f;
						spiderman.base.esta_pulando = false;
						spiderman.current_frame_y = 65; // volta sprite normal
					}
				}

				// --- COLETA DE MOEDA ---
				int tile_col = spiderman.pos_x / TILE;
				int tile_row = spiderman.pos_y / TILE;

				if (mapa[tile_row][tile_col] == 13) {
					score++;
					mapa[tile_row][tile_col] = 0; // remove moeda
				}
			}
		}

		// Renderização
		al_clear_to_color(al_map_rgb(255, 255, 255));

		al_draw_bitmap(bg, bg_x, 0, 0);
		al_draw_bitmap(bg, bg_x + 1280, 0, 0);

		// desenha mapa
		for (int linha = 0; linha < 23; linha++) {
			for (int coluna = 0; coluna < 80; coluna++) {
				int t = mapa[linha][coluna];
				if (t == 0) continue;

				float draw_x = coluna * TILE + tile_offset_x;
				float draw_y = linha * TILE;

				// só desenha se ainda estiver na tela
				if (draw_x > -TILE && draw_x < largura_tela)
					al_draw_bitmap(tiles[t], draw_x, draw_y, 0);
			}
		}


		//al_draw_filled_rectangle(0, 0, 200, 40, al_map_rgb(255, 255, 255));
		//al_draw_text(font, al_map_rgb(0, 0, 0), 10, 10, 0, score_text);

		// desenha spiderman
		al_draw_bitmap_region(
			spriteSpiderman,
			50 * (int)spiderman.frame,
			spiderman.current_frame_y,
			42, 73,
			spiderman.pos_x, spiderman.pos_y,
			0
		);

		al_flip_display();
	}

	// --- Finalização ---
	for (int i = 1; i <= 13; i++) al_destroy_bitmap(tiles[i]);
	al_destroy_bitmap(bg);
	al_destroy_bitmap(spriteSpiderman);
	al_destroy_font(font);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);

	return 0;
}
