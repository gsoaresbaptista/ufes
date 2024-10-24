#ifndef INSTITUICAO_HPP_
#define INSTITUICAO_HPP_

// Include das classes do programa
#include "Programa.hpp"
#include "ComparacoesCapes.hpp"

// Include das bibliotecas padroe
#include <string>
#include <vector>

using namespace std;

namespace capes {

    class Programa;

    class Instituicao {
        private:
            string nome;
            string sigla;
            vector<Programa*> programas;
        public:
            /*
            * Construtor da classe Instituicao
            *
            * - Os parametros sao respectivamente:
            * @param nome string com o nome da instituicao.
            * @param sigla string com a sigla da instituicao.
            */
            Instituicao(const string&, const string&);

            /*
            * Getter para a sigla da instituicao
            *
            * @return string com a sigla da instituicao.
            */
            string getSigla() const;

            /*
            * Getter para o nome da instituicao
            *
            * @return string com a sigla da instituicao.
            */
            string getNome() const;

            /*
            * 'Getter' para as producoes
            *
            * Percorre a lista de producoes, ja ordenada
            * e as imprime no ostream passado.
            */
            void getProducoes(ostream&);

            /*
            * Adiciona um programa na instituicao,
             * verificando previamente se a instituicao
             * ja foi inserida.
             *
             * @param programa Ponteiro para programa.
            */
            void addPrograma(Programa*);

            ~ Instituicao() {};
    };

}

#endif