#ifndef PERSONAGEM_H
#define PERSONAGEM_H

#include <stdbool.h>
#include <allegro5/allegro.h>

// --- Dimensões fixas do Spiderman ---
#define SPIDERMAN_W 42
#define SPIDERMAN_H 73

// Estrutura base para atributos comuns a qualquer personagem
typedef struct {
    bool esta_pulando;
    float tempo_pulo;
    float duracao_pulo;
    float velocidade;
} PersonagemBase;

// Estrutura completa do Spiderman
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

// ---- Funções do personagem ----

// Inicializa um personagem Spiderman
void inicializarSpiderman(Spiderman* sp);

// Atualiza física do pulo e velocidade vertical
void atualizarFisicaSpiderman(Spiderman* sp, float gravidade, float vel_max_queda);

// Desenha o personagem
void desenharSpiderman(
    Spiderman* sp,
    ALLEGRO_BITMAP* sprite
);

// Aplica o pulo (quando aperta espaço)
void aplicarPulo(Spiderman* sp, float impulso);

#endif
