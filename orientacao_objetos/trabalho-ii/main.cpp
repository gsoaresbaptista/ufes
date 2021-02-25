#include <iostream>
#include "Capes.hpp"

using namespace std;
using namespace capes;

int main(){
    string endereco, arquivo, comando;
    cin >> endereco >> arquivo;

    try {
        // Inicializa com o primeiro arquivo
        Capes capes(endereco + arquivo);
        for(int i = 0; i < 6; i++) {
            capes.processar();
            cin >> arquivo;
            capes.atualizarArquivo(endereco + arquivo);
        }
        capes.processar();
        // Executa o comando
        cin >> comando;
        capes.executarComando(comando);
    } catch(const exception& e) {
        cout << e.what() << endl;
    }
    
    return 0;
}