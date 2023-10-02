#ifndef PROBLEMA_CONDICOES_H
#define PROBLEMA_CONDICOES_H

#include <vector>
#include <iostream>

class ProblemaCondicoes {
    private:
        int n; // Total de Entregas. Logo, é também o total de vértices que receberão entregas. O total de vértices
        //é n+1, já que é preciso considerar também o depósito como vértice
        int k; // Total de Veículos
        int Q; // Capacidade de cada Veículo
        int L; // Mínimo de Entregas sem terceirização
        int r; // Custo da utilização de um carro para a empresa
        std::vector<int> d; // Demanda dos clientes por cada entrega
        std::vector<int> p; // Custos de terceirização para cada vértice
        std::vector<std::vector<int>> c; // Custo associado a um caminho

    public:
        // Getters
        int getN() const;

        int getK() const;

        int getQ() const;

        int getL() const;

        int getR() const;

        std::vector<int> getD() const;

        std::vector<int> getP() const;

        std::vector<std::vector<int>> getC() const;
        // Setters
        void setN(int n);

        void setK(int k);

        void setQ(int Q);

        void setL(int L);

        void setR(int r);

        void setD(const std::vector<int>& d);

        void setP(const std::vector<int>& p);

        void setC(const std::vector<std::vector<int>>& c);

        void printaObjeto();
};

#endif