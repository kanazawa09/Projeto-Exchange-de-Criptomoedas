#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "funcoes.h"


int main() {
    srand(time(NULL));
    if (interface_login()) {
        menu_admin();
    } else {
        printf("Login falhou. Verifique seu CPF e senha.\n");
    }
    return 0;
}
