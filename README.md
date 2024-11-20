# Projeto-Exchange-de-Criptomoedas
Projeto 3 da Exchange de Criptomoedas

Participantes e Matrícula: 

RODRIGO INOUE CASTANHO 	 | RA: 24.124.037-3

Wilson Massaiolli Chacon | RA: 24.124.038-1 



Igor Eizo Ramos Kanazawa | RA: 24.124.040-7 


==========================================================================================================================================================

Informacoes de login dos Investidores

        	CPF	 SENHA	SALDO Reais

	{"12345678901", "1234", 1000.0},
 
 	{"23456789012", "2345", 1500.0},
        
	{"34567890123", "3456", 2000.0},
        
	{"45678901234", "4567", 2500.0},
        
	{"56789012345", "5678", 3000.0},
        
	{"67890123456", "6789", 3500.0},
        
	{"78901234567", "7890", 4000.0},
        
	{"89012345678", "8901", 4500.0},
        
	{"90123456789", "9012", 5000.0},
        
	{"01234567890", "0123", 5500.0}


Informacoes de login do ADIMINISTRADOR:


             CPF	 SENHA

   	{"12345678901", "1234"}

---------------------------------------------------------------------------------------------------------------
Desenvolvimento de algorítmos [Projeto C]

OBJETIVO:

Este projeto possui dois programas uma para os investidoress chamado inovestidor.c e um para o administrador chamado admin.c

 - O programa do investidor faz uso de 5 arquivos binarios, uma para o saldo do investidor, um para o extrato do investidor, um para realizar o login do investidor, e dois para armazenar o nome das criptomoedas e suas cotacoes respectivamente.
 - O programa do administrador possui um arquivo exclusivo para realizar o login do administrador e a capacidade de manipular toddos os demais arquivos binarios, seja editar as informacoes dos usuarios ou criptomoedas.

		
=====================================================================================================

FORMA DE COMPILAÇÃO E EXECUÇÃO

Para compilar e executar os programas, você precisará de:

Compilador C (como GCC), para compilar corretamente os arquivos.

Para rodar o programa do investidor os arquivos devem ser compilados juntos: investidor.c, globais.c e funcoes.c .

Para rodar o programa do admin os arquivos devem ser compilados juntos: adm.c, globais.c e funcoes.c .

Sistema que suporte um terminal para rodar o programa como por exemplo: vscode.

Pode ser usado este codigo do administrador no CMD para compilar os arquivos ( gcc adm.c funcoes.c globais.c -o pADM.exe ), e este para rodar ( .\pADM.exe )

Pode ser usado este codigo do investidor no CMD para compilar os arquivos ( gcc investidor.c funcoes.c globais.c -o investidor_programa.exe ), e este para rodar ( .\investidor_programa.exe )

======================================================================================================

FORMA DE USAR O PROGRAMA DO INVESTIDOR:

1- Ao iniciar o programa a funcao login_user() sera iniciada. Nela o usuário deve informar seu CPF e SENHA de 4 digitos numericos. Se estaas informacoes existirem no arquivo user_login_info.bin o programa retorna um valor positivo disponibilizando o menu.

2- Se o login for realizado com sucesso a funcao menu_principal() sera chamada, disponibilizando o menu com todas as operacoes possiveis ao inveestidor.

3- Quando qualquer operacao for chamada a funcao seguranca_user() sera iniciada. nela o usuario deve novamnete informar sua senha para proseguir a operacao.

4- TODAS OPERACOES:

a) consultar_saldo(). Esta funcao ira abrir e ler o arquivo do que contem o saldo do investidor logado e entao imprime no console quantos reais e criptomoedas ele possui.

b) consultar_extrato().Esta funcao ira abrir e ler o arquivo do que contem o extrato do investidor logado e entao imprime no console as ultimas 100 transacoes deste investidor, contendo horario, valor e taxa.

c) depositar(). Esta funcao permite ao usuario depositar reais em sua conta, apos confirmar a operacao o valor desejado sera adicionado as seu  saldo.

d) sacar(). Esta funcao permite ao usuario sacar reais em sua conta, apos confirmar a operacao o valor desejado sera removido as seu saldo, nao permitindo o saldo ficar negativo.

e) comprar_cripto(). Esta funcao permite o usuario compraar criptomoedas. Ela funcionao ao abrir e ler os arquivos que contem o nome e cotacao destas criptomoedas, ele entao exibe estas informacoes ao usuario e apos ele selecionar qual criptomoeda e o valor que ele deseja comprar o arquivo de saldo do usuario é aberto para realizar a operacao, removendo o valor de reais e adicionando o numero de criptomoedas. Se o saldo de reais for insuficiente a operacao é cancelada. No final de tudo a operacao é registrada no extrato do usuaario.

f) vender_cripto(). Esta funcao permite o usuario vender criptomoedas. Ela funcionao ao abrir e ler os arquivos que contem o saldo do usuario e o arquivo com nome e cotacoes das criptomoedas, ele entao exibe estas informacoes ao usuario e apos ele selecionar qual criptomoeda e o valor que ele deseja vender a operacao é realizada, removendo o valor da critomoeda selecionada e adicionando o valor em reais. Se o saldo de criptomoedas selecionaada for insuficiente a operacao é cancelada. No final de tudo a operacao é registrada no extrato do usuaario.

g) atualizar_cotacao(). Esta funcao permite ao usuario atualizar a cotacao das criptomoedas, almentando ou diminuindo suas cotacoes em no maximo ou minimo 5%. Ela funcona ao abrir os aarquivos com nome e cotacoes daas criptomoedas, gerando um valor novo pra cada criptomoeda e exibindo estas novas cotacoes ao usuario antes de salvar as novas cotacoes nos aarquivos binarios.


FORMA DE USAR O PROGRAMA DO ADMINISTRADOR:

1- Ao iniciar o programa a funcao interface_login() sera iniciada. Nela o admin deve informar seu CPF e SENHA de 4 digitos numericos. Se estaas informacoes existirem no arquivo admin_login_info.bin o programa retorna um valor positivo disponibilizando o menu.

2- Se o login for realizado com sucesso a funcao menu_admin() sera chamada, disponibilizando o menu com todas as operacoes possiveis ao inveestidor.

3- TODAS OPERACOES:

a) add_user() permite ao administrador adicionar um novo investido ao programa, basta ele inserir o nome, cpf e senha do novo usuario que estas informacoes serao salvas e o novo usuario podera utiliazar o programa investidor.c.

b) remov_user() permite ao administrador remover um usuario existente do sistema, basta ele digitar o cpf que as informacoes serao exibidas, e apos confirmar a operacao o usuario é apagado do sistema.

c) add_cripto() permite ao administrador adicionar uma nova criptomoeda ao programa, basta inserir o nome e cotacao inicial que as informacoes serao exibidas e salvas no programa, podenso ser acessadas no programa dos investidores. 

d) remov_cripto() permite ao administrador remover uma criptomoeda do programa, basta inserir o nome  que as informacoes serao exibidas, apos confirmar a acao o nome e cotacao da criptomoeda seerao apagados.

e) consultar_saldo_admin(). Esta funcao ira abrir e ler o arquivo do que contem o saldo do investidor que o admin selecionar e entao imprime no console quantos reais e criptomoedas ele possui.

f) consultar_extrato_admin().Esta funcao ira abrir e ler o arquivo do que contem o extrato do investidor que o admin selecionar e entao imprime no console as ultimas 100 transacoes deste investidor, contendo horario, valor e taxa.

g) atualizar_cotacao(). Esta funcao permite ao usuario atualizar a cotacao das criptomoedas, almentando ou diminuindo suas cotacoes em no maximo ou minimo 5%. Ela funcona ao abrir os aarquivos com nome e cotacoes daas criptomoedas, gerando um valor novo pra cada criptomoeda e exibindo estas novas cotacoes ao usuario antes de salvar as novas cotacoes nos aarquivos binario

===========================================================================================================================

