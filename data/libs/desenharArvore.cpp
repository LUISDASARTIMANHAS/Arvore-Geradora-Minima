#include "fileSys.cpp"
#include "desenharArvore.h"
#include 

void montarArvore(){
    int tamanho,i,j,star;

    printf("Tamanho: ");
    tamanho = input();

    // define o tamanho da arvore
    for (i = 1; i <= tamanho; i++){
            // define o espacamento
        for (j = 1; j <= tamanho-i; j++){
            printf(" ");
        }
        // desenha a arvore 
        for ( star = 1; star <= i*2-1; star++){
            printf("*");
        }
        printf("\n");
    }
    
    printf("\n");
    }

void montarArvoreString(int tamanho,char vetor[]){
    int i,j,star;
    // define o tamanho da arvore
    for (i = 1; i <= tamanho; i++){
            // define o espacamento
        for (j = 1; j <= tamanho-i; j++){
            printf(" ");
        }
        // desenha a arvore 
        for ( star = 1; star <= i*2-1; star++){
            printf("%s",vetor[i]);
        }
        printf("\n");
    }
    
    printf("\n");
}

int main(){
    printf("\n");
    printf("cidade: xpto");
    printf("\n\t cidade: xpt2");
    return 0;
}