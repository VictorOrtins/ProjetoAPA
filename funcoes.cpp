#include "funcoes.hpp"
#include "ils.hpp"

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
        for(unsigned int i = 0; i < temp.getRota().size(); i++){
            if(!(i == 0 || i == temp.getRota().size() - 1)){
                arquivo << temp.getRota().at(i) << " ";
            }
        }
    }

    return true;
}

bool testaSolucao(Solucao* solucao, ProblemaCondicoes* condicoes){
    bool ret = true;
    int custoRoteamento = 0;
    int custoVeiculos = 0;

    for(int i = 0; i < solucao->getNumeroRotas(); i++){
        custoVeiculos += condicoes->getCustoCarro();

        Rota &rota = solucao->getRotas().at(i);

        std::vector<int> &rotaVector = rota.getRota();
        int capacidadeRota = 0;
        for(unsigned int j = 0; j < rotaVector.size() - 1; j++){
            custoRoteamento += condicoes->getCustoCaminho().at(rotaVector.at(j)).at(rotaVector.at(j + 1));
        }

        for(unsigned int j = 1; j < rotaVector.size() - 1; j++){
            capacidadeRota += condicoes->getDemandaClientes().at( rotaVector.at(j) - 1 );
        }

        if(capacidadeRota > condicoes->getCapacidadeVeiculo()){
            printf("\tCapacidade\n");
            ret = false;
        }

    }

    int custoTerceirizar = 0;
    for(int vertice : solucao->getClientesTerceirizados()){
        custoTerceirizar += condicoes->getCustoTerceirizacao().at(vertice - 1);
    }

    if(custoTerceirizar != solucao->getCustoTerceirizacao()){
        printf("\tterceirizacao\n");
        ret = false;
    }

    if(custoRoteamento != solucao->getCustoRoteamento()){
        printf("\troteamento\n");
        ret = false;
    }

    if(custoVeiculos != solucao->getCustoVeiculos()){
        printf("\tveiculos\n");
        ret = false;
    }

    if(custoRoteamento + custoTerceirizar + custoVeiculos != solucao->getValorSolucao()){
        printf("\ttotal\n");
        ret = false;
    }

    if(condicoes->getMinimoEntregas() > solucao->getEntregasNaoTerceirizadas()){
        printf("\tMinimo Entregas\n");
        ret = false;
    }

    return ret;
}

//Testa todas as instâncias em uma pasta
void testeInstancias(std::string nomePastaInstancias, std::string nomePastaDestino, std::string nomeCsvTabela){
    std::vector<std::string> nomesArquivos; //Vector que terá o nome dos arquivos

    for (const auto& entry : std::filesystem::directory_iterator(nomePastaInstancias)) {
        if(entry.is_regular_file()){ //Se de fato for um arquivo
            nomesArquivos.push_back(entry.path().filename().string()); //Colocar na lista
        }
    }

    ProblemaCondicoes *condicoes;
    Solucao* solucao;
    RetornoGuloso* retornoGuloso;

    std::vector<int> valoresOtimos{1029,2052,2040,1046,471,565,569,471
    ,1672,3302,3301,1672,605,777,777,605,650,933,939,656,
    801,1203,1208,802,934,1503,1510,932,428,506,559,408};

    std::ofstream csv;
    csv.open(nomeCsvTabela);
    csv << "instancia,otimo,valorHeuristica,tempoHeuristica,gapHeuristica,valorVND,tempoVND,gapVND,valorILS,tempoILS,gapILS" << std::endl;


    for(unsigned int i = 0; i < nomesArquivos.size(); i++){ //Em cada arquivo
        std::string strCsv;
        strCsv = nomesArquivos[i] + "," + std::to_string(valoresOtimos[i]) + ",";

        std::cout << "Arquivo: " << nomesArquivos[i] << "Otimo: " << valoresOtimos[i] << "\n";
        condicoes = leArquivo(nomePastaInstancias + "/" + nomesArquivos[i]); //Lê o arquivo

        if(condicoes == nullptr){ //Se houve algum erro na hora de criar as condições
            std::cout << "ERRO!\n";
            return;
        }
        retornoGuloso = algoritmoGuloso(condicoes); //Faz o algoritmo guloso
        solucao = retornoGuloso->solucao; 

        printf("\tTempo de Execucao Guloso: %.6fs\n", retornoGuloso->seconds);

        char tempStr[10];
        std::sprintf(tempStr, "%.6fs", retornoGuloso->seconds);

        int valorHeuristica = solucao->getValorSolucao();
        strCsv += std::to_string(valorHeuristica) + "," + tempStr + "," + std::to_string( (valorHeuristica - valoresOtimos[i])/(valoresOtimos[i]*1.0)*100 ) + ",";

        if(solucao == nullptr){ //Se algo der errado na criação da solução
            std::cout << "ERRO!\n";
            return;
        }

        double tempo = VND(condicoes, solucao);

        std::sprintf(tempStr, "%.6fs", tempo);
        valorHeuristica = solucao->getValorSolucao();
        strCsv += std::to_string(valorHeuristica) + "," + tempStr + "," + std::to_string( (valorHeuristica - valoresOtimos[i])/(valoresOtimos[i]*1.0)*100 ) + ",";

        printf("\tTempo de execucao VND: %.6fs\n", tempo);

        RetornoILS* retornoILS;
        retornoILS = ILS(condicoes,200);

        tempo = retornoILS->tempo;
        solucao = retornoILS->solucao;

        std::sprintf(tempStr, "%.6fs", tempo);
        valorHeuristica = solucao->getValorSolucao();
        strCsv += std::to_string(valorHeuristica) + "," + tempStr + "," + std::to_string( (valorHeuristica - valoresOtimos[i])/(valoresOtimos[i]*1.0)*100 );


        bool certo = testaSolucao(solucao, condicoes);
        if(!certo){
            std::cout << "Solucao: " << nomesArquivos[i] << " esta errada!";
        }

        escreveArquivo(nomePastaDestino + "/" + nomesArquivos[i], solucao); //Escreve no arquivo

        csv << strCsv << std::endl;

        delete condicoes;
        delete solucao;
        delete retornoGuloso;
        delete retornoILS;
        std::cout << std::endl;
    }

    csv.close();
}