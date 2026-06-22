#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Máximo de clientes que podem ser cadastrados.
// Usado tanto para dimensionar a alocação dinâmica em main() quanto como
// limite de iteração em todas as funções que percorrem o vetor de clientes.
#define MAX_CLIENTES 5

/*
 * Estrutura que representa um cliente da locadora.
 *
 * Campos:
 * - nome, cnh, telefone, email, cep: dados cadastrais armazenados como texto.
 * - cpf: armazenado como char (string), e não como número, pois o CPF pode
 *        conter zeros à esquerda (ex: "01234567890"), que seriam perdidos
 *        se fosse guardado em um tipo numérico como int ou long.
 * - idade: armazenada como inteiro, pois é usada em comparações numéricas.
 * - ativo: funciona como "soft delete". Em vez de remover o cliente
 *          fisicamente do vetor (o que exigiria deslocar todos os elementos
 *          seguintes), apenas marcamos ativo = 0. Isso simplifica bastante
 *          a exclusão e a reutilização de posições livres no cadastro.
 */
typedef struct {
	char nome[100], cnh[20], cpf[15], telefone[20], email[100], cep[10];
	int idade, ativo;
} Cliente;

/*
 * limparTexto
 * Remove o caractere de nova linha ('\n') deixado no final de uma string
 * lida com fgets(), substituindo-o por '\0' (fim de string).
 *
 * Parâmetros:
 * - str: string a ser tratada (modificada diretamente, por referência).
 *
 * Retorno: void (a função não retorna valor; ela altera 'str' no local).
 */
void limparTexto(char *str) {
	str[strcspn(str, "\n")] = '\0';
}

/*
 * limparBuffer
 * Descarta todos os caracteres restantes no buffer de entrada (stdin) até
 * encontrar uma nova linha ou o fim do arquivo.
 *
 * Por que é necessário: scanf("%d", ...) não consome o '\n' deixado no
 * buffer após o Enter. Se um fgets() for chamado em seguida sem essa
 * limpeza, ele lê apenas esse '\n' residual e retorna uma string vazia,
 * "pulando" a pergunta seguinte. Por isso limparBuffer() é chamada sempre
 * entre um scanf() e um fgets() subsequente.
 *
 * Parâmetros: nenhum.
 * Retorno: void.
 */
void limparBuffer() {
	int c;
	while((c = getchar()) != '\n' && c != EOF);
}

/*
 * buscarPorCpf
 * Procura, dentre os clientes ativos, aquele cujo CPF é igual ao informado.
 *
 * Parâmetros:
 * - vetor: ponteiro para o início do vetor de clientes (array de structs).
 * - qtd: quantidade atual de clientes cadastrados (não utilizado na busca
 *        em si, pois sempre percorremos até MAX_CLIENTES verificando o
 *        campo 'ativo' — mantido na assinatura por padronização com as
 *        demais funções de busca).
 * - cpfBusca: string com o CPF a ser procurado.
 *
 * Retorno: índice (posição) do cliente no vetor, caso encontrado;
 *          -1 caso nenhum cliente ativo possua esse CPF.
 */
int buscarPorCpf(Cliente *vetor, int qtd, char *cpfBusca) {
	// Fail-fast: ignoramos clientes com ativo == 0, pois representam
	// registros "excluídos" (soft delete) e não devem ser encontrados.
	for(int i = 0; i < MAX_CLIENTES; i++)
		if(vetor[i].ativo == 1 && strcmp(vetor[i].cpf, cpfBusca) == 0) {
			return i;
		}
	return -1;
}

/*
 * buscarPorEmail
 * Procura, dentre os clientes ativos, aquele cujo e-mail é igual ao
 * informado. Usada principalmente para impedir cadastro de e-mail
 * duplicado.
 *
 * Parâmetros:
 * - vetor: ponteiro para o vetor de clientes.
 * - qtd: quantidade atual de clientes (não utilizado, ver buscarPorCpf).
 * - emailBusca: string com o e-mail a ser procurado.
 *
 * Retorno: índice do cliente, ou -1 se não encontrado.
 */
int buscarPorEmail(Cliente *vetor, int qtd, char *emailBusca) {
	for(int i = 0; i < MAX_CLIENTES; i++)
		if(vetor[i].ativo == 1 && strcmp(vetor[i].email, emailBusca) == 0) {
			return i;
		}
	return -1;
}

/*
 * buscarPorTelefone
 * Procura, dentre os clientes ativos, aquele cujo telefone é igual ao
 * informado. Usada para impedir cadastro de telefone duplicado.
 *
 * Parâmetros:
 * - vetor: ponteiro para o vetor de clientes.
 * - qtd: quantidade atual de clientes (não utilizado, ver buscarPorCpf).
 * - telefoneBusca: string com o telefone a ser procurado.
 *
 * Retorno: índice do cliente, ou -1 se não encontrado.
 */
int buscarPorTelefone(Cliente *vetor, int qtd, char *telefoneBusca) {
	for(int i = 0; i < MAX_CLIENTES; i++)
		if(vetor[i].ativo == 1 && strcmp(vetor[i].telefone, telefoneBusca) == 0) {
			return i;
		}
	return -1;
}

/*
 * validarDDD
 * Verifica se os dois primeiros dígitos do telefone correspondem a um
 * DDD oficial válido no Brasil.
 *
 * Parâmetros:
 * - telefone: string contendo o telefone completo (DDD + número).
 *
 * Retorno: 1 se o DDD é válido; 0 caso contrário.
 */
int validarDDD(char *telefone) {
	// Lista fixa com todos os DDDs oficiais brasileiros.
	const char *dddsValidos[] = {
		"11", "12", "13", "14", "15", "16", "17", "18", "19",
		"21", "22", "24", "27", "28",
		"31", "32", "33", "34", "35", "37", "38",
		"41", "42", "43", "44", "45", "46", "47", "48", "49",
		"51", "53", "54", "55",
		"61", "62", "63", "64", "65", "66", "67", "68", "69",
		"71", "73", "74", "75", "77", "79",
		"81", "82", "83", "84", "85", "86", "87", "88", "89",
		"91", "92", "93", "94", "95", "96", "97", "98", "99"
	};

	char ddd[3];

	// Extrai apenas os dois primeiros caracteres do telefone (o DDD)
	// para compará-los isoladamente com a lista de DDDs válidos.
	strncpy(ddd, telefone, 2);
	ddd[2] = '\0';

	for(int i = 0; i < 67; i++) {
		if(strcmp(ddd, dddsValidos[i]) == 0) {
			return 1; // DDD válido
		}
	}

	return 0; // DDD inválido
}

/*
 * validarEmail
 * Verifica se o e-mail possui um formato minimamente válido: precisa
 * conter um '@' e um '.' posicionado depois desse '@'.
 *
 * Observação: esta é uma validação simplificada (não cobre todas as
 * regras formais de e-mail), suficiente para o escopo do trabalho.
 *
 * Parâmetros:
 * - email: string contendo o e-mail a ser validado.
 *
 * Retorno: 1 se o formato é considerado válido; 0 caso contrário.
 */
int validarEmail(char *email) {
	char *arroba = strchr(email, '@');   // primeira ocorrência de '@'
	char *ponto = strrchr(email, '.');   // última ocorrência de '.'

	// A comparação de ponteiros (ponto > arroba) funciona porque ambos
	// apontam para posições dentro da mesma string: se o ponto está em
	// um endereço de memória "depois" do arroba, ele vem depois no texto.
	if(arroba != NULL && ponto != NULL && ponto > arroba) {
		return 1;
	}
	return 0;
}

/*
 * validarCnhCarro
 * Verifica se a categoria da CNH informada permite a condução de
 * automóveis (categorias B, C, D ou E), aceitando tanto letra maiúscula
 * quanto minúscula.
 *
 * Parâmetros:
 * - cnh: string contendo a(s) categoria(s) da CNH do cliente.
 *
 * Retorno: 1 se alguma categoria compatível com carro foi encontrada;
 *          0 caso contrário.
 */
int validarCnhCarro(char *cnh) {
	// Aceita qualquer combinação de categorias (ex: "AB", "BE") desde que
	// contenha pelo menos uma letra que habilite o cliente a dirigir carro.
	if(strchr(cnh, 'B') != NULL || strchr(cnh, 'b') != NULL ||
	        strchr(cnh, 'C') != NULL || strchr(cnh, 'c') != NULL ||
	        strchr(cnh, 'D') != NULL || strchr(cnh, 'd') != NULL ||
	        strchr(cnh, 'E') != NULL || strchr(cnh, 'e') != NULL) {
		return 1;
	}
	return 0;
}

/*
 * ehCpfValido
 * Verifica se o CPF informado possui exatamente 11 dígitos numéricos.
 * Não valida os dígitos verificadores do CPF (regra de cálculo oficial),
 * apenas o formato (tamanho e tipo de caractere).
 *
 * Parâmetros:
 * - cpf: string contendo o CPF a ser validado.
 *
 * Retorno: 1 se o CPF tem exatamente 11 dígitos numéricos; 0 caso contrário.
 */
int ehCpfValido(char *cpf) {
	// Fail-fast: testa o tamanho antes de percorrer caractere a caractere,
	// evitando trabalho desnecessário caso o CPF já esteja claramente errado.
	if(strlen(cpf) != 11) {
		return 0;
	}

	for(int i = 0; i < 11; i++) {
		if(!isdigit(cpf[i])) {
			return 0; // Encontrou uma letra ou símbolo, é inválido!
		}
	}

	return 1; // Tudo certo, só tem números
}

/*
 * validarIdade
 * Verifica se a idade informada está dentro de uma faixa considerada
 * válida para locação (maior de idade e até 120 anos, como limite
 * superior de sanidade dos dados).
 *
 * Parâmetros:
 * - idade: valor inteiro da idade a ser validada.
 *
 * Retorno: 1 se a idade está entre 18 e 120 (inclusive); 0 caso contrário.
 */
int validarIdade(int idade) {
	if(idade >= 18 && idade <= 120) {
		return 1; // Idade válida
	}
	return 0; // Idade inválida
}

/*
 * validarCEP
 * Verifica se o CEP possui exatamente 8 dígitos numéricos (sem hífen).
 *
 * Parâmetros:
 * - cep: string contendo o CEP a ser validado.
 *
 * Retorno: 1 se válido; 0 caso contrário.
 */
int validarCEP(char *cep) {
	if(strlen(cep) != 8) {
		return 0;
	}

	for(int i = 0; i < 8; i++) {
		if(!isdigit(cep[i])) {
			return 0;
		}
	}

	return 1;
}

/*
 * validarTelefoneTamanho
 * Verifica se o telefone possui exatamente 11 dígitos numéricos
 * (DDD + 9 dígitos do número, padrão de celular brasileiro).
 *
 * Parâmetros:
 * - telefone: string contendo o telefone a ser validado.
 *
 * Retorno: 1 se válido; 0 caso contrário.
 */
int validarTelefoneTamanho(char *telefone) {
	if(strlen(telefone) != 11) {
		return 0;
	}

	for(int i = 0; i < 11; i++) {
		if(!isdigit(telefone[i])) {
			return 0;
		}
	}

	return 1;
}

/*
 * validarNome
 * Verifica se o nome informado tem pelo menos 5 caracteres e não contém
 * nenhum dígito numérico.
 *
 * Parâmetros:
 * - nome: string contendo o nome a ser validado.
 *
 * Retorno: 1 se válido; 0 caso contrário.
 */
int validarNome(char *nome) {
	if(strlen(nome) < 5) {
		return 0;
	}

	for(int i = 0; nome[i] != '\0'; i++) {
		if(isdigit(nome[i])) {
			return 0;
		}
	}

	return 1;
}

/*
 * cadastrarCliente
 * Conduz o fluxo completo de cadastro de um novo cliente: solicita cada
 * campo via terminal, valida individualmente, verifica duplicidade
 * (CPF, telefone e e-mail) e só então grava os dados no vetor.
 *
 * Parâmetros:
 * - vetor: ponteiro para o vetor de clientes, onde o novo registro será
 *          inserido.
 * - qtd: ponteiro para a variável que guarda a quantidade de clientes
 *        ativos. É incrementada (*qtd)++ ao final de um cadastro bem
 *        sucedido. Recebida por ponteiro porque a função precisa alterar
 *        o valor original mantido em menuPrincipal(), e não apenas uma
 *        cópia local.
 *
 * Retorno: void. Em caso de qualquer validação falhar, a função
 *          interrompe o cadastro com 'return' e nada é gravado no vetor.
 */
void cadastrarCliente(Cliente *vetor, int *qtd) {
	int pos = -1;

	// Procura a primeira posição livre (ativo == 0) no vetor para reutilizar
	// o espaço de um cliente excluído anteriormente, em vez de exigir que
	// o vetor cresça — já que seu tamanho é fixo (MAX_CLIENTES).
	for(int i = 0; i < MAX_CLIENTES; i++) {
		if(vetor[i].ativo == 0) {
			pos = i;
			break;
		}
	}

	if(pos == -1) {
		printf("\n[ERRO] Limite de clientes atingido!\n");
		return;
	}

	char cpfTemp[15];
	printf("\nCPF (apenas 11 numeros): ");
	limparBuffer();
	fgets(cpfTemp, 15, stdin);
	limparTexto(cpfTemp);

	// Fail-fast: valida o formato do CPF antes de verificar duplicidade,
	// pois não há motivo para buscar no vetor um CPF que já é inválido.
	if(!ehCpfValido(cpfTemp)) {
		printf("\n[ERRO] CPF invalido! Digite apenas 11 numeros.\n");
		return;
	}

	if(buscarPorCpf(vetor, *qtd, cpfTemp) != -1) {
		printf("\n[ERRO] CPF duplicado!\n");
		return;
	}

	int idadeTemp;
	printf("Idade: ");
	scanf("%d", &idadeTemp);

	if(!validarIdade(idadeTemp)) {
		printf("\n[ERRO] Idade invalida!\n");
		return;
	}

	char cnhTemp[20];
	printf("CNH: ");
	// Limpa o '\n' deixado pelo scanf("%d", ...) anterior antes de usar
	// fgets(), evitando que ele leia uma linha vazia (ver limparBuffer()).
	limparBuffer();
	fgets(cnhTemp, 20, stdin);
	limparTexto(cnhTemp);

	if(!validarCnhCarro(cnhTemp)) {
		printf("\n[ERRO] CNH nao compativel com carro!\n");
		return;
	}

	char telefoneTemp[20];
	printf("Telefone: ");
	fgets(telefoneTemp, 20, stdin);
	limparTexto(telefoneTemp);

	// Regra de negócio: todo celular brasileiro tem '9' como terceiro
	// caractere (posição 2), logo após o DDD de 2 dígitos. Verificamos
	// o tamanho mínimo antes de acessar o índice 2, para não ler memória
	// fora dos limites caso o usuário digite um telefone muito curto.
	if(strlen(telefoneTemp) < 3 || telefoneTemp[2] != '9') {
		printf("\n[ERRO] Telefone invalido! O numero deve iniciar com 9 apos o DDD.\n");
		return;
	}

	if(!validarTelefoneTamanho(telefoneTemp)) {
		printf("\n[ERRO] Telefone deve possuir 11 numeros.\n");
		return;
	}

	if(!validarDDD(telefoneTemp)) {
		printf("\n[ERRO] DDD invalido!\n");
		return;
	}

	if(buscarPorTelefone(vetor, *qtd, telefoneTemp) != -1) {
		printf("\n[ERRO] Telefone duplicado!\n");
		return;
	}

	char emailTemp[100];
	printf("Email: ");
	fgets(emailTemp, 100, stdin);
	limparTexto(emailTemp);

	if(!validarEmail(emailTemp)) {
		printf("\n[ERRO] Email invalido!\n");
		return;
	}

	if(buscarPorEmail(vetor, *qtd, emailTemp) != -1) {
		printf("\n[ERRO] E-mail duplicado!\n");
		return;
	}

	char nomeTemp[100];
	printf("Nome: ");
	fgets(nomeTemp, 100, stdin);
	limparTexto(nomeTemp);

	if(!validarNome(nomeTemp)) {
		printf("\n[ERRO] Nome invalido!\n");
		return;
	}

	char cepTemp[10];
	printf("CEP: ");
	fgets(cepTemp, 10, stdin);
	limparTexto(cepTemp);

	if(!validarCEP(cepTemp)) {
		printf("\n[ERRO] CEP invalido!\n");
		return;
	}

	// Garantia de consistência: só copiamos os dados para o vetor depois
	// que TODOS os campos passaram pela validação. Se gravássemos campo a
	// campo durante as validações acima, um erro no último campo (CEP, por
	// exemplo) deixaria o cliente parcialmente cadastrado e marcado como
	// inativo, com dados "lixo" ocupando a posição no vetor.
	strcpy(vetor[pos].cpf, cpfTemp);
	vetor[pos].idade = idadeTemp;
	strcpy(vetor[pos].cnh, cnhTemp);
	strcpy(vetor[pos].telefone, telefoneTemp);
	strcpy(vetor[pos].email, emailTemp);
	strcpy(vetor[pos].nome, nomeTemp);
	strcpy(vetor[pos].cep, cepTemp);
	vetor[pos].ativo = 1;
	(*qtd)++;

	printf("\n[OK] Cliente cadastrado com sucesso!\n");
}

/*
 * listarClientes
 * Percorre todo o vetor de clientes e imprime os dados de cada cliente
 * ativo no terminal.
 *
 * Parâmetros:
 * - vetor: ponteiro para o vetor de clientes a ser percorrido.
 * - qtd: quantidade atual de clientes ativos (recebida por valor, pois
 *        a função apenas consulta a informação e não precisa alterá-la
 *        — diferente de cadastrarCliente/excluirCliente, que recebem
 *        *qtd por ponteiro justamente porque modificam esse valor).
 *
 * Retorno: void. A saída é feita diretamente via printf, sem retorno
 *          de dados para quem chamou a função.
 */
void listarClientes(Cliente *vetor, int qtd) {
	int total = 0;
	printf("\nCLIENTES\n");

	// Percorre todas as posições do vetor (até MAX_CLIENTES) verificando
	// individualmente o campo 'ativo', pois 'qtd' informa apenas o total
	// de clientes ativos, e não em quais índices eles estão armazenados
	// (um cliente excluído no meio do vetor deixa um "buraco" no índice).
	for(int i = 0; i < MAX_CLIENTES; i++) {
		if(vetor[i].ativo == 1) {
			printf("\nNome: %s\nCPF: %s\nIdade: %d\nCNH: %s\nTelefone: %s\nEmail: %s\nCEP: %s\n",
			       vetor[i].nome, vetor[i].cpf, vetor[i].idade, vetor[i].cnh, vetor[i].telefone, vetor[i].email, vetor[i].cep);
			total++;
		}
	}
	if(total == 0) {
		printf("Nenhum cliente cadastrado.\n");
	}
}

/*
 * consultarCliente
 * Solicita um CPF ao usuário e exibe os principais dados do cliente
 * correspondente, caso exista.
 *
 * Parâmetros:
 * - vetor: ponteiro para o vetor de clientes.
 * - qtd: quantidade atual de clientes ativos (apenas repassada para
 *        buscarPorCpf, sem necessidade de ser alterada aqui).
 *
 * Retorno: void.
 */
void consultarCliente(Cliente *vetor, int qtd) {
	char cpfBusca[15];
	printf("\nCPF para buscar: ");
	limparBuffer();
	fgets(cpfBusca, 15, stdin);
	limparTexto(cpfBusca);

	int pos = buscarPorCpf(vetor, qtd, cpfBusca);
	if(pos == -1) {
		printf("\n[ERRO] Nao encontrado!\n");
	} else {
		printf("\n[OK] Encontrado!\nNome: %s\nCPF: %s\nEmail: %s\n", vetor[pos].nome, vetor[pos].cpf, vetor[pos].email);
	}
}

/*
 * alterarCliente
 * Localiza um cliente pelo CPF e permite alterar um único campo
 * específico (escolhido por um submenu numérico), reaplicando a mesma
 * validação usada no cadastro original para o campo escolhido.
 *
 * Parâmetros:
 * - vetor: ponteiro para o vetor de clientes, cujo registro encontrado
 *          será modificado diretamente (vetor[pos].campo = ...).
 * - qtd: quantidade atual de clientes ativos, usada nas buscas de
 *        duplicidade (CPF/telefone/e-mail) ao validar o novo valor.
 *
 * Retorno: void. Cada ramo do submenu trata seu próprio erro e usa
 *          'return' para interromper a função caso a validação falhe,
 *          evitando que o fluxo caia incorretamente em outro ramo.
 */
void alterarCliente(Cliente *vetor, int qtd) {
	char cpfBusca[15], novoCpf[15], novoTelefone[20], novoEmail[100], novaCnh[20], novoNome[100], novoCep[10];
	int novaIdade;

	printf("\nCPF para alterar: ");
	limparBuffer();
	fgets(cpfBusca, 15, stdin);
	limparTexto(cpfBusca);

	int pos = buscarPorCpf(vetor, qtd, cpfBusca);
	if(pos == -1) {
		printf("\n[ERRO] Nao encontrado!\n");
		return;
	}

	int opcaoAlterar = 0;
	printf("\nO que deseja alterar?\n1. CPF\n2. Nome\n3. Idade\n4. CNH\n5. Telefone\n6. Email\n7. CEP\nEscolha: ");
	scanf("%d", &opcaoAlterar);

	if(opcaoAlterar == 1) {
		printf("Novo CPF: ");
		limparBuffer();
		fgets(novoCpf, 15, stdin);
		limparTexto(novoCpf);

		if(!ehCpfValido(novoCpf)) {
			printf("\n[ERRO] CPF invalido!\n");
			return;
		}

		// Verifica duplicidade somente depois de confirmar que o formato
		// do novo CPF é válido (fail-fast), evitando busca desnecessária.
		if(buscarPorCpf(vetor, qtd, novoCpf) != -1) {
			printf("\n[ERRO] CPF ja cadastrado!\n");
			return;
		}

		strcpy(vetor[pos].cpf, novoCpf);
		printf("\n[OK] CPF atualizado!\n");

	} else if(opcaoAlterar == 2) {
		printf("Novo Nome: ");
		limparBuffer();
		fgets(novoNome, 100, stdin);
		limparTexto(novoNome);

		if(!validarNome(novoNome)) {
			printf("\n[ERRO] Nome invalido! Deve conter pelo menos 5 caracteres.\n");
			return;
		}

		strcpy(vetor[pos].nome, novoNome);
		printf("\n[OK] Nome atualizado!\n");

	} else if(opcaoAlterar == 3) {
		printf("Nova Idade: ");
		limparBuffer();
		scanf("%d", &novaIdade);
		limparBuffer();

		// Reaproveita a mesma regra usada no cadastro (validarIdade),
		// em vez de repetir a comparação "novaIdade < 18" isoladamente,
		// para manter uma única fonte de verdade sobre o que é válido.
		if(!validarIdade(novaIdade)) {
			printf("\n[ERRO] Idade invalida!\n");
			return;
		}

		vetor[pos].idade = novaIdade;
		printf("\n[OK] Idade atualizada!\n");

	} else if(opcaoAlterar == 4) {
		printf("Nova CNH: ");
		limparBuffer();
		fgets(novaCnh, 20, stdin);
		limparTexto(novaCnh);

		if(!validarCnhCarro(novaCnh)) {
			printf("\n[ERRO] CNH nao compativel com carro!\n");
			return;
		}

		strcpy(vetor[pos].cnh, novaCnh);
		printf("\n[OK] CNH atualizada!\n");

	} else if(opcaoAlterar == 5) {
		printf("Novo Telefone: ");
		limparBuffer();
		fgets(novoTelefone, 20, stdin);
		limparTexto(novoTelefone);

		if(!validarTelefoneTamanho(novoTelefone)) {
			printf("\n[ERRO] Telefone deve possuir 11 numeros.\n");
			return;
		}

		if(!validarDDD(novoTelefone)) {
			printf("\n[ERRO] DDD invalido!\n");
			return;
		}

		if(novoTelefone[2] != '9') {
			printf("\n[ERRO] O telefone deve iniciar com 9 apos o DDD.\n");
			return;
		}

		if(buscarPorTelefone(vetor, qtd, novoTelefone) != -1) {
			printf("\n[ERRO] Telefone ja em uso!\n");
			return;
		}

		strcpy(vetor[pos].telefone, novoTelefone);
		printf("\n[OK] Telefone atualizado!\n");

	} else if(opcaoAlterar == 6) {
		printf("Novo Email: ");
		limparBuffer();
		fgets(novoEmail, 100, stdin);
		limparTexto(novoEmail);

		// Mesma validação de formato aplicada no cadastro original,
		// garantindo que a alteração não relaxe uma regra já existente.
		if(!validarEmail(novoEmail)) {
			printf("\n[ERRO] Email invalido!\n");
			return;
		}

		if(buscarPorEmail(vetor, qtd, novoEmail) != -1) {
			printf("\n[ERRO] E-mail ja em uso!\n");
			return;
		}

		strcpy(vetor[pos].email, novoEmail);
		printf("\n[OK] Email atualizado!\n");

	} else if(opcaoAlterar == 7) {
		printf("Novo CEP: ");
		limparBuffer();
		fgets(novoCep, 10, stdin);
		limparTexto(novoCep);

		if(!validarCEP(novoCep)) {
			printf("\n[ERRO] CEP invalido! Deve possuir 8 numeros.\n");
			return;
		}

		strcpy(vetor[pos].cep, novoCep);
		printf("\n[OK] CEP atualizado!\n");

	} else {
		printf("\n[ERRO] Opcao invalida!\n");
	}
}

/*
 * excluirCliente
 * Localiza um cliente pelo CPF e o marca como inativo (soft delete),
 * sem removê-lo fisicamente do vetor.
 *
 * Parâmetros:
 * - vetor: ponteiro para o vetor de clientes.
 * - qtd: ponteiro para a quantidade de clientes ativos, decrementada
 *        (*qtd)-- após a exclusão. Recebida por ponteiro pelo mesmo
 *        motivo de cadastrarCliente: a função precisa alterar a variável
 *        original de menuPrincipal(), não uma cópia local.
 *
 * Retorno: void.
 */
void excluirCliente(Cliente *vetor, int *qtd) {
	char cpfBusca[15];
	printf("\nCPF para excluir: ");
	limparBuffer();
	fgets(cpfBusca, 15, stdin);
	limparTexto(cpfBusca);

	int pos = buscarPorCpf(vetor, *qtd, cpfBusca);
	if(pos == -1) {
		printf("\n[ERRO] Nao encontrado!\n");
		return;
	}

	// Soft delete: apenas desativamos o registro em vez de deslocar todos
	// os elementos seguintes do vetor para "tapar o buraco". Isso é O(1)
	// em vez de O(n), e a posição liberada será reaproveitada no próximo
	// cadastro (ver busca por 'ativo == 0' em cadastrarCliente).
	vetor[pos].ativo = 0;
	(*qtd)--;

	printf("\n[OK] Removido com sucesso!\n");
}

/*
 * submenuClientes
 * Exibe o submenu de gerenciamento de clientes em loop, repassando o
 * vetor e a quantidade de clientes para a função correspondente à opção
 * escolhida, até que o usuário decida voltar ao menu principal.
 *
 * Parâmetros:
 * - vetor: ponteiro para o vetor de clientes, repassado para as funções
 *          de cadastro/listagem/consulta/alteração/exclusão.
 * - qtd: ponteiro para a quantidade de clientes ativos. É repassado por
 *        ponteiro às funções que precisam alterá-lo (cadastrar/excluir)
 *        e desreferenciado (*qtd) ao repassar para as que só leem o
 *        valor (listar/consultar/alterar).
 *
 * Retorno: void.
 */
void submenuClientes(Cliente *vetor, int *qtd) {
	int opcao = 0;
	while(opcao != 6) {
		printf("\n--- PAINEL DE CLIENTES ---\n");
		printf("1. Cadastrar cliente\n");
		printf("2. Listar clientes\n");
		printf("3. Consultar cliente\n");
		printf("4. Alterar cliente\n");
		printf("5. Excluir cliente\n");
		printf("6. Voltar ao menu principal\n");
		printf("Escolha uma opcao: ");
		scanf("%d", &opcao);

		if(opcao == 1) {
			cadastrarCliente(vetor, qtd);
		} else if(opcao == 2) {
			listarClientes(vetor, *qtd);
		} else if(opcao == 3) {
			consultarCliente(vetor, *qtd);
		} else if(opcao == 4) {
			alterarCliente(vetor, *qtd);
		} else if(opcao == 5) {
			excluirCliente(vetor, qtd);
		} else if(opcao == 6) {
			printf("Voltando...\n");
			return;
		} else {
			printf("\n[ERRO] Opcao invalida!\n");
		}
	}
}

/*
 * submenuCarros
 * Exibe o submenu de controle de frota (módulo ainda não implementado).
 * As opções de cadastro/listagem/etc. de veículos encerram o programa
 * propositalmente (exit(1)) como sinalizador temporário de funcionalidade
 * pendente, até que essas funções sejam desenvolvidas pelo grupo.
 *
 * Parâmetros: nenhum.
 * Retorno: void.
 */
void submenuCarros() {
	int opcao = 0;
	while(opcao != 6) {
		printf("\n--- CONTROLE DE FROTA ---\n");
		printf("1. Cadastrar veiculo\n");
		printf("2. Listar frota\n");
		printf("3. Consultar veiculo\n");
		printf("4. Alterar dados do veiculo\n");
		printf("5. Remover veiculo\n");
		printf("6. Voltar ao menu principal\n");
		printf("Escolha uma opcao: ");
		scanf("%d", &opcao);

		if(opcao == 1) {
			// TODO: implementar cadastro de veículo.
			printf("Chamando funcao de cadastrar veiculo...\n");
			exit(1);
		} else if(opcao == 6) {
			printf("Voltando...\n");
			return;
		} else if(opcao > 1 && opcao < 6) {
			// TODO: implementar demais operações de frota (listar, consultar,
			// alterar, remover).
			printf("Chamando outra funcao de frota...\n");
			exit(1);
		} else {
			printf("\n[ERRO] Opcao invalida!\n");
		}
	}
}

/*
 * submenuAlugueis
 * Exibe o submenu de gerenciamento de locações (módulo ainda não
 * implementado), seguindo o mesmo padrão de placeholder de submenuCarros().
 *
 * Parâmetros: nenhum.
 * Retorno: void.
 */
void submenuAlugueis() {
	int opcao = 0;
	while(opcao != 2) {
		printf("\n--- CENTRAL DE LOCACOES ---\n");
		printf("1. Listar locacoes ativas\n");
		printf("2. Voltar ao menu principal\n");
		printf("Escolha uma opcao: ");
		scanf("%d", &opcao);

		if(opcao == 1) {
			// TODO: implementar listagem de locações ativas.
			printf("Chamando funcao de listar locacoes...\n");
			exit(1);
		} else if(opcao == 2) {
			printf("Voltando...\n");
			return;
		} else {
			printf("\n[ERRO] Opcao invalida!\n");
		}
	}
}

/*
 * menuPrincipal
 * Inicializa o vetor de clientes (marcando todas as posições como
 * inativas) e exibe o menu principal do sistema em loop, direcionando o
 * usuário para o submenu correspondente até que ele escolha encerrar.
 *
 * Parâmetros:
 * - vetorClientes: ponteiro para o vetor de clientes alocado
 *                  dinamicamente em main(), compartilhado entre todos os
 *                  submenus que manipulam clientes.
 *
 * Retorno: void.
 */
void menuPrincipal(Cliente *vetorClientes) {
	// Inicialização defensiva: malloc() não zera a memória alocada, então
	// os campos 'ativo' começariam com valor indeterminado (lixo de
	// memória) se não fossem explicitamente zerados aqui.
	for(int i = 0; i < MAX_CLIENTES; i++) {
		vetorClientes[i].ativo = 0;
	}

	int qtdClientes = 0;
	int opcao = 0;

	while(opcao != 4) {
		printf("\n=== MENU PRINCIPAL ===\n");
		printf("1. Gerenciar clientes\n");
		printf("2. Gerenciar carros\n");
		printf("3. Gerenciar alugueis\n");
		printf("4. Sair do Sistema\n");
		printf("Escolha uma opcao: ");
		scanf("%d", &opcao);

		if(opcao == 1) {
			// Passamos &qtdClientes (endereço da variável) porque
			// submenuClientes() e, por consequência, cadastrarCliente()/
			// excluirCliente() precisam alterar o valor real de
			// qtdClientes, e não apenas uma cópia local.
			submenuClientes(vetorClientes, &qtdClientes);
		} else if(opcao == 2) {
			submenuCarros();
		} else if(opcao == 3) {
			submenuAlugueis();
		} else if(opcao == 4) {
			printf("Encerrando o sistema de locacao...\n");
			return;
		} else {
			printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
		}
	}
}

/*
 * main
 * Ponto de entrada do programa. Aloca dinamicamente o vetor de clientes,
 * verifica se a alocação foi bem-sucedida, inicia o menu principal e,
 * por fim, libera a memória alocada antes de encerrar o programa.
 *
 * Parâmetros: nenhum.
 * Retorno: 0 em caso de execução normal; 1 caso a alocação de memória
 *          falhe.
 */
int main() {
	// Alocação dinâmica: reserva espaço na heap para MAX_CLIENTES
	// estruturas Cliente, em vez de declarar um vetor estático fixo.
	// O cast (Cliente *) é implícito aqui (C permite atribuir void* a
	// outro tipo de ponteiro diretamente), mas seria obrigatório em C++:
	// Cliente *vetorClientes = (Cliente *) malloc(sizeof(Cliente) * MAX_CLIENTES);
	Cliente *vetorClientes = malloc(sizeof(Cliente) * MAX_CLIENTES);

	// Tratamento de erro: verifica se a memória foi alocada com sucesso
	// pelo sistema operacional antes de prosseguir. Se a alocação falhar
	// (memória insuficiente, por exemplo), malloc() retorna NULL, e usar
	// esse ponteiro sem essa checagem causaria comportamento indefinido
	// (provavelmente um crash) na primeira tentativa de acesso ao vetor.
	if(vetorClientes == NULL) {
		printf("\n[ERRO] Falha ao alocar memoria!\n");
		return 1;
	}

	menuPrincipal(vetorClientes);

	// Libera a memória alocada dinamicamente, devolvendo-a ao sistema
	// operacional. Toda chamada a malloc() bem-sucedida deve ter uma
	// chamada a free() correspondente, para evitar vazamento de memória.
	free(vetorClientes);

	return 0;
}