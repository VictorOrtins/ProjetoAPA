#include "ProblemaCondicoes.hpp"
#include "funcoes.hpp"
#include "Solucao.hpp"
#include "vnd.hpp"

int main(int argc, char* argv[]){
    /* Arquivo de teste */
    
    // ProblemaCondicoes* condicoes = leArquivo("instancias/n14k5_A.txt"); //Lê o arquivo de instância
    // if (condicoes == nullptr){ //Checa se houve algum problema em abrí-lo
    //     std::cout << "Problema na leitura do arquivo!\n";
    // }

    // RetornoGuloso *retorno = algoritmoGuloso(condicoes);
    // Solucao* solucao = retorno->solucao; //Acha a solução do algoritmo guloso
    // std::cout << solucao->getEntregasNaoTerceirizadas() << "\n";

    // melhorTerceirizacao(condicoes, solucao);

    // std::cout << solucao->getEntregasNaoTerceirizadas();

    // escreveArquivo("final2.txt", solucao); //Escreve no arquivo final


    // delete condicoes;
    // delete solucao;

    testeInstancias("instancias", "finalComVND");
}





