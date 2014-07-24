//============================================================================
// Name        : main.cpp
// Author      : Igor
// Version     : 0.0.0.1
// Description : Dynamic Ant Programming in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <queue>
#include <time.h>
#include <math.h>

using namespace std;

#define MAX_ELEMENTOS 200
#define NUM_TERMINAIS 2
#define NUM_FUNCOES 5

/* Definição das estruturas de dados */

enum tp_no {START=-1, TERMINAL=0, FUNCAO=1};
enum tp_terminais { T_X, T_CT };
enum tp_funcoes { T_SOMA=0, T_SUB=1, T_MUL=2, T_DIV=3, T_SEN=4, T_COS=5};
char aridade_nao_terminais[] = {2, 2, 2, 2, 1, 1};

typedef struct{
	tp_no tipo;
	int valor;
	char aridade;
}type_no;

typedef struct item_p{
	type_no elemento;
	int indice;
	struct item_p *filhos[2];

}type_itemp;

typedef struct{
	int caminho[2*MAX_ELEMENTOS];
	type_itemp programa;
	int tamanho;
	float fitness;
}type_ant;

#define STRINGIFY(A) A
#define CONCAT(A,B,C) ABC

#ifndef PROBLEMA
	#define PROBLEMA "problema_3_20.hpp"
#endif

//#include "problema1.hpp"
#include "problema_3_20.hpp"
//#include "problema_3_20.hpp"

/* Variáveis */

/* -- Y = 3X
float x[] = {1.0, 2.0, 3.0};
float y[] = {3.0, 6.0, 9.0};
*/

type_ant * formigas;
type_no elementos[MAX_ELEMENTOS]; //Elementos da iteração corrente
float **feromonios; //[MAX_ELEMENTOS*2][MAX_ELEMENTOS];

int m_iteracoes; //Número de iterações do algoritmo
int m_n_ants;    //Número de formigas
int m_elementos; //Número corrente de nós
float m_alpha;   //Fator de importância do histórico de feromônios
float m_beta;    //Fator de importância da heurística
float m_rho;     //Taxa de evaporação dos feromônios
float m_tal_min; //Valor mínimo de feromônio em uma aresta
float m_tal_max; //Valor máximo de feromônio em uma aresta
float m_tal_ins; //Valor de feromônio atribuído a uma linha/coluna inseridos
int m_seed;
int m_total_execucoes;
type_ant m_best_so_far, m_best_it; //Melhor caminho da iteração

float probabilidades[MAX_ELEMENTOS]; //Probabilidade de cada elemento ser escolhido
char  visitados[MAX_ELEMENTOS];

/**/

/* Protótipos das funções */

float ** CriaTabelaFeromonios(int n_elementos){

	int i;

	n_elementos = MAX_ELEMENTOS;

	float ** t = (float**)malloc(2*n_elementos*sizeof(float*));

	for(i=0;i<2*n_elementos;i++){
		t[i] = (float*)malloc(n_elementos*sizeof(float));
	}

	return t;
}

void  ObtemParametros(int argc, char * argv[]);
void  InicializaFeromonios();
void  ConstroiCaminho(int id_formiga);
float CalculaProbabilidade(int i, int j);
void  AvaliaPrograma(int id);
void  AtualizaFeromonios();
void  DeletaNos();
void  DeletaNo(int k);
void  InicializaNos();
void  InsereNos(int iteracao);
float randr(int min, int max);
float rand01();
char * ObtemNomeElemento(tp_no tipo, int valor);
void ImprimePrograma(type_itemp *programa);
void ImprimeTabelaFeromonios();
/**/

using namespace std;

int main(int argc, char * argv[]) {

	/* Inicialização */
	ObtemParametros(argc, argv);

	//ImprimeTabelaFeromonios();

	int count = 1, i, totalNos=0;
	int sucesso=0;
	int execucao=1;

	while(execucao <= m_total_execucoes){

		srand(execucao);

		m_best_so_far.fitness = -1;
		count=1;
		totalNos=0;

		InicializaNos();
		InicializaFeromonios();

		/* Critério de parada */
		while(count <= m_iteracoes){

	#ifndef EXPERIMENTO
			printf("===================================\n");
			printf("Iteração %d\n", count);
			printf("Tamanho da tabela de feromonios: %d\n", m_elementos);
			printf("Média: %f\n", (float)totalNos/(float)count);
	#endif

			totalNos+=m_elementos;

			m_best_it.fitness = -1;

			/* Constrói o caminho para cada formiga */
			for(i=0; i< m_n_ants; i++){

				ConstroiCaminho(i);
				AvaliaPrograma(i);

				if(formigas[i].fitness > m_best_it.fitness){
					m_best_it = formigas[i];
				}

	/*
	#ifndef EXPERIMENTO
				printf("Programa: %d\n", i);
				ImprimePrograma(&formigas[i].programa);
				printf("\nFitness: %f\n", formigas[i].fitness);
				printf("\n");
	#endif
	*/
			}

	#ifndef EXPERIMENTO
			printf("Melhor programa da iteração: \n");
			ImprimePrograma(&m_best_it.programa);
			printf("\nFitness: %f\n", m_best_it.fitness);
			printf("\n");
	#endif

			if(m_best_it.fitness > m_best_so_far.fitness)
				m_best_so_far = m_best_it;

	#ifndef EXPERIMENTO
			printf("Melhor programa até então: \n");
			ImprimePrograma(&m_best_so_far.programa);
			printf("\nFitness: %f\n", m_best_so_far.fitness);
			printf("\n");
	#endif

			if (m_best_so_far.fitness >=1.0 - pow(10, -4)) {
				//printf("A solução foi encontrada\t 1 \t%d",count);
				break;
			}

			//printf("----------- Inicializando AtualizaFeromonios ----------\n");
			AtualizaFeromonios();
			//printf("----------- Finalizou AtualizaFeromonios ----------\n");

			//printf("----------- Inicializando DeletaNos ----------\n");
			DeletaNos();
			//printf("----------- Finalizou DeletaNos ----------\n");

			//printf("----------- Inicializando InsereNos ----------\n");
			InsereNos(count);
			//printf("----------- Finalizou InsereNos ----------\n");

			//ImprimeTabelaFeromonios();
			count++;
		}

		if (m_best_so_far.fitness >= 1.0 - pow(10, -4)) {

			/*printf("A solução foi encontrada\t 1 \t %d\t", count);
			ImprimePrograma(&m_best_so_far.programa);
			printf("\n");*/
			sucesso++;
		}
		else{
			//printf("A solução nao foi encontrada\t 0 \t best fitness:\t%f\t", m_best_so_far.fitness);
			//ImprimePrograma(&m_best_so_far.programa);
			//printf("\n");
		}

		execucao++;
	}

	printf("Total de execuções: %d, sucessos: %d\n", m_total_execucoes, sucesso);
	printf("Taxa de sucesso: %f\n", (float)sucesso/(float)m_total_execucoes);

	return 0;
}

/* Implementação das funções */

void  ObtemParametros(int argc, char * argv[]){

	if(argc < 1){
		printf("Usage: <m_iteracoes> <m_formigas> <m_rho> <m_tau_min> <m_tau_max> <m_seed> <m_total_execucoes>\n");
		exit(0);
	}

	if(argc >=7){

		m_iteracoes = atoi(argv[1]);
		m_n_ants = atoi(argv[2]);
		m_rho = atof(argv[3]);
		m_tal_min = atof(argv[4]);
		m_tal_max = atof(argv[5]);
		m_seed = atoi(argv[6]);
		m_total_execucoes = atoi(argv[7]);
		//srand(m_seed);
		//printf("Iterações:%d, formigas:%d, rho:%f, tau_min:%f, tau_max:%f, m_seed:%d\n",
		//m_iteracoes, m_n_ants, m_rho, m_tal_min,  m_tal_max, m_seed);
		//printf("Seed:%d\n", m_seed);
	}
	else{
		m_iteracoes = 1000;
		m_n_ants  = 50;
		m_rho     = 0.5f;
		m_tal_min = 0.01;
		m_tal_max = 1.00f;
		srand(time(NULL));
	}

	m_tal_ins = m_tal_max;

    formigas = (type_ant *) malloc(m_n_ants * sizeof(type_ant));
}

void InicializaFeromonios(){

	if(feromonios == NULL){
		feromonios = CriaTabelaFeromonios(m_elementos);
	}

	int i,j;

	for(i=0;i<2*MAX_ELEMENTOS;i++){
		for(j=0;j<MAX_ELEMENTOS;j++){
			feromonios[i][j] = 0;
		}
	}

	for(i=0;i<2*m_elementos;i++){
		for(j=0;j<m_elementos;j++){
			feromonios[i][j] = m_tal_ins;
		}
	}
}

int compara_floats(const void* a, const void* b){

	float* p1 = (float*)a;
	float* p2 = (float*)b;
    return p1 > p2;
}

void ordena(float * p){
	qsort(p, m_elementos, sizeof(float), (int(*)(const void*, const void*))compara_floats);
}

void ConstroiCaminho(int id){

	int i,j,selecionado;

	type_ant * formiga = &formigas[id];

	formiga->programa.elemento.tipo = START;
	formiga->programa.indice = 0;
	formiga->programa.elemento.aridade = 1;
	formiga->programa.filhos[0] = NULL;
	formiga->programa.filhos[1] = NULL;

	//As formigas sempre partem do nó START
	formiga->caminho[0] = 0;
	formiga->tamanho = 0;

	for(i=0;i<m_elementos;i++){
		visitados[i] = 0;
	}

	//Fila para construir a árvore do programa em largura
	queue <type_itemp*> fila;
	fila.push(&formiga->programa);

	while(!fila.empty()){

		if(formiga->tamanho >= MAX_ELEMENTOS){
			printf("Erro no mapeamento do programa. Tamanho excedeu o máximo de elementos \n");
			exit(0);
		}

		type_itemp* atual = fila.front();
		fila.pop();

		for(j=0; j < atual->elemento.aridade; j++){

			int maior=1;
			int possibilidades=0;
			selecionado=1;

			float total=0.0f;

			for(i=1; i<m_elementos; i++){
				probabilidades[i] = CalculaProbabilidade((atual->indice*2)+j, i);
				total += probabilidades[i];

				if(probabilidades[i])
					possibilidades++;

				if(probabilidades[i] > probabilidades[maior])
					maior = i;
			}

			/*if(total<=0.9f){
				//printf("total das probabilidades:%f\n",total);
				//exit(0);
			}*/

			//Seleção via roleta
		    float r = rand01();

			float tot = 0;

			for (i = 1; i < m_elementos; i++) {
				tot += probabilidades[i];
				if (tot >= r){
					selecionado=i;
					break;
				}
			}
			//printf("selecionado:%s\n", ObtemNomeElemento(elementos[selecionado].tipo, elementos[selecionado].valor));

			//TODO: adicionar métodos para criar novos nós

			type_itemp *novo = (type_itemp*)malloc(sizeof(type_itemp));
			novo->elemento = elementos[selecionado];
			novo->indice = selecionado;
			novo->filhos[0]  = NULL;
			novo->filhos[1]  = NULL;

			atual->filhos[j] = novo;

			fila.push(novo);
			formiga->caminho[formiga->tamanho] = (atual->indice*2)+j;
			formiga->tamanho+=1;
			formiga->caminho[formiga->tamanho] = selecionado;
			formiga->tamanho+=1;

			if(novo->elemento.tipo == FUNCAO){
				visitados[selecionado] = 1;
			}

			if(formiga->caminho[formiga->tamanho-1] <0 || formiga->caminho[formiga->tamanho-2]<0){
				printf("\n");
			}

			//printf("Nó selecionado: %s\n", ObtemNomeElemento(novo->elemento.tipo, (int)novo->elemento.valor));
		}
	}

	/*printf("Programa %d: ", id);
	ImprimePrograma(&formiga->programa);
	printf("\n");*/
}

float Avalia(type_itemp *p, float x, float y){

	float op1,op2;

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
				op1 = Avalia(p->filhos[0], x, y);
				op2 = Avalia(p->filhos[1], x, y);
				if(op2 != 0)
					return op1/op2;
				else
					return 1;
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

	int i, n = sizeof(y)/(sizeof(float));
	float erro = 0;

	for(i=0;i<n;i++){
		erro += pow(y[i] - Avalia(programa, x[i], y[i]), 2);
	}

	formiga->fitness = 1.0f/(1.0f+(erro/n));

	if(isnan(formiga->fitness) || isinf(formiga->fitness))
		formiga->fitness = 0;

	//printf("Fitness: %f\n\n", formiga->fitness);
}

float CalculaProbabilidade(int i, int j){

	if(visitados[j]){
		return 0.0f;
	}

	int k;
	float total=0;

	for(k=1; k < m_elementos; k++){
		if(!visitados[k]){
			total+=feromonios[i][k];
		}
	}

	if(total == 0){
		total = 1;
	}

	return feromonios[i][j]/(total);
}

void AtualizaFeromonios(){

	int i,j,h;

	//Evaporação
	for(i=0 ; i< 2*m_elementos; i++){
		for(j=0; j< m_elementos; j++){
			feromonios[i][j] = (1-m_rho)*feromonios[i][j];
		}
	}

	type_ant *f = &m_best_it;

	//Reforço de feromônios somente no caminho da melhor solução da iteração
	for (i = 0 ; i < f->tamanho-1; i+=2 ) {

		j = f->caminho[i];
		h = f->caminho[i+1];

		feromonios[j][h] += f->fitness;

		if(feromonios[j][h] > m_tal_max)
			feromonios[j][h] = m_tal_max;
	}
}

void InicializaNos(){

	int i;

	m_elementos = 0;

	type_no start;
	start.tipo = START;
	elementos[m_elementos++] = start;

	for(i=0; i < NUM_TERMINAIS;i++){

		//if(rand()%2==0){
			type_no novo;
			novo.tipo    = TERMINAL;
			novo.valor   = i;
			novo.aridade = 0;
			elementos[m_elementos++] = novo;
		//}
	}

	int k=0;

	while(k<1){

		for(i=0; i<NUM_FUNCOES;i++){
			//if(rand()%2==0){
				type_no novo;
				novo.tipo   = FUNCAO;
				novo.valor  = i;
				novo.aridade = aridade_nao_terminais[i];
				elementos[m_elementos++] = novo;
			//}
		}
		k++;
	}
}

void InsereNos(int iteracao){

	int i, possuiTerminais;
	float soma = 0.0f;
	float prob, aleatorio;

	for(i=0 ; i <m_n_ants ; i++ ){
		soma+=(formigas[i].tamanho)/2;
	}

	soma = soma/(float)m_n_ants;
	prob = (float)soma/((float)m_elementos-1.0);
	aleatorio = rand01();

	//Deve inserir outro elemento?
	if(aleatorio <= prob){

		possuiTerminais = 0;

		for(i=0; i < m_elementos; i++){
			if(elementos[i].tipo == TERMINAL){
				possuiTerminais = 1;
				break;
			}
		}

		type_no novo;

		int tipo = (int)randr(0, NUM_TERMINAIS + NUM_FUNCOES - 1);

		if(!possuiTerminais || tipo < NUM_TERMINAIS){

			//Inserir terminal
			int elemento = tipo;
			novo.tipo   = TERMINAL;
			novo.valor  = elemento;
			novo.aridade = 0;
		}
		else{
			//Inserir função
			int elemento = tipo - NUM_TERMINAIS;
			novo.tipo    = FUNCAO;
			novo.valor   = elemento;
			novo.aridade = aridade_nao_terminais[elemento];
		}

		elementos[m_elementos] = novo;

		/*
		 Depósito inicial de feromônios
	    */

		/*
		 * Alteração: ao invés de atribuir o feromônio máximo, adiciona o maior feromonio
		 * da linha
		 */


		if(novo.tipo == TERMINAL){
			feromonios[0][m_elementos] = 0;
			i=1;
		}
		else{
			i=0;
		}

		for(;i<=2*m_elementos; i++){

//#define V_2

#ifdef V_1
			feromonios[i][m_elementos] = m_tal_ins;
#endif

#ifdef V_2
			feromonios[i][m_elementos] = -1;
			int l;
			for(l=0;l<m_elementos;l++){
				if(feromonios[i][l] > feromonios[i][m_elementos]){
					feromonios[i][m_elementos] = feromonios[i][l];
				}
			}
#endif

#ifdef V_3

			float linha[MAX_ELEMENTOS];
			int a;
			for(a=0; a < m_elementos; a++){
				linha[a] = feromonios[i][a];
				//printf("%f,", linha[a]);
			}

			//printf("\nordenado:\n");
			for(a=0; a < m_elementos; a++){
			//	printf("%f,", linha[a]);
			}

			float mediana = linha[m_elementos/2];

			//printf("\n selecionado: %f, M=%d\n", mediana, m_elementos);


			feromonios[i][m_elementos] = mediana;
#endif

		}

		if(novo.tipo == FUNCAO){

			for(i=0;i<= m_elementos; i++){
				feromonios[2*m_elementos][i] = m_tal_ins;
				feromonios[1+(2*m_elementos)][i] = m_tal_ins;
			}
		}

		m_elementos++;
	}
	else{
		//printf("iteracao %d - nos nao inseridos. prob=%f, r=%f \n", iteracao, prob,aleatorio);
	}
}

void DeletaNos(){

	int i,k=1, deletar;

	while(k<m_elementos){

		deletar=1;

		for(i=0;i< 2*m_elementos; i++){
			if(feromonios[i][k] >= m_tal_min){
				deletar = 0;
				break;
			}
		}

		if(deletar){
			//printf("\nNÓ DELETADO = %d\n", k);
			//printf("NÓ DELETADO\n\n");
			DeletaNo(k);
			//ImprimeTabelaFeromonios();
		}
		else{
			k++;
		}
	}
}

void  DeletaNo(int k){

	int i,j;

	//Exclui da lista de nós
	for(i=k; i < m_elementos-1;i++){
		elementos[i] = elementos[i+1];

		/*for(j=0;j <m_elementos;j++)
			printf("'%s'\t", ObtemNomeElemento(elementos[j].tipo, elementos[j].valor));
		printf("\n");*/
	}

	//Exclui a coluna da tabela de feromônios (deslocando as colunas para esquerda, de k+1 até m_elementos-1)
	for(i=0; i < 2*m_elementos; i++){
		for(j=k; j < m_elementos-1; j++){
			feromonios[i][j] = feromonios[i][j+1];
		}
	}

	//Exclui a linha da tabela de feromônios (deslocando as linhas para cima, de 2*k até 2*m_elementos)
	for(i=2*k; i < (2*m_elementos)-4; i+=2){
		for(j=k; j< m_elementos-1;j++){
			feromonios[i][j] = feromonios[i+2][j];
			feromonios[i+1][j] = feromonios[i+3][j];
		}
	}

	m_elementos--;

	//printf("Nó deletado. A lista agora contém %d elementos.\n", m_elementos);
}

char * ObtemNomeElemento(tp_no tipo, int valor){

	if(tipo == START){
		return "S";
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

void ImprimeTabelaFeromonios(){
	int i,j;

	printf("Tamanho da tabela de feromonios: %d x %d\n", m_elementos, m_elementos*2);

	for(j=0;j <m_elementos;j++)
		printf("'%s'\t", ObtemNomeElemento(elementos[j].tipo, elementos[j].valor));
	printf("\n");

	for(i=0;i <2*m_elementos;i++){
		for(j=0;j <m_elementos;j++)
			printf("%.8f\t", feromonios[i][j]);
		printf("\n");
	}
}
