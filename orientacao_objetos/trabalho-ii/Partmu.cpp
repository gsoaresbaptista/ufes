#include "Partmu.hpp"

namespace producoes {

     Partmu::Partmu(const string& cidade,
      const string& natureza, const int& paginas, const int& tipo, const string& edi,
      const string& form) : Producao("", "", cidade, natureza, paginas, tipo),
      editora(edi), formacao(form) {}

    void Partmu::print(ostream& out) const {
         out << natureza << ";" << editora << ";" << cidade
            << ";" << formacao << ";";
        //Verifica se paginas sera impresso
         if(paginas != 0) out << paginas;
    }

    bool Partmu::operator<(const Producao& ref) {
        //Converte para Partmu para poder acessar os dados
        Partmu& o = (Partmu&) ref;
        //Compara os dados individualmente
        int cmp = natureza.compare(o.natureza);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = editora.compare(o.editora);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = cidade.compare(o.cidade);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = formacao.compare(o.formacao);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = paginas - o.paginas;
        if(cmp<0) return true; else return false;
    }

}