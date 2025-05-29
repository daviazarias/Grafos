ARQS_FONTE = $(wildcard *.c)

all: grafos

grafos: $(ARQS_FONTE)
	@ gcc *.c -o grafos
	@ ./grafos grafo.txt
	@ dot -Tpng grafo.dot -o grafo.png

clean:
	@ rm grafos grafo.dot grafo.png