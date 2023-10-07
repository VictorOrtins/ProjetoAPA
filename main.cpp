#include "ProblemaCondicoes.hpp"
#include "funcoes.hpp"
#include "Solucao.hpp"

int main(int argc, char* argv[]){
    ProblemaCondicoes* condicoes = leArquivo("instancia.txt"); //Lê o arquivo de instância
    if (condicoes == nullptr){ //Checa se houve algum problema em abrí-lo
        std::cout << "Problema na leitura do arquivo!\n";
    }

    Solucao* solucao = algoritmoGuloso(condicoes); //Acha a solução do algoritmo guloso

    // solucao->printaSolucao();

    escreveArquivo("final.txt", solucao); //Escreve no arquivo final

    delete condicoes;
    delete solucao;
}





