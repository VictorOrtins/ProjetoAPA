#include "funcoes.hpp"
#include <filesystem>


//Faz o split de uma string em um vector de inteiros
std::vector<int> split(const std::string& text) {
    std::vector<int> tokens;
    std::istringstream iss(text);
    int number;

    while (iss >> number) {
        tokens.push_back(number);
    }

    return tokens;
}

//Lê o arquivo de instância. Ele precisa estar no formato descrito no sigaa para funcionar
ProblemaCondicoes* leArquivo(std::string nomeArquivo){
    ProblemaCondicoes* condicoes = new ProblemaCondicoes();

    std::ifstream arquivo;
    arquivo.open(nomeArquivo);

    if(!arquivo.is_open()){
        std::cout << "Erro no arquivo!\n";
        return nullptr;
    }

    int temp;
    arquivo >> temp;
    condicoes->setTotalEntregas(temp);

    arquivo >> temp;
    condicoes->setTotalVeiculos(temp);

    arquivo >> temp;
    condicoes->setCapacidadeVeiculo(temp);

    arquivo >> temp;
    condicoes->setMinimoEntregas(temp);

    arquivo >> temp;
    condicoes->setCustoCarro(temp);

    std::string tempString;
    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);

    std::vector<int> tempVector = split(tempString);
    condicoes->setDemandaClientes(tempVector);

    std::getline(arquivo, tempString);
    std::getline(arquivo, tempString);

    tempVector = split(tempString);
    condicoes->setCustoTerceirizacao(tempVector);

    std::vector<std::vector<int>> tempMatriz;

    std::getline(arquivo, tempString);
    for (int i = 0; i < condicoes->getTotalEntregas() + 1; i++){
        std::getline(arquivo, tempString);
        tempVector = split(tempString);
        tempMatriz.push_back(tempVector);
    }

    condicoes->setCustoCaminho(tempMatriz);

    arquivo.close();
    return condicoes;
}

//Cria uma solução para testes
Solucao* criaSolucao(){
    Solucao* solucao = new Solucao();

    solucao->setCustoRoteamento(23);
    solucao->setCustoVeiculos(10);
    solucao->setCustoTerceirizacao(3);
    solucao->setClientesTerceirizados(std::vector<int>{6});
    solucao->setNumeroRotas(2);
    solucao->setRotas(std::vector<std::vector<int>>{ {1,2}, {3,4,5} });

    return solucao;
}

//Escreve de volta no arquivo final
bool escreveArquivo(std::string arquivoDestino, Solucao* solucao){
    std::ofstream arquivo(arquivoDestino);

    if(!arquivo.is_open()){
        std::cout << "Problema ao escrever no arquivo!";
        return false;
    }

    arquivo << solucao->getValorSolucao() << std::endl;
    arquivo << solucao->getCustoRoteamento() << std::endl;
    arquivo << solucao->getCustoVeiculos() << std::endl;
    arquivo << solucao->getCustoTerceirizacao() << std::endl;
    arquivo << std::endl;

    for(int i : solucao->getClientesTerceirizados()){
        arquivo << i << " ";
    }

    arquivo << std::endl;
    arquivo << std::endl;

    arquivo << solucao->getNumeroRotas();
    for(std::vector<int> temp : solucao->getRotas()){
        arquivo << std::endl;
        for(int i: temp){
            arquivo << i << " ";
        }
    }

    return true;
}


Solucao* algoritmoGuloso(ProblemaCondicoes *condicoes){

    Solucao* solucao = new Solucao(); //Cria a solução

    const int INFINITO = std::numeric_limits<int>::max();

    std::vector<std::vector<int>> custoCaminhos = condicoes->getCustoCaminho(); //Pega a matriz de custos
    std::vector<int> demandas = condicoes->getDemandaClientes(); //Pega o vetor de demandas

    int capacidadeMaximaVeiculo = condicoes->getCapacidadeVeiculo(); //Pega a capacidade máxima por veículo
    int numRotas = condicoes->getTotalVeiculos(); //Pega o máximo de rotas que podem ser feitas
    std::vector<int> custoTerceirizar = condicoes->getCustoTerceirizacao(); //Pega os custos de terceirização

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
    std::vector<int> rota; //Vector auxiliar para armazenar os vértices da rota atual 

    while(verticesRestantes.size() > 0){ //O loop principal continua até que não tenha vértices restantes
        std::vector<int> verticeCustos = custoCaminhos.at(verticeAtual); //Vector auxiliar
        //que armazenas os custos para sair do vértice atual

        //Se a rota atual pode adicionar o vértice atual, porém já está cheia
        if(capacidadeAtualRota.at(rotaAtual) == capacidadeMaximaVeiculo){
            if(rotaAtual + 1 == numRotas){ //Se as rotas acabaram
                //Terceirizar o resto dos vértices
               for(int vertice : verticesRestantes){
                    custoAtualTerceirizacao += custoTerceirizar.at(vertice - 1); //Calcula a terceirização
                    clientesTerceirizados.push_back(vertice); //Adiciona nos clientes terceirizados
                }
                break;
            }

            //Se ainda há rotas, terminar a rota. Ou seja, sair do vértice atual e ir pro 0
            custoAtualSolucao += verticeCustos.at(0); //Adiciona o custo de ir para 0
            verticeAtual = 0; //Início da próxima rota, logo começa em 0
            solucao->addRota(rota); //Adiciona rota na solução
            rota.clear(); //Limpa a rota atual, já que ela não pode adicionar mais vértices
            rotaAtual++; //Troca a rota
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
               for(int vertice : verticesRestantes){
                    custoAtualTerceirizacao += custoTerceirizar.at(vertice - 1);
                    clientesTerceirizados.push_back(vertice);
                }
                break;
            }

            //Coloca o 0 no fim da rota atual e troca para a próxima
            custoAtualSolucao += verticeCustos.at(0);
            verticeAtual = 0;
            solucao->addRota(rota);
            rota.clear();
            rotaAtual++;
            continue;
        }

        //Já que foi achado o vértice a ser adicionado, coloca na solução
        custoAtualSolucao += verticeCustos.at(verticeMenorCusto); //Aumenta o custo da solução pela adição do vértice
        capacidadeAtualRota.at(rotaAtual) += demandas.at(verticeMenorCusto - 1); //Aumenta a capacidade atual da rota
        //Note que, como as demandas tem tamanho da qtd de entregas, ou seja, n, precisamos utilizar o -1
        //pra pegar o índice correto

        rota.push_back(verticeMenorCusto); //Adiciona o vértice de menor custo na rota
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
    solucao->addRota(rota);

    //Atualiza a solução com os valores encontrados
    solucao->setCustoRoteamento(custoAtualSolucao); //Custo das rotas
    solucao->setCustoTerceirizacao(custoAtualTerceirizacao); //Custo de terceirizar
    solucao->setCustoVeiculos( (rotaAtual + 1)*condicoes->getCustoCarro()); //Custo dos veículos
    solucao->setNumeroRotas( (rotaAtual + 1)); //Quantidade de rotas utilizadas
    solucao->setClientesTerceirizados(clientesTerceirizados); //Coloca os clientes terceirizados

    return solucao;
}

void testeInstancias(std::string nomePastaInstancias, std::string nomePastaDestino){
    std::vector<std::string> nomesArquivos;

    for (const auto& entry : std::filesystem::directory_iterator(nomePastaInstancias)) {
        if(entry.is_regular_file()){
            nomesArquivos.push_back(entry.path().filename().string());
        }
    }

    ProblemaCondicoes *condicoes;
    Solucao* solucao;

    for(std::string str : nomesArquivos){
        condicoes = leArquivo(nomePastaInstancias + "/" + str);

        if(condicoes == nullptr){
            std::cout << "ERRO!\n";
            return;
        }

        solucao = algoritmoGuloso(condicoes);

        if(solucao == nullptr){
            std::cout << "ERRO!\n";
            return;
        }

        escreveArquivo(nomePastaDestino + "/" + str, solucao);

        delete condicoes;
        delete solucao;
    }
}