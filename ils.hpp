#include "Solucao.hpp"
#include "ProblemaCondicoes.hpp"
#include <vector>


typedef struct{
    Solucao* solucao;
    double tempo;
}RetornoILS;

RetornoILS* ILS(ProblemaCondicoes* condicoes, int maxIter);

void perturbacao(ProblemaCondicoes* condicoes, Solucao* solucao);

void swapAleatorio(ProblemaCondicoes* condicoes, Solucao* solucao);

void terceirizaAleatorio(ProblemaCondicoes* condicoes, Solucao* solucao);

void perturbacao(ProblemaCondicoes* condicoes, Solucao* solucao);