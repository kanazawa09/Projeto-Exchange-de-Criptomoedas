#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"


//FUNCAO REALIZAR LOGIN
void login_user() {
    FILE *file = fopen("user_login_info.bin", "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de login");
        return;
    }

    // Lê os dados do arquivo
    while (fread(&usuarios[num_usuarios], sizeof(Usuario), 1, file) && num_usuarios < MAX_USERS) {
        num_usuarios++;
    }

    fclose(file);
}

int efetuar_login(char *cpf, char *senha) {
    for (int i = 0; i < num_usuarios; i++) {
        if (strcmp(usuarios[i].cpf, cpf) == 0 && strcmp(usuarios[i].senha, senha) == 0) {
            user_logado = i; // Armazena o índice do usuário logado
            strcpy(senha_usuario, usuarios[i].senha); // Armazena a senha do usuário logado
            printf("Login bem-sucedido! Usuário logado: %s (Número: %d)\n", usuarios[user_logado].cpf, user_logado + 1);
            return 1; // Login bem-sucedido
        }
    }
    return 0; // Login falhou
}


// FUNCAO DE SEGURANCA
int seguranca_user() {
    char senha_input[SENHA_LENGTH];
    printf("Digite sua senha para continuar: ");
    scanf("%s", senha_input);

    // Compara a senha informada com a senha armazenada
    if (strcmp(senha_input, senha_usuario) == 0) {
        return 1; // Senha correta
    } else {
        printf("Senha incorreta. Operação cancelada.\n");
        return 0; // Senha incorreta
    }
}


//FUNCAO CONSULTAR SALDO
void consultar_saldo() {
    if (user_logado != -1) {
        // Exibe o saldo em reais
        printf("Saldo de Reais: R$ %.2f\n", usuarios[user_logado].saldo_reais);
        
        char cripto_nome[CRIPTO_LENGTH];
        float cripto_valores[MAX_CRIPTOS];

        // Lê os nomes das criptomoedas
        FILE *file_cripto = fopen("cripto_nome.bin", "rb");
        if (file_cripto == NULL) {
            perror("Erro ao abrir o arquivo de criptomoedas");
            return;
        }

        // Lê os nomes das criptomoedas
        for (int i = 0; i < MAX_CRIPTOS; i++) {
            fread(cripto_nome, sizeof(char), CRIPTO_LENGTH, file_cripto);
            printf("Criptomoeda %d: %s\n", i + 1, cripto_nome);
        }
        fclose(file_cripto);

        // Lê o valor da criptomoeda do arquivo do usuário
        char user_file[20];
        sprintf(user_file, "user%d.bin", user_logado + 1); // Ex: user1.bin
        FILE *file_user = fopen(user_file, "rb");
        if (file_user == NULL) {
            perror("Erro ao abrir o arquivo do usuário");
            return;
        }

        // Lê o saldo em reais e os valores das criptomoedas
        for (int i = 0; i < MAX_CRIPTOS; i++) {
            fread(&cripto_valores[i], sizeof(float), 1, file_user);
            if (i == 0) {
                // O primeiro valor é o saldo em reais, que já foi impresso
                continue; // Pula a impressão do saldo novamente
            } else {
                printf("Valor da Criptomoeda %d: R$ %.2f\n", i, cripto_valores[i]);
            }
        }
        fclose(file_user);
    } else {
        printf("Nenhum usuário logado.\n");
    }
}


//FUNCAO CONSULTAR EXTRATO
void consultar_extrato() {
    if (user_logado != -1) {
        // Nome do arquivo do extrato do usuário
        char extrato_file[20];
        sprintf(extrato_file, "extrato%d.bin", user_logado + 1); // Ex: extrato1.bin

        // Abrindo o arquivo do extrato
        FILE *file_extrato = fopen(extrato_file, "rb");
        if (file_extrato == NULL) {
            perror("Erro ao abrir o arquivo de extrato");
            return;
        }

        // Lê as transações e conta quantas foram lidas
        Transacao transacoes[MAX_TRANSACOES];
        size_t total_transacoes = fread(transacoes, sizeof(Transacao), MAX_TRANSACOES, file_extrato);
        fclose(file_extrato);

        // Se houver menos de 100 transações, ajusta o número para exibir
        if (total_transacoes > MAX_TRANSACOES) {
            total_transacoes = MAX_TRANSACOES;
        }

        // Exibe as transações
        printf("Últimas %zu transações:\n", total_transacoes);
        for (size_t i = 0; i < total_transacoes; i++) {
            printf("Data: %s | Valor: R$ %.2f | Taxa: R$ %.2f\n", 
                   transacoes[i].data, transacoes[i].valor, transacoes[i].taxa);
        }
    } else {
        printf("Nenhum usuário logado.\n");
    }
}


//FUNCAO DEPOSITAR REAIS
void depositar() {
    if (user_logado != -1) {
        // Nome do arquivo do usuário
        char user_file[20];
        sprintf(user_file, "user%d.bin", user_logado + 1); // Ex: user1.bin

        // Abrindo o arquivo do usuário para leitura
        FILE *file_user = fopen(user_file, "rb+"); // rb+ para leitura e escrita
        if (file_user == NULL) {
            perror("Erro ao abrir o arquivo do usuário");
            return;
        }

        // Lendo o saldo atual do usuário
        float saldo_atual;
        fread(&saldo_atual, sizeof(float), 1, file_user);

        // Solicita o valor a ser depositado
        float valor_deposito;
        printf("Digite o valor que deseja depositar: R$ ");
        scanf("%f", &valor_deposito);

        // Confirmação do depósito
        printf("Você deseja depositar R$ %.2f? (s/n): ", valor_deposito);
        char confirmacao;
        scanf(" %c", &confirmacao); // O espaço antes de %c ignora espaços em branco

        if (confirmacao == 's' || confirmacao == 'S') {
            // Atualiza o saldo
            saldo_atual += valor_deposito;

            // Move o ponteiro do arquivo para o início para escrever o novo saldo
            fseek(file_user, 0, SEEK_SET);
            fwrite(&saldo_atual, sizeof(float), 1, file_user);

            printf("Depósito de R$ %.2f realizado com sucesso! Novo saldo: R$ %.2f\n", valor_deposito, saldo_atual);
        } else {
            printf("Depósito cancelado.\n");
        }

        fclose(file_user);
    } else {
        printf("Nenhum usuário logado.\n");
    }
}


//FUNCAO SACAR REAIS
void sacar() {
    if (user_logado != -1) {
        // Nome do arquivo do usuário
        char user_file[20];
        sprintf(user_file, "user%d.bin", user_logado + 1); // Ex: user1.bin

        // Abrindo o arquivo do usuário para leitura e escrita
        FILE *file_user = fopen(user_file, "rb+"); // rb+ para leitura e escrita
        if (file_user == NULL) {
            perror("Erro ao abrir o arquivo do usuário");
            return;
        }

        // Lendo o saldo atual do usuário
        float saldo_atual;
        fread(&saldo_atual, sizeof(float), 1, file_user);

        // Solicita o valor a ser sacado
        float valor_saque;
        printf("Digite o valor que deseja sacar: R$ ");
        scanf("%f", &valor_saque);

        // Verifica se o saldo é suficiente
        if (valor_saque > saldo_atual) {
            printf("Saldo insuficiente. Seu saldo atual é R$ %.2f.\n", saldo_atual);
        } else {
            // Confirmação do saque
            printf("Você deseja sacar R$ %.2f? (s/n): ", valor_saque);
            char confirmacao;
            scanf(" %c", &confirmacao); // O espaço antes de %c ignora espaços em branco

            if (confirmacao == 's' || confirmacao == 'S') {
                // Atualiza o saldo
                saldo_atual -= valor_saque;

                // Move o ponteiro do arquivo para o início para escrever o novo saldo
                fseek(file_user, 0, SEEK_SET);
                fwrite(&saldo_atual, sizeof(float), 1, file_user);

                printf("Saque de R$ %.2f realizado com sucesso! Novo saldo: R$ %.2f\n", valor_saque, saldo_atual);
            } else {
                printf("Saque cancelado.\n");
            }
        }

        fclose(file_user);
    } else {
        printf("Nenhum usuário logado.\n");
    }
}


//FUNCAO COMPRAR CRIPTOMOEDAS
void comprar_cripto() {
    if (user_logado == -1) {
        printf("Nenhum usuario logado.\n");
        return;
    }

    // Abrindo o arquivo de cotacoes e nomes das criptomoedas
    char cripto_nome[MAX_CRIPTOS][CRIPTO_LENGTH];
    float cripto_valores[MAX_CRIPTOS];

    FILE *file_nome = fopen("cripto_nome.bin", "rb");
    FILE *file_valores = fopen("cripto_cotacao.bin", "rb");
    if (file_nome == NULL || file_valores == NULL) {
        perror("Erro ao abrir os arquivos de criptomoedas");
        if (file_nome) fclose(file_nome);
        if (file_valores) fclose(file_valores);
        return;
    }

    // Lendo os nomes e valores das criptomoedas
    for (int i = 0; i < MAX_CRIPTOS; i++) {
        fread(cripto_nome[i], sizeof(char), CRIPTO_LENGTH, file_nome);
        fread(&cripto_valores[i], sizeof(float), 1, file_valores);
    }
    fclose(file_nome);
    fclose(file_valores);

    // Exibindo as criptomoedas e suas cotacoes
    printf("Criptomoedas disponiveis:\n");
    for (int i = 0; i < MAX_CRIPTOS; i++) {
        printf("%d. %s - R$ %.2f\n", i + 1, cripto_nome[i], cripto_valores[i]);
    }

    // Solicitar a escolha da criptomoeda
    int escolha;
    printf("Escolha a criptomoeda que deseja comprar (1-%d): ", MAX_CRIPTOS);
    scanf("%d", &escolha);

    if (escolha < 1 || escolha > MAX_CRIPTOS) {
        printf("Escolha invalida.\n");
        return;
    }

    // Solicitar a quantidade a ser comprada
    float quantidade;
    printf("Digite a quantidade de %s que deseja comprar: ", cripto_nome[escolha - 1]);
    scanf("%f", &quantidade);

    // Calcular o custo total
    float custo_total = cripto_valores[escolha - 1] * quantidade;

    // Verificar se o usuario tem saldo suficiente
    char user_file[20];
    sprintf(user_file, "user%d.bin", user_logado + 1);
    FILE *file_user = fopen(user_file, "rb+");
    if (file_user == NULL) {
        perror("Erro ao abrir o arquivo do usuario");
        return;
    }

    // Lendo o saldo atual do usuario
    float saldo_atual;
    fread(&saldo_atual, sizeof(float), 1, file_user);

    if (custo_total > saldo_atual) {
        printf("Saldo insuficiente. Seu saldo atual e R$ %.2f.\n", saldo_atual);
        fclose(file_user);
        return;
    }

    // Atualizando o saldo
    saldo_atual -= custo_total;
    fseek(file_user, 0, SEEK_SET);
    fwrite(&saldo_atual, sizeof(float), 1, file_user);
    fclose(file_user);

    // Adicionando a transacao ao extrato
    char extrato_file[20];
    sprintf(extrato_file, "extrato%d.bin", user_logado + 1);
    FILE *file_extrato = fopen(extrato_file, "ab"); // ab para adicionar no final
    if (file_extrato == NULL) {
        perror("Erro ao abrir o arquivo de extrato");
        return;
    }

    // Criando a transacao
    Transacao nova_transacao;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(nova_transacao.data, "%02d/%02d/%04d %02d:%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    nova_transacao.valor = custo_total;
    nova_transacao.taxa = custo_total * 0.02; // Exemplo de taxa de 2%

    // Escrevendo a transacao no extrato
    fwrite(&nova_transacao, sizeof(Transacao), 1, file_extrato);
    fclose(file_extrato);

    printf("Compra de %.2f %s realizada com sucesso. Custo total: R$ %.2f\n", quantidade, cripto_nome[escolha - 1], custo_total);
}


//FUNCAO VENDER CRIPTOMOEDAS
void vender_cripto() {
    if (user_logado == -1) {
        printf("Nenhum usuario logado.\n");
        return;
    }

    // Abrindo o arquivo de cotações e nomes das criptomoedas
    char cripto_nome[MAX_CRIPTOS][CRIPTO_LENGTH];
    float cripto_valores[MAX_CRIPTOS];

    FILE *file_nome = fopen("cripto_nome.bin", "rb");
    FILE *file_valores = fopen("cripto_cotacao.bin", "rb");
    if (file_nome == NULL || file_valores == NULL) {
        perror("Erro ao abrir os arquivos de criptomoedas");
        if (file_nome) fclose(file_nome);
        if (file_valores) fclose(file_valores);
        return;
    }

    // Lendo os nomes e valores das criptomoedas
    for (int i = 0; i < MAX_CRIPTOS; i++) {
        fread(cripto_nome[i], sizeof(char), CRIPTO_LENGTH, file_nome);
        fread(&cripto_valores[i], sizeof(float), 1, file_valores);
    }
    fclose(file_nome);
    fclose(file_valores);

    // Verificando as criptomoedas que o usuário possui
    char user_file[20];
    sprintf(user_file, "user%d.bin", user_logado + 1);
    FILE *file_user = fopen(user_file, "rb");
    if (file_user == NULL) {
        perror("Erro ao abrir o arquivo do usuario");
        return;
    }

    // Lendo a quantidade de cada criptomoeda na carteira do usuário
    float carteira[MAX_CRIPTOS];
    fread(carteira, sizeof(float), MAX_CRIPTOS, file_user);
    fclose(file_user);

    // Exibindo as criptomoedas e suas quantidades na carteira
    printf("Criptomoedas em sua carteira:\n");
    for (int i = 0; i < MAX_CRIPTOS; i++) {
        if (carteira[i] > 0) {
            printf("%d. %s - %.2f\n", i + 1, cripto_nome[i], carteira[i]);
        }
    }

    // Solicitar a escolha da criptomoeda
    int escolha;
    printf("Escolha a criptomoeda que deseja vender (1-%d): ", MAX_CRIPTOS);
    scanf("%d", &escolha);

    if (escolha < 1 || escolha > MAX_CRIPTOS || carteira[escolha - 1] <= 0) {
        printf("Escolha invalida ou voce nao possui essa criptomoeda.\n");
        return;
    }

    // Solicitar a quantidade a ser vendida
    float quantidade;
    printf("Digite a quantidade de %s que deseja vender: ", cripto_nome[escolha - 1]);
    scanf("%f", &quantidade);

    // Verificar se o usuário possui a quantidade desejada
    if (quantidade <= 0 || quantidade > carteira[escolha - 1]) {
        printf("Quantidade invalida. Voce possui apenas %.2f %s.\n", carteira[escolha - 1], cripto_nome[escolha - 1]);
        return;
    }

    // Calcular o valor da venda
    float valor_venda = cripto_valores[escolha - 1] * quantidade;

    // Atualizando a carteira do usuário
    carteira[escolha - 1] -= quantidade;

    // Atualizando o saldo do usuário
    char user_file_update[20];
    sprintf(user_file_update, "user%d.bin", user_logado + 1);
    file_user = fopen(user_file_update, "rb+");
    if (file_user == NULL) {
        perror("Erro ao abrir o arquivo do usuario");
        return;
    }

    float saldo_atual;
    fread(&saldo_atual, sizeof(float), 1, file_user);
    saldo_atual += valor_venda; // Adicionando o valor da venda ao saldo
    fseek(file_user, 0, SEEK_SET);
    fwrite(&saldo_atual, sizeof(float), 1, file_user);
    fclose(file_user);

    // Atualizando a carteira no arquivo do usuário
    file_user = fopen(user_file_update, "wb");
    if (file_user == NULL) {
        perror("Erro ao abrir o arquivo do usuario");
        return;
    }
    fwrite(carteira, sizeof(float), MAX_CRIPTOS, file_user);
    fclose(file_user);

    // Adicionando a transacao ao extrato
    char extrato_file[20];
    sprintf(extrato_file, "extrato%d.bin", user_logado + 1);
    FILE *file_extrato = fopen(extrato_file, "ab"); // ab para adicionar no final
    if (file_extrato == NULL) {
        perror("Erro ao abrir o arquivo de extrato");
        return;
    }

    // Criando a transacao
    Transacao nova_transacao;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(nova_transacao.data, "%02d/%02d/%04d %02d:%02d:%02d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec);
    nova_transacao.valor = valor_venda;
    nova_transacao.taxa = valor_venda * 0.02; // Exemplo de taxa de 2%

    // Escrevendo a transacao no extrato
    fwrite(&nova_transacao, sizeof(Transacao), 1, file_extrato);
    fclose(file_extrato);

    printf("Venda de %.2f %s realizada com sucesso. Valor total: R$ %.2f\n", quantidade, cripto_nome[escolha - 1], valor_venda);
}


//FUNCAO ATUALIZAR COTACAO
void atualizar_cotacao() {
    // Abrir o arquivo de cotações
    FILE *file_valores = fopen("cripto_cotacao.bin", "rb+");
    if (file_valores == NULL) {
        perror("Erro ao abrir o arquivo de cotacoes");
        return;
    }

    // Ler os valores atuais das criptomoedas
    float cripto_valores[MAX_CRIPTOS];
    fread(cripto_valores, sizeof(float), MAX_CRIPTOS, file_valores);

    // Inicializar o gerador de números aleatórios
    srand(time(NULL));

    // Atualizar as cotações
    printf("Novas cotacoes:\n");
    for (int i = 0; i < MAX_CRIPTOS; i++) {
        // Gerar uma variação aleatória entre -5% e +5%
        float variacao = ((rand() % 100) / 100.0f * 10 - 5) / 100.0f; // Entre -0.05 e +0.05
        cripto_valores[i] *= (1 + variacao); // Atualiza o valor com a variação

        // Exibir a nova cotação
        printf("Criptomoeda %d: Novo valor = R$ %.2f\n", i + 1, cripto_valores[i]);
    }

    // Voltar ao início do arquivo e escrever os novos valores
    fseek(file_valores, 0, SEEK_SET);
    fwrite(cripto_valores, sizeof(float), MAX_CRIPTOS, file_valores);

    // Fechar o arquivo
    fclose(file_valores);
}


//FUNCAO DISPONIBILIZA O MENU
void menu_principal() {
    int opcao;
    do {
        printf("\nMenu Principal:\n");
        printf("Operações disponíveis:\n");
        printf("1. Consultar Saldo\n");
        printf("2. Consultar Extrato\n");
        printf("3. Depositar Reais\n");
        printf("4. Sacar Reais\n");
        printf("5. Comprar Criptomoedas\n");
        printf("6. Vender Criptomoedas \n");
        printf("7. Atualizar Cotacoes \n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                if (seguranca_user()) {
                    consultar_saldo();
                }
                break;
            case 2:
                if (seguranca_user()) {
                    consultar_extrato();
                }
                break;
            case 3:
                if (seguranca_user()) {
                    depositar();
                }
                break;
            case 4:
                if (seguranca_user()) {
                    sacar();
                }
                break;
            case 5:
                if (seguranca_user()) {
                    comprar_cripto();
                }
                break;
            case 6:
                if (seguranca_user()) {
                    vender_cripto();
                }
                break;
            case 7:
                if (seguranca_user()) {
                    atualizar_cotacao();
                }
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (opcao != 0);
}
