#include "Traducao.hpp"

namespace producoes {

     Traducao::Traducao(const string& titulo, const string& idioma,
             const string& cidade, const string& natureza, const int& paginas,
             const int& tipo, const string& edi, const string& trad)
             : Producao(titulo, idioma, cidade, natureza, paginas,
              tipo), editora(edi), traducao(trad) {}

    void Traducao::print(ostream& out) const {
        out << natureza << ";" << titulo << ";" << idioma
            << ";" << editora << ";" << cidade << ";"
            << traducao << ";";
        //Verifica se paginas sera impresso
        if(paginas > 0) out <<  paginas;
    }

    bool Traducao::operator<(const Producao& ref) {
        //Converte para Traducao para poder acessar os dados
        Traducao& o = (Traducao&) ref;
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
        cmp = traducao.compare(o.traducao);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = paginas - o.paginas;
        if(cmp<0) return true; else return false;
    }

}