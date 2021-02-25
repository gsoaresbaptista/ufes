#include <iostream>
#include "ComparacoesProducoes.hpp"

namespace producoes {

    bool ComparacoesProducoes::comparar(Producao* o1, Producao* o2) {
        // Retorna se o primeiro apontado e menor que o segundo
        return *o1 < *o2;
    }

}