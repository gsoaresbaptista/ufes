#ifndef PROGRAMA_HPP_
#define PROGRAMA_HPP_

// Include das bibliotecas padroes
#include <string>
#include <vector>
#include <ostream>

// Include da biblioteca util
#include "Util.hpp"

// Include das classes do programa
#include "Producao.hpp"
#include "Instituicao.hpp"
#include "ComparacoesCapes.hpp"

using namespace std;
using namespace producoes;
using namespace cpp_util;

namespace capes {

    class Instituicao;

    class Programa {
        private:
            string codigo;
            string titulo;
            vector<Instituicao*> instituicoes;
            vector<Producao*> producoes;
        public:
			/*
			* Construtor da classe Programa
			*
			* - Parametros sao, respectivamente:
			* @param cod string codigo do programa.
			* @param tit string titulo do programa.
			*/
            Programa(const string&, const string&);

			/*
			* Adiciona uma instituicao no Programa, verificando
			* previamente se esta ja se encontra no programa.
			*
			* @param instituicao Ponteiro para instituicao.
			*/
            void addInstituicao(Instituicao*);

			/*
			* Adiciona uma producao no Programa, verificando
			* previamente se esta ja se encontra no programa.
			*
			* @param producao Ponteiro para producao.
			*/
            void addProducao(Producao*);

            /*
            * Verifica se o programa esta em rede
            *
            * return true caso tenha mais de uma instituicao
            * ligada ao programa, false caso contraio.
            */
            bool emRede();

            /*
             * Getter para o codigo do programa
             *
             * @return string com o codigo do programa
            */
            string getCodigo();

            /*
            * Getter para o titulo do programa
            *
            * @return string com o titulo do programa
            */
            string getTitulo();

            /*
            * Getter para as producoes do programa
            *
            * @param tipo inteiro correspondente ao tipo de producao.
            * @return vector com as producoes com as producoes
            * com o mesmo tipo do que o informado.
            */
            vector<Producao*> getProducoes(const int&);

            /*
            * 'Getter' para as instituicoes do programa
            *
            * Imprime no ostream informado as instituicoes que
            * estao ligadas ao programa em questao.
            */
            void getInstituicoes(ostream&);

            /**
             * Efetua as contagem das producoes do programa
             *
             * @return vector<int> com a quantidade de cada
             * producao, e na ultima posicao a quantidade de
             * paginas.
             */
            vector<int> efetuarContagem();

            /*
             * 'Getter' para o relatorio das producoes
             *
             * Imprime um relatorio acerca das producoes do
             * programa em questao no ostream informado.
             *
             * @param out ostream onde sera impresso os dados.
             */
            void getRelatorio(ostream&);

            /*
             * Retorna o numero de producoes do programa
             *
             * @return inteiro correspondente ao numero de
             * producoes registradas no programa(tamanho do
             * vetor de producoes).
             */
            int getNumeroDeProducoes();

            /*
            * Sobreescrita do operador de saida do programa
            */
            friend ostream& operator<< (ostream&, Programa&);

            ~ Programa() {};
    };

}

#endif