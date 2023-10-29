#include "ils.hpp"
#include "guloso.hpp"
#include "vnd.hpp"
#include <cstdlib>
#include <ctime>
#include <random>

RetornoILS* ILS(ProblemaCondicoes* condicoes, int maxIter){
    auto inicio = std::chrono::high_resolution_clock::now();

    RetornoGuloso* retorno = algoritmoGuloso(condicoes);
    Solucao solucaoAtual = *retorno->solucao;

    VND(condicoes, &solucaoAtual);

    Solucao solucaoTemp;

    for(int i = 0; i < maxIter; i++){
        solucaoTemp = solucaoAtual;

        perturbacao(condicoes, &solucaoTemp);

        VND(condicoes, &solucaoTemp);

        if(solucaoTemp.getValorSolucao() < solucaoAtual.getValorSolucao()){
            solucaoAtual = Solucao(&solucaoTemp);
        }
    }
    auto resultado = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - inicio);
    double seconds = static_cast<double>(resultado.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

    solucaoAtual.printaSolucao();

    RetornoILS *retornoILS = new RetornoILS();
    retornoILS->solucao = new Solucao(&solucaoAtual);
    retornoILS->tempo = seconds;

    return retornoILS;
}

void perturbacao(ProblemaCondicoes* condicoes, Solucao* solucao){

    terceirizaAleatorio(condicoes, solucao);

    swapAleatorio(condicoes, solucao);

}


void swapAleatorio(ProblemaCondicoes* condicoes, Solucao* solucao){
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::vector<Rota>& rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho

    int indexRota = std::rand() % rotas.size();

    std::uniform_int_distribution<> dist(0,rotas.size() - 1);

    std::vector<int> &rotaVector = rotas.at(indexRota).getRota();

    if(rotaVector.size() == 3){
        if(rotas.size() == 1){
            return;
        }
        indexRota = dist(gen);
        rotaVector = rotas.at(indexRota).getRota();
    }

    std::uniform_int_distribution<> dist2(1,rotaVector.size() - 2);


    int indexVertice1 = dist2(gen);
    
    int indexVertice2 = dist2(gen);
    while(indexVertice2 == indexVertice1){
        indexVertice2 = dist2(gen);
    }

    int vertice1, verticeAntes1, verticeDepois1;
    int vertice2, verticeAntes2, verticeDepois2;

    vertice1 = rotaVector.at(indexVertice1);

    verticeAntes1 = rotaVector.at(indexVertice1 - 1);
    verticeDepois1 = rotaVector.at(indexVertice1 + 1);


    vertice2 = rotaVector.at(indexVertice2);
    verticeAntes2 = rotaVector.at(indexVertice2 - 1);
    verticeDepois2 = rotaVector.at(indexVertice2 + 1);

    int custoSwapDelta;

    if(indexVertice2 == indexVertice1 + 1){ //Se os vértices são adjacentes. Há intersecção de arestas nesse caso, então o cálculo é menorr
        custoSwapDelta = -custoCaminhos.at(verticeAntes1).at(vertice1) - custoCaminhos.at(vertice1).at(vertice2) 
        - custoCaminhos.at(vertice2).at(verticeDepois2) +
        custoCaminhos.at(verticeAntes1).at(vertice2) + custoCaminhos.at(vertice2).at(vertice1) 
        + custoCaminhos.at(vertice1).at(verticeDepois2);

        //-Ci-1,i - Ci,j - Cj,j+1 + Ci-1,j + Cj,i + Ci,j + 1
    }
    else{ //Se os vértices não são adjacentes
        custoSwapDelta = -custoCaminhos.at(verticeAntes1).at(vertice1) - custoCaminhos.at(vertice1).at(verticeDepois1) 
        - custoCaminhos.at(verticeAntes2).at(vertice2) - custoCaminhos.at(vertice2).at(verticeDepois2) +
        custoCaminhos.at(verticeAntes1).at(vertice2) + custoCaminhos.at(vertice2).at(verticeDepois1) 
        + custoCaminhos.at(verticeAntes2).at(vertice1) + custoCaminhos.at(vertice1).at(verticeDepois2);

        //-Ci-1,i - Ci,i+1 - Cj-1,j - Cj,j+1 + Ci-1,j Cj,i+1 + Cj-1,i = Ci,j+1
    }

    trocaVertices(indexVertice1, indexVertice2, rotaVector); //Faz a troca dos vértices
    rotas.at(indexRota).setCustoRota(rotas.at(indexRota).getCustoRota() + custoSwapDelta); //Atualiza o custo da rota
    solucao->setCustoRoteamento(solucao->getCustoRoteamento() + custoSwapDelta); //Custo do roteamento é atualizado
}


void terceirizaAleatorio(ProblemaCondicoes* condicoes, Solucao* solucao){
    std::random_device rd;
    std::mt19937 gen(rd());

    if(solucao->getEntregasNaoTerceirizadas() == condicoes->getMinimoEntregas()){ //Se a solução não pode mais terceirizar entregas
        return;
    }

    std::vector<Rota> &rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>> &custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho
    std::vector<int> &custoTerceirizacao = condicoes->getCustoTerceirizacao(); //Pega os custos de terceirização de um vértice
    std::vector<int> &demandas = condicoes->getDemandaClientes();

    std::uniform_int_distribution<> dist(0,rotas.size() - 1);

    int indexRota = dist(gen);

    std::vector<int> &rotaVector = rotas.at(indexRota).getRota();


    std::uniform_int_distribution<> dist2(1,rotaVector.size() - 2);

    int indexVertice = dist2(gen);

    int verticeI, verticeAntesI, verticeDepoisI;

    verticeI = rotaVector.at(indexVertice);
    verticeAntesI = rotaVector.at(indexVertice - 1);
    verticeDepoisI = rotaVector.at(indexVertice + 1);

    int deltaRoteamento, custoTerceirizarDelta;

    deltaRoteamento = -custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeDepoisI) +
                        custoCaminhos.at(verticeAntesI).at(verticeDepoisI); 
    // -Ci-1,i - Ci,i+1 + Ci-1,i+1

    custoTerceirizarDelta = custoTerceirizacao.at(verticeI - 1); //Pega-se i-1 pela indexação do vector de terceirização
    // T(i)
    solucao->addClienteTerceirizado(rotas.at(indexRota).getRota().at(indexVertice)); //Adiciona o cliente terceirizado ao vector de clientes terceirizados

    //Atualiza a capacidade atual da rota
    int verticeMelhorI = rotas.at(indexRota).getRota().at(indexVertice);
    rotas.at(indexRota).setCapacidadeAtualRota(rotas.at(indexRota).getCapacidadeAtualRota() - demandas.at(verticeMelhorI - 1));
    
    //Apaga da rota o vértice terceirizado
    rotas.at(indexRota).getRota().erase(rotas.at(indexRota).getRota().begin() + indexVertice);

    rotas.at(indexRota).setCustoRota(rotas.at(indexRota).getCustoRota() + deltaRoteamento);

    //Atualiza o custo de roteamento
    solucao->setCustoRoteamento(solucao->getCustoRoteamento() + deltaRoteamento);

    //Atualiza o custo de terceirização
    solucao->setCustoTerceirizacao(solucao->getCustoTerceirizacao() + custoTerceirizarDelta);

    //Já que houve terceirização, diminui o número de entregas não terceirizadas
    solucao->setEntregasNaoTerceirizadas(solucao->getEntregasNaoTerceirizadas() - 1);

    if (rotas.at(indexRota).getRota().size() == 2){ //Se a rota só tem 0-0, essa rota não existe mais e precisa ser excluída
        solucao->removeRota(indexRota); //Remove da rota a solução
        solucao->setCustoVeiculos(solucao->getCustoVeiculos() - condicoes->getCustoCarro());
    }
}
