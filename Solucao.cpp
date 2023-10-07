#include "Solucao.hpp"
#include <iostream>

Solucao::Solucao(){
    this->valorSolucao = 0;
    this->custoRoteamento = 0;
    this->custoVeiculos = 0;
    this->custoTerceirizacao = 0;
}
 
 int Solucao::getValorSolucao() const {
    return valorSolucao;
}

int Solucao::getCustoRoteamento() const {
    return custoRoteamento;
}

int Solucao::getCustoVeiculos() const {
    return custoVeiculos;
}

int Solucao::getCustoTerceirizacao() const {
    return custoTerceirizacao;
}

const std::vector<int>& Solucao::getClientesTerceirizados() const {
    return clientesTerceirizados;
}

int Solucao::getNumeroRotas() const {
    return numeroRotas;
}

const std::vector<std::vector<int>>& Solucao::getRotas() const {
    return rotas;
}

void Solucao::setCustoRoteamento(int custo) {
    this->custoRoteamento = custo;
    atualizaValorSolucao();
}

void Solucao::setCustoVeiculos(int custo) {
    this->custoVeiculos = custo;
    atualizaValorSolucao();
}

void Solucao::setCustoTerceirizacao(int custo) {
    this->custoTerceirizacao = custo;
    atualizaValorSolucao();
}

void Solucao::setClientesTerceirizados(const std::vector<int>& clientes) {
    this->clientesTerceirizados = clientes;
    atualizaValorSolucao();
}

void Solucao::setNumeroRotas(int numero) {
    this->numeroRotas = numero;
    atualizaValorSolucao();
}

void Solucao::setRotas(const std::vector<std::vector<int>>& rotas) {
    this->rotas = rotas;
    atualizaValorSolucao();
}

void Solucao::atualizaValorSolucao(){
    this->valorSolucao = this->custoRoteamento + this->custoTerceirizacao + this->custoVeiculos;
}

void Solucao::addRota(std::vector<int> rota){
    this->rotas.push_back(rota);
}

void Solucao::printaSolucao(){
    std::cout << "Valor Total da Solução: "<<  this->valorSolucao << "\n";
    std::cout << "Custo de Roteamento: " << this->custoRoteamento << "\n";
    std::cout << "Custo associado a utilizacao dos veiculos: " << this->custoVeiculos << "\n";
    std::cout << "Custo de Terceirizacao: "  << this->custoTerceirizacao << "\n";
    std::cout << "Lista de Clientes Terceirizados: ";

    for (int i : clientesTerceirizados){
        std::cout << i << " ";
    }

    std::cout << "\n";
    std::cout << "Numero de Rotas: " << numeroRotas << "\n";

    for (int i = 0; i < numeroRotas; i++){
        std::cout << "Rota " << i + 1 << ": ";
        for(int j: rotas[i]){
            std::cout << j << " ";
        }
        std::cout << "\n";
    }
}