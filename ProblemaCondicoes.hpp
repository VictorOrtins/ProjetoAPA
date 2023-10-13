#ifndef PROBLEMA_CONDICOES_H
#define PROBLEMA_CONDICOES_H

#include <vector>
#include <iostream>

class ProblemaCondicoes {
    private:
        int totalEntregas; // Total de Entregas. Logo, é também o total de vértices que receberão entregas. O total de vértices
        //é n+1, já que é preciso considerar também o depósito como vértice
        int totalVeiculos; // Total de Veículos
        int capacidadeVeiculo; // Capacidade de cada Veículo
        int minimoEntregas; // Mínimo de Entregas sem terceirização
        int custoCarro; // Custo da utilização de um carro para a empresa
        std::vector<int> demandaClientes; // Demanda dos clientes por cada entrega
        std::vector<int> custoTerceirizacao; // Custos de terceirização para cada vértice
        std::vector<std::vector<int>> custoCaminho; // Custo associado a um caminho

    public:

        ProblemaCondicoes();
        // Getters
        int getTotalEntregas() const;

        int getTotalVeiculos() const;

        int getCapacidadeVeiculo() const;

        int getMinimoEntregas() const;

        int getCustoCarro() const;

        std::vector<int>& getDemandaClientes();

        std::vector<int>& getCustoTerceirizacao();

        std::vector<std::vector<int>>& getCustoCaminho();
        // Setters
        void setTotalEntregas(int n);

        void setTotalVeiculos(int k);

        void setCapacidadeVeiculo(int Q);

        void setMinimoEntregas(int L);

        void setCustoCarro(int r);

        void setDemandaClientes(const std::vector<int>& d);

        void setCustoTerceirizacao(const std::vector<int>& p);

        void setCustoCaminho(const std::vector<std::vector<int>>& c);

        void printaObjeto();
};

#endif