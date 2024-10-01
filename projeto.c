#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "projeto.h"


void tracos() {
    printf("-------------------------------------------------\n");
}

void salvar_usuarios(Usuario usuarios[], int quantidade) {
    FILE *file = fopen("usuarios.bin", "wb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para escrita");
        return;
    }
    fwrite(usuarios, sizeof(Usuario), quantidade, file);
    fclose(file);
}

int carregar_usuarios(Usuario usuarios[]) {
    FILE *file = fopen("usuarios.bin", "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo para leitura");
        return 0; // Retorna 0 se não conseguir abrir o arquivo
    }
    int quantidade = fread(usuarios, sizeof(Usuario), MAX_USUARIOS, file);
    fclose(file);
    return quantidade; // Retorna a quantidade de usuários lidos
}

int login(Usuario usuarios[]) {
    char cpf[12];
    int senha;
    
    printf("Digite o CPF: ");
    scanf("%s", cpf);
    printf("Digite a senha: ");
    scanf("%d", &senha);

    for (int i = 0; i < MAX_USUARIOS; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0 && usuarios[i].senha == senha) {
            return i; // Retorna o índice do usuário logado
        }
    }
    printf("CPF ou senha inválidos!\n");
    return -1; // Retorna -1 se o login falhar
}

Cotacao carregar_cotacoes() {
    tracos();
    Cotacao cotacoes;
    FILE *arquivo_cotacao = fopen("cotacao.bin", "rb");
    if (arquivo_cotacao == NULL) {
        printf("Erro ao abrir o arquivo de cotacoes.\n");
        cotacoes.valor_bitcoin = 0;
        cotacoes.valor_ethereum = 0;
        cotacoes.valor_ripple = 0;
        return cotacoes;
    }

    fread(&cotacoes, sizeof(Cotacao), 1, arquivo_cotacao);
    fclose(arquivo_cotacao);
    tracos();
    return cotacoes;
}

void consultar_saldo(int i, Usuario usuarios[]) {
    tracos();
    printf("Nome: %s\n", usuarios[i].nome);
    printf("CPF: %s\n", usuarios[i].cpf);
    printf("Saldo em Reais: %.2lf\n", usuarios[i].saldo_reais);
    printf("Saldo em Bitcoin: %.2lf\n", usuarios[i].saldo_bitcoin);
    printf("Saldo em Ethereum: %.2lf\n", usuarios[i].saldo_ethereum);
    printf("Saldo em Ripple: %.2lf\n", usuarios[i].saldo_ripple);
    tracos();
}

void consultar_extrato(int i) {
    tracos();
    FILE *arquivo_extrato;
    Transacao extrato[MAX_TRANSACOES];
    int contador = 0;

    char nome_arquivo_extrato[50];
    sprintf(nome_arquivo_extrato, "extrato_user%d.bin", i);
    arquivo_extrato = fopen(nome_arquivo_extrato, "rb");
    if (arquivo_extrato == NULL) {
        printf("Erro ao abrir o arquivo do usuario %d.\n", i);
        return;
    }

    while (contador < MAX_TRANSACOES && fread(&extrato[contador], sizeof(Transacao), 1, arquivo_extrato)) {
        contador++;
    }

    fclose (arquivo_extrato);

    printf("Extrato das ultimas %d transacoes:\n", contador);
    for (int j = 0; j < contador; j++) {
        printf("Transacao %d: %s - Valor: %.2f\n", j + 1, extrato[j].descricao, extrato[j].valor);
    }

    if (contador == 0) {
        printf("Nenhuma transacao encontrada.\n");
    }
    tracos();
}

void depositar(int i, Usuario usuarios[]) {
    tracos();
    double valor_deposito;
    FILE *arquivo_info_usuario = fopen("info_usuario.bin", "rb+");
    if (arquivo_info_usuario == NULL) {
        printf("Erro ao abrir o arquivo de informacoes do usuario.\n");
        return;
    }

    printf("Digite o valor a ser depositado: R$ ");
    scanf("%lf", &valor_deposito);

    usuarios[i].saldo_reais += valor_deposito;

    fseek(arquivo_info_usuario, i * sizeof(Usuario), SEEK_SET);
    fwrite(&usuarios[i], sizeof(Usuario), 1, arquivo_info_usuario);
    fclose(arquivo_info_usuario);

    printf("Deposito de R$ %.2lf realizado com sucesso! Novo saldo: R$ %.2lf\n", valor_deposito, usuarios[i].saldo_reais);
    tracos();
}

void sacar(int i, Usuario usuarios[]) {
    tracos();
    double valor_saque;
    int senha_digitada;
    FILE *arquivo_info_usuario = fopen("info_usuario.bin", "rb+");
    if (arquivo_info_usuario == NULL) {
        printf("Erro ao abrir o arquivo de informacoes do usuario.\n");
        return;
    }

    printf("Digite sua senha para realizar o saque: ");
    scanf("%d", &senha_digitada);

    if (senha_digitada != usuarios[i].senha) {
        printf("Senha incorreta. Saque nao realizado.\n");
        fclose(arquivo_info_usuario);
        return;
    }

    printf("Digite o valor do saque: R$ ");
    scanf("%lf", &valor_saque);

    if (valor_saque > usuarios[i].saldo_reais) {
        printf("Saldo insuficiente. Saque nao realizado.\n");
        fclose(arquivo_info_usuario);
        return;
    }

    usuarios[i].saldo_reais -= valor_saque;

    fseek(arquivo_info_usuario, i * sizeof(Usuario), SEEK_SET);
    fwrite(&usuarios[i], sizeof(Usuario), 1, arquivo_info_usuario);
    fclose(arquivo_info_usuario);

    printf("Saque de R$ %.2lf realizado com sucesso! Novo saldo: R$ %.2lf\n", valor_saque, usuarios[i].saldo_reais);
    tracos();
}

void comprar_criptomoedas(int i, Usuario usuarios[]) {
    tracos();
    int tipo_criptomoeda;
    double valor_compra, valor_cripto;
    double taxa, valor_total;
    int senha_digitada;
    FILE *arquivo_info_usuario;
    FILE *arquivo_extrato;
    Transacao transacao;

    Cotacao cotacoes = carregar_cotacoes();

    if (cotacoes.valor_bitcoin == 0) {
        printf("Erro ao carregar as cotacoes. Tente novamente mais tarde.\n");
        return;
    }

    arquivo_info_usuario = fopen("info_usuario.bin", "rb+");
    if (arquivo_info_usuario == NULL) {
        printf("Erro ao abrir o arquivo de informacoes do usuario.\n");
        return;
    }

    printf("Digite sua senha: ");
    scanf("%d", &senha_digitada);

    if (senha_digitada != usuarios[i].senha) {
        printf("Senha incorreta. Compra nao realizada.\n");
        fclose(arquivo_info_usuario);
        return;
    }

    printf("Escolha a criptomoeda para comprar:\n1. Bitcoin\n2. Ethereum\n3. Ripple\nSelecione: ");
    scanf("%d", &tipo_criptomoeda);

    printf("Digite o valor a ser investido: R$ ");
    scanf("%lf", &valor_compra);

    switch (tipo_criptomoeda) {
        case 1:
            valor_cripto = valor_compra / cotacoes.valor_bitcoin;
            taxa = valor_compra * 0.02;
            valor_total = valor_compra + taxa;
            if (valor_total > usuarios[i].saldo_reais) {
                printf("Saldo insuficiente para comprar Bitcoin.\n");
                fclose(arquivo_info_usuario);
                return;
            }
            break;
        case 2:
            valor_cripto = valor_compra / cotacoes.valor_ethereum;
            taxa = valor_compra * 0.01;
            valor_total = valor_compra + taxa;
            if (valor_total > usuarios[i].saldo_reais) {
                printf("Saldo insuficiente para comprar Ethereum.\n");
                fclose(arquivo_info_usuario);
                return;
            }
            break;
        case 3:
            valor_cripto = valor_compra / cotacoes.valor_ripple;
            taxa = valor_compra * 0.01;
            valor_total = valor_compra + taxa;
            if (valor_total > usuarios[i].saldo_reais) {
                printf("Saldo insuficiente para comprar Ripple.\n");
                fclose(arquivo_info_usuario);
                return;
            }
            break;
        default:
            printf("Opcao invalida. Compra nao realizada.\n");
            fclose(arquivo_info_usuario);
            return;
    }

    printf("Confirmar compra? (1-Sim / 0-Nao): ");
    int confirmacao;
    scanf("%d", &confirmacao);

    if (confirmacao == 1) {
        usuarios[i].saldo_reais -= valor_total;
        if (tipo_criptomoeda == 1) usuarios[i].saldo_bitcoin += valor_cripto;
        else if (tipo_criptomoeda == 2) usuarios[i].saldo_ethereum += valor_cripto;
        else if (tipo_criptomoeda == 3) usuarios[i].saldo_ripple += valor_cripto;

        fseek(arquivo_info_usuario, i * sizeof(Usuario), SEEK_SET);
        fwrite(&usuarios[i], sizeof(Usuario), 1, arquivo_info_usuario);

        char nome_arquivo_extrato[50];
        sprintf(nome_arquivo_extrato, "extrato_user%d.bin", i);
        arquivo_extrato = fopen(nome_arquivo_extrato, "ab");
        if (arquivo_extrato == NULL) {
            printf("Erro ao abrir o arquivo de extrato do usuario.\n");
            fclose(arquivo_info_usuario);
            return;
        }

        sprintf(transacao.descricao, "Compra de %s", tipo_criptomoeda == 1 ? "Bitcoin" : tipo_criptomoeda == 2 ? "Ethereum" : "Ripple");
        transacao.valor = valor_compra;
        transacao.taxa = taxa;
        time_t now = time(NULL);
        strftime(transacao.data, sizeof(transacao.data), "%Y-%m-%d %H:%M:%S", localtime(&now));

        fwrite(&transacao, sizeof(Transacao), 1, arquivo_extrato);

        fclose(arquivo_extrato);

        printf("Compra realizada com sucesso! Novo saldo: R$ %.2lf\n", usuarios[i].saldo_reais);
    } else {
        printf("Compra cancelada.\n");
    }
    tracos();
    fclose(arquivo_info_usuario);
}

void vender_criptomoedas(int i, Usuario usuarios[]) {
    tracos();
    int tipo_criptomoeda;
    double valor_venda;
    double taxa;
    double valor_total;
    int senha_digitada;
    FILE *arquivo_info_usuario;
    FILE *arquivo_extrato;
    Transacao transacao;

    // Carrega as cotações
    Cotacao cotacao = carregar_cotacoes();

    // Abre o arquivo de informações do usuário
    arquivo_info_usuario = fopen("info_usuario.bin", "rb+");
    if (arquivo_info_usuario == NULL) {
        printf("Erro ao abrir o arquivo de informacoes do usuario.\n");
        return;
    }

    // Verifica a senha do usuário
    printf("Digite sua senha para realizar a venda: ");
    scanf("%d", &senha_digitada);
    if (senha_digitada != usuarios[i].senha) {
        printf("Senha incorreta. Venda nao realizada.\n");
        fclose(arquivo_info_usuario);
        return;
    }

    // Escolha da criptomoeda para venda
    printf("Escolha a criptomoeda para vender:\n");
    printf("1. Bitcoin\n");
    printf("2. Ethereum\n");
    printf("3. Ripple\n");
    printf("Selecione: ");
    scanf("%d", &tipo_criptomoeda);

    // Valor da venda
    printf("Digite o valor da criptomoeda a ser vendida: R$ ");
    scanf("%lf", &valor_venda);

    // Cálculo da venda e verificação de saldo
    switch (tipo_criptomoeda) {
        case 1: // Bitcoin
            taxa = valor_venda * 0.02;
            valor_total = valor_venda - taxa;
            if (valor_venda / cotacao.valor_bitcoin > usuarios[i].saldo_bitcoin) {
                printf("Saldo insuficiente para vender Bitcoin.\n");
                fclose(arquivo_info_usuario);
                return;
            }
            break;
        case 2: // Ethereum
            taxa = valor_venda * 0.01;
            valor_total = valor_venda - taxa;
            if (valor_venda / cotacao.valor_ethereum > usuarios[i].saldo_ethereum) {
                printf("Saldo insuficiente para vender Ethereum.\n");
                fclose(arquivo_info_usuario);
                return;
            }
            break;
        case 3: // Ripple
            taxa = valor_venda * 0.01;
            valor_total = valor_venda - taxa;
            if (valor_venda / cotacao.valor_ripple > usuarios[i].saldo_ripple) {
                printf("Saldo insuficiente para vender Ripple.\n");
                fclose(arquivo_info_usuario);
                return;
            }
            break;
        default:
            printf("Opcao invalida. Venda nao realizada.\n");
            fclose(arquivo_info_usuario);
            return;
    }

    // Resumo da venda
    printf("Resumo da Venda:\n");
    printf("Criptomoeda: %s\n", tipo_criptomoeda == 1 ? "Bitcoin" : tipo_criptomoeda == 2 ? "Ethereum" : "Ripple");
    printf("Valor da Venda: R$ %.2lf\n", valor_venda);
    printf("Taxa: R$ %.2lf\n", taxa);
    printf("Valor Total a ser creditado: R$ %.2lf\n", valor_total);
    printf("Deseja confirmar a venda? (1 - Sim, 0 - Nao): ");
    int confirmacao;
    scanf("%d", &confirmacao);

    if (confirmacao == 1) {
        // Atualiza os saldos
        usuarios[i].saldo_reais += valor_total;

        switch (tipo_criptomoeda) {
            case 1:
                usuarios[i].saldo_bitcoin -= valor_venda / cotacao.valor_bitcoin;
                strcpy(transacao.descricao, "Venda de Bitcoin");
                break;
            case 2:
                usuarios[i].saldo_ethereum -= valor_venda / cotacao.valor_ethereum;
                strcpy(transacao.descricao, "Venda de Ethereum");
                break;
            case 3:
                usuarios[i].saldo_ripple -= valor_venda / cotacao.valor_ripple;
                strcpy(transacao.descricao, "Venda de Ripple");
                break;
        }

        // Registra a transação
        transacao.valor = valor_total; // Armazena o valor total recebido
        transacao.taxa = taxa;
        time_t now = time(NULL);
        strftime(transacao.data, sizeof(transacao.data), "%Y-%m-%d %H:%M:%S", localtime(&now));

        // Salva as informações do usuário
        fseek(arquivo_info_usuario, i * sizeof(Usuario), SEEK_SET);
        fwrite(&usuarios[i], sizeof(Usuario), 1, arquivo_info_usuario);

        // Abre o arquivo de extrato
        char nome_arquivo_extrato[50];
        sprintf(nome_arquivo_extrato, "extrato_user%d.bin", i);
        arquivo_extrato = fopen(nome_arquivo_extrato, "ab");
        if (arquivo_extrato == NULL) {
            printf("Erro ao abrir o arquivo de extrato do usuario.\n");
            fclose(arquivo_info_usuario);
            return;
        }

        fwrite(&transacao, sizeof(Transacao), 1, arquivo_extrato);
        fclose(arquivo_extrato);

        printf("Venda realizada com sucesso! Novo saldo em Reais: R$ %.2lf\n", usuarios[i].saldo_reais);
    } else {
        printf("Venda cancelada.\n");
    }
    tracos();
    fclose(arquivo_info_usuario);
}

double atualizar_cotacao(double cotacao_atual) {
    tracos();
    double fator = ((rand() % 101) / 100.0) - 0.05;
    double nova_cotacao = cotacao_atual * (1 + fator);

    if (nova_cotacao < 0) {
        nova_cotacao = 0;
    }
    tracos();
    return nova_cotacao;
}

void atualizar_cotacoes_arquivo() {
    tracos();
    FILE *arquivo_cotacao;
    double cotacoes[3];

    arquivo_cotacao = fopen("cotacao.bin", "rb+");
    if (arquivo_cotacao == NULL) {
        printf("Erro ao abrir o arquivo cotacao.bin.\n");
        return;
    }

    fread(cotacoes, sizeof(double), 3, arquivo_cotacao);

    cotacoes[0] = atualizar_cotacao(cotacoes[0]);
    cotacoes[1] = atualizar_cotacao(cotacoes[1]);
    cotacoes[2] = atualizar_cotacao(cotacoes[2]);

    fseek(arquivo_cotacao, 0, SEEK_SET);
    fwrite(cotacoes, sizeof(double), 3, arquivo_cotacao);

    fclose(arquivo_cotacao);

    printf("Cotacoes Atualizadas:\n");
    printf("Bitcoin: R$ %.2lf\n", cotacoes[0]);
    printf("Ethereum: R$ %.2lf\n", cotacoes[1]);
    printf("Ripple: R$ %.2lf\n", cotacoes[2]);
    tracos();
}