#include "Anais.hpp"

namespace producoes {

    Anais::Anais(const string& titulo, const string& idioma, const string& cidade,
            const string& natureza, const int& paginas, const int& tipo, const string& evento_)
            : Producao(titulo, idioma, cidade, natureza, paginas, tipo), evento(evento_) {}


    void Anais::print(ostream& out) const {
        out << natureza << ";" << titulo << ";" << idioma
            << ";" << evento << ";" << cidade << ";";
        //Verifica se paginas sera impresso
        if(paginas != 0) out << paginas;
    }

    string Anais::getEvento() {
         return this->evento;
    }

    bool Anais::operator<(const Producao& ref) {
        //Converte para Anais para poder acessar os dados
        Anais& o = (Anais&) ref;
        //Compara os dados individualmente
        int cmp = natureza.compare(o.natureza);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = titulo.compare(o.titulo);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = idioma.compare(o.idioma);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = evento.compare(o.evento);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = cidade.compare(o.cidade);
        if(cmp<0) return true; else if(cmp>0) return false;
        cmp = paginas - o.paginas;
        if(cmp<0) return true; else return false;
    }

}