#include "guloso.hpp"

RetornoGuloso* algoritmoGuloso(ProblemaCondicoes *condicoes){

    auto inicio = std::chrono::high_resolution_clock::now();

    Solucao* solucao = new Solucao(); //Cria a solução

    const int INFINITO = std::numeric_limits<int>::max();

    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Pega a matriz de custos
    std::vector<int>& demandas = condicoes->getDemandaClientes(); //Pega o vetor de demandas

    int capacidadeMaximaVeiculo = condicoes->getCapacidadeVeiculo(); //Pega a capacidade máxima por veículo
    int numRotas = condicoes->getTotalVeiculos(); //Pega o máximo de rotas que podem ser feitas
    std::vector<int>& custoTerceirizar = condicoes->getCustoTerceirizacao(); //Pega os custos de terceirização

    std::vector<int> verticesRestantes; //Inicia o vector auxiliar de vértices restantes. Será iniciado
    //com todos os vértices

    for(int i = 1; i <= condicoes->getTotalEntregas(); i++){
        verticesRestantes.push_back(i);
    }

    int verticeAtual = 0; //Vértice inicial sempre será o depósito
    int rotaAtual = 0; //As rotas serão indexadas começando de 0 para facilitar o entendimento

    std::vector<int> capacidadeAtualRota; //Capacidade atual que a rota possui

    for(int i = 0; i < numRotas; i++){
        capacidadeAtualRota.push_back(0); //Todas as rotas não tem nada, logo começam com 0
    }

    int custoAtualSolucao = 0; //Custo inicial da solução
    int custoAtualTerceirizacao = 0; //Custo da terceirização

    std::vector<int> clientesTerceirizados; //Vector para armazenar os clientes que foram terceirizados
    Rota rota; //Vector auxiliar para armazenar os vértices da rota atual 

    rota.addVertice(0);
    while(verticesRestantes.size() > 0){ //O loop principal continua até que não tenha vértices restantes
        std::vector<int> &verticeCustos = custoCaminhos.at(verticeAtual); //Vector auxiliar

        //que armazenas os custos para sair do vértice atual
        //Se a rota atual conseguiu adicionar o vértice atual, porém encheu após isso
        if(capacidadeAtualRota.at(rotaAtual) == capacidadeMaximaVeiculo){
            if(rotaAtual + 1 == numRotas){ //Se as rotas acabaram
                //Terceirizar o resto dos vértices
                terceiriza(verticesRestantes, custoAtualTerceirizacao, custoTerceirizar, clientesTerceirizados);
                break;
            }

            //Se ainda há rotas, terminar a rota. Ou seja, sair do vértice atual e ir pro 0
            trocaRota(custoAtualSolucao, verticeCustos, verticeAtual, solucao, rota, rotaAtual, custoCaminhos);
            continue;
        }

        //Agora a ideia é procurar o vértice de menor custo

        int verticeMenorCusto = -1;
        int menorCusto = INFINITO; //Como não temos ainda menor custo, setamos em infinito
        

        for(int vertice: verticesRestantes){ //Dentre os vértices restantes
            //Procuramos o que pode ser adicionado pensando na capacidade e o que tem o menor custo
            //Se ele couber e tiver menos custo que o vértice de menor custo atual
            if(verticeCustos.at(vertice) < menorCusto && capacidadeAtualRota.at(rotaAtual) + demandas.at(vertice - 1) <= capacidadeMaximaVeiculo){
                //Atualiza o vértice de menor custo e o menor custo
                verticeMenorCusto = vertice;
                menorCusto = verticeCustos.at(vertice);
            }
        }

        //Se não é mais possível adicionar vértices na rota atual
        if(menorCusto == INFINITO){
            //Terceiriza
            if(rotaAtual + 1 == numRotas){
                terceiriza(verticesRestantes, custoAtualTerceirizacao, custoTerceirizar, clientesTerceirizados);
            }

            //Coloca o 0 no fim da rota atual e troca para a próxima
            trocaRota(custoAtualSolucao, verticeCustos, verticeAtual, solucao, rota, rotaAtual, custoCaminhos);
            continue;
        }

        //Já que foi achado o vértice a ser adicionado, coloca na solução
        custoAtualSolucao += verticeCustos.at(verticeMenorCusto); //Aumenta o custo da solução pela adição do vértice
        capacidadeAtualRota.at(rotaAtual) += demandas.at(verticeMenorCusto - 1); //Aumenta a capacidade atual da rota
        //Note que, como as demandas tem tamanho da qtd de entregas, ou seja, n, precisamos utilizar o -1
        //pra pegar o índice correto

        rota.addVertice(verticeMenorCusto); //Adiciona o vértice de menor custo na rota
        rota.setCustoRota(rota.getCustoRota() + verticeCustos.at(verticeMenorCusto));
        
        verticeAtual = verticeMenorCusto; //Vai para o vértice adicionado

        //Itera sobre o vector para achar o vértice removido
        for (auto it = verticesRestantes.begin(); it != verticesRestantes.end();) {
            if (*it == verticeMenorCusto) { //Se o vértice de menor custo foi achado
                verticesRestantes.erase(it);
                break;
            } else {
                ++it;
            }
        }

    }

    //Fecha a última rota, ou seja, após adicionar o último vértice, volta a última rota para o depósito
    custoAtualSolucao += custoCaminhos.at(verticeAtual).at(0);
    capacidadeAtualRota.at(rotaAtual) += demandas.at(verticeAtual - 1);

    rota.setCustoRota(rota.getCustoRota() + custoCaminhos.at(verticeAtual).at(0));
    rota.addVertice(0);

    solucao->addRota(rota);

    //Atualiza a solução com os valores encontrados
    solucao->setCustoRoteamento(custoAtualSolucao); //Custo das rotas
    solucao->setCustoTerceirizacao(custoAtualTerceirizacao); //Custo de terceirizar
    solucao->setCustoVeiculos( (rotaAtual + 1)*condicoes->getCustoCarro()); //Custo dos veículos
    solucao->setNumeroRotas( (rotaAtual + 1)); //Quantidade de rotas utilizadas
    solucao->setClientesTerceirizados(clientesTerceirizados); //Coloca os clientes terceirizados

    auto resultado = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - inicio);
    double seconds = static_cast<double>(resultado.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

    RetornoGuloso* retornoGuloso = new RetornoGuloso();
    retornoGuloso->solucao = solucao;
    retornoGuloso->seconds = seconds;

    return retornoGuloso;
}

void terceiriza(std::vector<int> &verticesRestantes, int custoAtualTerceirizacao, std::vector<int> &custoTerceirizar, std::vector<int> &clientesTerceirizados){
    for(int vertice : verticesRestantes){
        custoAtualTerceirizacao += custoTerceirizar.at(vertice - 1);
        clientesTerceirizados.push_back(vertice);
    }
}

void trocaRota(int &custoAtualSolucao, std::vector<int> &verticeCustos, int &verticeAtual, Solucao *solucao, Rota &rota, int &rotaAtual, std::vector<std::vector<int>> &custoCaminhos){
    custoAtualSolucao += verticeCustos.at(0); //Adiciona o custo de ir para 0
    rota.setCustoRota(rota.getCustoRota() + custoCaminhos.at(verticeAtual).at(0));
    rota.addVertice(0);
    solucao->addRota(rota); //Adiciona rota na solução
    rota.clearRota(); //Limpa a rota atual, já que ela não pode adicionar mais vértices

    rota.addVertice(0);
    verticeAtual = 0; //Início da próxima rota, logo começa em 0
    rotaAtual++; //Troca a rota
}