#ifndef ROTA_H
#define ROTA_H

#include <vector>
#include <iostream>

#include "ProblemaCondicoes.hpp"

class Rota{
    private:
        std::vector<int> rota;
        int custoRota;

    public:
        Rota();
        void setRota(std::vector<int> rota);
        void setCustoRota(int custoRota);
        std::vector<int>& getRota();
        int getCustoRota();
        void addVertice(int vertice);
        void clearRota();
        void printRota();
        void atualizarCustoRota(ProblemaCondicoes *condicoes);

};

#endif