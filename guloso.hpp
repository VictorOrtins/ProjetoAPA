#ifndef GULOSO_H
#define GULOSO_H

#include "Solucao.hpp"
#include "ProblemaCondicoes.hpp"
#include <limits>
#include <chrono>

struct RetornoGuloso{
    Solucao* solucao;
    double seconds;
};

//Procura uma solução gulosa para o problema. A ideia é começar uma rota, ir até que a capacidade dela estoure e só depois ir para a próxima
RetornoGuloso* algoritmoGuloso(ProblemaCondicoes *condicoes);

//Terceiriza as rotas que sobraram
void terceiriza(std::vector<int> &verticesRestantes, int custoAtualTerceirizacao, std::vector<int> &custoTerceirizar, std::vector<int> &clientesTerceirizados);

//Troca a rota atual, adiciona o vértice 0 no final da rota anterior e coloca 0 no início da próxima rota e atualiza o custo da rota que passou
void trocaRota(int &custoAtualSolucao, std::vector<int> &verticeCustos, int &verticeAtual, Solucao *solucao, Rota &rota, int &rotaAtual, std::vector<std::vector<int>> &custoCaminhos);

#endif