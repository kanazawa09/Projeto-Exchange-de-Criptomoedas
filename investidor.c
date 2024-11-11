
int main() {
    int menu_option;

        while (menu_option != 8) {
            printf("1. Consultar saldo\n");
            printf("2. Consultar extrato\n");
            printf("3. Depositar\n");
            printf("4. Sacar\n");
            printf("5. Comprar criptomoedas\n");
            printf("6. Vender criptomoedas\n");
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
                        consultar_saldo(i, usuarios);
                        break;
                    case 2:
                        consultar_extrato(i);
                        break;
                    case 3:
                        depositar(i, usuarios);
                        break;
                    case 4:
                        sacar(i, usuarios);
                        break;
                    case 5:
                        comprar_criptomoedas(i, usuarios);
                        break;
                    case 6:
                        vender_criptomoedas(i, usuarios);
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
