int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    // Chama a função para carregar os usuários
    login_user();

    char cpf[CPF_LENGTH], senha[SENHA_LENGTH];
    printf("Informe seu CPF: ");
    scanf("%s", cpf);
    printf("Informe sua senha (4 dígitos): ");
    scanf("%s", senha);

    if (efetuar_login(cpf, senha)) {
        menu_principal(); // Chama o menu principal após login bem-sucedido
    } else {
        printf("Login falhou. Verifique seu CPF e senha.\n");
    }
    
    printf("Fim do Programa\n");

    return 0;
}
