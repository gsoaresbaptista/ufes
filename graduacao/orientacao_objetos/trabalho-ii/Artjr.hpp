#ifndef ARTJR_HPP_
#define ARTJR_HPP_

#include "Producao.hpp"
#include "Util.hpp"
#include <string>
#include <ctime>

using namespace std;
using namespace cpp_util;

namespace producoes {

    class Artjr : public Producao {
        private:
            string issn;
            time_t data;
        public:
            /**
            * Construtor da Classe Artjr
            *
            * - Os parametros sao, respectivamente:
            * @param titulo string titulo.
            * @param idioma string idioma.
            * @param cidade string cidade.
            * @param paginas int quantidade de paginas.
            * @param tipo int representando o id da producao.
            * @param data time_t com a data da producao.
            * @param issn_ string issn.
            */
            Artjr(const string&, const string&, const string&, 
              const int&, const int&, const time_t&, const string&);

            /***
            * Getter para time_t data
            *
            * @return long int correspondente a data da producao.
            */
            long int getData() {
                return this->data;
            }

            /***
            * Getter para string ISSN
            *
            * @return string correspondente ao issn da producao.
            */
            string getIssn() {
                return this->issn;
            }

            /*
            * Sobreescrita do metodo abstrado print, utilizado nas horas
            * em que o operador de saida (<<) e' chamada para qualquer
            * producao, portanto, o metodo print e responsavel por
            * modificar ostream (cout) com a saida correspondente do
            * tipo ARTJR.
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


            virtual ~ Artjr() {};
    };

}

#endif