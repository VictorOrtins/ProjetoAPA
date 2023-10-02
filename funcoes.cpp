#include "funcoes.hpp"
#include <fstream>

std::vector<int> split(const std::string& text, char sep)
{
    std::vector<int> tokens;
    std::size_t start = 0, end = 0;

    while ((end = text.find(sep, start)) != std::string::npos)
    {
        std::string temp = text.substr(start, end - start);

        try {
            int numero = std::stoi(temp);
            tokens.push_back(numero);
        } catch (const std::invalid_argument& e) {
            throw e;
        } catch (const std::out_of_range& e) {
            throw e;
        }

        start = end + 1;
    }

    std::string temp = text.substr(start);

    try {
        int numero = std::stoi(temp);
        tokens.push_back(numero);
    } catch (const std::invalid_argument& e) {
        throw e;
    } catch (const std::out_of_range& e) {
        throw e;
    }

    return tokens;
}

ProblemaCondicoes* leArquivo(std::string nomeArquivo){
    ProblemaCondicoes* condicoes = new ProblemaCondicoes();

    std::ifstream arquivo;
    arquivo.open(nomeArquivo);

    if(!arquivo.is_open()){
        std::cout << "Erro no arquivo!\n";
        return nullptr;
    }

    int temp;
    arquivo >> temp;
    condicoes->setN(temp);

    arquivo >> temp;
    condicoes->setK(temp);

    arquivo >> temp;
    condicoes->setQ(temp);

    arquivo >> temp;
    condicoes->setL(temp);

    arquivo >> temp;
    condicoes->setR(temp);

    std::string tempString;
    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);

    std::vector<int> tempVector = split(tempString, ' ');
    condicoes->setD(tempVector);

    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);

    tempVector = split(tempString, ' ');
    condicoes->setP(tempVector);

    std::vector<std::vector<int>> tempMatriz;

    std::getline(arquivo, tempString);
    for (int i = 0; i < condicoes->getN() + 1; i++){
        std::getline(arquivo, tempString);
        tempVector = split(tempString, ' ');
        tempMatriz.push_back(tempVector);
    }

    condicoes->setC(tempMatriz);

    arquivo.close();
    return condicoes;
}