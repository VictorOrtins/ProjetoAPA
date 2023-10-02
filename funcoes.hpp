#ifndef FUNCOES_H
#define FUNCOES_H

#include <vector>
#include "ProblemaCondicoes.hpp"
#include <string>

std::vector<int> split(const std::string& text, char sep);

ProblemaCondicoes* leArquivo(std::string nomeArquivo);

#endif