#include "ProblemaCondicoes.hpp"

ProblemaCondicoes::ProblemaCondicoes(){
    this->n = 0;
    this->k = 0;
    this->Q = 0;
    this->L = 0;
    this->r = 0;
}

int ProblemaCondicoes::getN() const {
    return n;
}

int ProblemaCondicoes::getK() const {
    return k;
}

int ProblemaCondicoes::getQ() const {
    return Q;
}

int ProblemaCondicoes::getL() const {
    return L;
}

int ProblemaCondicoes::getR() const {
    return r;
}

std::vector<int> ProblemaCondicoes::getD() const {
    return d;
}

std::vector<int> ProblemaCondicoes::getP() const {
    return p;
}

std::vector<std::vector<int>> ProblemaCondicoes::getC() const {
    return c;
}

// Setters
void ProblemaCondicoes::setN(int n) {
    this->n = n;
}

void ProblemaCondicoes::setK(int k) {
    this->k = k;
}

void ProblemaCondicoes::setQ(int Q) {
    this->Q = Q;
}

void ProblemaCondicoes::setL(int L) {
    this->L = L;
}

void ProblemaCondicoes::setR(int r) {
    this->r = r;
}

void ProblemaCondicoes::setD(const std::vector<int>& d) {
    this->d = d;
}

void ProblemaCondicoes::setP(const std::vector<int>& p) {
    this->p = p;
}

void ProblemaCondicoes::setC(const std::vector<std::vector<int>>& c) {
    this->c = c;
}

void ProblemaCondicoes::printaObjeto(){
    std::cout << "n: " << n << "\n";
    std::cout << "k: " << k << "\n";
    std::cout << "Q: " << Q << "\n";
    std::cout << "L: " << L << "\n";
    std::cout << "r: " << r << "\n";

    std::cout << "d: ";
    for (int i : d){
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "p: ";
    for (int i : p){
        std::cout << i << " ";
    }
    std::cout << "\n";

    std::cout << "c: ";
    for (std::vector<int> i : c){
        std::cout << "\n";
        for (int j : i){
            std::cout << j << " ";
        }
    }

}