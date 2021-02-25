#ifndef PRODUCAO_HPP_
#define PRODUCAO_HPP_

#include <string>
#include <ostream>
#include <sstream>

using namespace std;

namespace producoes {

    class Producao {
        protected:
            string titulo;
            string idioma;
            string cidade;
            string natureza;
            int paginas;
            int tipo;
        public:
            /*
             * Construtor da Classe Producoes
             *
             * - Os parametros sao, respectivamente:
             * @param titulo string titulo.
             * @param idioma string idioma.
             * @param cidade string cidade.
             * @param natureza string natureza.
             * @param paginas int quantidade de paginas.
             * @param tipo int representando o id da producao.
             */
            Producao(const string&, const string&,
                    const string&, const string&,
                    const int&, const int&);

            /*
            * Getter para int Paginas
            *
            * @return inteiro com o numero de paginas.
            */
            int getPaginas() const;

            /*
            * Getter para int tipo
            *
            * @return inteiro com o id do tipo da producao.
            */
            int getTipo() const;

            friend ostream& operator<< (ostream&, const Producao&);

            /*
            * Metodo abstrado print, utilizado nas horas em que o
            * operador de saida (<<) e' chamada para alguma producao
            * arbritaria, portanto, o metodo print e responsavel por
            * modificar o ostream (cout) com a saida correspondente do
            * tipo de producao chamada, por isso abstrato e virtual.
            * */
            virtual void print(ostream&) const = 0;

            /*
            * Sobreescrita do operador de menor, usado para comparacoes,
            * principalmente na hora de ordenacao com a funcao sort,
            * observe que nenhum tipo de verificacao e' feita quanto a
            * conversao de tipo.
            *
            * @param ref Producao que deseja verificar se e' menor.
            * @return true caso ref seja menor do que this, false
            * caso ref seja maior que this.
            */
            virtual bool operator<(const Producao&) = 0;

            /*
             * Metodo criado para pegar a impressao de um
             * objeto em um ostream e retornar como um texto,
             * funcionando semelhante ao toString do Java.
             *
             * @return string com a impressao padrao do objeto
             * chamado.
             */
            virtual string toString() const;

            virtual ~ Producao() {};

    };

}

#endif