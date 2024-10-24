#ifndef ARTPE_HPP_
#define ARTPE_HPP_

#include "Producao.hpp"
#include <string>
#include "Util.hpp"

using namespace cpp_util;

using namespace std;

namespace producoes {

    class Artpe : public Producao {
        private:
            string editora;
            int volume;
            string issn;
            int fasciculo;
            int serie;
        public:
            /**
            * Construtor da Classe Artpe
            *
            * - Os parametros sao, respectivamente:
            * @param idioma string idioma.
            * @param cidade string cidade.
            * @param natureza string natureza.
            * @param paginas int quantidade de paginas.
            * @param tipo int representando o id da producao.
            * @param edi string editora.
            * @param vol int volume.
            * @param fas int fasciculo.
            * @param ser int serie.
            */
            Artpe(const string&, const string&, const string&, const int&,
                    const int&, const string&, const int&, const string&,
                    const int&, const int&);

            /*
            * Sobreescrita do metodo abstrado print, utilizado nas horas
            * em que o operador de saida (<<) e' chamada para qualquer
            * producao, portanto, o metodo print e responsavel por
            * modificar ostream (cout) com a saida correspondente do
            * tipo ARTPE.
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

            virtual ~ Artpe() {};
    };

}

#endif