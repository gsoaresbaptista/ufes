#ifndef INDICE_HPP_
#define INDICE_HPP_

#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

/**
 * Cabecalho do arquivo CSV da CAPES, ou seja, tem registrado todas
 * os nomes necessario dos arquivos de cada tipo de producao, usados
 * para saber os indices de cada informacao, tornando-se desnecessario
 * utilizar numeros fixos para recolher dados dos vetores de string.
 *
 */
namespace capes {
  
    class Indice {
        private:
            unordered_map<string, int> indices;
        public:
            Indice(const vector<string>&);

            int codigoPPG() ;
            int dataDePublicacao() ;
            int editora() ;
            int editoraTradu() ;
            int idioma() ;
            int issn() ;
            int isbn() ;
            int natureza() ;
            int cidade() ;
            int cidadePais() ;
            int paginasContribuicao() ;
            int nomePPG() ;
            int evento() ;
            int nomeIES() ;
            int tituloProducao() ;
            int paginaFinal() ;
            int paginaInicial() ;
            int paginas() ;
            int siglaIES() ;
            int subTipoDeProducao() ;
            int tipoDeProducao() ;
            int volume() ;
            int fasciculo() ;
            int serie() ;
            int formacaoInstrumental() ;
            int idiomaTraducao() ;
    };
    
}

#endif