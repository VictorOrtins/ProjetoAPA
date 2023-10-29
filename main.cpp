#include "ProblemaCondicoes.hpp"
#include "funcoes.hpp"
#include "Solucao.hpp"
#include "vnd.hpp"
#include "ils.hpp"
#include <random>

int main(int argc, char* argv[]){
    /* Arquivo de teste */
    
    // ProblemaCondicoes* condicoes = leArquivo("instancias/n199k17_A.txt"); //Lê o arquivo de instância
    // if (condicoes == nullptr){ //Checa se houve algum problema em abrí-lo
    //     std::cout << "Problema na leitura do arquivo!\n";
    // }

    // RetornoGuloso *retorno = algoritmoGuloso(condicoes);
    // Solucao* solucao = retorno->solucao; //Acha a solução do algoritmo guloso

    // VND(condicoes, solucao);

    // ILS(condicoes, 100);

    // bool certo = testaSolucao(solucao, condicoes);
    // if(!certo){
    //     printf("Solucao errada!");
    // }

    // escreveArquivo("final.txt", solucao); //Escreve no arquivo final

    // delete condicoes;
    // delete solucao;
    // delete retorno;

    testeInstancias("instancias", "finalComVND", "resultados17.csv");
}





