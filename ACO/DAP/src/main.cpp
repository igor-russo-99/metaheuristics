//============================================================================
// Name        : main.cpp
// Author      : Igor
// Version     : 0.0.0.1
// Description : Dynamic Ant Programming in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <math.h>

using namespace std;

#define MAX_ELEMENTOS 100
#define NUM_TERMINAIS 2
#define NUM_FUNCOES 6
#define INFINITE 99999999999;

/* Definição das estruturas de dados */

enum tp_no {START=-1, TERMINAL=0, FUNCAO=1};
enum tp_terminais { T_X, T_CT };
enum tp_funcoes { T_SOMA=0, T_SUB=1, T_MUL=2, T_DIV=3, T_SEN=4, T_COS=5};
char aridade_nao_terminais[] = {2, 2, 2, 2, 1, 1};

typedef struct{
	tp_no tipo;
	char valor;
	char aridade;
}type_no;

typedef struct item_p{
	type_no elemento;
	int indice;
	struct item_p *filhos[2];

}type_itemp;

typedef struct{
	char caminho[MAX_ELEMENTOS];
	type_itemp programa;
	int tamanho;
	float fitness;
}type_ant;

/**/

/* Variáveis */
/*
float x[] = {4.242065,
		-0.661788,
		3.614091,
		3.782840,
		5.028121,
		-2.826935,
		-1.312550,
		3.450526,
		-1.944478,
		1.093670};

float y[] = {422.397797,
		-0.321852,
		234.488647,
		276.997375,
		796.610657,
		46.438061,
		1.116988,
		198.194885,
		8.780380,
		5.028621};*/

float x[] = {1.0, 2.0, 3.0};

float y[] = {1.0, 2.0, 3.0};

type_ant * formigas;
type_no elementos[MAX_ELEMENTOS]; //Elementos da iteração corrente
float feromonios[MAX_ELEMENTOS][(MAX_ELEMENTOS*2)+1]; //+1 para o nó Start

int m_iteracoes; //Número de iterações do algoritmo
int m_n_ants;    //Número de formigas
int m_elementos; //Número corrente de nós
float m_alpha;   //Fator de importância do histórico de feromônios
float m_beta;    //Fator de importância da heurística
float m_rho;     //Taxa de evaporação dos feromônios
float m_tal_min; //Valor mínimo de feromônio em uma aresta
float m_tal_ins; //Valor de feromônio atribuído a uma linha/coluna inseridos
type_ant m_best_so_far; //Melhor caminho da iteração

float probabilidades[MAX_ELEMENTOS]; //Probabilidade de cada elemento ser escolhido
char  visitados[MAX_ELEMENTOS];

/**/

/* Protótipos das funções */

void  ObtemParametros(int argc, char * argv[]);
void  InicializaFeromonios();
void  ConstroiCaminho(int id_formiga);
float CalculaProbabilidade(int i, int j);
void AvaliaPrograma(int id);
void  AtualizaFeromonios();
void  DeletaNos();
void  DeletaNo(int k);
void  InicializaNos();
void  InsereNos(int iteracao);
float randr(int min, int max);
float rand01();
char * ObtemNomeElemento(tp_no tipo, int valor);
void ImprimePrograma(type_itemp *programa);

/**/

int main(int argc, char * argv[]) {

	/* Inicialização */
	ObtemParametros(argc, argv);
	InicializaNos();
	InicializaFeromonios();

	int count = 1, i;

	/* Critério de parada */
	while(count <= m_iteracoes){

		/* Constrói o caminho para cada formiga */
		for(i=0; i< m_n_ants;i++){
			ConstroiCaminho(i);
			AvaliaPrograma(i);
		}

		//Obtém o melhor caminho da iteração

		AtualizaFeromonios();
		DeletaNos();
		InsereNos(count);

		count++;
	}

	return 0;
}

/* Implementação das funções */

void  ObtemParametros(int argc, char * argv[]){

	m_iteracoes = 2;
	m_n_ants  = 100;
	m_alpha   = 0.9f;
	m_beta    = 0.1f;
	m_rho     = 0.0f;
	m_tal_min = 0.0f;

	//Nota: interessante para incentivar escolhas de novos caminhos
	m_tal_ins = 0.2f;

    formigas = (type_ant *) malloc(m_n_ants * sizeof(type_ant));
}

void InicializaFeromonios(){

	int i,j;
	for(i=0;i<2*m_elementos;i++){
		for(j=0;j< m_elementos;j++){
			feromonios[i][j] = m_tal_ins;
		}
	}
}

int compara_floats(const void* a, const void* b){

	float* p1 = (float*)a;
	float* p2 = (float*)b;

    return p1 < p2;
}

void ordena(float * p){
	qsort(p, m_elementos, sizeof(float), (int(*)(const void*, const void*))compara_floats);
}

void ConstroiCaminho(int id){

	int i,j,selecionado;
	float aleatorio;

	type_ant * formiga = &formigas[id];

	formiga->programa.elemento.tipo    = START;
	formiga->programa.indice=0;
	formiga->programa.elemento.aridade = 1;
	formiga->programa.filhos[0] = NULL;
	formiga->programa.filhos[1] = NULL;

	formiga->tamanho = 1;
	//restantes = 1;
	//atual=0;fabs

	for(i=0;i<m_elementos;i++){
		visitados[i] = 0;
	}

	//Fila para construir a árvore do programa em largura
	queue <type_itemp*> fila;
	fila.push(&formiga->programa);

	while(!fila.empty()){

		type_itemp* atual = fila.front();
		fila.pop();

		for(j=0; j<atual->elemento.aridade; j++){

			int maior=1;

			//TODO: rever escolha baseada na probabilidade
			float total=0.0f, soma_parcial=0.0f;
			for(i=1; i<m_elementos; i++){
				probabilidades[i] = CalculaProbabilidade((atual->indice*2)+j, i);
				total += probabilidades[i];

				if(probabilidades[i] > probabilidades[maior])
					maior = i;
			}

			aleatorio = rand01();
			selecionado = 1;
			soma_parcial = probabilidades[selecionado];

			while(soma_parcial <= aleatorio && selecionado < m_elementos){
				selecionado++;
				soma_parcial += probabilidades[selecionado];
			}

			if(soma_parcial <= aleatorio && selecionado == m_elementos){
				selecionado = maior;
			}

			//TODO: adicionar métodos para criar novos nós

			type_itemp *novo = (type_itemp*)malloc(sizeof(type_itemp));
			novo->elemento = elementos[selecionado];
			novo->indice = selecionado;
			novo->filhos[0] = NULL;
			novo->filhos[1] = NULL;
			atual->filhos[j] = novo;

			fila.push(novo);

			if(novo->elemento.tipo == FUNCAO){
				visitados[selecionado] = 1;
			}
			//printf("Elemento adicionado: %s\n", ObtemNomeElemento(novo->elemento.tipo, (int)novo->elemento.valor));
		}
	}

	printf("Programa %d: ", id);
	ImprimePrograma(&formiga->programa);
	printf("\n");
}

float Avalia(type_itemp *p, float x, float y){

	if(p->elemento.tipo == TERMINAL){

		switch (p->elemento.valor) {
			case T_X:
				return x;
			default:
				return 1.0f;
		}
	}
	else{
		switch (p->elemento.valor) {
			case T_SOMA:
				return Avalia(p->filhos[0], x, y) + Avalia(p->filhos[1], x, y);
			case T_SUB:
				return Avalia(p->filhos[0], x, y) - Avalia(p->filhos[1], x, y);
			case T_MUL:
				return Avalia(p->filhos[0], x, y) * Avalia(p->filhos[1], x, y);
			case T_DIV:
				return Avalia(p->filhos[0], x, y) / Avalia(p->filhos[1], x, y);
			case T_SEN:
				return sin(Avalia(p->filhos[0], x, y));
			case T_COS:
				return cos(Avalia(p->filhos[0], x, y));
			default:
				return 0;
		}
	}
}

void AvaliaPrograma(int id){

	type_ant *formiga    = &formigas[id];
	type_itemp *programa = &formiga->programa;

	//Pula o nó START
	programa = programa->filhos[0];

	int i,j, k, n = sizeof(y)/(sizeof(float));
	float erro = 0;

	for(i=0;i<n;i++){
		erro += pow(Avalia(programa, x[i], y[i]) - y[i],2);
	}

	formiga->fitness = 1.0f/(1.0f+erro);

	if(isnan(formiga->fitness) || isinf(formiga->fitness))
		formiga->fitness = 0;

	printf("Fitness: %f\n\n", formiga->fitness);

}

float CalculaProbabilidade(int i, int j){

	if(visitados[j]){
		return 0;
	}

	int k;
	float total=0;

	for(k=0 ; k < m_elementos; k++){
		if(!visitados[k]){
			total+=feromonios[i][k];
		}
	}

	if(total == 0){
		total = 1;
	}

	//printf("elemento=%d, total=%f, prob=%f\n",j, total, feromonios[i][j]/total);

	return feromonios[i][j]/total;
}

void AtualizaFeromonios(){

	int i,j,h;

	//Evaporação
	for(i=0 ; i< 2*m_elementos; i++){
		for(j=0; j< m_elementos; j++){
			feromonios[i][j] = (1-m_rho)*feromonios[i][j];
		}
	}

	//Reforço de feromônios somente no caminho da melhor solução da iteração
	for ( i = 0 ; i < m_best_so_far.tamanho; i++ ) {
		j = m_best_so_far.caminho[i];
		h = m_best_so_far.caminho[i+1];

		feromonios[j][h] += m_best_so_far.fitness;
	}
}

void InicializaNos(){

	int i;

	type_no start;
	start.tipo = START;
	elementos[m_elementos++] = start;

	for(i=0; i<NUM_TERMINAIS;i++){
		type_no novo;
		novo.tipo    = TERMINAL;
		novo.valor   = i;
		novo.aridade = 0;
		elementos[m_elementos++] = novo;
	}

	for(i=0; i<NUM_FUNCOES;i++){
		type_no novo;
		novo.tipo   = FUNCAO;
		novo.valor  = i;
		novo.aridade = aridade_nao_terminais[i];
		elementos[m_elementos++] = novo;
	}
}

void InsereNos(int iteracao){

	int i;
	float soma = 0.0f;
	float prob, aleatorio;

	/* Dúvida: no paper está "número de nós gerados por cada formiga" */
	for(i=0 ; i<m_n_ants ; i++ )
		soma+=formigas[i].tamanho;

	soma = soma*(1.0f/m_n_ants);
	prob = soma/m_elementos;
	aleatorio = rand()/(float)RAND_MAX;

	//Deve inserir outro elemento?
	if(aleatorio >= prob){

		type_no novo;

		if(rand()%2 == 0){
			//Inserir terminal
			int elemento = rand() % NUM_TERMINAIS;
			novo.tipo   = TERMINAL;
			novo.valor  = elemento;
			novo.aridade = 0;
		}
		else{
			//Inserir função
			int elemento = rand() % NUM_FUNCOES;
			novo.tipo    = FUNCAO;
			novo.valor   = elemento;
			novo.aridade = aridade_nao_terminais[elemento];
		}

		elementos[m_elementos++] = novo;

		/*
		 Depósito inicial de feromônios
	    */
		for(i=0;i< m_elementos; i++){
			feromonios[m_elementos][i] = m_tal_ins;
		}
		for(i=0;i< 2*m_elementos; i++){
			feromonios[i][m_elementos] = m_tal_ins;
		}
	}
}

void DeletaNos(){

	int i,k=0, deletar;

	while(k<m_elementos){
		deletar=0;

		for(i=0;i< m_elementos; i++){
			if(feromonios[k][i] != m_tal_min){
				deletar = 0;
			}
		}
		for(i=0;i< 2*m_elementos; i++){
			if(feromonios[i][k] != m_tal_min){
				deletar = 0;
			}
		}

		if(deletar){
			DeletaNo(k);
		}
		else{
			k++;
		}
	}
}

void  DeletaNo(int k){

	//TODO: Alocar dinamicamente e só trocar os ponteiros

	int i,j;

	//Exclui da lista de nós
	for(i=k; i< m_elementos-1;i++){
		elementos[k] = elementos[k+1];
	}

	//Exclui a coluna da tabela de feromônios
	for(i=k; i< m_elementos-1;i++){
		feromonios[i][k] = feromonios[i][k+1];
	}

	//Exclui a linha da tabela de feromônios
	for(i=k; i< (2*m_elementos)-1;i++){
		feromonios[k][i] = feromonios[k+1][i];
	}

	m_elementos--;

	printf("Nó deletado. A lista agora contém %d elementos.\n", m_elementos);
}

char * ObtemNomeElemento(tp_no tipo, int valor){

	if(tipo == START){
		return "START";
	}
	if(tipo == TERMINAL){
		switch(valor){

			case T_X:
				return "X";
			case T_CT:
				return "1";
			default:
				return "erro";
		}
	}
	else{
		switch(valor){

			case T_SOMA:
				return "+";
			case T_SUB:
				return "-";
			case T_MUL:
				return "*";
			case T_DIV:
				return "/";
			case T_SEN:
				return "sen";
			case T_COS:
				return "cos";
			default:
				return "erro";
		}
	}
}

void ImprimePrograma(type_itemp *programa){

	char text[MAX_ELEMENTOS];

	if(programa->elemento.tipo == START){
		printf("START->");
		ImprimePrograma(programa->filhos[0]);
	}
	else if(programa->elemento.tipo == TERMINAL){

		switch((int)programa->elemento.valor){
			case T_X:
				printf("X");
				break;
			case T_CT:
				printf("1");
				break;
			default:
				printf("erro");
		}
	}
	else{
		switch(programa->elemento.valor){

			case T_SOMA:
				printf("(");
				ImprimePrograma(programa->filhos[0]);
				printf("+");
				ImprimePrograma(programa->filhos[1]);
				printf(")");
				break;
			case T_SUB:
				printf("(");
				ImprimePrograma(programa->filhos[0]);
				printf("-");
				ImprimePrograma(programa->filhos[1]);
				printf(")");
				break;
			case T_MUL:
				printf("(");
				ImprimePrograma(programa->filhos[0]);
				printf("*");
				ImprimePrograma(programa->filhos[1]);
				printf(")");
				break;
			case T_DIV:
				printf("(");
				ImprimePrograma(programa->filhos[0]);
				printf("/");
				ImprimePrograma(programa->filhos[1]);
				printf(")");
				break;
			case T_SEN:
				printf("sen(");
				ImprimePrograma(programa->filhos[0]);
				printf(")");
				break;
			case T_COS:
				printf("cos(");
				ImprimePrograma(programa->filhos[0]);
				printf(")");
				break;
			default:
				printf("erro");
				break;
		}
	}



	/*queue<type_itemp> fila;
	fila.push(*programa);
	char text[MAX_ELEMENTOS];

	while(!fila.empty()){

		type_itemp atual = fila.front();
		fila.pop();

		if(atual.filhos[0] != NULL){
			fila.push(*atual.filhos[0]);
		}
		if(atual.filhos[1] != NULL){
			fila.push(*atual.filhos[1]);
		}

		printf("%s ", ObtemNomeElemento(atual.elemento.tipo, (int)atual.elemento.valor));
	}
	printf("\n");*/
}

float rand01()
{
	return (float)rand()/(float)RAND_MAX;
}

float randr(int min, int max)
{
	double scaled = (double)rand()/RAND_MAX;
	return (max - min +1)*scaled + min;
}
