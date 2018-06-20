#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matriz.h"
#include <stdint.h>

int M, N, T; /*em ordem Linhas, Colunas, Threads*/
float V;    /*Valor a ser buscado*/
float **matriz, **auxiliar; /*Matriz a ser alocada dinamicamente*/
FILE *dados;
char nome[21];
FILE *resultado;
typedef struct{
	int lin;
	int col;
}pos;

int contador=0;

void *busca(void *t){         /*metodo de busca do valor na matriz*/
	int id = *((int *) t);
	int nomethread = id;
	while(id < M){
            int coluna = 0;
		while (coluna < N){
			if (matriz[id][coluna] == V){
				fprintf(resultado,"%d %d \n", id, coluna);
				contador++;
			}
			coluna = coluna + 1;
		}
		id = id + T;
	}
	pthread_exit(0);
}

void dontthreadonme(int num_threads){  /*método para criação de threads separadas!*/
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

void printsorted(FILE *arq){
	int i, x_cont, y_cont, aux_lin, aux_col;
	if (contador == 0){
		printf("Valor nao encontrado\n");
	}
	else{
		pos posicao[contador];
		for(i=0;i<contador; i++){
			fscanf(arq, "%d %d", &posicao[i].lin, &posicao[i].col);
			printf("Posicao atual e contador: %d %d %d\n",posicao[i].lin,posicao[i].col,contador);
		}
		for(x_cont=0;x_cont<contador-1; x_cont++){
			if(posicao[x_cont+1].lin > posicao[x_cont].lin){
					printf("%d",x_cont);
					aux_lin = posicao[x_cont].lin;
					aux_col = posicao[x_cont].col;
					posicao[x_cont].lin = posicao[x_cont+1].lin;
					posicao[x_cont].col = posicao[x_cont+1].col;
					posicao[x_cont+1].lin = aux_lin;
					posicao[x_cont].col = aux_col;

			}
			
		}
		for(x_cont=0;x_cont<contador-1; x_cont++){
			if(posicao[x_cont].lin == posicao[x_cont+1].lin){
				if(posicao[x_cont].col < posicao[x_cont+1].col){
					aux_col = posicao[x_cont].col;
					posicao[x_cont].col = posicao[x_cont+1].col;
					posicao[x_cont+1].col = aux_col;
				}
			x_cont++;		

			}
		}
		y_cont = 0;
		printf("Encontrado nas seguintes coordenadas: \n");
		while(y_cont< contador){
			printf("Linha:%d    Coluna:%d \n",posicao[y_cont].lin, posicao[y_cont].col);
			y_cont++;
		}
	}
}


int main(){
	printf("insira na ordem: linhas, colunas, threads e valor a ser encontrado\n");
	scanf("%d %d %d %f" , &M,&N,&T,&V);
	printf("agora, insira o nome do arquivo NA MESMA PASTA desse programa\n");
	scanf("%s", nome); /*recebe o nome do arquivo*/
	matriz = alocaMatriz(M, N);
	auxiliar = alocaMatriz(M, N);

	dados = fopen(nome,"r"); /*abre o arquivo para leitura dos dados*/
	if(dados == NULL){       /*caso o arquivo nao exista encerra e notifica*/
		printf("Erro, nao foi possivel abrir o arquivo\n");
		return -1;
	}
	resultado = fopen("output.txt","w+"); /*abre o arquivo para leitura dos dados*/


	preencheMatriz(dados,matriz, M, N);
	fclose(dados);
	dontthreadonme(T);
	int x = 0;
	int y = 0;

	printsorted(resultado);

	fclose(resultado);
	matriz = liberaMatriz(M, N, matriz);
	auxiliar = liberaMatriz(M, N, auxiliar);	
	pthread_exit(NULL);
	return 0;
}
