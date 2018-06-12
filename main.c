#include <stdio.h>
#include <stdlib.h>

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

int main()
{
    int M, N, T; /*em ordem Linhas, Colunas, Threads*/
    float V;    /*Valor a ser buscado*/
    float **matriz; /*Matriz a ser alocada dinamicamente*/
    FILE *dados;
    char nome[100];

    scanf("%d %d %d %f" , &M,&N,&T,&V);
    scanf("%s", nome); /*recebe o nome do arquivo*/

    dados = fopen(nome,"r"); /*abre o arquivo para leitura dos dados*/
    if(dados == NULL){       /*caso o arquivo nao exista encerra e notifica*/
	    printf("Erro, nao foi possivel abrir o arquivo\n");
	    return -1;
    }
    matriz = alocaMatriz(M, N);
    //preencheMatriz(dados,matriz);
    matriz = liberaMatriz(M, N, matriz);
    fclose(dados);	

    return 0;
}
