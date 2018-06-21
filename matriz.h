#include <stdio.h>
#include <stdlib.h>

//Função para alocação dinâmica da matriz dado os parametros de seu tamanho
double **alocaMatriz (int M, int N){
    double **k; /*ponteiro para a matriz*/
    int i;

    k = (double**)calloc(M, sizeof(double*));//aloca as linhas da matriz
    for(i = 0; i < M; i++){ //aloca as colunas da matriz
        k[i] = (double*) calloc(N, sizeof(double));
    }
    printf("memoria da matriz foi alocada\n");
    return (k); //retorna o ponteiro para a matriz
}

//funcao para desalocar a matriz apos finalizar
double **liberaMatriz (int M, int N, double **mat)
{
  int  i;
  for (i=0; i<M; i++) free (mat[i]); //libera as linhas da matriz
  free (mat);      //libera a matriz
  return (NULL); //retorna um ponteiro nulo
}

//Puxa do arquivo txt para a Matriz, os valores que serão alocados
void preencheMatriz (FILE *arq, double **mat, int lin, int col){

    int linha;//Variável de linhas
    int coluna;//Variável de colunas

    for(linha = 0; linha < lin; linha++){
        for(coluna = 0; coluna < col; coluna++){
                fscanf(arq, "%f", &mat[linha][coluna]);
        }
    }
}