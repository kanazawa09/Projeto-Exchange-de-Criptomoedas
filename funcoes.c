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
            printf("Login bem-sucedido! Usuario logado: %s (Numero: %d)\n", usuarios[user_logado].cpf, user_logado + 1);
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
        printf("Senha incorreta. Operacao cancelada.\n");
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
            perror("Erro ao abrir o arquivo do usuario");
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
        printf("Nenhum usuario logado.\n");
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
        printf("Nenhum usuario logado.\n");
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
            perror("Erro ao abrir o arquivo do usuario");
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
        printf("Voce deseja depositar R$ %.2f? (s/n): ", valor_deposito);
        char confirmacao;
        scanf(" %c", &confirmacao); // O espaço antes de %c ignora espaços em branco

        if (confirmacao == 's' || confirmacao == 'S') {
            // Atualiza o saldo
            saldo_atual += valor_deposito;

            // Move o ponteiro do arquivo para o início para escrever o novo saldo
            fseek(file_user, 0, SEEK_SET);
            fwrite(&saldo_atual, sizeof(float), 1, file_user);

            printf("Deposito de R$ %.2f realizado com sucesso! Novo saldo: R$ %.2f\n", valor_deposito, saldo_atual);
        } else {
            printf("Deposito cancelado.\n");
        }

        fclose(file_user);
    } else {
        printf("Nenhum usuario logado.\n");
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
            perror("Erro ao abrir o arquivo do usuario");
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
            printf("Saldo insuficiente. Seu saldo atual e R$ %.2f.\n", saldo_atual);
        } else {
            // Confirmação do saque
            printf("Voca deseja sacar R$ %.2f? (s/n): ", valor_saque);
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
        printf("Nenhum usuario logado.\n");
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
        printf("Operações disponiveis:\n");
        printf("1. Consultar Saldo\n");
        printf("2. Consultar Extrato\n");
        printf("3. Depositar Reais\n");
        printf("4. Sacar Reais\n");
        printf("5. Comprar Criptomoedas\n");
        printf("6. Vender Criptomoedas \n");
        printf("7. Atualizar Cotacoes \n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
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





// FUNCOES ADMIN  


// Renomeie a variável para evitar conflito
Usuario usuarios_list[MAX_USERS]; // Mudado para 'usuarios_list'
int num_usuarios = 0;

Admin admins[MAX_ADMIN]; // Renomeado para evitar conflito
int num_admin = 0;
int admin_logado = -1; // Índice do administrador logado, -1 indica que ninguém está logado

// Prototipagem da função login_admin
int login_admin(char *cpf, char *senha);


//FUNCAO REALIZAR LOGIN
void interface_login() {
    char cpf[12];
    char senha[5];

    printf("Informe seu CPF: ");
    scanf("%11s", cpf); // Limita a 11 caracteres
    printf("Informe sua senha: ");
    scanf("%4s", senha); // Limita a 4 caracteres

    if (login_admin(cpf, senha)) {
        printf("Login bem-sucedido!\n");
        return 1;
        // Continue com a lógica do sistema
    } else {
        printf("CPF ou senha invalidos. Tente novamente.\n");
        return 0;
    }
}

int login_admin(char *cpf, char *senha) {
    Admin admin;
    FILE *file = fopen("admin_login_info.bin", "rb");
    if (file == NULL) {
        perror("Erro ao abrir o arquivo de login");
        return 0; // Falha ao abrir o arquivo
    }

    while (fread(&admin, sizeof(Admin), 1, file)) {
        if (strcmp(admin.cpf, cpf) == 0 && strcmp(admin.senha, senha) == 0) {
            fclose(file);
            return 1; // Login bem-sucedido
        }
    }

    fclose(file);
    return 0; // Login falhou
}


//FUNCAO ADICIONAR USUARIO
void add_user() {
    // Verifica se há espaço para novos usuários
    if (num_admin >= MAX_USERS) {
        printf("Número máximo de usuários atingido.\n");
        return;
    }

    // Encontra um arquivo vazio para o novo usuario
    char filename[20];
    int usuario_num = -1;
    for (int i = 0; i < MAX_USERS; i++) {
        snprintf(filename, sizeof(filename), "user%d.bin", i);
        FILE *file = fopen(filename, "rb");
        if (file == NULL) {
            // O arquivo nao existe, então podemos usá-lo
            usuario_num = i;
            break;
        }
        fclose(file);
    }

    if (usuario_num == -1) {
        printf("Numero maximo de usuarios atingido.\n");
        return;
    }

    // Solicita informações do novo usuario
    Usuario novo_usuario; // Estrutura para o novo usuario
    char nome[50]; // Variável para armazenar o nome
    printf("Informe o nome do novo usuario: ");
    scanf("%s", nome); // Coleta o nome, mas nao armazena em arquivo
    printf("Informe o CPF do novo usuario: ");
    scanf("%s", novo_usuario.cpf);
    printf("Informe a senha do novo usuario (4 dígitos): ");
    scanf("%s", novo_usuario.senha);

    // Exibe as informações para confirmação
    printf("\nConfirme as informações do novo usuario:\n");
    printf("Nome: %s\n", nome);
    printf("CPF: %s\n", novo_usuario.cpf);
    printf("Senha: %s\n", novo_usuario.senha);
    printf("Deseja confirmar? (s/n): ");
    char confirm;
    scanf(" %c", &confirm); // Lê a confirmação

    if (confirm == 's' || confirm == 'S') {
        // Adiciona o CPF e a senha ao arquivo de login
        FILE *login_file = fopen("user_login_info.bin", "ab");
        if (login_file == NULL) {
            perror("Erro ao abrir o arquivo de login");
            return;
        }
        fwrite(&novo_usuario, sizeof(Usuario), 1, login_file);
        fclose(login_file);

        // Atualiza o número de usuários
        num_usuarios++;
        printf("Usuario adicionado com sucesso!\n");
    } else {
        printf("Cadastro do usuario cancelado.\n");
    }
}

void remov_user() {
    char cpf[CPF_LENGTH];
    printf("Informe o CPF do usuario a ser removido: ");
    scanf("%s", cpf);

    // Busca o usuario no arquivo de login
    Usuario usuario_encontrado; // Mude para Usuario
    int usuario_index = -1;
    FILE *login_file = fopen("user_login_info.bin", "rb+");
    if (login_file == NULL) {
        perror("Erro ao abrir o arquivo de login");
        return;
    }

    // Lê os dados do arquivo e procura o CPF
    while (fread(&usuario_encontrado, sizeof(Usuario), 1, login_file)) { // Mude para Usuario
        if (strcmp(usuario_encontrado.cpf, cpf) == 0) {
            usuario_index = ftell(login_file) / sizeof(Usuario) - 1; // Índice do usuario encontrado
            break;
        }
    }

    if (usuario_index == -1) {
        printf("Usuario com CPF %s nao encontrado.\n", cpf);
        fclose(login_file);
        return;
    }

    // Exibe os dados do usuario encontrado
    printf("Usuario encontrado:\n");
    printf("CPF: %s\n", usuario_encontrado.cpf);
    printf("Senha: %s\n", usuario_encontrado.senha);
    printf("Saldo: %.2f\n", usuario_encontrado.saldo_reais); // Agora correto
    printf("Deseja confirmar a exclusao? (1 - Sim, 0 - Nao): ");
    int confirmar;
    scanf("%d", &confirmar);

    if (confirmar != 1) {
        printf("Operacao cancelada.\n");
        fclose(login_file);
        return;
    }

    // Remover o usuario do arquivo de login
    // Para isso, vamos criar um novo arquivo temporário
    FILE *temp_file = fopen("temp_user_login_info.bin", "wb");
    if (temp_file == NULL) {
        perror("Erro ao criar arquivo temporário");
        fclose(login_file);
        return;
    }

    // Copia todos os usuários, exceto o que será removido
    fseek(login_file, 0, SEEK_SET);
    while (fread(&usuario_encontrado, sizeof(Usuario), 1, login_file)) {
        if (strcmp(usuario_encontrado.cpf, cpf) != 0) {
            fwrite(&usuario_encontrado, sizeof(Usuario), 1, temp_file);
        }
    }

    fclose(login_file);
    fclose(temp_file);

    // Substitui o arquivo original pelo temporário
    remove("user_login_info.bin");
    rename("temp_user_login_info.bin", "user_login_info.bin");

    // Limpa o arquivo do usuario correspondente
    char filename[20];
    snprintf(filename, sizeof(filename), "user%d.bin", usuario_index);
    remove(filename); // Remove o arquivo do usuario

    printf("Usuario com CPF %s removido com sucesso!\n", cpf);
}


//FUNCAO ADICIONAR CRITOMOEDA
void add_cripto() {
    char nome[CRIPTO_LENGTH];
    char cotacao[CRIPTO_LENGTH]; // Usando o mesmo tamanho para a cotação

    // Coletar dados do usuario
    printf("Informe o nome da nova criptomoeda: ");
    scanf("%s", nome);
    printf("Informe a cotacao inicial da nova criptomoeda: ");
    scanf("%s", cotacao);

    // Abrir arquivos em modo append
    FILE *nome_file = fopen("cripto_nome.bin", "ab");
    FILE *cotacao_file = fopen("cripto_cotacao.bin", "ab");

    if (nome_file == NULL || cotacao_file == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }

    // Escrever dados nos arquivos
    fwrite(nome, sizeof(char), strlen(nome) + 1, nome_file); // +1 para incluir o caractere nulo
    fwrite(cotacao, sizeof(char), strlen(cotacao) + 1, cotacao_file); // +1 para incluir o caractere nulo

    // Fechar arquivos
    fclose(nome_file);
    fclose(cotacao_file);

    printf("Criptomoeda %s adicionada com cotacao inicial %s com sucesso!\n", nome, cotacao);
}


//FUNCAO REMOVER CRITOMOEDA
void remov_cripto() {
    char nome[CRIPTO_LENGTH];
    printf("Informe o nome da criptomoeda a ser removida: ");
    scanf("%s", nome);

    // Abrir arquivos para leitura
    FILE *nome_file = fopen("cripto_nome.bin", "rb");
    FILE *cotacao_file = fopen("cripto_cotacao.bin", "rb");

    if (nome_file == NULL || cotacao_file == NULL) {
        perror("Erro ao abrir os arquivos");
        return;
    }

    char nome_atual[CRIPTO_LENGTH];
    char cotacao_atual[CRIPTO_LENGTH];
    int encontrado = 0;

    // Busca a criptomoeda no arquivo de nomes
    while (fread(nome_atual, sizeof(char), CRIPTO_LENGTH, nome_file)) {
        fread(cotacao_atual, sizeof(char), CRIPTO_LENGTH, cotacao_file); // Lê a cotação correspondente

        if (strcmp(nome_atual, nome) == 0) {
            encontrado = 1;
            break; // Criptomoeda encontrada
        }
    }

    if (!encontrado) {
        printf("Criptomoeda com nome %s nao encontrada.\n", nome);
        fclose(nome_file);
        fclose(cotacao_file);
        return;
    }

    // Exibe os dados da criptomoeda encontrada
    printf("Criptomoeda encontrada:\n");
    printf("Nome: %s\n", nome_atual);
    printf("Cotação: %s\n", cotacao_atual);
    printf("Deseja confirmar a exclusao? (1 - Sim, 0 - Nao): ");
    int confirmar;
    scanf("%d", &confirmar);

    if (confirmar != 1) {
        printf("Operacao cancelada.\n");
        fclose(nome_file);
        fclose(cotacao_file);
        return;
    }

    // Remover a criptomoeda dos arquivos
    // Criar arquivos temporários
    FILE *temp_nome_file = fopen("temp_cripto_nome.bin", "wb");
    FILE *temp_cotacao_file = fopen("temp_cripto_cotacao.bin", "wb");

    if (temp_nome_file == NULL || temp_cotacao_file == NULL) {
        perror("Erro ao criar arquivos temporários");
        fclose(nome_file);
        fclose(cotacao_file);
        return;
    }

    // Copia todos os dados, exceto a criptomoeda que será removida
    fseek(nome_file, 0, SEEK_SET);
    fseek(cotacao_file, 0, SEEK_SET);
    while (fread(nome_atual, sizeof(char), CRIPTO_LENGTH, nome_file)) {
        fread(cotacao_atual, sizeof(char), CRIPTO_LENGTH, cotacao_file);
        if (strcmp(nome_atual, nome) != 0) {
            fwrite(nome_atual, sizeof(char), CRIPTO_LENGTH, temp_nome_file);
            fwrite(cotacao_atual, sizeof(char), CRIPTO_LENGTH, temp_cotacao_file);
        }
    }

    // Fechar arquivos
    fclose(nome_file);
    fclose(cotacao_file);
    fclose(temp_nome_file);
    fclose(temp_cotacao_file);

    // Substituir os arquivos originais pelos temporários
    remove("cripto_nome.bin");
    rename("temp_cripto_nome.bin", "cripto_nome.bin");
    remove("cripto_cotacao.bin");
    rename("temp_cripto_cotacao.bin", "cripto_cotacao.bin");

    printf("Criptomoeda %s removida com sucesso!\n", nome);
}


void consultar_saldo_admin() {
    char cpf[CPF_LENGTH];
    printf("Informe o CPF do investidor: ");
    scanf("%s", cpf);

    // Abrir o arquivo user_login_info.bin
    FILE *login_info_file = fopen("user_login_info.bin", "rb");
    if (login_info_file == NULL) {
        printf("Erro ao abrir o arquivo de login.\n");
        return;
    }

    int numero_usuario = -1;
    char cpf_temp[CPF_LENGTH];
    
    // Ler o arquivo e buscar o número do usuario correspondente ao CPF
    for (int i = 1; i <= MAX_USERS; i++) {
        fread(cpf_temp, sizeof(char), CPF_LENGTH, login_info_file);
        if (strcmp(cpf_temp, cpf) == 0) {
            numero_usuario = i; // Encontrou o número do usuario
            break;
        }
    }

    fclose(login_info_file);

    if (numero_usuario == -1) {
        printf("CPF nao encontrado.\n");
        return;
    }

    // Criar nome do arquivo com base no número do usuario
    char arquivo_nome[20]; // Tamanho suficiente para "userX.bin"
    sprintf(arquivo_nome, "user%d.bin", numero_usuario);

    // Abrir o arquivo do usuario para leitura
    FILE *user_file = fopen(arquivo_nome, "rb");
    if (user_file == NULL) {
        printf("Erro ao abrir o arquivo do investidor.\n");
        return;
    }

    // Ler o saldo em reais
    float saldo_reais;
    fread(&saldo_reais, sizeof(float), 1, user_file);
    printf("Saldo em Reais do investidor com CPF %s: R$ %.2f\n", cpf, saldo_reais);

    // Ler as quantidades de criptomoedas
    float quantidades[MAX_CRIPTOS]; // Alterado de MAX_CRIPTOMOEDAS para MAX_CRIPTOS
    fread(quantidades, sizeof(float), MAX_CRIPTOS, user_file);

    // Abrir o arquivo cripto_nome.bin para ler os nomes das criptomoedas
    FILE *cripto_nome_file = fopen("cripto_nome.bin", "rb");
    if (cripto_nome_file == NULL) {
        printf("Erro ao abrir o arquivo de nomes das criptomoedas.\n");
        fclose(user_file);
        return;
    }

    // Ler e exibir os nomes das criptomoedas
    char nome_cripto[DATA_LENGTH];
    printf("Quantidades de criptomoedas:\n");
    for (int i = 0; i < MAX_CRIPTOS; i++) {
        fread(nome_cripto, sizeof(char), DATA_LENGTH, cripto_nome_file);
        if (feof(cripto_nome_file)) break; // Fim do arquivo
        printf("%s: %.2f\n", nome_cripto, quantidades[i]);
    }

    fclose(cripto_nome_file);
    fclose(user_file);
}

//FUNCAO CONSULTAR EXTRATO
void consultar_extrato_admin() {
    char cpf[CPF_LENGTH];
    printf("Informe o CPF do investidor: ");
    scanf("%s", cpf);

    // Abrir o arquivo user_login_info.bin
    FILE *login_info_file = fopen("user_login_info.bin", "rb");
    if (login_info_file == NULL) {
        printf("Erro ao abrir o arquivo de login.\n");
        return;
    }

    int numero_usuario = -1;
    char cpf_temp[CPF_LENGTH];
    
    // Ler o arquivo e buscar o número do usuario correspondente ao CPF
    for (int i = 1; i <= MAX_USERS; i++) {
        fread(cpf_temp, sizeof(char), CPF_LENGTH, login_info_file);
        if (strcmp(cpf_temp, cpf) == 0) {
            numero_usuario = i; // Encontrou o número do usuario
            break;
        }
    }

    fclose(login_info_file);

    if (numero_usuario == -1) {
        printf("CPF nao encontrado.\n");
        return;
    }

    // Criar nome do arquivo com base no número do usuario
    char arquivo_nome[20]; // Tamanho suficiente para "extratoX.bin"
    sprintf(arquivo_nome, "extrato%d.bin", numero_usuario);

    // Abrir o arquivo do extrato para leitura
    FILE *extrato_file = fopen(arquivo_nome, "rb");
    if (extrato_file == NULL) {
        printf("Erro ao abrir o arquivo de extrato do investidor.\n");
        return;
    }

    // Ler e exibir as transações do extrato
    char data[DATA_LENGTH];
    float valor;
    printf("Extrato do investidor com CPF %s:\n", cpf);
    for (int i = 0; i < MAX_TRANSACOES; i++) {
        fread(data, sizeof(char), DATA_LENGTH, extrato_file);
        fread(&valor, sizeof(float), 1, extrato_file);
        if (feof(extrato_file)) break; // Fim do arquivo
        printf("Data: %s, Valor: R$ %.2f\n", data, valor);
    }

    fclose(extrato_file);
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
void menu_admin() {
    int opcao;
    do {
        printf("\nMenu Principal:\n");
        printf("Operacoes disponiveis:\n");
        printf("1. Cadastrar Usuario\n");
        printf("2. Excluir Usuario\n");
        printf("3. Cadastrar Criptomoeda\n");
        printf("4. Excluir Criptomoeda\n");
        printf("5. Consultar Saldo\n");
        printf("6. Consultar Extrato\n");
        printf("7. Atualizar Cotacoes \n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                add_user();
                break;
            case 2:
                remov_user();
                break;
            case 3:
                add_cripto();
            case 4:
                remov_cripto();
                break;
            case 5:
                consultar_saldo_admin();
            case 6:
                consultar_extrato_admin();
                break;
            case 7:
                atualizar_cotacao();
                break;
            case 0:
                printf("Saindo...\n");
                break;
            default:
                printf("Opcao invvlida. Tente novamente.\n");
        }
    } while (opcao != 0);
}
