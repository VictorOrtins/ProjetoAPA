#ifndef FUNCOES_H
#define FUNCOES_H

#include "Solucao.hpp"
#include "ProblemaCondicoes.hpp"
#include "guloso.hpp"
#include "vnd.hpp"

#include <vector>
#include <string>
#include <fstream>
#include <limits>
#include <algorithm>
#include <filesystem>

//Faz o split de uma string em um vector de inteiros
std::vector<int> split(const std::string& text);

//Lê o arquivo de instância. Ele precisa estar no formato descrito no sigaa para funcionar
ProblemaCondicoes* leArquivo(std::string nomeArquivo);

//Cria uma solução para testes
Solucao* criaSolucao();

//Escreve de volta no arquivo final
bool escreveArquivo(std::string arquivoDestino, Solucao* solucao);

//Testa todas as instâncias em uma pasta
void testeInstancias(std::string nomePastaInstancias, std::string nomePastaDestino);


#endif