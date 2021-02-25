#include "Artjr.hpp"

namespace producoes {

    Artjr::Artjr(const string& titulo, const string& idioma, const string& cidade,
      const int& paginas, const int& tipo, const time_t& data_, const string& issn_)
      : Producao(titulo, idioma, cidade, "", paginas, tipo),
      issn(issn_), data(data_) {}

    void Artjr::print(ostream& out) const {
        out << titulo << ";" << idioma << ";" << cidade
            << ";" << formatDate(data, DATE_FORMAT_PT_BR_SHORT)
            << ";" << issn << ";";
        //Verifica se paginas sera impresso
        if(paginas > 0) out << paginas;
    }

    bool Artjr::operator<(const Producao& ref) {
        //Converte para Artjr para poder acessar os dados
        Artjr& o = (Artjr&) ref;
        //Compara os dados individualmente
        int cmp = titulo.compare(o.titulo);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = idioma.compare(o.idioma);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = cidade.compare(o.cidade);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = data - o.data;
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = issn.compare(o.issn);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = paginas - o.paginas;
        if(cmp<0) return true; else return false;
    }

}