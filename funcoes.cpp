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
    condicoes->setTotalEntregas(temp);

    arquivo >> temp;
    condicoes->setTotalVeiculos(temp);

    arquivo >> temp;
    condicoes->setCapacidadeVeiculo(temp);

    arquivo >> temp;
    condicoes->setMinimoEntregas(temp);

    arquivo >> temp;
    condicoes->setCustoCarro(temp);

    std::string tempString;
    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);

    std::vector<int> tempVector = split(tempString, ' ');
    condicoes->setDemandaClientes(tempVector);

    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);

    tempVector = split(tempString, ' ');
    condicoes->setCustoTerceirizacao(tempVector);

    std::vector<std::vector<int>> tempMatriz;

    std::getline(arquivo, tempString);
    for (int i = 0; i < condicoes->getTotalEntregas() + 1; i++){
        std::getline(arquivo, tempString);
        tempVector = split(tempString, ' ');
        tempMatriz.push_back(tempVector);
    }

    condicoes->setCustoCaminho(tempMatriz);

    arquivo.close();
    return condicoes;
}

Solucao* criaSolucao(){
    Solucao* solucao = new Solucao();

    solucao->setCustoRoteamento(23);
    solucao->setCustoVeiculos(10);
    solucao->setCustoTerceirizacao(3);
    solucao->setClientesTerceirizados(std::vector<int>{6});
    solucao->setNumeroRotas(2);
    solucao->setRotas(std::vector<std::vector<int>>{ {1,2}, {3,4,5} });

    return solucao;
}

bool escreveArquivo(std::string arquivoDestino, Solucao* solucao){
    std::ofstream arquivo(arquivoDestino);

    if(!arquivo.is_open()){
        std::cout << "Problema ao escrever no arquivo!";
        return false;
    }

    arquivo << solucao->getValorSolucao() << std::endl;
    arquivo << solucao->getCustoRoteamento() << std::endl;
    arquivo << solucao->getCustoVeiculos() << std::endl;
    arquivo << solucao->getCustoTerceirizacao() << std::endl;
    arquivo << std::endl;

    for(int i : solucao->getClientesTerceirizados()){
        arquivo << i << " ";
    }

    arquivo << std::endl;
    arquivo << std::endl;

    arquivo << solucao->getNumeroRotas();
    for(std::vector<int> temp : solucao->getRotas()){
        arquivo << std::endl;
        for(int i: temp){
            arquivo << i << " ";
        }
    }

    return true;
}