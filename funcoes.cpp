#include "funcoes.hpp"

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
    Rota rota1;
    rota1.setRota({1,2});
    Rota rota2;
    rota2.setRota({3,4,5});
    solucao->setRotas(std::vector<Rota>{rota1, rota2});

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
    for(Rota temp : solucao->getRotas()){
        arquivo << std::endl;
        // for(int i: temp.getRota()){
        //     arquivo << i << " ";
        // }
        for(unsigned int i = 0; i < temp.getRota().size(); i++){
            if(!(i == 0 || i == temp.getRota().size() - 1)){
                arquivo << temp.getRota().at(i) << " ";
            }
        }
    }

    return true;
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