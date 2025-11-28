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

// Enum seria uma maneira mais organizada de fazer: DEFINE MENU 0, DEFINE PLAYING 1 r DEFINE PLACAR 2.
enum GameState { // o estado atual em que o jogo está
	MENU,
	PLAYING,
	PLACAR
};

int TILE = 32;
int larguraMapa = 40;
int moedas_coletadas = 0;
int mapa[23][40];
float tile_offset_x = 0;
char score_text[30];
int score = 0;

typedef struct {
	bool esta_pulando;
	float tempo_pulo;
	float duracao_pulo;
	float velocidade;
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

void checarColisaoComTiles(Spiderman* spiderman, int mapa[][40], int TILE, float tile_offset_x)
{
	float posMapaX = spiderman->pos_x - tile_offset_x;
	int larguraMapaPx = 40 * TILE;

	// wrap do mapa
	if (posMapaX < 0)
		posMapaX = fmod(posMapaX + larguraMapaPx * 10000, larguraMapaPx);

	// base do sprite
	float baseY = spiderman->pos_y + SPIDERMAN_H;

	// pontos de colisão na base
	float posMapaX1 = (spiderman->pos_x - tile_offset_x + 5);
	float posMapaX2 = (spiderman->pos_x - tile_offset_x + SPIDERMAN_W - 5);

	// wrap X
	posMapaX1 = fmod(posMapaX1 + larguraMapaPx * 10000, larguraMapaPx);
	posMapaX2 = fmod(posMapaX2 + larguraMapaPx * 10000, larguraMapaPx);

	// converter para tiles
	int col1 = posMapaX1 / TILE;
	int col2 = posMapaX2 / TILE;
	int row = baseY / TILE;

	// limites
	if (col1 < 0) col1 = 0;
	if (col1 > 39) col1 = 39;
	if (col2 < 0) col2 = 0;
	if (col2 > 39) col2 = 39;
	if (row < 0) row = 0;
	if (row > 22) row = 22;

	int t1 = mapa[row][col1];
	int t2 = mapa[row][col2];

	// --- COLISÃO COM CHÃO ---
	if (tileSolido(t1) || tileSolido(t2)) {
		spiderman->pos_y = row * TILE - SPIDERMAN_H;
		spiderman->vel_y = 0;
		spiderman->base.esta_pulando = false;
		spiderman->current_frame_y = 65;
	}
	else {
		spiderman->base.esta_pulando = true;
	}
}

int coletarMoedas(Spiderman* spiderman, int mapa[][40], int TILE, int tile_offset_x) {
	// Captura a posição do personagem no mapa
	// Sem o offset do tile, a posição X estaria errada, muito à direita do personagem
	float posMapaX1 = (spiderman->pos_x - tile_offset_x + 5);
	float posMapaX2 = (spiderman->pos_x - tile_offset_x + 42 - 5);

	float posMapaY1 = spiderman->pos_y + 5;
	float posMapaY2 = spiderman->pos_y + 73 - 5;

	int larguraMapaPx = 40 * TILE;

	// wrap X do mapa para mapas repetidos, garantindo a posição dentro do limite.
	posMapaX1 = fmod(posMapaX1 + larguraMapaPx * 10000, larguraMapaPx);
	posMapaX2 = fmod(posMapaX2 + larguraMapaPx * 10000, larguraMapaPx);

	int col1 = posMapaX1 / TILE;
	int col2 = posMapaX2 / TILE;

	int row1 = posMapaY1 / TILE;
	int row2 = posMapaY2 / TILE;

	// limites
	int tiles[4][2] = {
		{row1, col1}, {row1, col2},
		{row2, col1}, {row2, col2}
	};

	for (int i = 0; i < 4; i++) {
		int r = tiles[i][0];
		int c = tiles[i][1];

		// limites do mapa, verificação extra
		if (r < 0 || r > 22 || c < 0 || c > 39) continue;

		if (tileMoeda(mapa[r][c])) {

			// remove moeda atual
			mapa[r][c] = 0;

			moedas_coletadas++;
		}
	}
	return moedas_coletadas;
}

void gerarMoedasAleatoriasTempoReal(int mapa[][40], float offset, int TILE, int largura_tela, int numero_copia, float pos_x_personagem)
{
	// Probabilidade base
	if (rand() % 25 != 0) return;

	int largura_extremidade_direita = 18;

	// Calcula a coluna inicial baseada na posição do personagem
	int coluna_personagem = (int)((pos_x_personagem - offset) / TILE);
	int coluna_inicial = coluna_personagem + 20;

	// Garante que não ultrapasse o limite do mapa
	if (coluna_inicial < 0) coluna_inicial = 0;
	if (coluna_inicial >= 40) return; // Personagem já está no final do mapa

	// Coluna final (extremidade direita do mapa)
	int coluna_final = 39;

	// Se a área é muito grande, limita para as últimas colunas
	if (coluna_final - coluna_inicial > largura_extremidade_direita) {
		coluna_inicial = coluna_final - largura_extremidade_direita + 1;
	}

	// Escolhe uma coluna aleatória na área à direita
	int coluna = coluna_inicial + (rand() % (coluna_final - coluna_inicial + 1));

	// Posição real da coluna na tela
	float x_col = coluna * TILE + offset;

	// Só gera se a coluna está visível ou prestes a entrar na tela
	if (x_col < -TILE || x_col > largura_tela + TILE)
		return;

	// Procura um tile sólido com espaço livre acima
	for (int linha = 1; linha < 23; linha++) {
		int abaixo = mapa[linha][coluna];
		int acima = mapa[linha - 1][coluna];

		bool solido = tileSolido(abaixo);

		if (solido && acima == 0) {
			mapa[linha - 1][coluna] = 13;
			return;
		}
	}
}

void renderizarMapaRepetindo(
	int mapa[][40],
	ALLEGRO_BITMAP* tiles[],
	float tile_offset_x,
	int linhas,
	int colunas,
	int largura_tela,
	int altura_tela,
	int TILE,
	int posicao_bg_x,
	float pos_x_personagem  // Adicione este parâmetro
) {
	int larguraMapa = colunas * TILE;

	int copia = 100;
	while (copia != 0) {
		copia--;
		float offset = tile_offset_x + copia * larguraMapa;

		// gerar moedas em tempo real para essa cópia do mapa
		if (offset + larguraMapa > -TILE && offset < largura_tela + TILE) {
			gerarMoedasAleatoriasTempoReal(mapa, offset, TILE, largura_tela, copia, pos_x_personagem);
		}

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

	//Variáveis do menu.
	// jogo já inicia com o menu selecionado (status do jogo = MENU).
	enum GameState gameState = MENU;
	int menu_selection = 0;
	// para não registrar as setas como pressionadas continuamente.
	int key_released = 1;

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
	al_init_ttf_addon();

	ALLEGRO_DISPLAY* display = al_create_display(largura_tela, altura_tela);
	al_set_window_position(display, 200, 200);
	al_set_window_title(display, "Spiderman: run to the home!");

	ALLEGRO_FONT* fonte_titulo = al_load_font("arial.ttf", 120, 0);    // Título grande
	ALLEGRO_FONT* fonte_menu = al_load_font("arial.ttf", 36, 0);      // Menu médio
	ALLEGRO_FONT* fonte_placar = al_load_font("arial.ttf", 28, 0);    // Placar

	ALLEGRO_TIMER* timer = al_create_timer(1.0 / 60.0);

	// Carrega bitmaps (caminhos mantidos)
	ALLEGRO_BITMAP* spriteSpiderman = al_load_bitmap("./img/Sprites/spiderman.png");
	ALLEGRO_BITMAP* bg = al_load_bitmap("./img/Background.png");

	// sugestão do copilot do proprio vs: aqui ele sugeriu verificar se as fontes foram carregadas corretamente
	//caso não, retornar para a fonte padrão do allegro
	if (!fonte_titulo) fonte_titulo = al_create_builtin_font();
	if (!fonte_menu) fonte_menu = al_create_builtin_font();
	if (!fonte_placar) fonte_placar = al_create_builtin_font();

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
				// ⭐ VOLTAR AO MENU DURANTE O JOGO
				else if (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					gameState = MENU;
				}
			}

			// timer = lógica do jogo
			if (event.type == ALLEGRO_EVENT_TIMER) {

				// BG scroll
				bg_x += bg_velocidade;
				if (bg_x <= -1280) bg_x = 0;

				// Tiles scroll
				tile_offset_x += tile_scroll_speed;

				// animação (usa velocidade para modular animação)
				spiderman.frame += spiderman.base.velocidade * 0.03f;
				if (spiderman.frame >= 3.0f) spiderman.frame = 0.0f;

				// --- FÍSICA DE GRAVIDADE ---
				spiderman.pos_y += spiderman.vel_y;
				spiderman.vel_y += GRAVIDADE;

				// limite de velocidade de queda
				if (spiderman.vel_y > 15.0f)
					spiderman.vel_y = 15.0f;


				checarColisaoComTiles(&spiderman, mapa, TILE, tile_offset_x);
				score = coletarMoedas(&spiderman, mapa, TILE, tile_offset_x);
				sprintf(score_text, "Moedas: %d", score);
			}
		}

		// Renderização
		al_clear_to_color(al_map_rgb(255, 255, 255));

		al_draw_bitmap(bg, bg_x, 0, 0);
		al_draw_bitmap(bg, bg_x + 1280, 0, 0);

		renderizarMapaRepetindo(
			mapa, tiles, tile_offset_x,
			23, 40, largura_tela, altura_tela, TILE, bg_x, spiderman.pos_x
		);

		//al_draw_filled_rectangle(0, 0, 200, 40, al_map_rgb(255, 255, 255));
		al_draw_text(fonte_placar, al_map_rgb(255, 255, 255), 20, 20, 0, score_text);


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
	al_destroy_font(fonte_titulo);
	al_destroy_font(fonte_menu);
	al_destroy_font(fonte_placar);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);

	return 0;
}
