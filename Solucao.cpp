#include "Solucao.hpp"
#include <iostream>

Solucao::Solucao(){
    this->valorSolucao = 0;
    this->custoRoteamento = 0;
    this->custoVeiculos = 0;
    this->custoTerceirizacao = 0;
    this->entregasNaoTerceirizadas = 0;
    this->numeroRotas = 0;
}

Solucao::Solucao(Solucao* nova){
    this->valorSolucao = nova->valorSolucao;
    this->custoRoteamento = nova->custoRoteamento;
    this->custoVeiculos = nova->custoVeiculos;
    this->custoTerceirizacao = nova->custoTerceirizacao;
    this->entregasNaoTerceirizadas = nova->entregasNaoTerceirizadas;
    this->numeroRotas = nova->numeroRotas;

    this->clientesTerceirizados = nova->clientesTerceirizados;
    this->rotas = nova->rotas;
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

std::vector<int>& Solucao::getClientesTerceirizados(){
    return clientesTerceirizados;
}

int Solucao::getNumeroRotas() const{
    return numeroRotas;
}

std::vector<Rota>& Solucao::getRotas(){
    return rotas;
}

int Solucao::getEntregasNaoTerceirizadas(){
    return entregasNaoTerceirizadas;
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

void Solucao::setRotas(const std::vector<Rota>& rotas) {
    this->rotas = rotas;
    atualizaValorSolucao();
}

void Solucao::setEntregasNaoTerceirizadas(int entregasNaoTerceirizadas){
    this->entregasNaoTerceirizadas = entregasNaoTerceirizadas;
}

void Solucao::atualizaValorSolucao(){
    this->valorSolucao = this->custoRoteamento + this->custoTerceirizacao + this->custoVeiculos;
}

//Adiciona uma rota nas rotas
void Solucao::addRota(Rota rota){
    this->rotas.push_back(rota);
    this->numeroRotas++;
}

void Solucao::addClienteTerceirizado(int clienteTerceirizado){
    clientesTerceirizados.push_back(clienteTerceirizado);
}

void Solucao::removeRota(int indice){
    rotas.erase(rotas.begin() + indice);
    numeroRotas--;
}

void Solucao::removeClienteTerceirizado(int indice){
    this->clientesTerceirizados.erase(clientesTerceirizados.begin() + indice);
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
        Rota rota = rotas.at(i);
        for(int j: rota.getRota()){
            std::cout << j << " ";
        }
        std::cout << "\n";
    }
}