#ifndef OUTRO_HPP_
#define OUTRO_HPP_

#include <string>
#include <ostream>
#include "Producao.hpp"

using namespace std;

namespace producoes {

    class Outro : public Producao {
        private:
            string editora;
        public:
            /**
            * Construtor da Classe Outro
            *
            * - Os parametros sao, respectivamente:
            * @param idioma string idioma.
            * @param cidade string cidade.
            * @param natureza string natureza.
            * @param paginas int quantidade de paginas.
            * @param tipo int representando o id da producao.
            * @param edi string editora.
            */
            Outro(const string&, const string&,
              const string&, const int&, const int&, const string&);

            /*
            * Sobreescrita do metodo abstrado print, utilizado nas horas
            * em que o operador de saida (<<) e' chamada para qualquer
            * producao, portanto, o metodo print e responsavel por
            * modificar ostream (cout) com a saida correspondente do
            * tipo Outro.
            * */
            virtual void print (ostream&) const;

            /**
            * Sobreescrita do operador de menor, usado para comparacoes,
            * principalmente na hora de ordenacao com a funcao sort,
            * observe que nenhum tipo de verificacao e' feita quanto a
            * conversao de tipo.
            *
            * @param ref Producao que deseja verificar se e' menor.
            * @return true caso ref seja menor do que this, false
            * caso ref seja maior que this.
            */
            virtual bool operator<(const Producao&);

            virtual ~ Outro() {};
    };

}

#endif