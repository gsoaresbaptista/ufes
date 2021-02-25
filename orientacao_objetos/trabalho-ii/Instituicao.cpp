#include "Instituicao.hpp"

namespace capes {

    Instituicao::Instituicao(const string& nome_, const string& sigla_)
        : nome(nome_), sigla(sigla_), programas() {}

    void Instituicao::addPrograma(Programa* programa) {
        vector<Programa*>::iterator it;
        // Verifica se ja existe um programa igual inserido
        it = find(programas.begin(), programas.end(), programa);
        // Se nao existe, o insere no vetor
        if(it == programas.end())
            programas.push_back(programa);
    }

    string Instituicao::getSigla() const {
        return sigla;
    }

    string Instituicao::getNome() const {
        return nome;
    }

    void Instituicao::getProducoes(ostream& out) {
        // Ordena os programas
        sort(this->programas.begin(), this->programas.end(),
             ComparacoesCapes::ordenarPrograma);
        /* Percorre os programas imprimindo as informacoes */
        for(auto& ppg : programas) {
            out << "\t- " << ppg->getTitulo() << ": "
                << ppg->getNumeroDeProducoes() << " producoes"
                << endl;
        }
    }

}