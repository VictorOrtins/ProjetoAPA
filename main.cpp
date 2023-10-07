#include "ProblemaCondicoes.hpp"
#include "funcoes.hpp"
#include "Solucao.hpp"

int main(int argc, char* argv[]){
    ProblemaCondicoes* condicoes = leArquivo("instancia.txt");
    if (condicoes == nullptr){
        std::cout << "Problema na leitura do arquivo!\n";
    }

    Solucao* solucao = algoritmoGuloso(condicoes);

    solucao->printaSolucao();

    escreveArquivo("final.txt", solucao);

    delete condicoes;
    delete solucao;
}





