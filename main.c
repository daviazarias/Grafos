#include <stdio.h>
#include <stdlib.h>
#include "grafos.h"

void DFS(Grafo *grafo);

FILE *abrirArquivo(char *nome)
{
    FILE *arquivo = fopen(nome,"r");

    if(!arquivo){
        printf("ERRO: Arquivo %s não foi encontrado.\n", nome);
        exit(2);
    }

    return arquivo;
}

int main(int argc, char **argv)
{
    if(argc != 2){
        printf("ERRO: Uso: %s <arquivo>\n", argv[0]);
        exit(1);
    }

    Grafo *grafo = criarGrafo(abrirArquivo(argv[1]));
    exibirGrafo(grafo);
    removeAresta("e","f",grafo);
    removeAresta("d","h",grafo);
    removeAresta("b","c",grafo);
    removeAresta("f","g",grafo);
    removeAresta("a","f",grafo);
    exibirGrafo(grafo);

    gerarDot("grafo.dot",grafo);

    liberarGrafo(grafo);

    return 0;
}