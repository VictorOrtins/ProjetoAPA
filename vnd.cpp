#include "vnd.hpp"

//Movimento de vizinhança de swap numa mesma rota. Procura fazer o melhor swap para cada uma das rotas numa mesma chamada
bool swap(ProblemaCondicoes *condicoes, Solucao *solucao){

    const int INFINITO = std::numeric_limits<int>::max(); //Infinito
    
    std::vector<Rota>& rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho

    int melhor_i = -1, melhor_j = -1; //Vai guardar os índices dos vértices que fazem o swap que melhor diminui o custo da solução
    int menorCustoDelta = INFINITO; //Guarda o custo da rota de melhor swap encontrado
    int rotaMelhorSwap = -1; //Guarda a rota em que o melhor swap ocorre

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

                    //-Ci-1,i - Ci,j - Cj,j+1 + Ci-1,j + Cj,i + Ci,j + 1
                }
                else{ //Se os vértices não são adjacentes
                    custoSwapDelta = -custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeDepoisI) 
                    - custoCaminhos.at(verticeAntesJ).at(verticeJ) - custoCaminhos.at(verticeJ).at(verticeDepoisJ) +
                    custoCaminhos.at(verticeAntesI).at(verticeJ) + custoCaminhos.at(verticeJ).at(verticeDepoisI) 
                    + custoCaminhos.at(verticeAntesJ).at(verticeI) + custoCaminhos.at(verticeI).at(verticeDepoisJ);

                    //-Ci-1,i - Ci,i+1 - Cj-1,j - Cj,j+1 + Ci-1,j Cj,i+1 + Cj-1,i = Ci,j+1
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
    //Swap comum
    int temp = rota.at(melhor_i);
    rota.at(melhor_i) = rota.at(melhor_j);
    rota.at(melhor_j) = temp;
}

//Movimento de vizinhança de um swap entre 2 rotas. Procura encontrar o melhor desses swaps e já fazer a troca.
bool swapInterRotas(ProblemaCondicoes *condicoes, Solucao *solucao){

    const int INFINITO = std::numeric_limits<int>::max(); //Infinito
    
    std::vector<Rota>& rotas = solucao->getRotas(); //Rotas registradas na solução
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Matriz de custos dos caminhos
    std::vector<int> &demandas = condicoes->getDemandaClientes();

    int capacidadeMaximaVeiculo = condicoes->getCapacidadeVeiculo();

    int rotaMelhorSwap1 = -1, rotaMelhorSwap2 = -1; //Indica em quais rotas ocorreu o melhor swap
    int melhorIndiceRota1 = -1, melhorIndiceRota2 = -1; //Índices dos melhores vértices da rota 1 e da rota 2
    int menorCustoSwap = INFINITO; //Menor custo da rota inteira após o swap da rota 1 e da rota 2
    int deltaRota1MelhorSwap = -1, deltaRota2MelhorSwap = -1; //Indica o delta da rota 1 e da rota 2 se ocorrer o melhor swap

    int melhorCapacidadeDeltaRota1 = -1;
    int melhorCapacidadeDeltaRota2 = -1;

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

                int capacidadeAtualRota1 = rota1.getCapacidadeAtualRota();


                for(unsigned int l = 1; l < rota2Vector.size() - 1; l++){ //Itera sobre os vértices possíveis da rota 2
                    int verticeL = rota2Vector.at(l); //Vértice da rota 2 na posição l
                    int verticeAntesL = rota2Vector.at(l - 1); //Vértice da rota 2 na posição l-1
                    int verticeDepoisL = rota2Vector.at(l + 1); //Vértice da rota 2 na posição l+1


                    int capacidadeAtualRota2 = rota2.getCapacidadeAtualRota();
                    int demandaVerticeL = demandas.at(verticeL - 1);
                    int demandaVerticeK = demandas.at(verticeK - 1);

                    if( (capacidadeAtualRota1 - demandaVerticeK + demandaVerticeL > capacidadeMaximaVeiculo 
                        || capacidadeAtualRota2 - demandaVerticeL + demandaVerticeK > capacidadeMaximaVeiculo)){
                            continue;
                    } //Se algum dos dois veículos não conseguem suportar a troca

                    int capacidadeRota1Delta =  - demandaVerticeK + demandaVerticeL;
                    int capacidadeRota2Delta =  - demandaVerticeL + demandaVerticeK;  

                    int custoSwapDeltaRota1, custoSwapDeltaRota2; //Custo das rotas 1 e 2 após o swap

                    custoSwapDeltaRota1 = -custoCaminhos.at(verticeAntesK).at(verticeK) - custoCaminhos.at(verticeK).at(verticeDepoisK) +
                                        custoCaminhos.at(verticeAntesK).at(verticeL) + custoCaminhos.at(verticeL).at(verticeDepoisK);
                    //-Ck-1,k - Ck,k+1 + Ck-1,L + Cl,k+1

                    custoSwapDeltaRota2 = -custoCaminhos.at(verticeAntesL).at(verticeL) - custoCaminhos.at(verticeL).at(verticeDepoisL) +
                                             custoCaminhos.at(verticeAntesL).at(verticeK) + custoCaminhos.at(verticeK).at(verticeDepoisL);
                    
                    //-Cl-1,l - Cl,l+1 + Cl-1,k + Ck,l+1


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

                        melhorCapacidadeDeltaRota1 = capacidadeRota1Delta;
                        melhorCapacidadeDeltaRota2 = capacidadeRota2Delta;
                    }
                }
            }
        }
    }

    if(menorCustoSwap < 0){ //Se o melhor custo de swap das 2 rotas for menor que o custo atual das 2 rotas
        // printf("Capacidade Maxima %d Inicio: %d %d\n", capacidadeMaximaVeiculo, rotas.at(rotaMelhorSwap1).getCapacidadeAtualRota(), rotas.at(rotaMelhorSwap2).getCapacidadeAtualRota());
        //Fazer a troca e atualizar os custos da rota
        trocaInterRota(melhorIndiceRota1, melhorIndiceRota2, rotas.at(rotaMelhorSwap1).getRota(), rotas.at(rotaMelhorSwap2).getRota());

        //Atualizar o custo das duas rotas
        rotas.at(rotaMelhorSwap1).setCustoRota(rotas.at(rotaMelhorSwap1).getCustoRota() + deltaRota1MelhorSwap);
        rotas.at(rotaMelhorSwap2).setCustoRota(rotas.at(rotaMelhorSwap2).getCustoRota() + deltaRota2MelhorSwap);

        rotas.at(rotaMelhorSwap1).setCapacidadeAtualRota(rotas.at(rotaMelhorSwap1).getCapacidadeAtualRota() + melhorCapacidadeDeltaRota1);
        rotas.at(rotaMelhorSwap2).setCapacidadeAtualRota(rotas.at(rotaMelhorSwap2).getCapacidadeAtualRota() + melhorCapacidadeDeltaRota2);

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

//Movimento de vizinhança que encontra a melhor terceirização e tenta concretizá-la, se vantajosa
bool melhorTerceirizacao(ProblemaCondicoes *condicoes, Solucao *solucao){
    if(solucao->getEntregasNaoTerceirizadas() == condicoes->getMinimoEntregas()){ //Se a solução não pode mais terceirizar entregas
        return false;
    }


    const int INFINITO = std::numeric_limits<int>::max(); //Infinito
    
    std::vector<Rota> &rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>> &custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho
    std::vector<int> &custoTerceirizacao = condicoes->getCustoTerceirizacao(); //Pega os custos de terceirização de um vértice
    std::vector<int> &demandas = condicoes->getDemandaClientes();

    int melhor_i = -1; //Melhor vértice para terceirizar
    int rotaMelhorTerceirizacao = -1; //Rota em que ocorre a melhor terceirização
    int menorCustoTerceirizarDelta = INFINITO; //Delta do custo da melhor tercerirização
    int menorDeltaRoteamento = -1; //Delta do roteamento após a terceirização
    int menorCustoSolucaoDelta = INFINITO; //Delta da solução total após a terceirização

    for(unsigned int rotaIndex = 0; rotaIndex < rotas.size(); rotaIndex++){ //Para cada rota
        Rota &rota = rotas.at(rotaIndex); //Pega a rota atual

        std::vector<int> &rotaVector = rota.getRota(); //Pega o vector da rota atual

        for(unsigned int i = 1; i < rotaVector.size() - 1; i++){ //Itera em todos os vértices possíveis (depois do 0 inicial e antes do 0 final)]
            int verticeI = rotaVector.at(i); //Vértice i

            int verticeDepoisI = rotaVector.at(i + 1); //Vértice i+1
            int verticeAntesI = rotaVector.at(i - 1); //Vértice i-1

            int custoTerceirizarDelta, deltaRoteamento;
            //Custo em delta da terceirização e custo em delta do roteamento após o movimento de vizinhança

            deltaRoteamento = -custoCaminhos.at(verticeAntesI).at(verticeI) - custoCaminhos.at(verticeI).at(verticeDepoisI) +
                               custoCaminhos.at(verticeAntesI).at(verticeDepoisI); 
            // -Ci-1,i - Ci,i+1 + Ci-1,i+1

            custoTerceirizarDelta = custoTerceirizacao.at(verticeI - 1); //Pega-se i-1 pela indexação do vector de terceirização
            // T(i)

            
            if (custoTerceirizarDelta + deltaRoteamento < menorCustoSolucaoDelta){ //Se a soma dos deltas de terceirizar e rotear for menor que os da melhor terceirização anterior
                melhor_i = i; //Atualizar o melhor vértice a ser terceirizado
                rotaMelhorTerceirizacao = rotaIndex; //Atualizar a rota em que ocorre a melhor terceirização
                menorCustoTerceirizarDelta = custoTerceirizarDelta; //Atualizar o custo de terceirização da melhor terceirização
                menorDeltaRoteamento = deltaRoteamento; //Atualizar o custo de roteamento depois da melhor terceirização
                menorCustoSolucaoDelta = custoTerceirizarDelta + deltaRoteamento; //Atualizar o custo da soliução após a melhor terceirização
            }
        }
    }


    if(menorCustoSolucaoDelta < 0){ //Se a melhor terceirização diminuir o custo da solução
        solucao->addClienteTerceirizado(rotas.at(rotaMelhorTerceirizacao).getRota().at(melhor_i)); //Adiciona o cliente terceirizado ao vector de clientes terceirizados

        //Atualiza a capacidade atual da rota
        int verticeMelhorI = rotas.at(rotaMelhorTerceirizacao).getRota().at(melhor_i);
        rotas.at(rotaMelhorTerceirizacao).setCapacidadeAtualRota(rotas.at(rotaMelhorTerceirizacao).getCapacidadeAtualRota() - demandas.at(verticeMelhorI - 1));
        
        //Apaga da rota o vértice terceirizado
        rotas.at(rotaMelhorTerceirizacao).getRota().erase(rotas.at(rotaMelhorTerceirizacao).getRota().begin() + melhor_i);

        rotas.at(rotaMelhorTerceirizacao).setCustoRota(rotas.at(rotaMelhorTerceirizacao).getCustoRota() + menorDeltaRoteamento);

        //Atualiza o custo de roteamento
        solucao->setCustoRoteamento(solucao->getCustoRoteamento() + menorDeltaRoteamento);

        //Atualiza o custo de terceirização
        solucao->setCustoTerceirizacao(solucao->getCustoTerceirizacao() + menorCustoTerceirizarDelta);

        //Já que houve terceirização, diminui o número de entregas não terceirizadas
        solucao->setEntregasNaoTerceirizadas(solucao->getEntregasNaoTerceirizadas() - 1);

        if (rotas.at(rotaMelhorTerceirizacao).getRota().size() == 2){ //Se a rota só tem 0-0, essa rota não existe mais e precisa ser excluída
            solucao->removeRota(rotaMelhorTerceirizacao); //Remove da rota a solução
            solucao->setCustoVeiculos(solucao->getCustoVeiculos() - condicoes->getCustoCarro());
        }




        return true; //Terceirização ocorreu
    }

    return false; //Terceirização não ocorreu
}

bool desterceirizaVertice(ProblemaCondicoes *condicoes, Solucao *solucao){

    const int INFINITO = std::numeric_limits<int>::max(); //Infinito

    std::vector<Rota>& rotas = solucao->getRotas(); //Pega as rotas da solução
    std::vector<std::vector<int>>& custoCaminhos = condicoes->getCustoCaminho(); //Pega o custo de cada caminho
    std::vector<int> &custoTerceirizacao = condicoes->getCustoTerceirizacao(); //Pega os custos de terceirização de um vértice
    std::vector<int> &demandas = condicoes->getDemandaClientes(); //Pega o vetor demandas dos clientes
    int capacidadeMaximaVeiculo = condicoes->getCapacidadeVeiculo(); //Pega a capacidade máxima de cada veículo

    std::vector<int> &verticesTerceirizados = solucao->getClientesTerceirizados(); //Pega os vértices que já foram terceirizados
    int qtdVerticesTerceirizados = verticesTerceirizados.size();

    int melhorCustoTerceirizacaoDelta = INFINITO; //Delta do custo de terceirização na melhor desterceirização
    int menorCustoSolucaoDelta = INFINITO; //Delta do custo da solução na melhor desterceirização
    int menorDeltaRoteamento = INFINITO; //Delta do custo do melhor roteamento

    int rotaMelhorDesterceirizacao = -1; //Rota em que será adicionado o vértice desterceirizado
    int indiceMelhorDesterceirizacao = -1; //Índice nos clientes terceirizados da melhor desterceirização
    int indiceNaRotaDesterceirizacao = -1; //índice na rota selecionada para fazer a inserção

    //Vê se é possível desterceirizar um vértice para uma rota já existente
    for(int i = 0; i < qtdVerticesTerceirizados; i++){ //Iterando sobre todos os vértices terceirizados]
        int verticeADesterceirizar = verticesTerceirizados.at(i); //Possivelmente o vértice que será desterceirizadp

        for(int j = 0; j < solucao->getNumeroRotas(); j++){ //Iterando sobre as rotas
            Rota &rota = rotas.at(j); //Pega a rota atual

            int capacidadeAtualRota = rota.getCapacidadeAtualRota(); //Pega a capacidade da rota

            if(capacidadeAtualRota + demandas.at(verticeADesterceirizar - 1) > capacidadeMaximaVeiculo){ //Se não dá pra adicionar o vértice na rota
                continue;
            }

            std::vector<int> &rotaVector = rota.getRota(); //Pega o vector da rota

            for (unsigned int k = 0; k < rotaVector.size() - 1; k++){ //Itera sobre a rota para achar o melhor lugar para
                                                            //inserir o vértice desterceirizado.
                                                            //Note que k = 0 quer dizer inserir entre os vértices 0 e 1.
                                                            //Isso faz com que no final termos que adicionar 1 na hora da inserção
                int verticeK = rotaVector.at(k); //Vértice K
                int verticeDepoisK = rotaVector.at(k + 1); //Vértice k + 1


                int custoTerceirizarDelta, deltaRoteamento; //Delta da terceirização e delta do roteamentp

                custoTerceirizarDelta = -custoTerceirizacao.at(verticeADesterceirizar - 1);
                //T(v)

                deltaRoteamento = -custoCaminhos.at(verticeK).at(verticeDepoisK) + 
                custoCaminhos.at(verticeK).at(verticeADesterceirizar) +
                custoCaminhos.at(verticeADesterceirizar).at(verticeDepoisK);
                //-C(k,k+1) + C(k,v) + C(v,k)

                if(deltaRoteamento + custoTerceirizarDelta < menorCustoSolucaoDelta){
                    //Se a soma dos dois deltas (ou seja, o delta da solução nesse movimento)
                    //for menor que o menor delta encontrado para a solução dentre os movimentos
                    
                    //Atualizar os índices
                    indiceMelhorDesterceirizacao = i;
                    rotaMelhorDesterceirizacao = j;
                    indiceNaRotaDesterceirizacao = k;

                    //Atualizar os deltas
                    menorDeltaRoteamento = deltaRoteamento;
                    melhorCustoTerceirizacaoDelta = custoTerceirizarDelta;
                    menorCustoSolucaoDelta = deltaRoteamento + custoTerceirizarDelta;
                }
            }
        }
    }


    if(menorCustoSolucaoDelta < 0){ 
        //Insere o vértice que será desterceirizado na rota correspondente
        auto iterator = rotas.at(rotaMelhorDesterceirizacao).getRota().begin();

        int clienteInsert = solucao->getClientesTerceirizados().at(indiceMelhorDesterceirizacao);
        rotas.at(rotaMelhorDesterceirizacao).getRota().insert(iterator + indiceNaRotaDesterceirizacao + 1, clienteInsert);

        //Atualizar custo da rota em que o vértice foi inserido
        rotas.at(rotaMelhorDesterceirizacao).setCustoRota(rotas.at(rotaMelhorDesterceirizacao).getCustoRota() + menorDeltaRoteamento);
        //Atualizar a capacidade atual da rota em que o vértice foi inserido
        rotas.at(rotaMelhorDesterceirizacao).setCapacidadeAtualRota(rotas.at(rotaMelhorDesterceirizacao).getCapacidadeAtualRota() + demandas.at(clienteInsert - 1));

        //Remove o vértice que acabou de ser desterceirizado da lista de clientes terceirizados
        solucao->removeClienteTerceirizado(indiceMelhorDesterceirizacao);

        //Atualiza o custo de roteamento
        solucao->setCustoRoteamento(solucao->getCustoRoteamento() + menorDeltaRoteamento);

        //Atualiza o custo de terceirização
        solucao->setCustoTerceirizacao(solucao->getCustoTerceirizacao() + melhorCustoTerceirizacaoDelta);

        //Atualiza o número de entregas que não foram terceirizadas
        solucao->setEntregasNaoTerceirizadas(solucao->getEntregasNaoTerceirizadas() + 1);


        return true;
    }

    return false;
}

double VND(ProblemaCondicoes *condicoes, Solucao *solucao){

    const int numEstruturasVizinhanca = 4;

    int estruturaAtual = 0;
    bool movimentoAconteceu = false;

    auto inicio = std::chrono::high_resolution_clock::now();

    while(estruturaAtual < numEstruturasVizinhanca){
        switch(estruturaAtual){
            case 2:{
                movimentoAconteceu = swap(condicoes, solucao);
                if(!movimentoAconteceu){
                    estruturaAtual++;
                }
                else{
                    estruturaAtual = 0;
                }
                break;
            }

            case 1:{
                movimentoAconteceu = swapInterRotas(condicoes, solucao);
                if(!movimentoAconteceu){
                    estruturaAtual++;
                }
                else{
                    estruturaAtual = 0;
                }
                break;
            }

            case 3:{
                movimentoAconteceu = melhorTerceirizacao(condicoes, solucao);
                if(!movimentoAconteceu){
                    estruturaAtual++;
                }
                else{
                    estruturaAtual = 0;
                }
                break;
            }
            case 0:{
                movimentoAconteceu = desterceirizaVertice(condicoes, solucao);
                if(!movimentoAconteceu){
                    estruturaAtual++;
                }
                else{
                    estruturaAtual = 0;
                }
                break;
            }
        }
    }

    auto resultado = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - inicio);
    double seconds = static_cast<double>(resultado.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den;

    return seconds;
}