#ifndef FUNCOES_H
#define FUNCOES_H

#include <vector>
#include "ProblemaCondicoes.hpp"
#include <string>
#include "Solucao.hpp"
#include <fstream>
#include <limits>
#include <algorithm>

std::vector<int> split(const std::string& text);

ProblemaCondicoes* leArquivo(std::string nomeArquivo);

Solucao* criaSolucao();

bool escreveArquivo(std::string arquivoDestino, Solucao* solucao);

Solucao* algoritmoGuloso(ProblemaCondicoes *condicoes);

void testeInstancias(std::string nomePastaInstancias, std::string nomePastaDestino);


#endif