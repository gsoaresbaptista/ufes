#include "CSV.hpp"

namespace csv {

    CSV::CSV(const string& endereco) : arquivo(endereco), dados() {
        // Verifica se o arquivo foi aberto corretamente.
        if(!arquivo.good())
            throw invalid_argument("Erro de I/O");
        // Atualiza a proxima linha
        proximaLinha();
    }

    void CSV::proximaLinha() {
        // Instancia e le a linha.
        string linha;
        getline(arquivo, linha);
        // Formata a linha e atualiza os dados
        Tokenizer token(linha, ';');
        dados = token.remaining();
    }

    bool CSV::fim() const {
        // Deve tentar ler uma linha em branco para
        // que a funcao retorne verdadeiro.
        return arquivo.eof();
    }

    string CSV::get(const int& indice) const {
        return dados[indice];
    }

    vector<string> CSV::getLinha() {
        return dados;
    }

    void CSV::alterarFluxo(const string& endereco) {
        // Fecha o arquivo atual
        arquivo.close();
        // Abre o novo arquivo
        arquivo.open(endereco);
        //Verifica se o arquivo foi aberto corretamente.
        if(!arquivo.good())
            throw invalid_argument("Erro de I/O");
        // Atualiza a proxima linha
        this->proximaLinha();
    }
}