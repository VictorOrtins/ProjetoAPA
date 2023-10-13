#include "vnd.hpp"

bool swap(ProblemaCondicoes *condicoes, Solucao *solucao){
    const int INFINITO = std::numeric_limits<int>::max();
    
    std::vector<Rota>& rotas = solucao->getRotas();
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho();

    int novoCustoRoteamento = 0;

    bool swapOcorreu = false;

    for(Rota &rota: rotas){
        std::vector<int>& vectorRota = rota.getRota();
        int custoRota = rota.getCustoRota();

        int melhor_i = -1, melhor_j = -1;
        int menorCusto = INFINITO;

        for(unsigned int i = 1; i < vectorRota.size() - 2; i++){
            int verticeI = vectorRota.at(i);
            int verticeAntesI = vectorRota.at(i - 1);
            int verticeDepoisI = vectorRota.at(i + 1);

            for(unsigned int j = i + 1; j < vectorRota.size() - 1; j++){
                int verticeJ = vectorRota.at(j);
                int verticeAntesJ = vectorRota.at(j - 1);
                int verticeDepoisJ = vectorRota.at(j + 1);

                int custoSwap;

                if(j == i + 1){
                    custoSwap = custoRota - custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeJ) 
                    - custoCaminhos.at(verticeJ).at(verticeDepoisJ) +
                    custoCaminhos.at(verticeAntesI).at(verticeJ) + custoCaminhos.at(verticeJ).at(verticeI) 
                    + custoCaminhos.at(verticeI).at(verticeDepoisJ);

                }
                else{
                    custoSwap = custoRota - custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeDepoisI) 
                    - custoCaminhos.at(verticeAntesJ).at(verticeJ) - custoCaminhos.at(verticeJ).at(verticeDepoisJ) +
                    custoCaminhos.at(verticeAntesI).at(verticeJ) + custoCaminhos.at(verticeJ).at(verticeDepoisI) 
                    + custoCaminhos.at(verticeAntesJ).at(verticeI) + custoCaminhos.at(verticeI).at(verticeDepoisJ);
                }

                if(custoSwap < menorCusto){
                    melhor_i = i;
                    melhor_j = j;
                    menorCusto = custoSwap;
                }
            }
        }

        if(menorCusto < custoRota){
            trocaVertices(melhor_i, melhor_j, menorCusto, vectorRota);
            rota.setCustoRota(menorCusto);
            novoCustoRoteamento += menorCusto;
            swapOcorreu = true;
        }
        else{
            novoCustoRoteamento += custoRota;
        }
    }

    solucao->setCustoRoteamento(novoCustoRoteamento);
    return swapOcorreu;
}

void trocaVertices(int melhor_i, int melhor_j, int menor_custo, std::vector<int> &rota){
    int temp = rota.at(melhor_i);
    rota.at(melhor_i) = rota.at(melhor_j);
    rota.at(melhor_j) = temp;
}