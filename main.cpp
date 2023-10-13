#include "ProblemaCondicoes.hpp"
#include "funcoes.hpp"
#include "Solucao.hpp"
#include "vnd.hpp"

int main(int argc, char* argv[]){
    ProblemaCondicoes* condicoes = leArquivo("instancias/n22k3_A.txt"); //Lê o arquivo de instância
    if (condicoes == nullptr){ //Checa se houve algum problema em abrí-lo
        std::cout << "Problema na leitura do arquivo!\n";
    }

    Solucao* solucao = algoritmoGuloso(condicoes); //Acha a solução do algoritmo guloso

    swap(condicoes, solucao);

    escreveArquivo("final.txt", solucao); //Escreve no arquivo final


    delete condicoes;
    delete solucao;

    // testeInstancias("instancias", "final");

}





