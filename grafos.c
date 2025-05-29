#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "grafos.h"

/*----------------------------------------------------------------------------*/
/*----------------------------MÉTODOS ESTÁTICOS-------------------------------*/
/*----------------------------------------------------------------------------*/

// Retorna uma aresta que aponta para o parâmetro vértice.
static struct aresta *criarAresta(struct vertice *vertice)
{
    struct aresta *novaAresta = malloc(sizeof(struct aresta));
    
    novaAresta->vertice = vertice;
    novaAresta->proxAresta = NULL;

    return novaAresta;
}

// Insere uma aresta na lista de adjacência de um vértice.
static void inserirAresta(struct aresta *novaAresta, struct vertice *cabeca)
{
    novaAresta->proxAresta = cabeca->adj;
    cabeca->adj = novaAresta;
}

// Desaloca toda a lista de adjacência de "vertice".
static void liberarArestas(Grafo *grafo, struct vertice *vertice)
{
    struct aresta *arestaAnt = NULL;
    struct aresta *aresta = vertice->adj;

    while(aresta){
        arestaAnt = aresta;
        aresta = aresta->proxAresta;
        grafo->qtdArestas--;
        free(arestaAnt);
    }
}

static struct aresta *buscaArestaVN(struct vertice *v1, char *nomeV2, struct aresta **ant)
{
    struct aresta *aresta = v1->adj;
    *ant = NULL;

    while(aresta && strcmp(aresta->vertice->nomeVertice,nomeV2)){
        *ant = aresta;
        aresta = aresta->proxAresta;
    }

    return aresta;
}

// Retorna "true" se a aresta (v1,v2) existir, e "false" caso contrário.
// Passar os endereços dos vértices v1 e v2 como parâmetros.
static struct aresta *buscaArestaV(struct vertice *v1, struct vertice *v2, struct aresta **ant)
{
    struct aresta *aresta = v1->adj;
    *ant = NULL;

    while(aresta && aresta->vertice != v2){
        *ant = aresta;
        aresta = aresta->proxAresta;
    }
    
    return aresta;
}

static bool removeArestaVN(struct vertice *v1, char *nomeV2, Grafo *grafo)
{
    struct aresta *ant = NULL;
    struct aresta *arestaRm = buscaArestaVN(v1,nomeV2,&ant);

    if(!arestaRm) return false;

    /*-----------Aresta existe------------*/ 

    if(!ant) v1->adj = arestaRm->proxAresta;
    else ant->proxAresta = arestaRm->proxAresta;

    free(arestaRm);
    grafo->qtdArestas--;
    return true;
}

static bool removeVerticeV(struct vertice *verticeRm, struct vertice *pai, Grafo *grafo)
{
    int i = --grafo->qtdVertices - 1;

    if(pai)  pai->proxVertice = verticeRm->proxVertice; 
    else grafo->listaVertices = verticeRm->proxVertice;

    liberarArestas(grafo,verticeRm);

    struct vertice *vertice = grafo->listaVertices;

    while(vertice)
    {
        removeArestaVN(vertice,verticeRm->nomeVertice,grafo);

        vertice->id = i--;
        vertice = vertice->proxVertice;
    }

    free(verticeRm);
    return true;
}

/*---------------------------------------------------------------------------*/
/*----------------------------MÉTODOS PÚBLICOS-------------------------------*/
/*---------------------------------------------------------------------------*/

// Cria uma nova aresta entre os vértices passados como argumento, inserindo-a
// em sua lista de adjacência. 
void novaAresta(Grafo *grafo, struct vertice *v1, struct vertice *v2)
{
    inserirAresta(criarAresta(v2),v1);
    grafo->qtdArestas++;
}

bool removeAresta(char *nomeV1, char *nomeV2, Grafo *grafo)
{
    struct vertice *v1, *aux;
    v1 = buscaVertice(nomeV1,grafo,&aux);

    return v1 && removeArestaVN(v1,nomeV2,grafo);
}

// Responde se existe um vértice com um determinado nome em um grafo.
// Altera o valor de vRetorna para NULL, caso o vértice não esteja no grafo,
// ou com o endereço do vértice, caso contrário.
struct vertice *buscaVertice(char *nome, Grafo *grafo, struct vertice **vPai)
{
    struct vertice *vertice = grafo->listaVertices;
    *vPai = NULL;

    while(vertice && strcmp(nome,vertice->nomeVertice)){
        *vPai = vertice;
        vertice = vertice->proxVertice;
    }

    return vertice;
}

// Cria um novo vértice do nome informado e o insere no grafo.
// Cria um novo grafo caso "grafo" seja NULL.
// Altera o parâmetro vértice para receber o vértice criado.
struct vertice *novoVertice(char *nome, Grafo *grafo)
{
    struct vertice *vertice = malloc(sizeof(struct vertice));

    strcpy(vertice->nomeVertice,nome);
    vertice->id = grafo->qtdVertices++;
    vertice->proxVertice = grafo->listaVertices;
    vertice->adj = NULL;

    grafo->listaVertices = vertice;
    return vertice;
}

// Seleciona um vértice do grafo pelo seu id.
struct vertice *selecionaVertice(Grafo *grafo, int id)
{
    if(id < 0 || id >= grafo->qtdVertices) 
        return NULL;

    /*------------Índice válido--------------*/

    struct vertice *v = grafo->listaVertices;

    while(v->id != id) 
        v = v->proxVertice;

    return v;
}

bool removeVertice(char *nome, Grafo *grafo)
{
    struct vertice *pai;
    struct vertice *verticeRm = buscaVertice(nome,grafo,&pai);

    return verticeRm && removeVerticeV(verticeRm,pai,grafo);
}

/*************************************************************************************** */

// Lê um arquivo e retorna um grafo.
Grafo *criarGrafo(FILE *arquivo)
{
    Grafo *grafo = malloc(sizeof(Grafo));
    grafo->qtdArestas = 0;
    grafo->qtdVertices = 0;
    grafo->listaVertices = NULL;

    char buffer1[NOME_MAX], buffer2[NOME_MAX];

    while(EOF != fscanf(arquivo, "%s", buffer1))
    {
        struct vertice *v1, *v2, *auxV;
        struct aresta *auxA;

        v1 = buscaVertice(buffer1,grafo,&auxV);
        if(!v1) {v1 = novoVertice(buffer1,grafo);}

        if(EOF == fscanf(arquivo,"%s",buffer2)) 
            break;

        v2 = buscaVertice(buffer2,grafo,&auxV);
        if(!v2) v2 = novoVertice(buffer2,grafo);
        
        if(!buscaArestaV(v1,v2,&auxA))
            novaAresta(grafo,v1,v2);
    }

    fclose(arquivo);
    return grafo;
}

// Exibe a lista de adjacência do grafo na saída padrão.
void exibirGrafo(Grafo *grafo)
{
    struct vertice *vertice = grafo->listaVertices;
    puts("Exibindo grafo...");
    while(vertice)
    {
        printf("[%d] %s -> ", vertice->id, vertice->nomeVertice);
        struct aresta *aresta = vertice->adj;
        while(aresta)
        {
            printf("%s ", aresta->vertice->nomeVertice);
            aresta = aresta->proxAresta;
        }
        putchar('\n');
        vertice = vertice->proxVertice;
    }
}

void gerarDot(char *nomeArquivo, Grafo *grafo)
{
    FILE *arquivo = fopen(nomeArquivo,"w");
    if(!arquivo){
        fprintf(stderr,"Não foi possível criar o arquivo %s.\n",nomeArquivo);
        return;
    }

    fprintf(arquivo,"digraph G {\n");
    for(struct vertice *v = grafo->listaVertices; v; v = v->proxVertice)
        fprintf(arquivo,"\t%s [color=orange, style=filled];\n",v->nomeVertice);

    for(struct vertice *v = grafo->listaVertices; v; v = v->proxVertice)
        for(struct aresta *a = v->adj; a; a = a->proxAresta)
            fprintf(arquivo,"\t%s -> %s\n",v->nomeVertice,a->vertice->nomeVertice);

    fputc('}',arquivo);
    fclose(arquivo);
}

void liberarGrafo(Grafo *grafo)
{
    struct vertice *vertice = grafo->listaVertices;
    while(vertice)
    {
        struct vertice *tempV = vertice->proxVertice;
        struct aresta *aresta = vertice->adj;
        while(aresta)
        {
            struct aresta *tempA = aresta->proxAresta;
            free(aresta);
            aresta = tempA;
        }
        free(vertice);
        vertice = tempV;
    }
    free(grafo);
}