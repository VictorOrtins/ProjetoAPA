#ifndef SOLUCAO_H
#define SOLUCAO_H

#include <vector>

class Solucao {
    private:
        int valorSolucao;
        int custoRoteamento;
        int custoVeiculos;
        int custoTerceirizacao;
        std::vector<int> clientesTerceirizados;
        int numeroRotas;
        std::vector<std::vector<int>> rotas;

        void atualizaValorSolucao();

    public:
        Solucao();
        int getValorSolucao() const;

        int getCustoRoteamento() const;

        int getCustoVeiculos() const;

        int getCustoTerceirizacao() const;

        std::vector<int> getClientesTerceirizados() const;

        int getNumeroRotas() const;

        std::vector<std::vector<int>> getRotas() const;

        void setValorSolucao(int valor);

        void setCustoRoteamento(int custo);

        void setCustoVeiculos(int custo);

        void setCustoTerceirizacao(int custo);

        void setClientesTerceirizados(const std::vector<int>& clientes);

        void setNumeroRotas(int numero);

        void setRotas(const std::vector<std::vector<int>>& rotas);

        void printaSolucao();

        
};

#endif