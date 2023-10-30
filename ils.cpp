#include "ils.hpp"
#include "guloso.hpp"
#include "vnd.hpp"
#include "funcoes.hpp"
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

    RetornoILS *retornoILS = new RetornoILS();
    retornoILS->solucao = new Solucao(&solucaoAtual);
    retornoILS->tempo = seconds;

    return retornoILS;
}

void perturbacao(ProblemaCondicoes* condicoes, Solucao* solucao){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<> dist(1,5);

    
    for(int i = 0; i < dist(gen) + 1; i++){
        swapInterRotasAleatorio(condicoes, solucao);
    }

    for(int i = 0; i < dist(gen) + 3; i++){
        terceirizaAleatorio(condicoes, solucao);
    }

    for(int i = 0; i < dist(gen); i++){
        swapAleatorio(condicoes, solucao);
    }
}


void swapAleatorio(ProblemaCondicoes* condicoes, Solucao* solucao){
    std::random_device rd;
    std::mt19937 gen(rd());
    
    std::vector<Rota>& rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho

    std::uniform_int_distribution<> dist(0,rotas.size() - 1);

    int indexRota = dist(gen);

    std::vector<int> &rotaVector = rotas.at(indexRota).getRota();

    if(rotaVector.size() == 3){
        return;
    }

    std::uniform_int_distribution<> dist2(1,rotaVector.size() - 2);


    int indexVertice1 = dist2(gen);
    
    int indexVertice2 = dist2(gen);
    if (indexVertice1 == indexVertice2){
        return;
    }

    int custoAntes = rotas.at(indexRota).getCustoRota();
    trocaVertices(indexVertice1, indexVertice2, rotaVector); //Faz a troca dos vértices

    int custoDepois = 0;
    for(unsigned int i = 0; i < rotaVector.size() - 1; i++){
        custoDepois += custoCaminhos.at(rotaVector.at(i)).at(rotaVector.at(i+1));
    }

    rotas.at(indexRota).setCustoRota(custoDepois); //Atualiza o custo da rota
    solucao->setCustoRoteamento(solucao->getCustoRoteamento() + custoDepois - custoAntes); //Custo do roteamento é atualizado
}

void swapInterRotasAleatorio(ProblemaCondicoes* condicoes, Solucao* solucao){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::vector<Rota> &rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>> &custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho
    std::vector<int> &demandas = condicoes->getDemandaClientes();
    int capacidadeMaximaVeiculo = condicoes->getCapacidadeVeiculo();

    std::uniform_int_distribution<> dist(0,rotas.size() - 1);

    int indexRota1 = dist(gen);
    int indexRota2 = dist(gen);
    if(indexRota1 == indexRota2){
        return;
    }

    std::vector<int> &rota1Vector = rotas.at(indexRota1).getRota();
    std::vector<int> &rota2Vector = rotas.at(indexRota2).getRota();

    std::uniform_int_distribution<> dist2(1,rota1Vector.size() - 2);
    int indexVerticeRota1 = dist2(gen);

    std::uniform_int_distribution<> dist3(1,rota2Vector.size() - 2);
    int indexVerticeRota2 = dist3(gen);

    int deltaCapacidadeRota1 = - demandas.at(rota1Vector.at(indexVerticeRota1) - 1) + demandas.at(rota2Vector.at(indexVerticeRota2) - 1);
    int deltaCapacidadeRota2 = + demandas.at(rota1Vector.at(indexVerticeRota1) - 1) - demandas.at(rota2Vector.at(indexVerticeRota2) - 1);

    if( (rotas.at(indexRota1).getCapacidadeAtualRota() + deltaCapacidadeRota1) > capacidadeMaximaVeiculo
    || (rotas.at(indexRota2).getCapacidadeAtualRota() + deltaCapacidadeRota2) > capacidadeMaximaVeiculo){
        return;
    }

    int custoAntesRota1 = rotas.at(indexRota1).getCustoRota();
    int custoAntesRota2 = rotas.at(indexRota2).getCustoRota();

    trocaInterRota(indexVerticeRota1, indexVerticeRota2, rota1Vector, rota2Vector);

    int custoDepoisRota1 = 0;
    int custoDepoisRota2 = 0;

    for(unsigned int i = 0; i < rota1Vector.size() - 1; i++){
        custoDepoisRota1 += custoCaminhos.at(rota1Vector.at(i)).at(rota1Vector.at(i + 1));
    }

    for(unsigned int i = 0; i < rota2Vector.size() - 1; i++){
        custoDepoisRota2 += custoCaminhos.at(rota2Vector.at(i)).at(rota2Vector.at(i + 1));
    }

    rotas.at(indexRota1).setCustoRota(custoDepoisRota1);
    rotas.at(indexRota2).setCustoRota(custoDepoisRota2);

    rotas.at(indexRota1).setCapacidadeAtualRota(rotas.at(indexRota1).getCapacidadeAtualRota() + deltaCapacidadeRota1);
    rotas.at(indexRota2).setCapacidadeAtualRota(rotas.at(indexRota2).getCapacidadeAtualRota() + deltaCapacidadeRota2);

    solucao->setCustoRoteamento(solucao->getCustoRoteamento() + custoDepoisRota1 - custoAntesRota1 + custoDepoisRota2 - custoAntesRota2);
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
