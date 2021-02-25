#include "Indice.hpp"

namespace capes {

    Indice::Indice(const vector<string>& cabecalho) {
        /* Percorre o cabecalho inserido na mapa com o indice
        correspondente */
        for(int i = 0; i < cabecalho.size(); i++) {
            indices.insert({cabecalho[i], i});
        }
    }

    int Indice::codigoPPG() {
        return this->indices.find("CD_PROGRAMA_IES")->second;
    }

    int Indice::dataDePublicacao() {
        return this->indices.find("DT_PUBLICACAO")->second;
    }

    int Indice::editora() {
        return this->indices.find("NM_EDITORA")->second;
    }

    int Indice::editoraTradu() {
        return this->indices.find("NM_EDITORA_TRADUCAO")->second;
    }

    int Indice::idioma() {
        return this->indices.find("DS_IDIOMA")->second;
    }

    int Indice::issn() {
        return this->indices.find("DS_ISSN")->second;
    }

    int Indice::isbn() {
        return this->indices.find("DS_ISBN")->second;
    }

    int Indice::natureza() {
        return this->indices.find("DS_NATUREZA")->second;
    }

    int Indice::cidade() {
        return this->indices.find("NM_CIDADE")->second;
    }

    int Indice::cidadePais() {
        return this->indices.find("NM_CIDADE_PAIS")->second;
    }

    int Indice::paginasContribuicao() {
        return this->indices.find("NR_PAGINAS_CONTRIBUICAO")->second;
    }

    int Indice::nomePPG() {
        return this->indices.find("NM_PROGRAMA_IES")->second;
    }

    int Indice::evento() {
        return this->indices.find("DS_EVENTO")->second;
    }

    int Indice::nomeIES() {
        return this->indices.find("NM_ENTIDADE_ENSINO")->second;
    }

    int Indice::tituloProducao() {
        return this->indices.find("NM_TITULO")->second;
    }

    int Indice::paginaFinal() {
        return this->indices.find("NR_PAGINA_FINAL")->second;
    }

    int Indice::paginaInicial() {
        return this->indices.find("NR_PAGINA_INICIAL")->second;
    }

    int Indice::paginas() {
        return this->indices.find("NR_PAGINAS")->second;
    }

    int Indice::siglaIES() {
        return this->indices.find("SG_ENTIDADE_ENSINO")->second;
    }

    int Indice::subTipoDeProducao() {
        return this->indices.find("ID_SUBTIPO_PRODUCAO")->second;
    }

    int Indice::tipoDeProducao() {
        return this->indices.find("ID_TIPO_PRODUCAO")->second;
    }

    int Indice::volume() {
        return this->indices.find("NR_VOLUME")->second;
    }

    int Indice::fasciculo() {
        return this->indices.find("DS_FASCICULO")->second;
    }

    int Indice::serie() {
        return this->indices.find("NR_SERIE")->second;
    }

    int Indice::formacaoInstrumental() {
        return this->indices.find("DS_FORMACAO_INSTRUMENTAL")->second;
    }

    int Indice::idiomaTraducao() {
        return this->indices.find("DS_IDIOMA_TRADUCAO")->second;
    }

}