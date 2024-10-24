#ifndef CAPES_HPP_
#define CAPES_HPP_

// Include das classes base do programa
#include "CSV.hpp"
#include "Indice.hpp"
#include "Instituicao.hpp"
#include "Programa.hpp"

// Include das classes de comparacao
#include "ComparacoesCapes.hpp"
#include "ComparacoesProducoes.hpp"

// Include da biblioteca util
#include "Util.hpp"

// Includes de bibliotecas padroes
#include <iostream>
#include <vector>
#include <stdexcept>
#include <unordered_map>

// Include das Producoes
#include "Producao.hpp"
#include "Anais.hpp"
#include "Artjr.hpp"
#include "Artpe.hpp"
#include "Outro.hpp"
#include "Traducao.hpp"
#include "Livro.hpp"
#include "Partmu.hpp"

using namespace csv;
using namespace producoes;

namespace capes {

    class Capes {
        private:
            CSV arquivo;
            Indice* indice;
            unordered_map<string, Instituicao*> instituicoes;
            unordered_map<string, Programa*> programas;
            vector<Producao*> producoes;
        public:
            /*
            *  Construtor da classe Capes, pode lancar um
            *  erro na pilha caso o arquivo nao seja aberto
            *  corretamente.
            */
            Capes(const string&);

            /*
            *   Le o arquivo csv populando as estrutudas de
            *   dados internas com as informacoes de todas
            *   linhas do arquivo.
            */
            void processar();


            /*
            * Funcoes responsaveis por instanciar as novas
            * producoes utilizando os dados atuais da linha
            * lida no csv.
            */
            // Instancia uma producao do tipo Anais
            Anais* novoAnais();
            // Instancia uma producao do tipo Artigo em jorais e revistas
            Artjr* novoArtjr();
            // Instancia uma producao do tipo Outro
            Outro* novoOutro();
            // Instancia uma producao do tipo Traducao
            Traducao* novaTraducao();
            // Instancia uma producao do tipo Artigo em periodicos
            Artpe* novoArtpe();
            // Instancia uma producao do tipo Livro
            Livro* novoLivro();
            // Instancia uma producao do tipo Partitura musical
            Partmu* novaPartmu();

            /*
             * Adicina uma nova instituicao
             *
             * Instancia uma nova instituicao com os dados da linha atual,
             * e a insere no mapa, caso a mesma ja nao tenha sido inserida.
             *
             * @return Ponteiro para a nova instituicao que acabou de ser
             * instanciada e inserida no mapa, ou caso ja exista, um ponteiro
             * para a instituicao que ja estava instanciada correspondente.
             */
            Instituicao* adicionarInstituicao();

            /*
            * Adicina um novo Programa de Pos Graduacao
            *
            * Instancia um novo programa com os dados da linha atual,
            * e o insere no mapa, caso o mesmo ja nao tenha sido inserido.
            *
            * @return Ponteiro para o novo programa que acabou de ser
            * instanciado e inserido no mapa, ou caso ja exista, um ponteiro
            * para o programa que ja estava instanciada correspondente.
            */
            Programa* adicionarPrograma();

            /*
            * Adicina uma nova Producao
            *
            * Instancia uma nova producao com os dados da linha atual,
            * e a insere no vetor.
            *
            * @return Ponteiro para a nova producao que acabou de ser
            * instanciada e inserida no vetor.
            */
            Producao* adicionarProducao();

            /*
             * Getter para pegar UM programa
             *
             * Pega um programa no mapa de acordo com o codigo informado.
             *
             * @param codigo string com o codigo do programa.
             * @return Ponteiro para o programa caso encontrado, caso contrario
             * retorna nullptr.
             */
            Programa* getPrograma(const string&);

            /*
             * Calcula a quantidade de paginas
             *
             *
             * @param inicial string com o numero da primeira pagina.
             * @param final string com o numero da ultima pagina.
             * @return Inteiro correspondente a quantidade de paginas
             * se este for valido, caso contrario retorna 0;
             */
            int calcularPaginas(const string&, const string&);

            /*
             * Pega inteiro correspondente ao tipo da producao
             *
             * @param tipo string com o nome da producao.
             * @return Inteiro correspondente ao tipo da producao
             * informado.
             */
            int producaoTipo(const string&);

            /*
             * Imprime o cabecalho da funcao informada no
             * ostream passado.
             *
             * @param tipo inteiro correspondente ao tipo da producao.
             * @param out ostream que sera impresso o cabecalho.
             */
            void imprimirCabecalho(const int&, ostream&);

            /*
             * Atualiza o arquivo csv
             *
             * @param endereco string com o caminho completo do
             * novo arquivo csv.
             */
            void atualizarArquivo(const string&);

            /*
             * Imprime um relatorio geral dos dados lidos.
             *
             * @return string com o relatorio para ser impresso.
             */
            string relatorio() const;

            /*
             * Executa um comando
             *
             * Executa um comando apartir do seu nome.
             *
             * @param string com o nome do comando
             */
            void executarComando(const string&);

            /*
             * Comando Rede
             *
             * Imprime, na saida padrao os programas que estao
             * em rede, ou seja, que estao ligas a mais de uma
             * instituicao.
             */
            void comandoRede();

            /*
             * Comando PPG
             *
             * Imprime um relatorio geral acerca do programa
             * informad na saida padrao, como o numero de
             * producao por tipo.
             *
             * @param codigo string com o codigo da producao.
             */
            void comandoPPG(const string&);

            /*
            * Comando IES
            *
            * Imprime um as instituicoes que tem a sigla
            * igual a informada, alem de seus programas e
            * a quantidade de producoes de cada um.
            *
            * @param sigla string com a sigla da instituicao
            */
            void comandoIES(const string&);

            /*
            * Comando CSV
            *
            * Imprime a saida padrao, as producoes do tipo pedido
            * para o programa com o codigo correspondente, no
            * formato de csv.
            *
            * @param codigo string com o codigo da producao
             * @param tipo inteiro com o numero correspondente da
             * producao que se deseja imprimir.
            */
            void comandoCSV(const string&, const int&);

            ~ Capes();
    };

}

#endif