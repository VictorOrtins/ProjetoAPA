#ifndef ROTA_H
#define ROTA_H

#include <vector>
#include <iostream>

#include "ProblemaCondicoes.hpp"

class Rota{
    private:
        std::vector<int> rota;
        int custoRota;
        int capacidadeAtualRota;

    public:
        Rota();
        void setRota(std::vector<int> rota);
        void setCustoRota(int custoRota);
        void setCapacidadeAtualRota(int capacidadeRota);
        
        std::vector<int>& getRota();
        int getCustoRota();
        int getCapacidadeAtualRota();
        //Adiciona o vértice na rota
        void addVertice(int vertice);

        //Apaga os vértices da rota
        void clearRota();

        //Printa a rota
        void printRota();

        //Atualiza o custo da rota
        void atualizarCustoRota(ProblemaCondicoes *condicoes);

};

#endif