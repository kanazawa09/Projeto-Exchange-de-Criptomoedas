#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"

// Renomeie a variável para evitar conflito
Usuario usuarios_list[MAX_USERS]; // Mudado para 'usuarios_list'
int num_usuarios = 0;

Admin admins[MAX_ADMIN]; // Renomeado para evitar conflito
int num_admin = 0;
int admin_logado = -1; // Índice do administrador logado, -1 indica que ninguém está logado

// Prototipagem da função login_admin
int login_admin(char *cpf, char *senha);

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Chama a interface de login do administrador
    interface_login();


        if (interface_login) {
        menu_admin(); // Chama o menu do admin após login bem-sucedido
    } else {
        printf("Login falhou. Verifique seu CPF e senha.\n");
    }

    return 0;
}
