#ifndef GULOSO_H
#define GULOSO_H

#include "Solucao.hpp"
#include "ProblemaCondicoes.hpp"
#include <limits>

Solucao* algoritmoGuloso(ProblemaCondicoes *condicoes);

void terceiriza(std::vector<int> &verticesRestantes, int custoAtualTerceirizacao, std::vector<int> &custoTerceirizar, std::vector<int> &clientesTerceirizados);

void trocaRota(int &custoAtualSolucao, std::vector<int> &verticeCustos, int &verticeAtual, Solucao *solucao, Rota &rota, int &rotaAtual, std::vector<std::vector<int>> &custoCaminhos);

#endif