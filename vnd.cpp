#include "vnd.hpp"

//Movimento de vizinhança de swap numa mesma rota. Procura fazer o melhor swap para cada uma das rotas numa mesma chamada
bool swap(ProblemaCondicoes *condicoes, Solucao *solucao){
    const int INFINITO = std::numeric_limits<int>::max(); //Infinito
    
    std::vector<Rota>& rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho

    int novoCustoRoteamento = 0; //Variável que calcula o novo custo do roteamento

    bool swapOcorreu = false; //Diz se o swap ocorreu ou não

    for(Rota &rota: rotas){ //Itera sobre todas as rotas
        std::vector<int>& vectorRota = rota.getRota(); //Pega o vector da rota atual
        int custoRota = rota.getCustoRota(); //Pega o custo da rota antes do swap

        int melhor_i = -1, melhor_j = -1; //Vai guardar os índices dos vértices que fazem o swap que melhor diminui o custo da solução
        int menorCusto = INFINITO; //Guarda o custo da rota de melhor swap encontrado

        for(unsigned int i = 1; i < vectorRota.size() - 2; i++){ //Itera sobre os vértices possíveis (Do primeiro vértice após o 0 inicial até o penúltimo antes do 0 final)
            int verticeI = vectorRota.at(i); //Pega o vértice que está na posição i
            int verticeAntesI = vectorRota.at(i - 1); //Pega o vértice na posição i - 1
            int verticeDepoisI = vectorRota.at(i + 1); //Pega o vértice na posição i + 1

            for(unsigned int j = i + 1; j < vectorRota.size() - 1; j++){ //Itera sobre todos os vértices após o i e até antes do vértice 0 final
                int verticeJ = vectorRota.at(j); //Pega o vértice da posição j
                int verticeAntesJ = vectorRota.at(j - 1); //Pega o vértice da posição j - 1
                int verticeDepoisJ = vectorRota.at(j + 1); //Pega o vértice da posição j + 1

                int custoSwap; //Calcula o custo do swap para a rota

                if(j == i + 1){ //Se os vértices são adjacentes. Há intersecção de arestas nesse caso, então o cálculo é menorr
                    custoSwap = custoRota - custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeJ) 
                    - custoCaminhos.at(verticeJ).at(verticeDepoisJ) +
                    custoCaminhos.at(verticeAntesI).at(verticeJ) + custoCaminhos.at(verticeJ).at(verticeI) 
                    + custoCaminhos.at(verticeI).at(verticeDepoisJ);

                    //rota - Ci-1,i - Ci,j - Cj,j+1 + Ci-1,j + Cj,i + Ci,j + 1

                }
                else{ //Se os vértices não são adjacentes
                    custoSwap = custoRota - custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeDepoisI) 
                    - custoCaminhos.at(verticeAntesJ).at(verticeJ) - custoCaminhos.at(verticeJ).at(verticeDepoisJ) +
                    custoCaminhos.at(verticeAntesI).at(verticeJ) + custoCaminhos.at(verticeJ).at(verticeDepoisI) 
                    + custoCaminhos.at(verticeAntesJ).at(verticeI) + custoCaminhos.at(verticeI).at(verticeDepoisJ);

                    //rota - Ci-1,i - Ci,i+1 - Cj-1,j - Cj,j+1 + Ci-1,j Cj,i+1 + Cj-1,i = Ci,j+1
                }

                if(custoSwap < menorCusto){ //Se o custo da rota com swap calculado agora for menor que o menor custo calculado da rota com um swap
                    //Atualizar os vértices envolvidos no melhor swap e o custo da rota com esse swap
                    melhor_i = i; 
                    melhor_j = j;
                    menorCusto = custoSwap;
                }
            }
        }

        if(menorCusto < custoRota){ //Se o menor custo da rota com o swap for menor que o custo da rota inicialmente
            trocaVertices(melhor_i, melhor_j, menorCusto, vectorRota); //Faz a troca dos vértices
            rota.setCustoRota(menorCusto); //Atualiza o custo da rota
            novoCustoRoteamento += menorCusto; //Atualiza o custo do roteamento atual
            swapOcorreu = true; //Swap ocorreu em algum momento, logo true
        }
        else{
            novoCustoRoteamento += custoRota; //Atualiza o custo do roteamento com o custo da rota sem swap
        }
    }

    solucao->setCustoRoteamento(novoCustoRoteamento); //Custo do roteamento é atualizado
    return swapOcorreu;
}

//Realiza a troca de vértices
void trocaVertices(int melhor_i, int melhor_j, int menor_custo, std::vector<int> &rota){
    int temp = rota.at(melhor_i);
    rota.at(melhor_i) = rota.at(melhor_j);
    rota.at(melhor_j) = temp;
}

//Movimento de vizinhança de um swap entre 2 rotas. Procura encontrar o melhor desses swaps e já fazer a troca.
//TEM QUE DEIXAR O SWAP OCORRER MESMO SE VANTAJOSO SÓ PARA UMA ROTA (SE DIMINUIR O CUSTO TOTAL DA SOLUÇÃO)
bool swapInterRotas(ProblemaCondicoes *condicoes, Solucao *solucao){
    const int INFINITO = std::numeric_limits<int>::max();
    
    std::vector<Rota>& rotas = solucao->getRotas();
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho();

    bool swapOcorreu = false;

    for(unsigned int i = 0; i < rotas.size() - 1; i++){ //Primeira rota
        for(unsigned int j = i + 1; j < rotas.size(); j++){ //Segunda rota
            Rota &rota1 = rotas.at(i); //Pega a primeira rota
            Rota &rota2 = rotas.at(j); //Pega a segunda rota

            std::vector<int> &rota1Vector = rota1.getRota(); //Vector da primeira rota
            std::vector<int> &rota2Vector = rota2.getRota(); //Vector da segunda rota

            int custoRota1 = rota1.getCustoRota(); //Custo da primeira rota
            int custoRota2 = rota2.getCustoRota(); //Custo da segunda rota

            int melhorRota1 = -1, melhorRota2 = -1; //Índices dos melhores vértices da rota 1 e da rota 2
            int menorCustoRota1 = INFINITO, menorCustoRota2 = INFINITO; //Menor custo da rota inteira após o swap da rota 1 e da rota 2


            for(unsigned int k = 1; k < rota1Vector.size() - 1; k++){ //Itera sobre os vértices possíveis da rota 1
                int verticeK = rota1Vector.at(k); //vértice da rota 1 na posição k
                int verticeAntesK = rota1Vector.at(k - 1); //Vértice da rota 1 na posição k-1
                int verticeDepoisK = rota1Vector.at(k + 1); //Vértice da rota 1 na posição k+1

                for(unsigned int l = 1; l < rota2Vector.size() - 1; l++){ //Itera sobre os vértices possíveis da rota 2
                    int verticeL = rota2Vector.at(l); //Vértice da rota 2 na posição l
                    int verticeAntesL = rota2Vector.at(l - 1); //Vértice da rota 2 na posição l-1
                    int verticeDepoisL = rota2Vector.at(l + 1); //Vértice da rota 2 na posição l+1

                    int custoSwapRota1, custoSwapRota2; //Custo das rotas 1 e 2 após o swap

                    custoSwapRota1 = custoRota1 - custoCaminhos.at(verticeAntesK).at(verticeK) - custoCaminhos.at(verticeK).at(verticeDepoisK) +
                                        custoCaminhos.at(verticeAntesK).at(verticeL) + custoCaminhos.at(verticeL).at(verticeDepoisK);

                    //rota1 - Ck-1,k - Ck,k+1 + Ck-1,L + Cl,k+1

                    custoSwapRota2 = custoRota2 - custoCaminhos.at(verticeAntesL).at(verticeL) - custoCaminhos.at(verticeL).at(verticeDepoisL) +
                                             custoCaminhos.at(verticeAntesL).at(verticeK) + custoCaminhos.at(verticeK).at(verticeDepoisL);

                    //rota2 - Cl-1,l - Cl,l+1 + Cl-1,k + Ck,l+1


                    if(custoSwapRota1 < menorCustoRota1 && custoSwapRota2 < menorCustoRota2){ //Se o custo total das duas rotas for menor que o menor custo encontrado após o swap das duas rotas
                        //Atualizar os índices e custos do melhor swap
                        melhorRota1 = k;
                        melhorRota2 = l;
                        menorCustoRota1 = custoSwapRota1;
                        menorCustoRota2 = custoSwapRota2;
                    }
                }
            }

            if(menorCustoRota1 < custoRota1 && menorCustoRota2 < custoRota2){ //Se o melhor custo de swap das 2 rotaas for menor que o custo atual das 2 rotas
                //Fazer a troca e atualizar os custos da rota
                trocaInterRota(melhorRota1, melhorRota2, rota1Vector, rota2Vector);
                rota1.setCustoRota(menorCustoRota1);
                rota2.setCustoRota(menorCustoRota2);
                if(!swapOcorreu){
                    swapOcorreu = true;
                }
            }
        }
    }

    int novoCustoSolucao = 0; //Novo custo da solução será a soma dos custos das rotas
    //Isso aqui provavelmente pode ser otimizado mas não achei outro jeito, ainda.
    for(unsigned int i = 0; i < rotas.size(); i++){
        novoCustoSolucao += rotas.at(i).getCustoRota();
    }

    solucao->setCustoRoteamento(novoCustoSolucao); //Atualiza o custo do roteamento
    return swapOcorreu;
}

//Faz a troca entre as rotas
void trocaInterRota(int indiceRota1, int indiceRota2, std::vector<int> &vectorRota1, std::vector<int> &vectorRota2){
    int temp = vectorRota1.at(indiceRota1);
    vectorRota1.at(indiceRota1) = vectorRota2.at(indiceRota2);
    vectorRota2.at(indiceRota2) = temp; 
}