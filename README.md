# Projeto-1-Exchange-de-Criptomoedas
Projeto 1 da Exchange de Criptomoedas

Projeto-1---Exchange-de-Criptomoeda
Participantes e Matrícula: Igor Eizo Ramos Kanazawa | RA: 24.124.040-7 
Wilson Massaiolli Chacon | RA: 24.124.038-1 
RODRIGO INOUE CASTANHO | RA: 24.124.037-3
==========================================================================================================================================================

Usuários disponíveis para o uso (somente dá pra acessar o banco com esses usuários)

--------------------------------------------------------------------------
{"Wilson", 1234, 1, "12345678901", 1000.00, 0.0, 0.0, 0.0}
{"Igor", 5678,   2, "23456789012", 2000.00, 0.0, 0.0, 0.0}
{"Rodrigo", 9101, 3, "34567890123", 1500.00, 0.0, 0.0, 0.0}

{nome}, {senha}, {id}, {cpf}, {reais}, {bitcoins}, {Etherium}, {ripple}
--------------------------------------------------------------------------






---------------------------------------------------------------------------------------------------------------
Desenvolvimento de algorítmos [Projeto C]

OBJETIVO:

O Objetivo do projeto é criar um banco de criptmoedas em linguagem C, onde há a presença de três moedas: Bitcoin, Ethereum, Ripple. Neste banco o usuário precisa realizar o cadastro informando: Nome, Senha, CPF, para conseguir acessar o banco.

Dentro do banco, o usuário tem a seguintes opções para realizar:

Sacar -Depositar -consultar o saldo -consultar extrato -comprar criptomoedas -carregar cotações -atualizar cotação -vender criptomoedas -main menu

		
=====================================================================================================

FORMA DE COMPILAÇÃO E EXECUÇÃO

-- Baixar todos os arquivos e rodar o executável 


======================================================================================================

FORMA DE USAR:

1- O usuário preenche os campos pendentes:(Nome, Senha, CPF)

2- o usuário tem o acesso liberado para utilizar sua conta no banco de criptmoedas.

3- o usuario digita o número que é associado à opção na qual ele deseja (no main-menu)

4- Funcionalidade das opções. a) sacar O programa solicita a senha do usuario para realizar o saque, assim que a senha é inserida e validada, o usuário pode digitar o valor do saque: ("Digite o valor do saque: R$ "). Escolha a quantia que deseja e dê enter para confirmar.

b) depositar O programa pergunta o valor que o usuário quer colocar para depositar: ("Digite o valor a ser depositado: R$ "), Escolha a quantia que desejar e dê enter para confirmar.

c) consultar extrato O programa vai buscar e mostrar ao usuário o extrato das ultimas transações feitas pelo mesmo.

d) comprar criptomoedas O programa vai solicitar a senha do usuário para realizar a compra, e então, buscar e mostar as cotações ao usuário, uma vez que estiver disponivel a compra, o programa mostrará as moedas disponíveis para comprar (Bitcoin, Ethereum, Ripple), então o usuário digita o valor que ele vai gastar para comprar ("Digite o valor a ser investido: R$ "). Caso o valor seja menor do que o valor da moeda representa, será mostrado ao usuário uma mensagem dizendo que ele não pode comprar devido ao saldo insuficiente:("Saldo insuficiente para comprar Bitcoin"), caso o usuário gaste a quantia suificente, o programa irá perguntar se o usuário quer confirmar a compra, e então se o usuário optar por confirmar esta compra, a moeda será comprada.

e) carregar cotações O programa vai abrir as cotações e mostrar o valor das respectivas moedas:(Bitcoin, Ethereum, Ripple).

f)atualizar cotação O Programa vai buscar as cotações e irá mostrar ao usuário as cotações atualizadas das moedas:(Bitcoin, Ethereum, Ripple).

g) consultar saldo O programa vai exibir as informações(Nome, CPF) e a quantia das moedas:(Reais, Bitcoin, Ethereum, Ripple) que o usuário possui.

h) menu main O programa irá retornar ao menu principal, em que o usuário terá de informar seu CPF e Senha, para acessar o menu principal.

i) vender criptomoedas O programa vai solicitar a senha do usuário, uma vez que estiver correta, o programa vai pedir a criptomoeda em que o usuário quer vender, o usuário escolhe a moeda desejada para a venda(Bitcoin, Ethereum, Ripple), e digita o valor que a criptomoeda vai ser vendida ("Digite o valor da criptomoeda a ser vendida: R$ "), se o usuário não tiver saldo o suficiente para realiza a venda, o programa dirá: ("Saldo insuficiente para vender (moeda)"), caso o usuário tenha o saldo suficiente para vender, o programa exibirá um resumo da venda, mostrando: A Criptomoeda escolhida, O valor da venda, a taxa que é cobrada na realização da venda, o valor total a ser creditado, e enfim, o programa vai pedir uma confirmação da venda. Quando o requisito for confirmado, estará confirmado a realização da venda.

===========================================================================================================================

