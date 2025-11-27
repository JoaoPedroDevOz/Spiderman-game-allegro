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

int TILE = 32;
int larguraMapa = 40;
int moedas_coletadas = 0;

typedef struct {
	bool esta_pulando;
	float tempo_pulo;
	float duracao_pulo;
	float velocidade; // velocidade horizontal / de movimento (já usada p/ animação)
} PersonagemBase;

typedef struct {
	PersonagemBase base;
	float pos_x;
	float pos_y;
	float vel_y;
	int largura;
	int altura;
	float frame;
	int current_frame_y;
} Spiderman;


bool tileSolido(int id) {
	return (
		id == 1 ||
		id == 2 ||
		id == 3 ||
		id == 5 ||
		id == 6 ||
		id == 11
		);
}

bool tileMoeda(int id) {
	return id == 13;
}

void renderizarMapaRepetindo(
	int mapa[][40],
	ALLEGRO_BITMAP* tiles[],
	float tile_offset_x,
	int linhas,
	int colunas,
	int largura_tela,
	int altura_tela,
	int TILE
) {
	int larguraMapa = colunas * TILE;

	int copia = 100;
	while (copia != 0) {
		//for (int copia = 0; copia < 20; copia++) {
		copia--;
		float offset = tile_offset_x + copia * larguraMapa;

		for (int linha = 0; linha < linhas; linha++) {
			for (int coluna = 0; coluna < colunas; coluna++) {

				int t = mapa[linha][coluna];
				if (t == 0) continue;

				float draw_x = coluna * TILE + offset;
				float draw_y = linha * TILE;

				if (draw_x > -TILE && draw_x < largura_tela)
					al_draw_bitmap(tiles[t], draw_x, draw_y, 0);
			}
		}
		//}
	}


	// quando o mapa inteiro sair da tela, reseta
	if (tile_offset_x <= -larguraMapa)
		tile_offset_x += larguraMapa;
}


int main() {
	// --- Variáveis ---
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

	// Pulo / Física
	const float GRAVIDADE = 0.5f;         // aceleração para baixo (ajuste para sentir melhor)
	const float IMPULSO_PULO = -11.0f;    // velocidade inicial para cima (negativa sobe)

	// --- mapa (mantive como estava) ---
	int mapa[23][40] = {
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

	float bg_x = 0;
	float bg_velocidade = -5; // velocidade do fundo (negativo = anda para a esquerda)

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
				// quando andar 1 TILE inteiro → gera coluna nova
				//if (tile_offset_x <= -TILE) {
				//	tile_offset_x = 0;  // corrige deslocamento
				//	//shiftMapAndGenerateNewColumn(mapa);
				//}

				// animação (usa velocidade para modular animação)
				spiderman.frame += spiderman.base.velocidade * 0.03f;
				if (spiderman.frame >= 3.0f) spiderman.frame = 0.0f;

				// --- FÍSICA DE GRAVIDADE ---
				spiderman.pos_y += spiderman.vel_y;
				spiderman.vel_y += GRAVIDADE;

				// limite de velocidade de queda
				if (spiderman.vel_y > 15.0f)
					spiderman.vel_y = 15.0f;

				printf("vel_y = %f | pos_y = %f\n", spiderman.vel_y, spiderman.pos_y);

				// --- COLISÃO COM TILES ---
				float posMapaX = spiderman.pos_x - tile_offset_x;
				int larguraMapaPx = 40 * TILE;

				// wrap do mapa
				if (posMapaX < 0)
					posMapaX = fmod(posMapaX + larguraMapaPx * 10000, larguraMapaPx);

				// --- COLISÃO: testar 2 pontos na base ---
				float baseY = spiderman.pos_y + 73;

				// 1) converter X do personagem para posição dentro do mapa
				float posMapaX1 = (spiderman.pos_x - tile_offset_x + 5);           // ponto esquerdo
				float posMapaX2 = (spiderman.pos_x - tile_offset_x + 42 - 5);      // ponto direito

				// corrigir wrap
				posMapaX1 = fmod(posMapaX1 + larguraMapaPx * 10000, larguraMapaPx);
				posMapaX2 = fmod(posMapaX2 + larguraMapaPx * 10000, larguraMapaPx);

				// colunas dos pés
				int col1 = posMapaX1 / TILE;
				int col2 = posMapaX2 / TILE;

				// linha dos pés
				int row = baseY / TILE;

				// seguranças
				if (col1 < 0) col1 = 0;
				if (col1 > 39) col1 = 39;
				if (col2 < 0) col2 = 0;
				if (col2 > 39) col2 = 39;
				if (row < 0) row = 0;
				if (row > 22) row = 22;

				int t1 = mapa[row][col1];
				int t2 = mapa[row][col2];

				// --- colisão com o chão ---
				if (tileSolido(t1) || tileSolido(t2)) {

					spiderman.pos_y = row * TILE - 73;   // alinha em cima do tile
					spiderman.vel_y = 0;
					spiderman.base.esta_pulando = false;
					spiderman.current_frame_y = 65;
				}
				else {
					spiderman.base.esta_pulando = true;
				}

				//checarColisaoChao(&spiderman, mapa, tile_offset_x, TILE);
				//updateHorizontalPhysics(mapa, &spiderman, 0.0f, tile_offset_x);

				// --- COLETA DE MOEDA ---
				//int tile_col = spiderman.pos_x / TILE;
				//int tile_row = spiderman.pos_y / TILE;

				//if (mapa[tile_row][tile_col] == 13) {
				//	score++;
				//	mapa[tile_row][tile_col] = 0; // remove moeda
				//}

				// --- COLETA DE MOEDAS ---
				{
					// Posição real do personagem no mapa (corrigindo scroll)
					float posMapaX1 = (spiderman.pos_x - tile_offset_x + 5);
					float posMapaX2 = (spiderman.pos_x - tile_offset_x + 42 - 5);

					float posMapaY1 = spiderman.pos_y + 5;
					float posMapaY2 = spiderman.pos_y + 73 - 5;

					int larguraMapaPx = 40 * TILE;

					// corrigir wrap do mapa
					posMapaX1 = fmod(posMapaX1 + larguraMapaPx * 10000, larguraMapaPx);
					posMapaX2 = fmod(posMapaX2 + larguraMapaPx * 10000, larguraMapaPx);

					// converter para tiles
					int col1 = posMapaX1 / TILE;
					int col2 = posMapaX2 / TILE;

					int row1 = posMapaY1 / TILE;
					int row2 = posMapaY2 / TILE;

					// --- verificar cada tile tocado ---
					int tiles[4][2] = {
						{row1, col1}, {row1, col2},
						{row2, col1}, {row2, col2}
					};

					for (int i = 0; i < 4; i++) {
						int r = tiles[i][0];
						int c = tiles[i][1];

						// limites de segurança
						if (r < 0 || r > 22 || c < 0 || c > 39) continue;

						if (tileMoeda(mapa[r][c])) {
							mapa[r][c] = 0;           // remove moeda do mapa
							moedas_coletadas++;
							printf("Moeda coletada! Total: %d\n", moedas_coletadas);
						}
					}
				}

			}
		}

		// Renderização
		al_clear_to_color(al_map_rgb(255, 255, 255));

		al_draw_bitmap(bg, bg_x, 0, 0);
		al_draw_bitmap(bg, bg_x + 1280, 0, 0);

		renderizarMapaRepetindo(
			mapa, tiles, tile_offset_x,
			23, 40, largura_tela, altura_tela, TILE
		);

		// desenha mapa
		//for (int linha = 0; linha < 23; linha++) {
		//	for (int coluna = 0; coluna < 40; coluna++) {
		//		int t = mapa[linha][coluna];
		//		if (t == 0) continue;

		//		float draw_x = coluna * TILE + tile_offset_x;
		//		float draw_y = linha * TILE;

		//		// só desenha se ainda estiver na tela
		//		if (draw_x > -TILE && draw_x < largura_tela)
		//			al_draw_bitmap(tiles[t], draw_x, draw_y, 0);
		//	}
		//}

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
