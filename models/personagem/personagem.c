#include "personagem.h"
#include <allegro5/allegro.h>

// Inicialização completa do personagem
void inicializarSpiderman(Spiderman* sp) {
    sp->pos_x = 425;
    sp->pos_y = 527;

    sp->vel_y = 0;

    sp->largura = SPIDERMAN_W;
    sp->altura = SPIDERMAN_H;

    sp->frame = 0.0f;
    sp->current_frame_y = 65;

    sp->base.esta_pulando = false;
    sp->base.tempo_pulo = 0.0f;
    sp->base.duracao_pulo = 0.5f;
    sp->base.velocidade = 5.0f;
}

// Atualiza gravidade + limite de queda
void atualizarFisicaSpiderman(Spiderman* sp, float gravidade, float vel_max_queda) {
    sp->pos_y += sp->vel_y;
    sp->vel_y += gravidade;

    if (sp->vel_y > vel_max_queda)
        sp->vel_y = vel_max_queda;
}

// Aplicar pulo
void aplicarPulo(Spiderman* sp, float impulso) {
    if (!sp->base.esta_pulando) {
        sp->base.esta_pulando = true;
        sp->base.tempo_pulo = 0.0f;
        sp->current_frame_y = 0;
        sp->vel_y = impulso;
    }
}

// Desenha sprite do spiderman
void desenharSpiderman(Spiderman* sp, ALLEGRO_BITMAP* sprite) {
    al_draw_bitmap_region(
        sprite,
        50 * (int)sp->frame,
        sp->current_frame_y,
        42, 73,
        sp->pos_x, sp->pos_y,
        0
    );
}
