#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matriz.h"
#include <stdint.h>

int M, N, T; /*em ordem Linhas, Colunas, Threads*/
float V;    /*Valor a ser buscado*/
float **matriz, **auxiliar; /*Matriz a ser alocada dinamicamente*/
FILE *dados;
char nome[100];


void* busca(void *t){         /*metodo de busca do valor na matriz*/
	int id = *((int *) t);
	printf("entrando no metodo de busca do %d\n", id);
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
	pthread_exit(0);
}

void dontthreadonme(int num_threads){  /*método para crianção de threads separadas!*/
	pthread_t *thread = NULL;
        int *argumentos = NULL;

	int c = T; /* [Andre] Não entendi o porque desse c = T !! */
	int rc;

        /*[Andre] Alocacao dinamica do vetor com os identificadores dos threads*/
        thread = malloc(sizeof(pthread_t) * num_threads);
        if (thread == NULL)
        {
            perror("Probemas na criação do vetor de identificadores de threads\n");
            exit(1);
        }
        
        /*[Andre] Alocacao dinamica do vetor com os argumentos dos threads*/
        argumentos = malloc(sizeof(int) * num_threads);
        if (argumentos == NULL)
        {
            perror("Probemas na criação do vetor de argumentos de threads\n");
            exit(1);
        }

	for(c = 0; c < num_threads; c++){
		printf("criando thread %d\n",c);
                argumentos[c] = c; 

		rc = pthread_create(&thread[c], NULL, busca,(void*) &argumentos[c]);
		if (rc){
			printf("ERROR: return code from pthread_create is %d\n", rc);
			exit(-1);
		} 

	}

        /*[Andre] Adicionado para sincronizar as threads.*/
        for (c=0; c < num_threads; c++) 
            pthread_join(thread[c], NULL);

        /*[Andre] Liberando as alocacoes dinamicas*/
        free(thread);
        free(argumentos);
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


	preencheMatriz(dados,matriz, M, N);
	dontthreadonme(T);
	int x = 0;
	int y = 0;
	while(x < M){
		while(y< N){
			if (auxiliar[x][y] == 1){
				printf("Valor encontrado na posicao %d, %d", x, y);
			}
			y = y + 1;
		}
		x = x + 1;
	}
	matriz = liberaMatriz(M, N, matriz);
	auxiliar = liberaMatriz(M, N, auxiliar);
	fclose(dados);	
	pthread_exit(NULL);
	return 0;
}

