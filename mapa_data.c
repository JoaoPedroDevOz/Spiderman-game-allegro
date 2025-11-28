#include "mapa.h"
#include <allegro5/allegro.h>
#include <math.h>

// --- Função: tileAt ---
int tileAt(int mapa[23][40], int linha, int coluna) {
    if (linha < 0 || linha >= 23) return 0;
    if (coluna < 0) {
        int largura = 40;
        coluna = (coluna % largura + largura) % largura;
    }
    if (coluna >= 40) coluna %= 40;
    return mapa[linha][coluna];
}

// --- Função: tileNaPos ---
int tileNaPos(int mapa[23][40], float x, float y, int tileSize, int offsetX) {
    float posNoMapa = x - offsetX;
    int larguraMapaPx = 40 * tilSize;

    if (posNoMapa < 0)
        posNoMapa = fmod(posNoMapa + larguraMapaPx * 10000, larguraMapaPx);

    int coluna = (int)(posNoMapa) / tileSize;
    int linha = (int)(y) / tileSize;

    return tileAt(mapa, linha, coluna);
}

// --- Função: checarColisao ---
bool checarColisao(int mapa[23][40], float x, float y, int tileSize, int offsetX) {
    int tile = tileNaPos(mapa, x, y, tileSize, offsetX);
    return (tile == 1 || tile == 2 || tile == 3);
}

// --- Função: pegarMoeda ---
bool pegarMoeda(int mapa[23][40], float x, float y, int tileSize, int offsetX) {
    int posNoMapa = x - offsetX;
    int larguraMapaPx = 40 * tileSize;

    if (posNoMapa < 0)
        posNoMapa = fmod(posNoMapa + larguraMapaPx * 10000, larguraMapaPx);

    int coluna = (int)(posNoMapa) / tileSize;
    int linha = (int)(y) / tileSize;

    int tile = tileAt(mapa, linha, coluna);

    if (tile == 4) { // moeda
        mapa[linha][coluna] = 0;
        return true;
    }

    return false;
}
