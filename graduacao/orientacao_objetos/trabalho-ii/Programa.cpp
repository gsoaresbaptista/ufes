#include <iostream>
#include "Programa.hpp"

namespace capes {

    Programa::Programa(const string& cod, const string& tit)
        : codigo(cod), titulo(tit), instituicoes() {}

    void Programa::addInstituicao(Instituicao* instituicao) {
        vector<Instituicao*>::iterator it;
        // Verifica se existe uma instituicao igual
        it = find(instituicoes.begin(), instituicoes.end(), instituicao);
        // Se nao existir a insere
        if(it == instituicoes.end())
            instituicoes.push_back(instituicao);
    }

    void Programa::addProducao(Producao* producao) {
        producoes.push_back(producao);
    }

    bool Programa::emRede() {
        if(instituicoes.size() > 1) return true;
        else return false;
    }

    void Programa::getInstituicoes(ostream& out) {
        // Ordena as instituicoes
        sort(this->instituicoes.begin(), this->instituicoes.end(), ComparacoesCapes::ordenarIES);
        // Iterra sobre as insituicoes imprimindo cada uma
        for(auto& ies: instituicoes) {
            out << "\t- " << ies->getSigla() << " (" <<
                ies->getNome() << ")" << endl;
        }
    }

    vector<Producao*> Programa::getProducoes(const int& tipo) {
        vector<Producao*> encontradas;
        // Percorre as producoes procurando as que tem mesmo id
        for(auto& prod : producoes) {
            if(prod->getTipo() == tipo) {
                /* Se encontrou verifica se ja existe a producao
                no vetor de encontradas */
                bool achou = false;
                for(auto& p2 : encontradas)
                    if(p2->toString() == prod->toString()) {
                        /* Se encontrou altera a flag e quebra
                        o laco */
                        achou = true;
                        break;
                    }
                // Se nao existe no vetor a insere
                if(!achou) encontradas.push_back(prod);
            }
        }
        // Retorna o vetor de producoes com o id informado
        return encontradas;
    }

    vector<int> Programa::efetuarContagem() {
        vector<int> contagem(8,0);
        // Percorre as producoes fazendo as contagens
        for(auto& prod: producoes) {
            int tipo = prod->getTipo();
            if(tipo == 8) contagem[0]++;
            if(tipo == 9) contagem[1]++;
            if(tipo == 10) contagem[2]++;
            if(tipo == 21) contagem[3]++;
            if(tipo == 25) contagem[4]++;
            if(tipo == 26) contagem[5]++;
            if(tipo == 28) contagem[6]++;
            // Incrementa as paginas
            contagem[7] += prod->getPaginas();
        }
        // Retorna o vetor populado
        return contagem;
    }

    void Programa::getRelatorio(ostream& out) {
        // Faz a contagem de dados
        vector<int> contagem = efetuarContagem();
        // Imprime os valores
        out << "\t- Artigos em anais de eventos: " << contagem[0] << endl;
        out << "\t- Artigos em jornais e revistas: " << contagem[1] << endl;
        out << "\t- Artigos em periodicos cientificos: " << contagem[4] << endl;
        out << "\t- Livros: " << contagem[5] << endl;
        out << "\t- Partituras musicais: " << contagem[6] << endl;
        out << "\t- Traducoes: " << contagem[3] << endl;
        out << "\t- Outros: " << contagem[2] << endl << endl;
        out << "Total de paginas produzidas pelo PPG: " << contagem[7];
    }

    ostream& operator<< (ostream& out, Programa& programa) {
        out << programa.codigo << ": " << programa.titulo << endl;
        return out;
    }

    string Programa::getCodigo() {
        return codigo;
    }

    string Programa::getTitulo() {
        return titulo;
    }

    int Programa::getNumeroDeProducoes() {
        return producoes.size();
    }
    
}