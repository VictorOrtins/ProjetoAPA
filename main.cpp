#include "ProblemaCondicoes.hpp"
#include "funcoes.hpp"

int main(int argc, char* argv[]){
    ProblemaCondicoes* condicoes = leArquivo("instancia.txt");
    if (condicoes == nullptr){
        std::cout << "Problema na leitura do arquivo!\n";
    }

    condicoes->printaObjeto();
}





