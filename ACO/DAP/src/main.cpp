//============================================================================
// Name        : main.cpp
// Author      : Igor
// Version     : 0.0.0.1
// Description : Dynamic Ant Programming in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <queue>

using namespace std;

#define MAX_ELEMENTOS 100
#define NUM_TERMINAIS 2
#define NUM_FUNCOES 6

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
float AvaliaPrograma(int * programa, int ** dados);
void  AtualizaFeromonios();
void  DeletaNos();
void  DeletaNo(int k);
void  InicializaNos();
void  InsereNos(int iteracao);
float randr(int min, int max);
char * ObtemNomeElemento(tp_no tipo, int valor);

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

	m_iteracoes = 100;
	m_n_ants  = 5;
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
	//atual=0;

	for(i=0;i<m_elementos;i++){
		visitados[i] = 0;
	}

	//Fila para construir a árvore do programa em largura
	queue <type_itemp> fila;
	fila.push(formiga->programa);

	while(!fila.empty()){

		type_itemp atual = fila.front();
		fila.pop();

		for(j=0;j < atual.elemento.aridade; j++){

			selecionado = 0;

			//TODO: rever escolha baseada na "probabilidade"

			for(i=1; i<m_elementos; i++){
				probabilidades[i] = CalculaProbabilidade((atual.indice*2)+j, i);

				if(probabilidades[i] > probabilidades[selecionado])
					selecionado = i;
			}

			//TODO: adicionar métodos para criar novos nós

			type_itemp *novo = (type_itemp*)malloc(sizeof(type_itemp));
			novo->elemento = elementos[selecionado];
			novo->indice = selecionado;
			novo->filhos[0] = NULL;
			novo->filhos[1] = NULL;

			formiga->programa.filhos[j] = novo;

			fila.push(*novo);

			printf("Elemento adicionado: %s\n", ObtemNomeElemento(novo->elemento.tipo, (int)novo->elemento.valor));
		}

	}
}

float CalculaProbabilidade(int i, int j){

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

	printf("elemento=%d, total=%f, prob=%f\n",j, total, feromonios[i][j]/total);

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
		novo.tipo   = TERMINAL;
		novo.valor  = i;
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
				return "*";
			case T_SEN:
				return "sen";
			case T_COS:
				return "cos";
			default:
				return "erro";
		}
	}
}


float randr(int min, int max)
{
	double scaled = (double)rand()/RAND_MAX;
	return (max - min +1)*scaled + min;
}


