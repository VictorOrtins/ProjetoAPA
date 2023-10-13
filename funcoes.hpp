#ifndef FUNCOES_H
#define FUNCOES_H

#include "Solucao.hpp"
#include "ProblemaCondicoes.hpp"
#include "guloso.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <filesystem>

std::vector<int> split(const std::string& text);

ProblemaCondicoes* leArquivo(std::string nomeArquivo);

Solucao* criaSolucao();

bool escreveArquivo(std::string arquivoDestino, Solucao* solucao);

void testeInstancias(std::string nomePastaInstancias, std::string nomePastaDestino);


#endif