/*1. Inicialize um array de distâncias com valores infinitos e um array de predecessores vazio.
2. Defina a distância da cidade inicial como zero e marque-a como visitada.
3. Enquanto houver cidades não visitadas:
   - Encontre a cidade não visitada com a menor distância estimada.
   - Para cada vizinho não visitado dessa cidade:
     - Calcule a distância estimada até esse vizinho.
     - Se essa distância for menor que a distância atual armazenada para o vizinho, atualize-a.
     - Atualize o predecessor do vizinho.
   - Marque a cidade atual como visitada.
4. Retorne a distância calculada até a cidade de destino.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define INICIO "--------INICIO--------"
#define RESULTADO "-----------RESULTADO-----------"
#define CORTE "---------------------------------"
#define MAX 13
#define TAM 30
#define INFINITO 9999.99
struct Cidade {
    char nome[TAM];
    float distancia;
    int visitada;
    int predecessor;
};
typedef struct Cidade Cidade;

void inicializarCidades(Cidade cidades[], int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        strcpy(cidades[i].nome, "");
        cidades[i].distancia = INFINITO;
        cidades[i].visitada = 0;
        cidades[i].predecessor = -1;
    }
}

int menorDistanciaNaoVisitada(Cidade cidades[], int tamanho) {
    float minDistancia = INFINITO;
    int indiceMenor = -1;

    for (int i = 0; i < tamanho; i++) {
        if (cidades[i].visitada == 0 && cidades[i].distancia <= minDistancia) {
            minDistancia = cidades[i].distancia;
            indiceMenor = i;
        }
    }
    return indiceMenor;
}

void imprimirCaminho(Cidade cidades[], int destino, char listagemNomes[][MAX]) {
    if (cidades[destino].predecessor != -1) {
        imprimirCaminho(cidades, cidades[destino].predecessor, listagemNomes);
        printf(" -> %s", listagemNomes[destino]);
    } else {
        printf("%s", listagemNomes[destino]);
    }
}

void encontraIndices(int quanti,char listagemNomes[][MAX], char nomeCity1[MAX], char nomeCity2[MAX],int *cidade1, int *cidade2){
    int i;
    for (i = 0; i < quanti; i++) {
        if (strcmp(listagemNomes[i], nomeCity1) == 0) {
            *cidade1 = i;
        }
        if (strcmp(listagemNomes[i], nomeCity2) == 0) {
            *cidade2 = i;
        }
    }
}

float algoritmoDijkstra(float cidades[][TAM], int quantidadeCidades, char listagemNomes[][MAX], char nomeCity1[MAX], char nomeCity2[MAX]) {
    Cidade cidadesGrafo[quantidadeCidades];
    int cidade1 = -1, cidade2 = -1;
    int i, vizinho;
    int cidadeAtual;
    inicializarCidades(cidadesGrafo, quantidadeCidades);
    encontraIndices(quantidadeCidades,listagemNomes,nomeCity1,nomeCity2,&cidade1,&cidade2);
    if (cidade1 != -1 && cidade2 != -1) {
        cidadesGrafo[cidade1].distancia = 0;
        for (i = 0; i < quantidadeCidades - 1; i++) {
            cidadeAtual = menorDistanciaNaoVisitada(cidadesGrafo, quantidadeCidades);
            cidadesGrafo[cidadeAtual].visitada = 1;

            for (vizinho = 0; vizinho < quantidadeCidades; vizinho++) {
                if ((!cidadesGrafo[vizinho].visitada && cidades[cidadeAtual][vizinho]) &&
                    (cidadesGrafo[cidadeAtual].distancia != INFINITO) && (cidadesGrafo[cidadeAtual].distancia + cidades[cidadeAtual][vizinho] < cidadesGrafo[vizinho].distancia)) {
                    cidadesGrafo[vizinho].distancia = cidadesGrafo[cidadeAtual].distancia + cidades[cidadeAtual][vizinho];
                    cidadesGrafo[vizinho].predecessor = cidadeAtual;
                }
            }
        }
        printf("Caminho encontrado:\n");
        imprimirCaminho(cidadesGrafo, cidade2,listagemNomes);
        printf("\nDistância total: %.2f\n", cidadesGrafo[cidade2].distancia);
        return cidadesGrafo[cidade2].distancia;
    } else {
        return -1; // Se as cidades de origem e destino não forem encontradas
    }
}

FILE * abrirArquivo(char * nomeArq, char * modo) {
    FILE * arq;
    arq = fopen( nomeArq, modo );
    if ( arq == NULL) {
        printf("ERRO ao abrir o arquivo.");
        exit(-1);
    }
    return arq;
}

void carregarArquivo1(FILE *arquivo, int *guarda) {
    char lixo;
    fscanf(arquivo, "%d", guarda);
    fscanf(arquivo, " %c", &lixo);
}

void carregarArquivo2(FILE *arquivo, int quanti, char resposta[][MAX]) {
    int i;
    char lixo;
    int guarda;
    for (i = 0; i < quanti; i++){
        fscanf(arquivo, "%d", &guarda);
        fscanf(arquivo, " %c", &lixo); 
        fscanf(arquivo, "%12s", resposta[i]); 
    }
}

void carregarArquivo3(FILE *arquivo, int quanti, float resposta[][TAM]) {
    int i, j;
    char lixo;
    fscanf(arquivo, "%c", &lixo);
    for (i = 0; i < quanti; i++){
        for (j = 0; j < quanti; j++){
            fscanf(arquivo, "%f", &resposta[i][j]);
            fscanf(arquivo, "%c", &lixo); 
        }
        
    }
}


void listar(char vet[][MAX], int quanti){
    int i, j;
    for (i = 0; i < quanti; i++){
        for (j = 0; j < MAX-1; j++){
            printf("%c",vet[i][j]);
        }
        printf("\n");
    }
    
}

void listar2(float vet[][MAX], int quanti){
    int i, j;
    for (i = 0; i < quanti; i++){
        for (j = 0; j < quanti; j++){
            printf("%6.2f | ",vet[i][j]);
        }
        printf("\n");
    }
}

void encontraMaiorMenorVizinhos(float cidades[][TAM], int quanti, int *cidadeMaisVizinhos, int *cidadeMenosVizinhos, int *maiorVizinhos, int *menorVizinhos) {
    *maiorVizinhos = 0;
    *menorVizinhos = TAM * TAM;
    for (int i = 0; i < quanti; i++) {
        int contVizinhos = 0;
        for (int j = 0; j < quanti; j++) {
            if (cidades[i][j] > 0) {
                contVizinhos++;
            }
        }
        if (contVizinhos > *maiorVizinhos) {
            *maiorVizinhos = contVizinhos;
            *cidadeMaisVizinhos = i;
        }
        if (contVizinhos < *menorVizinhos && contVizinhos > 0) {
            *menorVizinhos = contVizinhos;
            *cidadeMenosVizinhos = i;
        }
    }
}

void defineVizinhos(float cidades[][TAM], int quanti, char listagemNomes[][MAX], int desci) {
    int cidadeMaisVizinhos, cidadeMenosVizinhos, maiorVizinhos, menorVizinhos;
    encontraMaiorMenorVizinhos(cidades, quanti, &cidadeMaisVizinhos, &cidadeMenosVizinhos, &maiorVizinhos, &menorVizinhos);

    if (desci == 0) {
        printf("A cidade com mais vizinhos é: %s\n", listagemNomes[cidadeMaisVizinhos]);
        printf("Quantidade de vizinhos: %d\n", maiorVizinhos);
    } else {
        printf("A cidade com menos vizinhos é: %s\n", listagemNomes[cidadeMenosVizinhos]);
        printf("Quantidade de vizinhos: %d\n", menorVizinhos);
    }
}

void encontraVizinhoDistancia(float cidades[][TAM], int quanti, int *cidadeMaisDistante, int *vizinhoMaisDistante, float *maiorDistancia, int *cidadeMaisProxima, int *vizinhoMaisProximo, float *menorDistancia) {
    *maiorDistancia = 0;
    *menorDistancia = 100.00 * 100.00;

    for (int i = 0; i < quanti - 1; i++) {
        for (int j = 0; j < quanti; j++) {
            if (cidades[i][j] > *maiorDistancia) {
                *maiorDistancia = cidades[i][j];
                *cidadeMaisDistante = i;
                *vizinhoMaisDistante = j;
            }
        }
    }

    for (int i = 0; i < quanti; i++) {
        for (int j = 0; j < quanti; j++) {
            if (cidades[i][j] < *menorDistancia && cidades[i][j] > 0) {
                *menorDistancia = cidades[i][j];
                *cidadeMaisProxima = i;
                *vizinhoMaisProximo = j;
            }
        }
    }
}

void vizinhoDistancia(float cidades[][TAM], int quanti, char listagemNomes[][MAX], int desci) {
    int cidadeMaisDistante, vizinhoMaisDistante, cidadeMaisProxima, vizinhoMaisProximo;
    float maiorDistancia, menorDistancia;
    encontraVizinhoDistancia(cidades, quanti, &cidadeMaisDistante, &vizinhoMaisDistante, &maiorDistancia, &cidadeMaisProxima, &vizinhoMaisProximo, &menorDistancia);

    if (desci == 0) {
        printf("A cidade com vizinho mais distante é: %s\n", listagemNomes[cidadeMaisDistante]);
        printf("Vizinho: %s\n", listagemNomes[vizinhoMaisDistante]);
        printf("Distância: %.2f\n", maiorDistancia);
    } else {
        printf("A cidade com vizinho mais próximo é: %s\n", listagemNomes[cidadeMaisProxima]);
        printf("Vizinho: %s\n", listagemNomes[vizinhoMaisProximo]);
        printf("Distância: %.2f\n", menorDistancia);
    }
}

void menorCaminho(float cidades[][TAM], int quantidadeCidades, char listagemNomes[][MAX]) {
    char nomeCity1[MAX], nomeCity2[MAX];
    printf("Digite o nome da primeira cidade: ");
    scanf(" %s", nomeCity1);
    printf("Digite o nome da segunda cidade: ");
    scanf(" %s", nomeCity2);
    algoritmoDijkstra(cidades, quantidadeCidades, listagemNomes, nomeCity1, nomeCity2);
}

int menu() {
    int op;
    printf("\n\nMENU CIDADES\n\n");
    printf("1 - Maior número de vizinhos\n");
    printf("2 - Menor número de vizinhos\n");
    printf("3 - Vizinho mais distante\n");
    printf("4 - Vizinho mais próximo\n");
    printf("5 - Distância entre duas cidades\n");
    printf("0 - Sair\n");
    do {
        printf("Escolha sua opção: ");
        scanf(" %d", &op);
    } while (op < 0 || op > 5);
    return op;
}

int main() {
    FILE * arquivo;
    arquivo = abrirArquivo("../arquivo/arq.txt", "r");
    int quantiCity;
    int op;
    carregarArquivo1(arquivo, &quantiCity);
    char listagemNomes[quantiCity][MAX];
    carregarArquivo2(arquivo, quantiCity, listagemNomes);
    float cidades[quantiCity][quantiCity];
    carregarArquivo3(arquivo, quantiCity, cidades);
    listar(listagemNomes,quantiCity);
    listar2(cidades,quantiCity);
    do{
        op = menu();
        switch (op) {
            case 1:
                defineVizinhos(cidades,quantiCity,listagemNomes,0);
                break;
            case 2:
                defineVizinhos(cidades,quantiCity,listagemNomes,1);
                break;
            case 3:
                vizinhoDistancia(cidades,quantiCity,listagemNomes,0);
                break;
            case 4:
                vizinhoDistancia(cidades,quantiCity,listagemNomes,1);
                break;
            case 5:
                menorCaminho(cidades,quantiCity,listagemNomes);
                break;
            case 0:
                break;
            default:
                printf("Opção inválida. Tente novamente.\n");
        }
    } while (op != 0);
    fclose(arquivo);
	return 0;
}