#include "Producao.hpp"

namespace producoes {
    
    Producao::Producao(const string& tit, const string& idi, 
      const string& cid, const string& nat, const int& pag,
      const int& tip) : titulo(tit), idioma(idi), cidade(cid),
       natureza(nat), paginas(pag), tipo(tip) {}
    
    ostream& operator<< (ostream& out, const Producao& producao) {
        /* Chama o metodo print, utilizando amarracao tardia
         * (metodo virtual) para imprimir a saida correspondente
         * ao tipo */
        producao.print(out);
        return out;
    }

    string Producao::toString() const {
        // Cria um ostream de string
        stringstream texto;
        // Joga a saida do print no ostream criado
        print(texto);
        return texto.str();
    };

    int Producao::getPaginas() const {
        return this->paginas;
    }

    int Producao::getTipo() const {
        return this->tipo;
    }

}