#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include "grafos.h"

#define BRANCO 0x00
#define CINZA 0x01
#define PRETO 0x02

struct info {
    int d, f;
    struct vertice *p;
    __uint8_t cor;
};

static int tempo = 0;

static void DFS_VISIT(struct vertice*,struct info*);
static void exibeInfo(Grafo*,struct info*,int);

void DFS(Grafo *grafo){

    struct info info[grafo->qtdVertices];
    
    for(int i = 0; i < grafo->qtdVertices; i++){
        info[i].cor = BRANCO;
        info[i].p = NULL; 
    }

    for(int i = grafo->qtdVertices - 1; i > 0; i--)
        if(info[i].cor == BRANCO)
            DFS_VISIT(selecionaVertice(grafo,i), info);

    exibeInfo(grafo, info, grafo->qtdVertices);
}

static void DFS_VISIT(struct vertice *vertice, struct info *info){

    int id = vertice->id;

    info[id].cor = CINZA;
    info[id].d = ++tempo;

    for(struct aresta *adj = vertice->adj; adj; adj = adj->proxAresta){
        struct vertice *u = adj->vertice;
        if(info[u->id].cor == BRANCO){
            info[u->id].p = vertice;
            DFS_VISIT(u,info);
        }
    }
    
    info[id].cor = PRETO;
    info[id].f = ++tempo;
}

static void exibeInfo(Grafo* grafo, struct info* info, int tam){

    puts("Busca em profundidade: ");
    for(struct vertice *v = grafo->listaVertices; v; v = v->proxVertice){
        struct info t = info[v->id];
        printf("%s [%d:%d] : Pai = %s\n", v->nomeVertice, t.d, t.f, (t.p) ? t.p->nomeVertice : "NULL");
    }
}