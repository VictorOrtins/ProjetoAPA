#ifndef FUNCOES_H
#define FUNCOES_H

#include <vector>
#include "ProblemaCondicoes.hpp"
#include <string>
#include "Solucao.hpp"
#include <fstream>
#include <limits>

std::vector<int> split(const std::string& text, char sep);

ProblemaCondicoes* leArquivo(std::string nomeArquivo);

Solucao* criaSolucao();

bool escreveArquivo(std::string arquivoDestino, Solucao* solucao);

Solucao* algoritmoGuloso(ProblemaCondicoes *condicoes);


#endif