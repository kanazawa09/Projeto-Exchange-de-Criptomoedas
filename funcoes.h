#ifndef funcoes_h
#define funcoes_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_USERS 10
#define MAX_ADMIN 1
#define CPF_LENGTH 12
#define SENHA_LENGTH 5
#define MAX_TRANSACOES 100
#define DATA_LENGTH 20
#define MAX_CRIPTOS 10
#define CRIPTO_LENGTH 50

typedef struct {
    char cpf[CPF_LENGTH];
    char senha[SENHA_LENGTH];
    float saldo_reais;
} Usuario;

typedef struct {
    char data[DATA_LENGTH];
    float valor;
    float taxa;
} Transacao;

typedef struct {
    char cpf[CPF_LENGTH];
    char senha[SENHA_LENGTH];
} Admin;

extern Usuario usuarios[MAX_USERS];
extern int num_usuarios;
extern int user_logado;
extern int num_admin;
extern char senha_usuario[SENHA_LENGTH];

 

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

int interface_login();
int login_admin(char *cpf, char *senha);
void add_user();
void remov_user();
void add_cripto();
void remov_cripto();
void consultar_saldo_admin();
void consultar_extrato_admin();
void menu_admin();

#endif
