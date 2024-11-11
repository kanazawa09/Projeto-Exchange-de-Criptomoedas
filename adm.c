#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "adm.h"

int main() {
    int menu_option;

        while (menu_option != 8) {
            printf("1. Cadastrar Investidor\n");
            printf("2. Excluir Investidor\n");
            printf("3. Cadastrar Criptomoeda\n");
            printf("4. Excluir Criptomoeda\n");
            printf("5. Consultar Saldo de Investidor\n");
            printf("6. Consultar Extrato de Investidor\n");
            printf("7. Atualizar cotacao\n");
            printf("8. Sair\n");
            printf("Selecao de Menu: ");
            scanf("%d", &menu_option);
            getchar();

            if (menu_option == 1 || menu_option == 2 || menu_option == 3 ||
                menu_option == 4 || menu_option == 5 || menu_option == 6 ||
                menu_option == 7)

                switch (menu_option) {
                    case 1:
                        add_investidor();
                        break;
                    case 2:
                        remov_investidor();
                        break;
                    case 3:
                        add_cripto();
                        break;
                    case 4:
                        remov_cripto();
                        break;
                    case 5:
                        verificar_saldo_invest();
                        break;
                    case 6:
                        verificar_extrato_invest();
                        break;
                    case 7:
                        atualizar_cotacoes_arquivo();
                        break;
                    case 8:
                        printf("Saindo...\n");
                        break;
                }

            if (menu_option != 8) {
                printf("Digite qualquer tecla para retornar ao menu: ");
                fgets(return_menu, sizeof(return_menu), stdin);
                printf("\n");
            }
        }

    printf("Fim do Programa\n");

    return 0;
}
