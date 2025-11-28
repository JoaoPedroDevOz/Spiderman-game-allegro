#ifndef MAPA_H
#define MAPA_H

#include <stdbool.h>
#include <allegro5/allegro.h>
#include "personagem.h"

// ---------- CONSTANTES DO MAPA ----------
#define MAPA_LINHAS 23
#define MAPA_COLUNAS 40

// ---------- IDENTIFICADORES ----------
bool tileSolido(int id);
bool tileMoeda(int id);

// ---------- COLISÃO COM TILES ----------
void checarColisaoComTiles(
    Spiderman* spiderman,
    int mapa[][MAPA_COLUNAS],
    int TILE,
    float tile_offset_x
);

// ---------- COLETA DE MOEDAS ----------
int coletarMoedas(
    Spiderman* spiderman,
    int mapa[][MAPA_COLUNAS],
    int TILE,
    int tile_offset_x
);

// ---------- GERAÇÃO DE MOEDAS DINÂMICA ----------
void gerarMoedasAleatoriasTempoReal(
    int mapa[][MAPA_COLUNAS],
    float offset,
    int TILE,
    int largura_tela,
    int numero_copia,
    float pos_x_personagem
);

// ---------- RENDERIZAÇÃO DO MAPA REPETIDO ----------
void renderizarMapaRepetindo(
    int mapa[][MAPA_COLUNAS],
    ALLEGRO_BITMAP* tiles[],
    float tile_offset_x,
    int linhas,
    int colunas,
    int largura_tela,
    int altura_tela,
    int TILE,
    int posicao_bg_x,
    float pos_x_personagem
);

#endif
