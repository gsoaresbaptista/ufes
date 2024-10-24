#include "ComparacoesCapes.hpp"

namespace capes {

    bool ComparacoesCapes::ordenarPrograma(Programa* p1, Programa* p2){
        return stringCompare(p1->getTitulo(), p2->getTitulo());
    }

    bool ComparacoesCapes::ordenarIES(Instituicao* i1, Instituicao* i2) {
        // Compara as siglas
        int cmp = i1->getSigla().compare(i2->getSigla());
        if(cmp > 0) return false; else if(cmp < 0) return true;
        // Se a sigla for igual, compara os nomes
        else return stringCompare(i1->getNome(), i2->getNome());
    }

    bool ComparacoesCapes::ordenarPorCodigo(Programa* p1, Programa* p2) {
        return stringCompare(p1->getCodigo(), p2->getCodigo());
    }

}