#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>
#include "Rota.hpp"

class Solucao {
    private:
        int valorSolucao; //Valor total da solução
        int custoRoteamento; //Custo apenas das rotas
        int custoVeiculos; //Custo da utilização dos veículos
        int custoTerceirizacao; //Custo da terceirização dos vértices
        std::vector<int> clientesTerceirizados; //Clientes que tiveram suas entregas terceirizadas
        int numeroRotas; //Número de rotas utilizadas
        std::vector<Rota> rotas; //Rotas utilizadas

        int entregasNaoTerceirizadas;

        void atualizaValorSolucao(); //Atualiza o valor da solução sempre que setCustoRoteamento,
        //setCustoVeiculos e setCustoTerceirizacao são chamados

    public:
        Solucao();
        int getValorSolucao() const;

        int getCustoRoteamento() const;

        int getCustoVeiculos() const;

        int getCustoTerceirizacao() const;

        std::vector<int>& getClientesTerceirizados();

        int getNumeroRotas() const;

        std::vector<Rota>& getRotas();

        int getEntregasNaoTerceirizadas();

        void setValorSolucao(int valor);

        void setCustoRoteamento(int custo);

        void setCustoVeiculos(int custo);

        void setCustoTerceirizacao(int custo);

        void setClientesTerceirizados(const std::vector<int>& clientes);

        void setNumeroRotas(int numero);

        void setRotas(const std::vector<Rota>& rotas);

        void setEntregasNaoTerceirizadas(int entregasNaoTerceirizadas);

        void printaSolucao();

        //Adiciona uma rota nas rotas
        void addRota(Rota rota);

        //Adiciona um cliente terceirizado
        void addClienteTerceirizado(int clienteTerceirizado);

        //Remove uma rota das rotas
        void removeRota(int indice);

        
};

#endif