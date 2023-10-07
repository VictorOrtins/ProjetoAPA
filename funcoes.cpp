#include "funcoes.hpp"


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

Solucao* algoritmoGuloso(ProblemaCondicoes *condicoes){

    Solucao* solucao = new Solucao();

    const int INFINITO = std::numeric_limits<int>::max();

    std::vector<std::vector<int>> custoCaminhos = condicoes->getCustoCaminho();
    std::vector<int> demandas = condicoes->getDemandaClientes();

    int capacidadeMaximaVeiculo = condicoes->getCapacidadeVeiculo();
    int numRotas = condicoes->getTotalVeiculos();
    std::vector<int> custoTerceirizar = condicoes->getCustoTerceirizacao();

    std::vector<int> verticesRestantes;

    for(int i = 1; i <= condicoes->getTotalEntregas(); i++){
        verticesRestantes.push_back(i);
    }

    int verticeAtual = 0;
    int rotaAtual = 0;

    std::vector<int> capacidadeAtualRota;

    for(int i = 0; i < numRotas; i++){
        capacidadeAtualRota.push_back(0);
    }

    int custoAtualSolucao = 0;
    int custoAtualTerceirizacao = 0;

    std::vector<int> clientesTerceirizados;
    std::vector<int> rota;

    while(verticesRestantes.size() > 0){
        std::cout << verticeAtual << " ";
        std::vector<int> verticeCustos = custoCaminhos.at(verticeAtual);

        if(capacidadeAtualRota.at(rotaAtual) == capacidadeMaximaVeiculo){
            if(rotaAtual + 1 == numRotas){
               for(int vertice : verticesRestantes){
                    custoAtualTerceirizacao += custoTerceirizar.at(vertice - 1);
                    clientesTerceirizados.push_back(vertice);
                }
                break;
            }

            custoAtualSolucao += verticeCustos.at(0);
            verticeAtual = 0;
            solucao->addRota(rota);
            rota.clear();
            rotaAtual++;
            continue;
        }

        int verticeMenorCusto = -1;
        int menorCusto = INFINITO;
        

        for(int vertice: verticesRestantes){
            if(verticeCustos.at(vertice) < menorCusto && capacidadeAtualRota.at(rotaAtual) + demandas.at(vertice - 1) <= capacidadeMaximaVeiculo){
                verticeMenorCusto = vertice;
                menorCusto = verticeCustos.at(vertice);
            }
        }

        if(menorCusto == INFINITO){
            if(rotaAtual + 1 == numRotas){
               for(int vertice : verticesRestantes){
                    custoAtualTerceirizacao += custoTerceirizar.at(vertice - 1);
                    clientesTerceirizados.push_back(vertice);
                }
                break;
            }

            custoAtualSolucao += verticeCustos.at(0);
            verticeAtual = 0;
            solucao->addRota(rota);
            rota.clear();
            rotaAtual++;
            continue;
        }

        custoAtualSolucao += verticeCustos.at(verticeMenorCusto);
        capacidadeAtualRota.at(rotaAtual) += demandas.at(verticeMenorCusto - 1);
        rota.push_back(verticeMenorCusto);
        verticeAtual = verticeMenorCusto;

        for (auto it = verticesRestantes.begin(); it != verticesRestantes.end();) {
            if (*it == verticeMenorCusto) {
                verticesRestantes.erase(it);
                break;
            } else {
                ++it;
            }
        }

    }

    custoAtualSolucao += custoCaminhos.at(verticeAtual).at(0);
    capacidadeAtualRota.at(rotaAtual) += demandas.at(verticeAtual - 1);
    solucao->addRota(rota);

    solucao->setCustoRoteamento(custoAtualSolucao);
    solucao->setCustoTerceirizacao(custoAtualTerceirizacao);
    solucao->setCustoVeiculos( (rotaAtual + 1)*condicoes->getCustoCarro());
    solucao->setNumeroRotas( (rotaAtual + 1));
    solucao->setClientesTerceirizados(clientesTerceirizados);

    return solucao;
}