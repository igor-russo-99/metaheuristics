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
#define NUM_FUNCOES 4

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
	char caminho[2*MAX_ELEMENTOS];
	type_itemp programa;
	int tamanho;
	float fitness;
}type_ant;

/*p5(x)*/
/*
float x[] = {
		-0.980000,
		-0.960000,
		-0.940000,
		-0.920000,
		-0.900000,
		-0.880000,
		-0.860000,
		-0.840000,
		-0.820000,
		-0.800000,
		-0.780000,
		-0.760000,
		-0.740000,
		-0.720000,
		-0.700000,
		-0.680000,
		-0.660000,
		-0.640000,
		-0.620000,
		-0.600000,
		-0.580000,
		-0.560000,
		-0.540000,
		-0.520000,
		-0.500000,
		-0.480000,
		-0.460000,
		-0.440000,
		-0.420000,
		-0.400000,
		-0.380000,
		-0.360000,
		-0.340000,
		-0.320000,
		-0.300000,
		-0.280000,
		-0.260000,
		-0.240000,
		-0.220000,
		-0.200000,
		-0.180000,
		-0.160000,
		-0.140000,
		-0.120000,
		-0.100000,
		-0.080000,
		-0.060000,
		-0.040000,
		-0.020000,
		-0.000000,
		0.020000,
		0.040000,
		0.060000,
		0.080000,
		0.100000,
		0.120000,
		0.140000,
		0.160000,
		0.180000,
		0.200000,
		0.220000,
		0.240000,
		0.260000,
		0.280000,
		0.300000,
		0.320000,
		0.340000,
		0.360000,
		0.380000,
		0.400000,
		0.420000,
		0.440000,
		0.460000,
		0.480000,
		0.500000,
		0.520000,
		0.540000,
		0.560000,
		0.580000,
		0.600000,
		0.620000,
		0.640000,
		0.660000,
		0.680000,
		0.700000,
		0.719999,
		0.739999,
		0.759999,
		0.779999,
		0.799999,
		0.819999,
		0.839999,
		0.859999,
		0.879999,
		0.899999,
		0.919999,
		0.939999,
		0.959999,
		0.979999,
		0.999999

};

float y[] = {
		-0.942345,
		-0.889162,
		-0.840139,
		-0.794977,
		-0.753390,
		-0.715109,
		-0.679875,
		-0.647445,
		-0.617586,
		-0.590080,
		-0.564719,
		-0.541307,
		-0.519659,
		-0.499601,
		-0.480970,
		-0.463612,
		-0.447382,
		-0.432146,
		-0.417778,
		-0.404160,
		-0.391183,
		-0.378744,
		-0.366750,
		-0.355112,
		-0.343750,
		-0.332588,
		-0.321558,
		-0.310595,
		-0.299640,
		-0.288640,
		-0.277544,
		-0.266307,
		-0.254884,
		-0.243238,
		-0.231330,
		-0.219127,
		-0.206595,
		-0.193703,
		-0.180421,
		-0.166720,
		-0.152571,
		-0.137946,
		-0.122814,
		-0.107146,
		-0.090910,
		-0.074075,
		-0.056604,
		-0.038462,
		-0.019608,
		-0.000000,
		0.020408,
		0.041666,
		0.063829,
		0.086956,
		0.111110,
		0.136360,
		0.162781,
		0.190456,
		0.219470,
		0.249919,
		0.281905,
		0.315537,
		0.350933,
		0.388219,
		0.427529,
		0.469008,
		0.512810,
		0.559098,
		0.608046,
		0.659839,
		0.714673,
		0.772756,
		0.834306,
		0.899556,
		0.968749,
		1.042143,
		1.120010,
		1.202633,
		1.290311,
		1.383358,
		1.482103,
		1.586888,
		1.698074,
		1.816036,
		1.941167,
		2.073875,
		2.214586,
		2.363746,
		2.521815,
		2.689275,
		2.866624,
		3.054381,
		3.253085,
		3.463292,
		3.685582,
		3.920554,
		4.168828,
		4.431046,
		4.707870,
		4.999989
};
*/

/* Variáveis */
/* Y = p4(x)*/
float x[] = {

		-0.900000,
		-0.800000,
		-0.700000,
		-0.600000,
		-0.500000,
		-0.400000,
		-0.300000,
		-0.200000,
		-0.100000,
		0.000000,
		0.100000,
		0.200000,
		0.300000,
		0.400000,
		0.500000,
		0.600000,
		0.700000,
		0.800000,
		0.900000,
		1.000000


};

float y[] = {

		-0.162900,
		-0.262400,
		-0.312900,
		-0.326400,
		-0.312500,
		-0.278400,
		-0.228900,
		-0.166400,
		-0.090900,
		0.000000,
		0.111100,
		0.249600,
		0.425100,
		0.649600,
		0.937500,
		1.305600,
		1.773101,
		2.361601,
		3.095101,
		4.000001
};


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
void ImprimeTabelaFeromonios();
/**/

int main(int argc, char * argv[]) {

	/* Inicialização */
	ObtemParametros(argc, argv);
	InicializaNos();
	InicializaFeromonios();
	ImprimeTabelaFeromonios();

	int count = 1, i, totalNos=0;
	float best_fitness=-1;

	m_best_so_far.fitness = -1;

	srand(time(NULL));

	/* Critério de parada */
	while(count <= m_iteracoes){

		printf("===================================\n");
		printf("Iteração %d\n", count);
		printf("Tamanho da tabela de feromonios: %d\n", m_elementos);
		printf("Média: %f\n", (float)totalNos/(float)count);
		totalNos+=m_elementos;

		m_best_it = formigas[0];

		/* Constrói o caminho para cada formiga */
		for(i=0; i< m_n_ants; i++){
			ConstroiCaminho(i);
			AvaliaPrograma(i);

			if(formigas[i].fitness > m_best_it.fitness){
				m_best_it = formigas[i];
			}

			/*printf("Programa: %d\n", i);
			ImprimePrograma(&formigas[i].programa);
			printf("\nFitness: %f\n", formigas[i].fitness);
			printf("\n");*/

		}

		printf("Melhor programa da iteração: \n");
		ImprimePrograma(&m_best_it.programa);
		printf("\nFitness: %f\n", m_best_it.fitness);
		printf("\n");

		if(m_best_it.fitness > m_best_so_far.fitness)
			m_best_so_far = m_best_it;

		printf("Melhor programa até então: \n");
		ImprimePrograma(&m_best_so_far.programa);
		printf("\nFitness: %f\n", m_best_so_far.fitness);
		printf("\n");


		if (m_best_so_far.fitness>=0.9) {
			printf("acnou\n");
		}

		printf("Antes:\n");

		ImprimeTabelaFeromonios();

		printf("\n\nDepois:\n\n");

		AtualizaFeromonios();

		ImprimeTabelaFeromonios();

		DeletaNos();

		InsereNos(count);

		ImprimeTabelaFeromonios();

		count++;
	}

	return 0;
}

/* Implementação das funções */

void  ObtemParametros(int argc, char * argv[]){

	m_iteracoes = 1000;
	m_n_ants  = 50;
	m_rho     = 0.5f;
	m_tal_min = 0.01;
	m_tal_max = 1.00f;

	//Nota: interessante para incentivar escolhas de novos caminhos
	m_tal_ins = m_tal_max;

    formigas = (type_ant *) malloc(m_n_ants * sizeof(type_ant));
}

void InicializaFeromonios(){

	feromonios = CriaTabelaFeromonios(m_elementos);

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
			selecionado=1;

			//TODO: rever escolha baseada na probabilidade
			float total=0.0f;

			for(i=1 ; i<m_elementos ; i++){
				probabilidades[i] = CalculaProbabilidade((atual->indice*2)+j, i);
				total += probabilidades[i];

				if(probabilidades[i] > probabilidades[maior])
					maior = i;
			}

			//printf("total das probabilidades:%f\n",total);

			/*if(total==0.0f){
				//printf("total das probabilidades:%f\n",total);
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

			if(tot < r && i == m_elementos){
				printf("Isto não deveria ocorrer. Linha %d\n", __LINE__);
				printf("Soma das probabilidades: %f\n", total);
				ImprimeTabelaFeromonios();
				exit(0);
			}

			//TODO: adicionar métodos para criar novos nós

			type_itemp *novo = (type_itemp*)malloc(sizeof(type_itemp));
			novo->elemento = elementos[selecionado];
			novo->indice = selecionado;
			novo->filhos[0]  = NULL;
			novo->filhos[1]  = NULL;

			atual->filhos[j] = novo;

			fila.push(novo);
			formiga->caminho[formiga->tamanho++] = (atual->indice*2)+j;
			formiga->caminho[formiga->tamanho++] = selecionado;

			//printf("tour->%d\n",formiga->caminho[formiga->tamanho-1] );

			if(novo->elemento.tipo == FUNCAO){
				visitados[selecionado] = 1;
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

	int i,j, k, n = sizeof(y)/(sizeof(float));
	float erro = 0;

	for(i=0;i<n;i++){
		erro += fabs(y[i] - Avalia(programa, x[i], y[i]));
	}

	formiga->fitness = 1.0f/(1.0f+erro);

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

	//printf("elemento=%d, total=%f, prob=%f\n",j, total, feromonios[i][j]/total);

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

	printf("Reforço:\n");


	//Reforço de feromônios somente no caminho da melhor solução da iteração
	for (i = 0 ; i < m_best_it.tamanho; i+=2 ) {

		j = m_best_it.caminho[i];
		h = m_best_it.caminho[i+1];
		feromonios[j][h] += m_best_it.fitness+1;

		printf("(%d)->(%d), ", j,h);

		if(feromonios[j][h] > m_tal_max)
			feromonios[j][h] = m_tal_max;
	}

	printf("\n");
}

void InicializaNos(){

	int i;
	float aleatorio;

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

	int i;
	float soma = 0.0f;
	float prob, aleatorio;

	/* Dúvida: no paper está "número de nós gerados por cada formiga" */
	for(i=0 ; i<m_n_ants ; i++ ){
		soma+=(formigas[i].tamanho)/2;
	}

	soma = soma/(float)m_n_ants;
	prob = (float)soma/((float)m_elementos);
	aleatorio = rand01();

	//Deve inserir outro elemento?
	if(aleatorio <= prob){

		int possuiTerminais = 0;

		for(i=0; i< m_elementos; i++){
			if(elementos[i].tipo == TERMINAL){
				possuiTerminais = 1;
				break;
			}
		}

		type_no novo;

		/*
		Se não possuir nenhum terminal, obrigatoriamente deve ser inserido um nó deste tipo.
		Caso contrário o mapeamento entrará em loop infinito.
		 */
		if(!possuiTerminais || rand()%2 == 0){
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

		elementos[m_elementos] = novo;

		/*
		 Depósito inicial de feromônios
	    */
		for(i=0;i<=2*m_elementos; i++){
			feromonios[i][m_elementos] = m_tal_ins;
		}
		for(i=0;i<= m_elementos; i++){
			feromonios[2*m_elementos][i] = m_tal_ins;
			feromonios[1+(2*m_elementos)][i] = m_tal_ins;
		}

		m_elementos++;
	}
	else{
		//printf("iteracao %d - nos nao inseridos. prob=%f, r=%f \n", iteracao, prob,aleatorio);
	}
}

void DeletaNos(){

	int i,k=1, deletar;

/*	queue<int>delecoes;

	for(k=1; k< m_elementos; k++){

		deletar=1;

		for(i=0;i<= 2*m_elementos; i++){
			if(feromonios[i][k] >= m_tal_min){
				deletar = 0;
			}
			else{
			}

		}

		if(deletar){
			printf("%d ", k);
			delecoes.push(k);
		}
	}

	printf("\nDeveriam ser deltados: %d\n", delecoes.size());*/

	k=1;

	while(k<m_elementos){

		deletar=1;

		for(i=0;i< 2*m_elementos; i++){
			if(feromonios[i][k] >= m_tal_min){
				deletar = 0;
				break;
			}
		}

		if(deletar){
			printf("\nNÓ DELETADO = %d\n", k);
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

	/*float **t2 = CriaTabelaFeromonios(m_elementos);

	//2
	for(i=0; i< 2*k ; i++){
		for(j=0; j < k; j++){
			t2[i][j] = feromonios[i][j];
		}
	}

	//3
	for(i=(2*k)+2; i< 2*m_elementos ; i++){
		for(j=0; j < k; j++){
			t2[i][j] = feromonios[i][j];
		}
	}

	//1
	for(i=0; i< 2*k; i++){
		for(j=k+1; j < m_elementos; j++){
			t2[i][j] = feromonios[i][j];
		}
	}

	//4
	for(i= (2*k)+2 ; i< 2*m_elementos; i++){
		for(j=k+1; j < m_elementos; j++){
			t2[i][j] = feromonios[i][j];
		}
	}
	free(feromonios);

	feromonios = t2;
	*/


	//Exclui da lista de nós
	for(i=k; i < m_elementos-1;i++){
		elementos[k] = elementos[k+1];
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
