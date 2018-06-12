#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int M, N, T; /*em ordem Linhas, Colunas, Threads*/
float V;    /*Valor a ser buscado*/
float **matriz, **auxiliar; /*Matriz a ser alocada dinamicamente*/
FILE *dados;
char nome[100];

/*funcao para alocacao dinamica da matriz dado os parametros de seu tamanho*/
float **alocaMatriz (int M, int N){
	float **k; /*ponteiro para a matriz*/
	int i;

	k = (float**)calloc(M, sizeof(float*));/*aloca as linhas da matriz*/
	for(i = 0; i < M; i++){ /*aloca as colunas da matriz*/
		k[i] = (float*) calloc(N, sizeof(float));
	}
	printf("foi alocada");
	return (k); /*retorna o ponteiro para a matriz*/
}

/*funcao para desalocar a matriz apos finalizar*/
float **liberaMatriz (int M, int N, float **mat)
{
	int  i;
	for (i=0; i<M; i++) free (mat[i]); /*libera as linhas da matriz*/
	free (mat);      /*libera a matriz*/
	return (NULL); /*retorna um ponteiro nulo*/
}

/*void preencheMatriz (FILE *arq, float mat[M][N]){
  int linha, coluna;
  float aux;

  for(linha = 0; linha < M; linha++){
  for(coluna = 0; coluna < N; coluna++){
  aux = fgetc(arq);
  mat[linha][coluna] = aux;
  }
  aux = fgetc(arq);
  }
  }*/
void *dontthreadonme(int num_threads){  /*método para crianção de threads separadas!*/
	pthread_t thread[num_threads];
	int t = V;
	int rc;
	for(t = 0; t < num_threads; t++){
		rc = pthread_create(&thread[t], NULL, threadBusca, (void *)t);


	}
}

void *threadBusca(void *t){  /*método que define quais linhas que define que cada thread vai ler*/
	int id = (int) t;
	busca(id);
	pthread_exit(0);
}

void *busca(int id){         /*metodo de busca do valor na matriz*/
	while(id < M){
		int coluna = 0;
		while (coluna < N){
			if (matriz[id][coluna] == V){
				auxiliar[id][coluna] = 1; /*digamos que colocamos um papel carbono por cima da matriz original e marcamos em outra matriz os valores encontrados*/
			}
			coluna = coluna + 1;
		}
		id = id + T;
	}
}



int main(){
	printf("insira na ordem: linhas, colunas, threads e valor a ser encontrado\n");
	scanf("%d %d %d %f" , &M,&N,&T,&V);
	scanf("%s", nome); /*recebe o nome do arquivo*/
	matriz = alocaMatriz(M, N);
	auxiliar = alocaMatriz(M, N);


	dados = fopen(nome,"r"); /*abre o arquivo para leitura dos dados*/
	if(dados == NULL){       /*caso o arquivo nao exista encerra e notifica*/
		printf("Erro, nao foi possivel abrir o arquivo\n");
		return -1;
	}

	//preencheMatriz(dados,matriz);
	dontthreadonme(T);
	//busca auxiliar por 1
	matriz = liberaMatriz(M, N, matriz);
	auxiliar = liberaMatriz(M, N, auxiliar);
	fclose(dados);	

	return 0;
}
