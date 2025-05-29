#ifndef _H_GRAFOS
#define _H_GRAFOS

#include <stdbool.h>

#define NOME_MAX 64

struct aresta {
    struct vertice *vertice;
    struct aresta *proxAresta;
};

struct vertice {
    int id;
    char nomeVertice[NOME_MAX];
    struct vertice *proxVertice;
    struct aresta *adj;
};

typedef struct grafo {
    int qtdVertices;
    int qtdArestas;
    struct vertice *listaVertices;
} Grafo;

Grafo *criarGrafo(FILE *arquivo);
void exibirGrafo(Grafo *grafo);
void liberarGrafo(Grafo *grafo);
void gerarDot(char *nomeArquivo, Grafo *grafo);

struct vertice *novoVertice(char *nomeNovoVertice, Grafo *grafo);
struct vertice *buscaVertice(char *nome, Grafo *grafo, struct vertice **vRetorna);
struct vertice *selecionaVertice(Grafo *grafo, int id);
bool removeVertice(char *nome, Grafo *grafo);

void novaAresta(Grafo *grafo, struct vertice *v1, struct vertice *v2);
bool removeAresta(char *nomeV1, char *nomeV2, Grafo *grafo);

#endif