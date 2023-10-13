#ifndef VND_H
#define VND_H

#include "Solucao.hpp"
#include "ProblemaCondicoes.hpp"
#include "Rota.hpp"

#include <limits>

bool swap(ProblemaCondicoes *condicoes, Solucao *solucao);
void trocaVertices(int melhor_i, int melhor_j, int menorCusto, std::vector<int>& vectorRota);

#endif