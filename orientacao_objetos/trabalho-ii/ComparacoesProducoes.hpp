//
// Created by gabriel on 07/07/19.
//

#ifndef COMPARACOESPRODUCOES_HPP_
#define COMPARACOESPRODUCOES_HPP_

#include "Util.hpp"

#include "Producao.hpp"
#include "Anais.hpp"
#include "Artjr.hpp"
#include "Artpe.hpp"
#include "Outro.hpp"
#include "Traducao.hpp"
#include "Livro.hpp"
#include "Partmu.hpp"

#include <vector>
#include <string>
#include <sstream>

using namespace cpp_util;
using namespace std;

namespace producoes {

    class ComparacoesProducoes {
        public:
            /*
             * Funcao de comparacao para producoes
             *
             * E' necessario utilizar esta funcao devido
             * as estruturas guardarem ponteiros, logo,
             * a funcao apenas retorna a se a segunda producao
             * e menor que a primeira.
             *
             * @param p1 Ponteiro para producao
             * @param p2 Ponteiro para producao
             */
            static bool comparar(Producao*, Producao*);
    };

}

#endif
