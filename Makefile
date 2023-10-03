CC = g++
CFLAGS = -Wall -g

main: main.o ProblemaCondicoes.o Solucao.o funcoes.o
	$(CC) $(CFLAGS) -o main main.o ProblemaCondicoes.o Solucao.o funcoes.o

main.o: main.cpp ProblemaCondicoes.h Solucao.h funcoes.h
	$(CC)  $(CFLAGS) -c main.cpp

ProblemaCondicoes.o: ProblemaCondicoes.h

Solucao.o: Solucao.h

funcoes.o: funcoes.h ProblemaCondicoes.h Solucao.h

