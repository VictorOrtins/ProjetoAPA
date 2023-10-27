#include "Rota.hpp"

Rota::Rota(){
    this->custoRota = 0;
    this->capacidadeAtualRota = 0;
}

void Rota::setRota(std::vector<int> rota){
    this->rota = rota;
}

void Rota::setCustoRota(int custoRota){
    this->custoRota = custoRota;
}

void Rota::setCapacidadeAtualRota(int capacidadeRota){
    this->capacidadeAtualRota = capacidadeRota;
}

std::vector<int>& Rota::getRota(){
    return this->rota;
}

int Rota::getCustoRota(){
    return this->custoRota;
}

int Rota::getCapacidadeAtualRota(){
    return this->capacidadeAtualRota;
}

void Rota::addVertice(int vertice){
    rota.push_back(vertice);
}

void Rota::clearRota(){
    rota.clear();
    this->custoRota = 0;
    this->capacidadeAtualRota = 0;
}

void Rota::printRota(){
    for(int i : rota){
        std::cout << i << " ";
    }
    std::cout << "\n";
}

void Rota::atualizarCustoRota(ProblemaCondicoes *condicoes){
    int novoCusto = 0;
    std::vector<std::vector<int>> &custoCaminhos = condicoes->getCustoCaminho();

    for(unsigned int i = 0; i < this->rota.size() - 1; i++){
        novoCusto += custoCaminhos.at(rota.at(i)).at(rota.at(i + 1));
    }
    this->custoRota = novoCusto;
}

