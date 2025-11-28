#include "menu.h"

void inicializarMenu(MenuState* menu) {
    menu->opcao_selecionada = MENU_START;
}

void atualizarMenu(MenuState* menu, bool* sair, bool* iniciarJogo, bool* abrirRanking)
{
    *sair = false;
    *iniciarJogo = false;
    *abrirRanking = false;

    ALLEGRO_KEYBOARD_STATE teclado;
    al_get_keyboard_state(&teclado);

    if (al_key_down(&teclado, ALLEGRO_KEY_UP)) {
        if (menu->opcao_selecionada > 0)
            menu->opcao_selecionada--;
        al_rest(0.15);
    }

    if (al_key_down(&teclado, ALLEGRO_KEY_DOWN)) {
        if (menu->opcao_selecionada < MENU_SAIR)
            menu->opcao_selecionada++;
        al_rest(0.15);
    }

    if (al_key_down(&teclado, ALLEGRO_KEY_ENTER)) {

        if (menu->opcao_selecionada == MENU_START)
            *iniciarJogo = true;

        else if (menu->opcao_selecionada == MENU_RANKING)
            *abrirRanking = true;

        else if (menu->opcao_selecionada == MENU_SAIR)
            *sair = true;

        al_rest(0.15);
    }
}
