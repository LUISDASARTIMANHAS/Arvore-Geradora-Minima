// Feito por: Lucas Garcia E Luis Augusto
#include "ArvoreGeradoraMinima.h"
#include "../data/libs/fileSys.cpp"

//=================================================
void exibirAGM(TGrafo *grafo) {
    printf("\nArestas da Árvore Geradora Mínima (AGM):\n");
    for (int i = 0; i < grafo->numCidades; i++) {
        TVizinho *vizinho = grafo->cidades[i].vizinhos;
        while (vizinho != NULL) {
            // Exibe apenas arestas que foram marcadas como parte da AGM
            if (vizinho->incluidoAGM) {
                printf("Cidade: %s - Vizinho: %s, Distância: %.2f\n",
                    grafo->cidades[i].nome, vizinho->nome, vizinho->distancia);
            }
            vizinho = vizinho->prox;
        }
    }
}
//=================================================
// Função para exibir todas as cidades e seus vizinhos
void exibirGrafo(TGrafo *grafo) {
    for (int i = 0; i < grafo->numCidades; i++) {
    printf("\nCidade: %s\n", grafo->cidades[i].nome);
    TVizinho *vizinho = grafo->cidades[i].vizinhos;
        while (vizinho != NULL) {
        printf(" Vizinho: %s, Distância: %.2f\n", vizinho->nome, vizinho->distancia);
        vizinho = vizinho->prox;
        }
    }
}

//=================================================
// Inicializa o grafo com uma capacidade inicial de cidades
void inicializarGrafo(TGrafo *grafo, int capacidade) {
    grafo->cidades = (TCidade*) malloc(capacidade * sizeof(TCidade));
    grafo->numCidades = 0;
    grafo->capacidade = capacidade;
}
//=================================================
// Função para inserir uma cidade no grafo
void inserirCidade(TGrafo *grafo, string nomeCidade) {
    // Verifica se a cidade já existe
    if (buscarCidade(grafo, nomeCidade) != NULL) {
        return;
    }

    // Verifica se o grafo precisa de mais capacidade
    if (grafo->numCidades == grafo->capacidade) {
        grafo->capacidade *= 2; // Dobra a capacidade
        TCidade *guardaCidades = (TCidade *) realloc(grafo->cidades, grafo->capacidade * sizeof(TCidade));
        
        if (guardaCidades == NULL) {
            printf("Erro ao realocar memória para cidades!\n");
            exit(1); // Encerra o programa em caso de erro
        }

        grafo->cidades = guardaCidades; // Atualiza o ponteiro se a realocação for bem-sucedida
    }

    // Insere a nova cidade
    strcpy(grafo->cidades[grafo->numCidades].nome, nomeCidade);
    grafo->cidades[grafo->numCidades].vizinhos = NULL;
    grafo->numCidades++;
}
//=================================================
void exibirCidade(TGrafo *grafo, char *nomeCidade) {
    TCidade *cidade = buscarCidade(grafo, nomeCidade);

    if (cidade == NULL) {
        printf("Cidade '%s' não encontrada!\n", nomeCidade);
        return;
    }

    printf("Cidade: %s\n", cidade->nome);
    TVizinho *vizinho = cidade->vizinhos;
    if (vizinho == NULL) {
        printf("  Não possui vizinhos cadastrados.\n");
    } else {
        while (vizinho != NULL) {
            printf("  Vizinho: %s, Distância: %.2f\n", vizinho->nome, vizinho->distancia);
            vizinho = vizinho->prox;
        }
    }
}

//=================================================
// Função para inserir um vizinho a uma cidade existente
void inserirVizinho(TGrafo *grafo,char *nomeCidade, char *nomeVizinho, double distancia) {
    TVizinho *novoVizinho = (TVizinho *) malloc(sizeof(TVizinho));
    TCidade *cidadeVizinha = buscarCidade(grafo, nomeCidade);

    if (cidadeVizinha == NULL) {
        printf("\n Cidade Vizinha '%s' não encontrada!\n", nomeVizinho);
        return;
    }

    strcpy(novoVizinho->nome, nomeVizinho);
    novoVizinho->distancia = distancia;
    novoVizinho->prox = cidadeVizinha->vizinhos;
    cidadeVizinha->vizinhos = novoVizinho;

    printf("\n Vizinho %s Inserida!",nomeVizinho);
}
//=================================================
// Busca uma cidade pelo nome
TCidade* buscarCidade(TGrafo *grafo, string nomeCidade) {
    for (int i = 0; i < grafo->numCidades; i++) {
        if (strcmp(grafo->cidades[i].nome, nomeCidade) == 0) {
            return &grafo->cidades[i];
        }
    }
    
    return NULL;
}
//=================================================
void removerCidade(TGrafo *grafo, char *nomeCidade) {
    int indice = -1;
    int i;
    // Encontra o índice da cidade a ser removida
    for (i = 0; i < grafo->numCidades; i++) {
        if (strcmp(grafo->cidades[i].nome, nomeCidade) == 0) {
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("Cidade '%s' não encontrada!\n", nomeCidade);
        return;
    }

    // Remove todos os vizinhos da cidade a ser excluída
    TVizinho *vizinho = grafo->cidades[indice].vizinhos;
    while (vizinho != NULL) {
        TVizinho *temp = vizinho;
        vizinho = vizinho->prox;
        free(temp);
    }

    // Remover a cidade como vizinha de outras cidades
    for (i = 0; i < grafo->numCidades; i++) {
        if (i != indice) {  // Não é necessário atualizar a cidade que está sendo removida
            TVizinho *atual = grafo->cidades[i].vizinhos;
            TVizinho *anterior = NULL;
            while (atual != NULL) {
                if (strcmp(atual->nome, nomeCidade) == 0) {
                    // Remover o vizinho
                    if (anterior == NULL) { // Primeiro da lista de vizinhos
                        grafo->cidades[i].vizinhos = atual->prox;
                    } else {
                        anterior->prox = atual->prox;
                    }
                    free(atual);
                    break;  // Como os nomes de vizinhos são únicos, saímos após encontrar
                }
                anterior = atual;
                atual = atual->prox;
            }
        }
    }

    // Remove a cidade do vetor
    for (i = indice; i < grafo->numCidades - 1; i++) {
        grafo->cidades[i] = grafo->cidades[i + 1];
    }

    grafo->numCidades--;
    printf("Cidade '%s' removida com sucesso e removida da lista de vizinhos de outras cidades!\n", nomeCidade);
}
//=================================================

// Função para inserir uma cidade e seus vizinhos no grafo
void inserir(TGrafo *grafo, string nomeCidade, string nomeVizinho, double distancia) {
    inserirCidade(grafo, nomeCidade);
    inserirVizinho(grafo,nomeCidade, nomeVizinho, distancia);
}

//=================================================
void inserirDadosDoArquivo(TGrafo *grafo, FILE *arquivo) {
    double distancia;
    string nomeCidade;
    string nomeVizinho;

    while(!feof(arquivo)){
        while(strcmp(nomeCidade,":") != 0 &&!feof(arquivo)){
            fscanf(arquivo,"%[^\n]s",nomeCidade);
            
            fscanf(arquivo,"\n%[^\n]s",nomeVizinho);
            fscanf(arquivo,"%lf",&distancia);
            if(strcmp(nomeVizinho,":") != 0){
                inserir(grafo,nomeCidade,nomeVizinho,distancia);
            }
            
        }
    }
}
//=================================================
double validarPeso(){
    double value;
    printf("\n Digite a Distancia entre as cidades: ");
    value = inputDouble();
    return value;
}
//=================================================
void validarCidade(char *destino){
    printf("\n Digite a Cidade: ");
    inputS(destino);
}
//=================================================
void validarVizinho(TGrafo *grafo,string nomeCidade, char *destino){
    TCidade *cidade = buscarCidade(grafo, nomeCidade);

    if (cidade == NULL) {
        printf("Cidade '%s' não encontrada!\n", nomeCidade);
        return;
    }
    printf("\n Digite o Vizinho: ");
    inputS(destino);
}
//=================================================
void cadastrarCidade(TGrafo *grafo){
    string nomeCidade;
    validarCidade(nomeCidade);
    inserirCidade(grafo, nomeCidade);
}
//=================================================
void cadastrarVizinho(TGrafo *grafo){
    string nomeCidade, nomeVizinho;
    double distancia;

    validarCidade(nomeCidade);
    validarVizinho(grafo,nomeCidade, nomeVizinho);
    distancia = validarPeso();
    inserirVizinho(grafo,nomeCidade,nomeVizinho,distancia);
    inserirVizinho(grafo,nomeVizinho,nomeCidade,distancia);
}
//=================================================
void deletarCidade(TGrafo *grafo){
    string nomeCidade;
    validarCidade(nomeCidade);
    removerCidade(grafo,nomeCidade);
}
//=================================================
void exibirCidadeEVizinho(TGrafo *grafo){
    string nomeCidade;
    validarCidade(nomeCidade);
    exibirCidade(grafo,nomeCidade);
}
//=================================================
void autosave(TGrafo *grafo){
    FILE *arq = abrirArquivo(ARQUIVOCIDADES,"w");
    int i = 0;

    // Loop que percorre o número total de cidades
    for (i = 0; i < grafo->numCidades; i++) {  
        // Obtém um ponteiro para a cidade atual
        TCidade *cidadeAtual = &grafo->cidades[i];  
        TVizinho *vizinho = cidadeAtual->vizinhos;
        fWiriteSTRING(arq,cidadeAtual->nome);

        // Percorre a lista de vizinhos
        while (vizinho != NULL) {
            fWiriteSTRING(arq,vizinho->nome);
            fWiriteINT(arq,vizinho->distancia);
            vizinho = vizinho->prox;  // Avança para o próximo vizinho
        }
        // coloca o separador de blocos de cidade
        fWiriteSTRING(arq,":");
    }
    
    fclose(arq);
    printf("\n Auto Save Completo!");
}
//=================================================
// Funções auxiliares para Union-Find
int find(int *pai, int i) {
    if (pai[i] != i)
        pai[i] = find(pai, pai[i]);
    return pai[i];
}
//=================================================
void unionSets(int *pai, int *rank, int u, int v) {
    int raizU = find(pai, u);
    int raizV = find(pai, v);
    if (rank[raizU] < rank[raizV]) {
        pai[raizU] = raizV;
    } else if (rank[raizU] > rank[raizV]) {
        pai[raizV] = raizU;
    } else {
        pai[raizV] = raizU;
        rank[raizU]++;
    }
}
//=================================================
int compararArestas(const void *a, const void *b) {
    TVizinho *arestaA = *(TVizinho **)a;
    TVizinho *arestaB = *(TVizinho **)b;
    return (arestaA->distancia > arestaB->distancia) - (arestaA->distancia < arestaB->distancia);
}
//=================================================
int buscarIndiceCidade(TGrafo *grafo, char *nomeCidade) {
    for (int i = 0; i < grafo->numCidades; i++) {
        if (strcmp(grafo->cidades[i].nome, nomeCidade) == 0) {
            return i;  // Retorna o índice da cidade
        }
    }
    return -1;  // Cidade não encontrada
}
//=================================================
// Função para encontrar e ordenar as arestas no grafo
void ordenarArestas(TGrafo *grafo, TVizinho **arestasOrdenadas, int *numArestas) {
    int totalArestas = 0;
    // Conta todas as arestas e armazena os ponteiros para ordenação
    for (int i = 0; i < grafo->numCidades; i++) {
        TVizinho *vizinho = grafo->cidades[i].vizinhos;
        while (vizinho != NULL) {
            int indiceVizinho = buscarIndiceCidade(grafo, vizinho->nome);
            if (i < indiceVizinho) { // Evita duplicação de arestas bidirecionais
                arestasOrdenadas[totalArestas++] = vizinho;
            }
            vizinho = vizinho->prox;
        }
    }
    *numArestas = totalArestas;

    // Função de comparação e ordenação das arestas por peso
    qsort(arestasOrdenadas, *numArestas, sizeof(TVizinho *), compararArestas);
}
//=================================================
void kruskalAGM(TGrafo *grafo) {
    int numCidades = grafo->numCidades;
    int numArestas = 0;
    
    // Aloca espaço para armazenar os ponteiros das arestas para ordenação
    TVizinho **arestasOrdenadas = malloc(sizeof(TVizinho *) * numCidades * numCidades);

    // Passo 1: Coleta e ordena as arestas
    ordenarArestas(grafo, arestasOrdenadas, &numArestas);

    // Inicializa estruturas Union-Find
    int *pai = malloc(numCidades * sizeof(int));
    int *rank = calloc(numCidades, sizeof(int));
    for (int i = 0; i < numCidades; i++) pai[i] = i;

    printf("Árvore Geradora Mínima (AGM) usando o Algoritmo de Kruskal:\n");

    // Passo 3: Seleciona as arestas de menor peso sem formar ciclos
    for (int i = 0; i < numArestas; i++) {
        TVizinho *aresta = arestasOrdenadas[i];
        int u = buscarIndiceCidade(grafo, grafo->cidades[i].nome); // índice de origem
        int v = buscarIndiceCidade(grafo, aresta->nome); // índice de destino

        if (u == -1 || v == -1) {
            printf("Erro: Cidade não encontrada.\n");
            continue;
        }

        if (find(pai, u) != find(pai, v)) {
            // Marca a aresta como parte da AGM
            aresta->incluidoAGM = 1;
            
            // Marca a aresta reversa na cidade vizinha para garantir inclusão bidirecional
            TVizinho *vizinhoReverso = grafo->cidades[v].vizinhos;
            while (vizinhoReverso != NULL) {
                if (strcmp(vizinhoReverso->nome, grafo->cidades[u].nome) == 0) {
                    vizinhoReverso->incluidoAGM = 1;
                    break;
                }
                vizinhoReverso = vizinhoReverso->prox;
            }

            // Exibe a aresta adicionada à AGM
            printf("Aresta escolhida: %s - %s, Peso: %.2f\n",
                   grafo->cidades[u].nome, aresta->nome, aresta->distancia);

            // Une os conjuntos
            unionSets(pai, rank, u, v);
        }
    }

    // Libera memória alocada
    free(arestasOrdenadas);
    free(pai);
    free(rank);
}
//=================================================
void primAGM(TGrafo *grafo, int cidadeInicial) {
    // Inicializa o array de cidades visitadas
    int *visitado = calloc(grafo->numCidades, sizeof(int));
    visitado[cidadeInicial] = 1;  // Começa pela cidade inicial

    printf("Árvore Geradora Mínima (AGM) usando o Algoritmo de Prim:\n");

    for (int numVisitados = 1; numVisitados < grafo->numCidades; numVisitados++) {
        double menorPeso = INFINITY;
        int u = -1, v = -1;
        TVizinho *vizinhoSelecionado = NULL;

        // Procura a menor aresta que conecta o conjunto de nós visitados com o restante
        for (int i = 0; i < grafo->numCidades; i++) {
            if (visitado[i]) {
                TVizinho *vizinho = grafo->cidades[i].vizinhos;
                while (vizinho != NULL) {
                    int indiceVizinho = buscarIndiceCidade(grafo, vizinho->nome);
                    
                    // Verifica se a cidade vizinha ainda não foi visitada e se a aresta é a menor encontrada
                    if (!visitado[indiceVizinho] && vizinho->distancia < menorPeso) {
                        menorPeso = vizinho->distancia;
                        u = i;  // Armazena o índice da cidade atual
                        v = indiceVizinho;  // Armazena o índice da cidade vizinha
                        vizinhoSelecionado = vizinho;  // Guarda a referência para o vizinho selecionado
                    }
                    vizinho = vizinho->prox;
                }
            }
        }

        // Marcar o nó vizinho selecionado como visitado e a aresta como parte da AGM
        if (v != -1 && vizinhoSelecionado != NULL) {
            visitado[v] = 1;
            vizinhoSelecionado->incluidoAGM = 1;  // Marca a aresta como parte da AGM
            
            // Também marca a aresta reversa na cidade vizinha para refletir a inclusão bidirecional na AGM
            TVizinho *vizinhoReverso = grafo->cidades[v].vizinhos;
            while (vizinhoReverso != NULL) {
                if (strcmp(vizinhoReverso->nome, grafo->cidades[u].nome) == 0) {
                    vizinhoReverso->incluidoAGM = 1;
                    break;
                }
                vizinhoReverso = vizinhoReverso->prox;
            }

            printf("Aresta escolhida: %s - %s, Peso: %.2f\n", grafo->cidades[u].nome, grafo->cidades[v].nome, menorPeso);
        }
    }
    
    free(visitado);
}

void usarAlgoritimoAGMPrim(TGrafo *grafo){
    printf("Gerando Árvore Geradora Mínima usando o Algoritmo de Prim...\n");
    double ini = clock();
    primAGM(grafo, 0); // Inicia a partir da cidade 0
    calcularTempo(ini);
    exibirAGM(grafo);  // Exibe as arestas da AGM
}

void usarAlgoritimoAGMKruskal(TGrafo *grafo){
    printf("Gerando Árvore Geradora Mínima usando o Algoritmo de Kruskal...\n");
    double ini = clock();
    kruskalAGM(grafo);
    calcularTempo(ini);
    exibirAGM(grafo);  // Exibe as arestas da AGM
}

//=================================================
void gerarArvoreGerMin(TGrafo *grafo){
    int opc = -1;
    while (opc != 0) {
        printf("\n======= Escolha o algoritmo para AGM =======\n");
        printf("1 - Algoritmo Prim\n");
        printf("2 - Algoritmo Kruskal\n");
        printf("0 - Voltar\n");
        printf("=====================\n");
        printf("Insira uma opcao: ");
        opc = input();
        
        switch (opc) {
            case 0:
                printf("\n Voltando e Salvando automaticamente...");
                // autosave(grafo);
                break;
            case 1:
                usarAlgoritimoAGMPrim(grafo);
                break;
            case 2:
                usarAlgoritimoAGMKruskal(grafo);
                break;
            default:
                printf("\n Opcao Invalida!\n");
                break;
        }
    }
}

//=================================================
void menu(TGrafo *grafo) {
    int opc = -1;
    while (opc != 0) {
        printf("\n======= GRAFO DE CIDADES =======\n");
        printf("1 - Inserir Cidade\n");
        printf("2 - Inserir Vizinho\n");
        printf("3 - Remover Vizinho e Cidade\n");
        printf("4 - Imprimir Cidade e seus Vizinhos\n");
        printf("5 - Imprimir Todo o Grafo\n");
        printf("6 - Gerar Arvore Geradora Minima\n");
        printf("0 - Sair\n");
        printf("=====================\n");
        printf("Insira uma opcao: ");
        opc = input();
        
        switch (opc) {
            case 0:
                printf("\n Saindo e Salvando automaticamente...");
                break;
            case 1:
                cadastrarCidade(grafo);
                break;
            case 2:
                cadastrarVizinho(grafo);
                break;
            case 3:
                deletarCidade(grafo);
                break;
            case 4:
                exibirCidadeEVizinho(grafo);
                break;
            case 5:
                exibirGrafo(grafo);
                break;
            case 6: 
                gerarArvoreGerMin(grafo);
                break;
            default:
                printf("\n Opcao Invalida!\n");
                break;
        }
    }
    autosave(grafo);
    exit(0);
}
//=================================================
void mapeandoGrafo(TGrafo *grafo){
    inicializarGrafo(grafo, 20);  // Inicializa com uma capacidade de 5 cidades

    // Abre o arquivo "cidades"
    FILE *arquivo = abrirArquivo(ARQUIVOCIDADES, "r");
    // Ler todas as cidades, vizinhos e distâncias
    inserirDadosDoArquivo(grafo,arquivo);

    // Fecha o arquivo
    fclose(arquivo);
    menu(grafo);
}
//=================================================
// Função principal (main)
int main() {
    TGrafo grafo;
    mapeandoGrafo(&grafo);
    return 0;
}