#ifndef VND_H
#define VND_H

#include "Solucao.hpp"
#include "ProblemaCondicoes.hpp"
#include "Rota.hpp"

#include <limits>
#include <chrono>

//Movimento de vizinhança de swap numa mesma rota. Procura fazer o melhor swap para cada uma das rotas numa mesma chamada
bool swap(ProblemaCondicoes *condicoes, Solucao *solucao);

//Realiza a troca de vértices
void trocaVertices(int melhor_i, int melhor_j, std::vector<int>& vectorRota);

//Movimento de vizinhança de um swap entre 2 rotas. Procura encontrar o melhor desses swaps e já fazer a troca.
bool swapInterRotas(ProblemaCondicoes *condicoes, Solucao *solucao);

//Faz a troca entre as rotas
void trocaInterRota(int indiceRota1, int indiceRota2, std::vector<int> &vectorRota1, std::vector<int> &vectorRota2);

//Movimento de vizinhança que tenta terceirizar um dos vértices da solução
bool melhorTerceirizacao(ProblemaCondicoes *condicoes, Solucao *solucao);

bool desterceirizaVertice(ProblemaCondicoes *condicoes, Solucao *solucao);

//Função que aplica as estruturas de vizinhança
double VND(ProblemaCondicoes *condicoes, Solucao *solucao);

#endif