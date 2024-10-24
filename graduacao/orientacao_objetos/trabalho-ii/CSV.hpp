#ifndef CSV_HPP_
#define CSV_HPP_

#include <string>
#include <fstream>
#include <vector>
#include "Util.hpp"
#include <stdexcept>
#include <iostream>
#include "Util.hpp"

using namespace std;
using namespace cpp_util;

namespace csv {

    class CSV {
        private:
            ifstream arquivo;
            vector<string> dados;
        public:
            /*  
            *   Cria um novo arquivo ifstream para leitura,
            *   caso o arquivo nao seja aberto corretamente
            *   lanca um erro na pilha de execucao.
            */        
            CSV(const string&);

            /*
             *  Le a proxima linha do arquivo e atualiza
             *  os dados com as novas informacoes. 
             */
            void proximaLinha();

            /*
             *  Verifica se o arquivo csv chegou no fim.
             *
             *  @return true caso o arquivo tenha acabado,
             *  caso contrario retorna false;
             */
            bool fim() const;

            /*
             *  Retorna o dados correspondente ao indice
             *  passado como parametro. 
            */
            string get(const int&) const;

            /*
             * Retorna um vetor com os dados da linha
             * todos separados de acordo com a especificacao
             * do arquivo.
             *
             * @return Vector<string> com os dados separados.
             */
            vector<string> getLinha();

            /*
             * Altera o arquivo de entrada sem a necessidade de
             * criar um novo objeto da classe CSV.
             */
            void alterarFluxo(const string&);

            ~ CSV() {};
    };

}

#endif