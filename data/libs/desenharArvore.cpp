#include "fileSys.cpp"
#include "desenharArvore.h"

void montarArvore (){
    int tamanho,i,j;

    printf("Tamanho: ");
    tamanho = input();

    // define o tamanho da arvore
    for (i = 0; i <= tamanho; i++){
        
        for (j = 0; j <= tamanho; j++){
            // define o espacamento
            printf("%d",j);
        }
        printf("\n");
    }
    printf("\n");
    }
