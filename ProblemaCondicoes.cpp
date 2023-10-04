#include "ProblemaCondicoes.hpp"

ProblemaCondicoes::ProblemaCondicoes(){
    this->totalEntregas = 0;
    this->totalVeiculos = 0;
    this->capacidadeVeiculo = 0;
    this->minimoEntregas = 0;
    this->custoCarro = 0;
}

int ProblemaCondicoes::getTotalEntregas() const {
    return totalEntregas;
}

int ProblemaCondicoes::getTotalVeiculos() const {
    return totalVeiculos;
}

int ProblemaCondicoes::getCapacidadeVeiculo() const {
    return capacidadeVeiculo;
}

int ProblemaCondicoes::getMinimoEntregas() const {
    return minimoEntregas;
}

int ProblemaCondicoes::getCustoCarro() const {
    return custoCarro;
}

std::vector<int> ProblemaCondicoes::getDemandaClientes() const {
    return demandaClientes;
}

std::vector<int> ProblemaCondicoes::getCustoTerceirizacao() const {
    return custoTerceirizacao;
}

std::vector<std::vector<int>> ProblemaCondicoes::getCustoCaminho() const {
    return custoCaminho;
}

// Setters
void ProblemaCondicoes::setTotalEntregas(int n) {
    this->totalEntregas = n;
}

void ProblemaCondicoes::setTotalVeiculos(int k) {
    this->totalVeiculos = k;
}

void ProblemaCondicoes::setCapacidadeVeiculo(int Q) {
    this->capacidadeVeiculo = Q;
}

void ProblemaCondicoes::setMinimoEntregas(int L) {
    this->minimoEntregas = L;
}

void ProblemaCondicoes::setCustoCarro(int r) {
    this->custoCarro = r;
}

void ProblemaCondicoes::setDemandaClientes(const std::vector<int>& d) {
    this->demandaClientes = d;
}

void ProblemaCondicoes::setCustoTerceirizacao(const std::vector<int>& p) {
    this->custoTerceirizacao = p;
}

void ProblemaCondicoes::setCustoCaminho(const std::vector<std::vector<int>>& c) {
    this->custoCaminho = c;
}

void ProblemaCondicoes::printaObjeto(){
    std::cout << "Total de Entregas: " << totalEntregas << "\n";
    std::cout << "Total de Veiculos: " << totalVeiculos << "\n";
    std::cout << "Capacidade de cada Veiculo: " << capacidadeVeiculo << "\n";
    std::cout << "Minimo de Entregas sem terceirizar: " << minimoEntregas << "\n";
    std::cout << "Custo de Utilizacao de cada Carro: " << custoCarro << "\n";

    std::cout << "Demanda dos clientes: ";
    for (int i : demandaClientes){
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "Custo de Terceirizacao: ";
    for (int i : custoTerceirizacao){
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "Custo de cada Caminho: ";
    for (std::vector<int> i : custoCaminho){
        std::cout << "\n";
        for (int j : i){
            std::cout << j << " ";
        }
    }
}