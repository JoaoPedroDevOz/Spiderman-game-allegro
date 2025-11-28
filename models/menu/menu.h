#ifndef MENU_H
#define MENU_H

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>

typedef enum {
    MENU_START,
    MENU_RANKING,
    MENU_SAIR
} OpcaoMenu;

typedef struct {
    OpcaoMenu opcao_selecionada;
} MenuState;

void inicializarMenu(MenuState* menu);
void atualizarMenu(MenuState* menu, bool* sair, bool* iniciarJogo, bool* abrirRanking);

#endif
