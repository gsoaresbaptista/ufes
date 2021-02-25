#include "Artpe.hpp"

namespace producoes {

    Artpe::Artpe(const string& idioma, const string& cidade, const string& natureza,
      const int& paginas, const int& tipo, const string& edi, const int& vol,
      const string& issn_, const int& fas, const int& ser): Producao("", idioma, cidade,
      natureza, paginas, tipo), issn(issn_), editora(edi), volume(vol), fasciculo(fas), serie(ser) {}

    void Artpe::print(ostream& out) const {
        out << natureza << ";" << idioma << ";" << editora
            << ";" << cidade << ";";
        //Verifica se os dados abaixo podem ser impressos
        if(volume > 0) out << volume;
        out << ";";
        if(fasciculo > 0) out <<  fasciculo;
        out << ";";
        if(serie > 0) out << serie;
        out << ";"<< issn << ";";
        if(paginas > 0) out << paginas;
    }

    bool Artpe::operator<(const Producao& ref) {
        //Converte para Artpe para poder acessar os dados
        Artpe& o = (Artpe&) ref;
        //Compara os dados individualmente
        int cmp = natureza.compare(o.natureza);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = idioma.compare(o.idioma);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = editora.compare(o.editora);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = cidade.compare(o.cidade);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = volume - o.volume;
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = fasciculo - o.fasciculo;
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = serie - o.serie;
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = issn.compare(o.issn);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = paginas - o.paginas;
        if(cmp<0) return true; else return false;
    }

}