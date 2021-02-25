#include "Livro.hpp"

namespace producoes {
    /**
     *
     * @param titulo
     * @param idioma
     * @param cidade
     * @param natureza
     * @param paginas
     * @param tipo
     * @param edi
     * @param isbn_
     */
     Livro::Livro(const string& titulo, const string& idioma, const string& cidade,
      const string& natureza, const int& paginas, const int& tipo, const string& edi,
      const string& isbn_): Producao(titulo, idioma, cidade, natureza, paginas, tipo),
      editora(edi), isbn(isbn_) {}

    void Livro::print(ostream& out) const {
        out << natureza << ";" << titulo << ";" << idioma
            << ";" << editora << ";" << cidade << ";"
            << isbn << ";";
        //Verifica se paginas sera impresso
        if(paginas != 0) out << paginas;
    }

    bool Livro::operator<(const Producao& ref) {
        //Converte para Livro para poder acessar os dados
        Livro& o = (Livro&) ref;
        //Compara os dados individualmente
        int cmp = natureza.compare(o.natureza);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = titulo.compare(o.titulo);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = idioma.compare(o.idioma);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = editora.compare(o.editora);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = cidade.compare(o.cidade);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = isbn.compare(o.isbn);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = paginas - o.paginas;
        if(cmp<0) return true; else return false;
    }

}