#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "matriz.h"
#include <stdint.h>
#include <string.h>


int M, N, T; //Na ordem digitada: Número de LINHAS, COLUNAS, e THREADS
double V;    //VALOR que deve ser encontrado pelas THREADS
double **matriz; //MATRIZ que será alocada dinamicamente
FILE *dados; //Variável que chamará o arquivo que contém os valores que vão ser inseridos na MATRIZ
char nome[21]; //Nome do arquivo
FILE *resultado; //Variável que chamará o arquivo que contém as posições encontradas pelas THREADS

//STRUCT que irá se transfomar num VETOR de STRUCT e irá receber as diversas linhas e colunas encontradas
typedef struct{ 
	int lin; //Linha encontrada
	int col; //Coluna encontrada
}pos; //Nome da STRUCT

int contador=0; //Contador que será incrementado quando for encontrado um valor pelas THREADS

//Método que as THREADS buscarão o valor na MATRIZ
void *busca(void *t){

	int id = *((int *) t); //ID da THREAD
	int nomethread = id; //Identificador da THREAD atual

	//Whiles que fazem a THREAD rodar pela MATRIZ e encontrar o valor correto
	while(id < M){
            int coluna = 0;
		while (coluna < N){
			if (matriz[id][coluna] == V){
				//Caso seja encontrado o valor, salva a posição atual da LINHA e COLUNA em um arquivo de output
				fprintf(resultado,"%d %d\n", id, coluna);
				//Explicado anteriormente: conta quantas vezes foi encontrado o valor necessário
				contador++;
			}
			coluna = coluna + 1;
		}
		id = id + T;
	}
	//Dá EXIT da THREAD
	pthread_exit(0);
}

//Método para criar as THREADS
void dontthreadonme(int num_threads){
	
	//Criando um ponteiro para um vetor de THREADS e o ARGUMENTO para a THREAD iniciar a busca  
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

//Função que ORGANIZA(SORT) os valores encontrados pelas THREADS e que foram colocados no arquvio "output.txt"
void printsorted(FILE *arq){

	int i;//Contador usado no FOR que guarda as informações do txt no vetor de STRUCT
	int x_cont;//Contador usado no FOR para organizar o vetor
	int y_cont; //COntador usado no FOR para organizar o vetor
	int aux_lin;//Salva a posição atual da linha nela
	int aux_col;//Salva a posição atual da coluna nela

	//Caso contador seja igual a 0, nenhum valor foi encontrado, logo não há o que organizar
	if (contador == 0){
		printf("Valor nao encontrado\n");
	}
	else{

		pos* posicao; //Declara uma variável da STRUCT POSICAO como um vetor na memória
		posicao = (pos *)malloc(sizeof(pos)* contador); //Separa memória de sistema para a variável "pos"
		pos aux_pos; //Declara uma variável da STRUCT POSICAO que será auxiliar
		rewind(arq);//Coloca o cursor na posição de arq(output.txt) inicial

		//Insere os valores de "output.txt" no VETOR da STRUCT
		for(i=0; i<contador; i++){
			fscanf(arq,"%d %d\n", &posicao[i].lin, &posicao[i].col);
		}

		//Roda da posição inicial do vetor até a última posição dele, comparando os valores
		for(i = 0; i < contador-1; i++){
			//Roda pelos valores para ordenar as linhas
			for(x_cont=0;x_cont<contador-1; x_cont++){
				if(posicao[x_cont].lin > posicao[x_cont+1].lin){
						printf("%d",x_cont);
						aux_pos = posicao[x_cont];
						posicao[x_cont] = posicao[x_cont+1];
						posicao[x_cont+1]= aux_pos;
				}
				
			}
			//Roda pelos valores para ordenar as colunas
			for(x_cont=0;x_cont<contador-1; x_cont++){
				if(posicao[x_cont].lin == posicao[x_cont+1].lin){
					if(posicao[x_cont].col > posicao[x_cont+1].col){
						aux_pos = posicao[x_cont];
						posicao[x_cont] = posicao[x_cont+1];
						posicao[x_cont+1] = aux_pos;
					}		

				}
			}
		}
		y_cont = 0;
		printf("Encontrado nas seguintes coordenadas: \n"); //Printa as coordenadas encontradas em ordem crescente
		while(y_cont< contador){
			printf("Linha:%d    Coluna:%d \n",posicao[y_cont].lin, posicao[y_cont].col);
			y_cont++;
		}
	}
}


int main(){
	printf("insira na ordem: linhas, colunas, threads e valor a ser encontrado\n");
	scanf("%d %d %d %f" , &M,&N,&T,&V); //Lê o número de linhas, colunas, threads e o valor desejado
	printf("agora, insira o nome do arquivo NA MESMA PASTA desse programa\n");
	scanf("%s", nome); //Lê o nome do arquivo
	matriz = alocaMatriz(M, N); //Chama a função que vai alocar a matriz dinamicamente com M e N
	
	dados = fopen(nome,"r"); //Abre o arquivo para leitura dos dados
	if(dados == NULL){       //Caso o arquivo nao exista encerra e notifica
		printf("Erro, nao foi possivel abrir o arquivo\n");
		return -1;
	}
	resultado = fopen("output.txt","w+"); //Abre o arquivo que as posições encontradas serão salvas para escrita, leitura e atualização

	preencheMatriz(dados,matriz, M, N);//Chama a função preencheMatriz para colocas os arquivos do txt para a matriz de acordo com o tamanho requirido
	fclose(dados);//Fecha o txt de valores de matriz
	dontthreadonme(T);//Chama a função de criação de threads

	printsorted(resultado);//Chama a função que vai organizar as posições encontradas

	fclose(resultado);//Fecha o txt de posições
	matriz = liberaMatriz(M, N, matriz);//Libera a matriz que foi alocada na memória
		
	pthread_exit(NULL);//Zera as threads e sai delas
	return 0;//Retorna 0 pro sistema
}
