#ifndef Funcoes_h
#define funcoes_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_TRANSACOES 100
#define MAX_USUARIOS 10
#define MAX_CPF_LENGTH 12
#define MAX_SENHA_LENGTH 20
#define MAX_NOME_LENGTH 50

typedef struct {
    char cpf[MAX_CPF_LENGTH];
    char senha[MAX_SENHA_LENGTH];
} Admin;

typedef struct {
    char nome[50];
    int senha;
    int id;
    char cpf[12];
    float saldo_reais;
    float saldo_bitcoin;
    float saldo_ethereum;
    float saldo_ripple;
} Usuario;

typedef struct {
    char descricao[100];
    double valor;
    double taxa;
    char data[20];
} Transacao;

typedef struct {
    double valor_bitcoin;
    double valor_ethereum;
    double valor_ripple;
} Cotacao;

// Funções do projeto
Cotacao carregar_cotacoes();
void consultar_saldo(int i, Usuario usuarios[]);
void consultar_extrato(int i);
void depositar(int i, Usuario usuarios[]);
void sacar(int i, Usuario usuarios[]);
void comprar_criptomoedas(int i, Usuario usuarios[]);
void vender_criptomoedas(int i, Usuario usuarios[]);
void atualizar_cotacoes_arquivo();
void salvar_usuarios(Usuario usuarios[], int quantidade);
int carregar_usuarios(Usuario usuarios[]);
int login(Usuario usuarios[]);
void tracos();

int login_adm();
void add_investidor();

#endif // PROJETO_H
