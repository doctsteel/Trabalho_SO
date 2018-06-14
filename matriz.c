#include <stdio.h>
#include <stdlib.h>
/*variaveis globais para poderem ser acessadas pelas threads*/
/*funcao para alocacao dinamica da matriz dado os parametros de seu tamanho*/
float **alocaMatriz (int M, int N){
    float **k; /*ponteiro para a matriz*/
    int i;

    k = (float**)calloc(M, sizeof(float*));/*aloca as linhas da matriz*/
    for(i = 0; i < M; i++){ /*aloca as colunas da matriz*/
        k[i] = (float*) calloc(N, sizeof(float));
    }
    printf("memoria da matriz foi alocada\n");
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

void preencheMatriz (FILE *arq, float **mat, int lin, int col){
    int linha, coluna;

    for(linha = 0; linha < lin; linha++){
        for(coluna = 0; coluna < col; coluna++){
                fscanf(arq, "%f", &mat[linha][coluna]);
        }
    }
}

