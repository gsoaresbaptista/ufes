#ifndef COMPARACOESCAPES_HPP_
#define COMPARACOESCAPES_HPP_

#include "Programa.hpp"
#include "Instituicao.hpp"

namespace capes {
    class Programa;
    class Instituicao;

    class ComparacoesCapes {
        public:
            /**
             * Funcao de ordenacao dos programas
             *
             * Ordena os programas de acordo com o titulo.
             *
             * @param p1 Ponteiro para programa
             * @param p2 Ponteiro para programa
             * @return true se a primeira e menor que a segunda
             * e falso se a primeira for maior ou igual a segunda.
             */
            static bool ordenarPrograma(Programa*, Programa*);

            /**
            * Funcao de ordenacao dos programas
            *
            * Ordena os programas de acordo o codigo.
            *
            * @param p1 Ponteiro para programa
            * @param p2 Ponteiro para programa
            * @return true se a primeira e menor que a segunda
            * e falso se a primeira for maior ou igual a segunda.
            */
            static bool ordenarPorCodigo(Programa* p1, Programa* p2);

            /**
            * Funcao de ordenacao as instituicoes
            *
            * Ordena as instituicoes de acordo com a sigla,
            * se as siglas forem iguais, ordena com o nome.
            *
            * @param p1 Ponteiro para programa
            * @param p2 Ponteiro para programa
            * @return true se a primeira e menor que a segunda
            * e falso se a primeira for maior ou igual a segunda.
            */
            static bool ordenarIES(Instituicao* i1, Instituicao* i2);

    };

}


#endif
