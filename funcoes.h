#ifndef Funcoes_h
#define funcoes_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_USERS 10
#define CPF_LENGTH 12
#define SENHA_LENGTH 5          // 4 dígitos + 1 para o terminador nulo
#define MAX_TRANSACOES 100      // Definindo o número máximo de transações a serem exibidas
#define DATA_LENGTH 20          // Definindo o tamanho máximo para a data
#define MAX_CRIPTOS 10          // Defina o número máximo de criptomoedas
#define CRIPTO_LENGTH 50        // Defina o comprimento máximo do nome da criptomoeda

typedef struct {
    char cpf[CPF_LENGTH];
    char senha[SENHA_LENGTH];
    float saldo_reais; // Adicionando um saldo para cada usuário
} Usuario;

Usuario usuarios[MAX_USERS];
int num_usuarios = 0;
int user_logado = -1; // Índice do usuário logado, -1 indica que ninguém está logado

typedef struct {
    char data[DATA_LENGTH]; // String para armazenar a data da transação
    float valor;            // Valor da transação
    float taxa;             // Taxa da transação
} Transacao;


// Variável global para armazenar a senha do usuário logado
char senha_usuario[SENHA_LENGTH];


// Funções do investido
void login_user();
int efetuar_login(char *cpf, char *senha);
int seguranca_user();
void consultar_saldo();
void consultar_extrato();
void depositar();
void sacar();
void comprar_cripto();
void vender_cripto();
void atualizar_cotacao();
void menu_principal();

//Funcoes do admin




#endif // PROJETO_H
