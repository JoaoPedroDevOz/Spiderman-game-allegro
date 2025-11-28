#include "mapa.h"
#include <allegro5/allegro.h>
#include <math.h>
#include <stdlib.h>

int moedas_coletadas = 0;

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

    if (posMapaX < 0)
        posMapaX = fmod(posMapaX + larguraMapaPx * 10000, larguraMapaPx);

    float baseY = spiderman->pos_y + SPIDERMAN_H;

    float posMapaX1 = (spiderman->pos_x - tile_offset_x + 5);
    float posMapaX2 = (spiderman->pos_x - tile_offset_x + SPIDERMAN_W - 5);

    posMapaX1 = fmod(posMapaX1 + larguraMapaPx * 10000, larguraMapaPx);
    posMapaX2 = fmod(posMapaX2 + larguraMapaPx * 10000, larguraMapaPx);

    int col1 = posMapaX1 / TILE;
    int col2 = posMapaX2 / TILE;
    int row = baseY / TILE;

    if (col1 < 0) col1 = 0;
    if (col1 > 39) col1 = 39;
    if (col2 < 0) col2 = 0;
    if (col2 > 39) col2 = 39;
    if (row < 0) row = 0;
    if (row > 22) row = 22;

    int t1 = mapa[row][col1];
    int t2 = mapa[row][col2];

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

    float posMapaX1 = (spiderman->pos_x - tile_offset_x + 5);
    float posMapaX2 = (spiderman->pos_x - tile_offset_x + SPIDERMAN_W - 5);

    float posMapaY1 = spiderman->pos_y + 5;
    float posMapaY2 = spiderman->pos_y + SPIDERMAN_H - 5;

    int larguraMapaPx = 40 * TILE;

    posMapaX1 = fmod(posMapaX1 + larguraMapaPx * 10000, larguraMapaPx);
    posMapaX2 = fmod(posMapaX2 + larguraMapaPx * 10000, larguraMapaPx);

    int col1 = posMapaX1 / TILE;
    int col2 = posMapaX2 / TILE;

    int row1 = posMapaY1 / TILE;
    int row2 = posMapaY2 / TILE;

    int tiles[4][2] = {
        {row1, col1}, {row1, col2},
        {row2, col1}, {row2, col2}
    };

    for (int i = 0; i < 4; i++) {
        int r = tiles[i][0];
        int c = tiles[i][1];

        if (r < 0 || r > 22 || c < 0 || c > 39) continue;

        if (tileMoeda(mapa[r][c])) {
            mapa[r][c] = 0;
            moedas_coletadas++;
        }
    }
    return moedas_coletadas;
}

void gerarMoedasAleatoriasTempoReal(int mapa[][40], float offset, int TILE, int largura_tela, int numero_copia, float pos_x_personagem)
{
    if (rand() % 25 != 0) return;

    int largura_extremidade_direita = 18;

    int coluna_personagem = (int)((pos_x_personagem - offset) / TILE);
    int coluna_inicial = coluna_personagem + 20;

    if (coluna_inicial < 0) coluna_inicial = 0;
    if (coluna_inicial >= 40) return;

    int coluna_final = 39;

    if (coluna_final - coluna_inicial > largura_extremidade_direita) {
        coluna_inicial = coluna_final - largura_extremidade_direita + 1;
    }

    int coluna = coluna_inicial + (rand() % (coluna_final - coluna_inicial + 1));

    float x_col = coluna * TILE + offset;

    if (x_col < -TILE || x_col > largura_tela + TILE)
        return;

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
    float pos_x_personagem
) {
    int larguraMapa = colunas * TILE;

    int copia = 100;
    while (copia != 0) {
        copia--;
        float offset = tile_offset_x + copia * larguraMapa;

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

    if (tile_offset_x <= -larguraMapa)
        tile_offset_x += larguraMapa;
}
