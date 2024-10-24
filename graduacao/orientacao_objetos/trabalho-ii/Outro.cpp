#include "Outro.hpp"

namespace producoes {

     Outro::Outro(const string& idioma, const string& cidade,
      const string& natureza, const int& paginas, const int& tipo, const string& edi)
    : Producao("", idioma, cidade, natureza, paginas, tipo), editora(edi) {}

    void Outro::print(ostream& out) const {
        out << natureza << ";" << idioma << ";" << editora
            << ";" << cidade << ";";
        //Verifica se paginas sera impresso
        if(paginas != 0) out << paginas;
    }

    bool Outro::operator<(const Producao& ref) {
        //Converte para Outro para poder acessar os dados
        Outro& o = (Outro&) ref;
        //Compara os dados individualmente
        int cmp = natureza.compare(o.natureza);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = idioma.compare(o.idioma);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = editora.compare(o.editora);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = cidade.compare(o.cidade);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = paginas - o.paginas;
        if(cmp<0) return true; else return false;
    }

}