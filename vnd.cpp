#include "vnd.hpp"

//Movimento de vizinhança de swap numa mesma rota. Procura fazer o melhor swap para cada uma das rotas numa mesma chamada
bool swap(ProblemaCondicoes *condicoes, Solucao *solucao){
    const int INFINITO = std::numeric_limits<int>::max(); //Infinito
    
    std::vector<Rota>& rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho

    int melhor_i = -1, melhor_j = -1; //Vai guardar os índices dos vértices que fazem o swap que melhor diminui o custo da solução
    int menorCustoDelta = INFINITO; //Guarda o custo da rota de melhor swap encontrado
    int rotaMelhorSwap = -1;

    for(unsigned int rotaIndex = 0; rotaIndex < rotas.size(); rotaIndex++){ //Itera sobre todas as rotas
        std::vector<int>& vectorRota = rotas.at(rotaIndex).getRota(); //Pega o vector da rota atual

        for(unsigned int i = 1; i < vectorRota.size() - 2; i++){ //Itera sobre os vértices possíveis (Do primeiro vértice após o 0 inicial até o penúltimo antes do 0 final)
            int verticeI = vectorRota.at(i); //Pega o vértice que está na posição i
            int verticeAntesI = vectorRota.at(i - 1); //Pega o vértice na posição i - 1
            int verticeDepoisI = vectorRota.at(i + 1); //Pega o vértice na posição i + 1

            for(unsigned int j = i + 1; j < vectorRota.size() - 1; j++){ //Itera sobre todos os vértices após o i e até antes do vértice 0 final
                int verticeJ = vectorRota.at(j); //Pega o vértice da posição j
                int verticeAntesJ = vectorRota.at(j - 1); //Pega o vértice da posição j - 1
                int verticeDepoisJ = vectorRota.at(j + 1); //Pega o vértice da posição j + 1

                int custoSwapDelta = 0; //Calcula o custo do swap para a rota

                if(j == i + 1){ //Se os vértices são adjacentes. Há intersecção de arestas nesse caso, então o cálculo é menorr
                    custoSwapDelta = -custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeJ) 
                    - custoCaminhos.at(verticeJ).at(verticeDepoisJ) +
                    custoCaminhos.at(verticeAntesI).at(verticeJ) + custoCaminhos.at(verticeJ).at(verticeI) 
                    + custoCaminhos.at(verticeI).at(verticeDepoisJ);

                    //rota - Ci-1,i - Ci,j - Cj,j+1 + Ci-1,j + Cj,i + Ci,j + 1
                }
                else{ //Se os vértices não são adjacentes
                    custoSwapDelta = -custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeDepoisI) 
                    - custoCaminhos.at(verticeAntesJ).at(verticeJ) - custoCaminhos.at(verticeJ).at(verticeDepoisJ) +
                    custoCaminhos.at(verticeAntesI).at(verticeJ) + custoCaminhos.at(verticeJ).at(verticeDepoisI) 
                    + custoCaminhos.at(verticeAntesJ).at(verticeI) + custoCaminhos.at(verticeI).at(verticeDepoisJ);

                    //rota - Ci-1,i - Ci,i+1 - Cj-1,j - Cj,j+1 + Ci-1,j Cj,i+1 + Cj-1,i = Ci,j+1
                }

                if(custoSwapDelta < menorCustoDelta){ //Se o custo da rota com swap calculado agora for menor que o menor custo calculado da rota com um swap
                    //Atualizar os vértices envolvidos no melhor swap e o custo da rota com esse swap
                    melhor_i = i; 
                    melhor_j = j;
                    menorCustoDelta = custoSwapDelta;
                    rotaMelhorSwap = rotaIndex;
                }
            }
        }
    }

    if(menorCustoDelta < 0){ //Se o delta do custo do swap for menor que 0 (ou seja, valha a pena fazer o swap)
        trocaVertices(melhor_i, melhor_j, rotas.at(rotaMelhorSwap).getRota()); //Faz a troca dos vértices
        rotas.at(rotaMelhorSwap).setCustoRota(rotas.at(rotaMelhorSwap).getCustoRota() + menorCustoDelta); //Atualiza o custo da rota
        solucao->setCustoRoteamento(solucao->getCustoRoteamento() + menorCustoDelta); //Custo do roteamento é atualizado
        return true; //Swap ocorreu
    }

    return false; //Swap ocorreu
}

//Realiza a troca de vértices
void trocaVertices(int melhor_i, int melhor_j, std::vector<int> &rota){
    int temp = rota.at(melhor_i);
    rota.at(melhor_i) = rota.at(melhor_j);
    rota.at(melhor_j) = temp;
}

//Movimento de vizinhança de um swap entre 2 rotas. Procura encontrar o melhor desses swaps e já fazer a troca.
bool swapInterRotas(ProblemaCondicoes *condicoes, Solucao *solucao){
    const int INFINITO = std::numeric_limits<int>::max();
    
    std::vector<Rota>& rotas = solucao->getRotas();
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho();

    int rotaMelhorSwap1 = -1, rotaMelhorSwap2 = -1; //Rotas do melhor swap
    int melhorIndiceRota1 = -1, melhorIndiceRota2 = -1; //Índices dos melhores vértices da rota 1 e da rota 2
    int menorCustoSwap = INFINITO; //Menor custo da rota inteira após o swap da rota 1 e da rota 2
    int deltaRota1MelhorSwap = -1, deltaRota2MelhorSwap = -1;

    for(unsigned int i = 0; i < rotas.size() - 1; i++){ //Primeira rota
        for(unsigned int j = i + 1; j < rotas.size(); j++){ //Segunda rota
            Rota &rota1 = rotas.at(i); //Pega a primeira rota
            Rota &rota2 = rotas.at(j); //Pega a segunda rota

            std::vector<int> &rota1Vector = rota1.getRota(); //Vector da primeira rota
            std::vector<int> &rota2Vector = rota2.getRota(); //Vector da segunda rota

            for(unsigned int k = 1; k < rota1Vector.size() - 1; k++){ //Itera sobre os vértices possíveis da rota 1
                int verticeK = rota1Vector.at(k); //vértice da rota 1 na posição k
                int verticeAntesK = rota1Vector.at(k - 1); //Vértice da rota 1 na posição k-1
                int verticeDepoisK = rota1Vector.at(k + 1); //Vértice da rota 1 na posição k+1

                for(unsigned int l = 1; l < rota2Vector.size() - 1; l++){ //Itera sobre os vértices possíveis da rota 2
                    int verticeL = rota2Vector.at(l); //Vértice da rota 2 na posição l
                    int verticeAntesL = rota2Vector.at(l - 1); //Vértice da rota 2 na posição l-1
                    int verticeDepoisL = rota2Vector.at(l + 1); //Vértice da rota 2 na posição l+1

                    int custoSwapDeltaRota1, custoSwapDeltaRota2; //Custo das rotas 1 e 2 após o swap

                    custoSwapDeltaRota1 = -custoCaminhos.at(verticeAntesK).at(verticeK) - custoCaminhos.at(verticeK).at(verticeDepoisK) +
                                        custoCaminhos.at(verticeAntesK).at(verticeL) + custoCaminhos.at(verticeL).at(verticeDepoisK);
                    //rota1 - Ck-1,k - Ck,k+1 + Ck-1,L + Cl,k+1

                    custoSwapDeltaRota2 = -custoCaminhos.at(verticeAntesL).at(verticeL) - custoCaminhos.at(verticeL).at(verticeDepoisL) +
                                             custoCaminhos.at(verticeAntesL).at(verticeK) + custoCaminhos.at(verticeK).at(verticeDepoisL);
                    
                    //rota2 - Cl-1,l - Cl,l+1 + Cl-1,k + Ck,l+1


                    if(custoSwapDeltaRota1 + custoSwapDeltaRota2 < menorCustoSwap){ //Se o custo total das duas rotas for menor que o menor custo encontrado após o swap das duas rotas
                        //Atualizar em quais rotas ocorre o melhor swap
                        rotaMelhorSwap1 = i;
                        rotaMelhorSwap2 = j;
                        //Atualizar os índices e custos do melhor swap
                        melhorIndiceRota1 = k;
                        melhorIndiceRota2 = l;
                        menorCustoSwap = custoSwapDeltaRota1 + custoSwapDeltaRota2;
                        //Delta do custo da rota no melhor swap
                        deltaRota1MelhorSwap = custoSwapDeltaRota1;
                        deltaRota2MelhorSwap = custoSwapDeltaRota2;
                    }
                }
            }
        }
    }

    if(menorCustoSwap < 0){ //Se o melhor custo de swap das 2 rotas for menor que o custo atual das 2 rotas
        //Fazer a troca e atualizar os custos da rota
        trocaInterRota(melhorIndiceRota1, melhorIndiceRota2, rotas.at(rotaMelhorSwap1).getRota(), rotas.at(rotaMelhorSwap2).getRota());
        rotas.at(rotaMelhorSwap1).setCustoRota(rotas.at(rotaMelhorSwap1).getCustoRota() + deltaRota1MelhorSwap);
        rotas.at(rotaMelhorSwap2).setCustoRota(rotas.at(rotaMelhorSwap2).getCustoRota() + deltaRota2MelhorSwap);
        solucao->setCustoRoteamento(solucao->getCustoRoteamento() + menorCustoSwap); //Atualiza o custo do roteamento
        return true; //Swap ocorreu
    }

    return false; //Swap não ocorreu
}

//Faz a troca entre as rotas
void trocaInterRota(int indiceRota1, int indiceRota2, std::vector<int> &vectorRota1, std::vector<int> &vectorRota2){
    int temp = vectorRota1.at(indiceRota1);
    vectorRota1.at(indiceRota1) = vectorRota2.at(indiceRota2);
    vectorRota2.at(indiceRota2) = temp; 
}

bool melhorTerceirizacao(ProblemaCondicoes *condicoes, Solucao *solucao){
    if(solucao->getEntregasNaoTerceirizadas() == condicoes->getMinimoEntregas()){ //Se a solução não pode mais terceirizar entregas
        return false;
    }

    const int INFINITO = std::numeric_limits<int>::max(); //Infinito
    
    std::vector<Rota>& rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho
    std::vector<int> &custoTerceirizacao = condicoes->getCustoTerceirizacao();

    int melhor_i = -1;
    int rotaMelhorTerceirizacao = -1;
    int menorCustoTerceirizarDelta = INFINITO;
    int menorDeltaRoteamento = -1;
    int menorCustoSolucaoDelta = -1;


    for(unsigned int rotaIndex = 0; rotaIndex < rotas.size(); rotaIndex++){
        Rota &rota = rotas.at(rotaIndex);

        std::vector<int> &rotaVector = rota.getRota();

        for(unsigned int i = 1; i < rotaVector.size() - 1; i++){
            int verticeI = rotaVector.at(i);
            int verticeDepoisI = rotaVector.at(i + 1);
            int verticeAntesI = rotaVector.at(i - 1);

            int custoTerceirizarDelta, deltaRoteamento;

            deltaRoteamento = -custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeDepoisI) +
                               custoCaminhos.at(verticeAntesI).at(verticeDepoisI);

            custoTerceirizarDelta = custoTerceirizacao.at(verticeI - 1);
            
            if (custoTerceirizarDelta + deltaRoteamento < menorCustoSolucaoDelta){
                melhor_i = i;
                rotaMelhorTerceirizacao = rotaIndex;
                menorCustoTerceirizarDelta = custoTerceirizarDelta;
                menorDeltaRoteamento = deltaRoteamento;
                menorCustoSolucaoDelta = custoTerceirizarDelta + deltaRoteamento;
            }
        }
    }

    if(menorCustoSolucaoDelta < 0){
        solucao->addClienteTerceirizado(rotas.at(rotaMelhorTerceirizacao).getRota().at(melhor_i));

        rotas.at(rotaMelhorTerceirizacao).getRota().erase(rotas.at(rotaMelhorTerceirizacao).getRota().begin() + melhor_i);

        solucao->setCustoRoteamento(solucao->getCustoRoteamento() + menorDeltaRoteamento);
        solucao->setCustoTerceirizacao(solucao->getCustoTerceirizacao() + menorCustoTerceirizarDelta);

        solucao->setEntregasNaoTerceirizadas(solucao->getEntregasNaoTerceirizadas() - 1);

        if (rotas.at(rotaMelhorTerceirizacao).getRota().size() == 2){ //Se a rota só tem 0-0, essa rota não existe mais e precisa ser excluída
            solucao->removeRota(rotaMelhorTerceirizacao);
        }

        return true;
    }

    return false;
}