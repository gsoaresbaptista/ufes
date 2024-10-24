#include "Capes.hpp"

namespace capes {

    Capes::Capes(const string& endereco) : arquivo(endereco), instituicoes(),
    programas(), producoes() {
         indice = new Indice(arquivo.getLinha());
    }

    void Capes::processar() {
        // Le a primeira linha que sera processada
        arquivo.proximaLinha();
        // Processa o restante do arquivo
        while(!arquivo.fim()){
            // Instancia os objetos
            Instituicao* instituicao = adicionarInstituicao();
            Programa* programa = adicionarPrograma();
            Producao* producao = adicionarProducao();
            // Os interligam
            instituicao->addPrograma(programa);
            programa->addInstituicao(instituicao);
            programa->addProducao(producao);
            // Atualiza a linha
            arquivo.proximaLinha();
        }
    }

    Instituicao* Capes::adicionarInstituicao() {
        string chave = arquivo.get(indice->nomeIES()) + arquivo.get(indice->siglaIES());
        // Busca o elemento no mapa
        if(instituicoes.find(chave) == instituicoes.end()) {
            // Cria uma nova Instituicao
            Instituicao* ies = new Instituicao(arquivo.get(indice->nomeIES()),
                arquivo.get(indice->siglaIES()));
            // Insere no mapa
            instituicoes.insert({chave, ies});
        }
        return instituicoes.find(chave)->second;
    }

    Programa* Capes::adicionarPrograma() {
        string chave = arquivo.get(indice->codigoPPG());
        // Busca o elemento no mapa
        if(programas.find(chave) == programas.end()) {
            // Cria um novo programa
            Programa* ppg = new Programa(arquivo.get(indice->codigoPPG()),
                arquivo.get(indice->nomePPG()));
            // Insere no mapa
            programas.insert({chave, ppg});
        }
        return programas.find(chave)->second;
    }

    Producao* Capes::adicionarProducao() {
        // Conveter o tipo para inteiro
        int tipo = stoi(arquivo.get(indice->subTipoDeProducao()));
        Producao* producao;
        // Intancia o tipo de acordo com o id informado
        switch (tipo) {
            case 8:
                producao = novoAnais();
                break;
            case 9:
                producao = novoArtjr();
                break;
            case 10:
                producao = novoOutro();
                break;
            case 21:
                producao = novaTraducao();
                break;
            case 25:
                producao = novoArtpe();
                break;
            case 26:
                producao = novoLivro();
                break;
            case 28:
                producao = novaPartmu();
                break;
            default:
                /* Caso nenhuma tenha sido encontrada
                lanca um erro na pilha */
                throw invalid_argument("Tipo invalido.");
        }
        // Insere a producao no vetor
        producoes.push_back(producao);
        return producao;
    }

    static int converterInteiro(const string& numero) {
        if(!isNumber(numero)) return 0;
        else return stod(numero);
    }

    Anais* Capes::novoAnais() {
        // Calcula a quantidade de paginas
        int paginas = calcularPaginas(
            arquivo.get(indice->paginaInicial()), arquivo.get(indice->paginaFinal()));
        // Retorna um novo Anais com os dados da linha atual
        return new Anais(arquivo.get(indice->tituloProducao()), arquivo.get(indice->idioma()),
            arquivo.get(indice->cidade()), arquivo.get(indice->natureza()), paginas, 8,
            arquivo.get(indice->evento()));
    }

    Artjr* Capes::novoArtjr() {
        // Calcula a quantidade de paginas
        int paginas = calcularPaginas(
            arquivo.get(indice->paginaInicial()), arquivo.get(indice->paginaFinal()));
        // Pega a string da dada e a converte
        string dataTexto = arquivo.get(indice->dataDePublicacao());
        if(!validDate(dataTexto, DATE_FORMAT_PT_BR_SHORT)) dataTexto = "0/00/0000";
        time_t data = parseDate(dataTexto, DATE_FORMAT_PT_BR_SHORT);
        // Retorna um novo Artjr com os dados da linha atual
        return new Artjr(arquivo.get(indice->tituloProducao()), arquivo.get(indice->idioma()),
            arquivo.get(indice->cidade()), paginas, 9, data, arquivo.get(indice->issn()));
    }

    Outro* Capes::novoOutro() {
        // Calcula a quantidade de paginas
        int paginas = converterInteiro(arquivo.get(indice->paginas()));
        // Retorna um novo Outro com os dados da linha atual
        return new Outro(arquivo.get(indice->idioma()),
            arquivo.get(indice->cidade()), arquivo.get(indice->natureza()),
            paginas, 10, arquivo.get(indice->editora()));
    }

    Traducao* Capes::novaTraducao() {
        // Calcula a quantidade de paginas
        int paginas = converterInteiro(arquivo.get(indice->paginas()));
        // Retorna uma nova traducao com os dados da linha atual
        return new Traducao(arquivo.get(indice->tituloProducao()), arquivo.get(indice->idioma()),
            arquivo.get(indice->cidade()), arquivo.get(indice->natureza()), paginas, 21,
            arquivo.get(indice->editoraTradu()), arquivo.get(indice->idiomaTraducao()));
    }

    Artpe* Capes::novoArtpe() {
        // Calcula a quantidade de paginas
        int paginas = calcularPaginas(arquivo.get(indice->paginaInicial()),
                arquivo.get(indice->paginaFinal()));
        // Converte os dados para inteiro
        int fasciculo = converterInteiro(arquivo.get(indice->fasciculo()));
        int serie = converterInteiro(arquivo.get(indice->serie()));
        int volume = converterInteiro(arquivo.get(indice->volume()));
        // Retorna um novo Artpe com os dados da linha atual
        return new Artpe(arquivo.get(indice->idioma()),
            arquivo.get(indice->cidade()), arquivo.get(indice->natureza()),
            paginas, 25, arquivo.get(indice->editora()), volume,
            arquivo.get(indice->issn()), fasciculo, serie);
    }

    Livro* Capes::novoLivro() {
        // Calcula a quantidade de paginas
        int paginas = converterInteiro(arquivo.get(indice->paginasContribuicao()));
        // Retorna um novo Artjr com os dados da linha atual
        return new Livro(arquivo.get(indice->tituloProducao()),
                arquivo.get(indice->idioma()), arquivo.get(indice->cidadePais()),
                arquivo.get(indice->natureza()), paginas, 26, arquivo.get(indice->editora()),
                arquivo.get(indice->isbn()));
    }

    Partmu* Capes::novaPartmu() {
        // Calcula a quantidade de paginas
        int paginas = converterInteiro(arquivo.get(indice->paginas()));
        // Retorna um novo Artjr com os dados da linha atual
        return new Partmu(arquivo.get(indice->cidade()), arquivo.get(indice->natureza()),
                paginas, 28, arquivo.get(indice->editora()),
                arquivo.get(indice->formacaoInstrumental()));
    }

    int Capes::calcularPaginas(const string& inicial, const string& final) {
        // Se um dos dois nao for numero retorna zero
        if(!isNumber(inicial) || !isNumber(final)) return 0;
        // Converte individualmente cada um e efetua a conta
        int pInicial = stod(inicial);
        int pFinal = stod(final);
        int conta = pFinal - pInicial;
        // Faz a verificacao requisitada
        if(conta <= 2000 && pInicial <= pFinal && pFinal >= 0 && pInicial >= 0)
            return conta + 1;
        else
            return 0;
    }

    string Capes::relatorio() const {
        // Cria uma string stream que sera transformada em string
        stringstream relatorio;
        // Variaveis acumuladoras
        int paginas = 0, producoesValidas = 0;
        // Itera sobre as producoes
        for(auto& producao : this->producoes) {
            int paginasAtuais = producao->getPaginas();
            paginas += paginasAtuais;
            if(paginasAtuais > 0) producoesValidas++;
        }
        // Impre na stringstream as informacoes
        double media = (double)paginas/producoesValidas;
        relatorio << "Instituicoes que publicaram em anais: " << this->instituicoes.size() << endl;
        relatorio << "PPGs que publicaram em anais: " << this->programas.size() << endl;
        relatorio << "Quantidade de producoes em anais: " << producoes.size() << endl;
        relatorio << "Quantidade de paginas publicadas em anais: " << paginas << endl;
        relatorio << "Media de paginas por publicacao: " << formatDouble(media, 1, LOCALE_PT_BR);
        // Retorna a string criada
        return relatorio.str();
    }

    void Capes::atualizarArquivo(const string& endereco) {
        // Altera o arquivo de entrada
        arquivo.alterarFluxo(endereco);
        // Atualiza o cabecalho
        delete indice;
        indice = new Indice(arquivo.getLinha());
    }

    int Capes::producaoTipo(const string& tipo) {
        if (tipo == "anais") return 8;
        else if(tipo == "artjr") return 9;
        else if(tipo == "outro") return 10;
        else if(tipo == "tradu") return 21;
        else if(tipo == "artpe") return 25;
        else if(tipo == "livro") return 26;
        else if(tipo == "partmu") return 28;
        else throw invalid_argument("Tipo invalido.");
    }

    void Capes::imprimirCabecalho(const int& tipo, ostream& out) {
        if(tipo == 8)
            out << "Natureza;Titulo;Idioma;Evento;Cidade;Paginas" << endl;
        else if(tipo == 9)
            out << "Titulo;Idioma;Cidade;Data;ISSN;Paginas" << endl;
        else if(tipo == 10)
            out << "Natureza;Idioma;Editora;Cidade;Paginas" << endl;
        else if(tipo == 21)
            out << "Natureza;Titulo;Idioma;Editora;Cidade;Traducao;Paginas" << endl;
        else if(tipo == 25)
            out << "Natureza;Idioma;Editora;Cidade;Volume;Fasciculo;Serie;ISSN;Paginas" << endl;
        else if(tipo == 26)
            out << "Natureza;Titulo;Idioma;Editora;Cidade;ISBN;Paginas" << endl;
        else if(tipo == 28)
            out << "Natureza;Editora;Cidade;Formacao;Paginas" << endl;
    }

    void Capes::executarComando(const string& comando) {
        if(comando == "rede") {
            comandoRede();
        } else if(comando == "ppg") {
            // Le os dados para o comando ppg
            string codigo;
            cin >> codigo;
            comandoPPG(codigo);
        } else if(comando == "ies") {
            // Le os dados para o comando ies
            string ies;
            cin >> ies;
            comandoIES(ies);
        } else if(comando == "csv") {
            // Le os dados para o comando csv
            string codigo, producao;
            cin >> codigo >> producao;
            comandoCSV(codigo, producaoTipo(producao));
        } else {
            // Lanca um erro na pilha
            throw invalid_argument("Comando desconhecido.");
        }
    }

    void Capes::comandoRede() {
        vector<Programa*> emRede;
        // Percorre o vetor em busca de programas em rede
        for(auto& programa: this->programas) {
            if(programa.second->emRede())
                emRede.push_back(programa.second); 
        }
        // Ordena os programas
        sort(emRede.begin(), emRede.end(), ComparacoesCapes::ordenarPorCodigo);
        // Imprime o vetor
        cout << "Programas em rede:" << endl;
        for(auto& programa: emRede) {
            cout << *programa;
            programa->getInstituicoes(cout);
        }
    }

    Programa* Capes::getPrograma(const string& codigo) {
        // Percorre os programas comparando os codigos
        for(auto& ppg : programas)
            if(ppg.second->getCodigo() == codigo) return ppg.second;
        // Nao foi encontrado, retorna null.
        return nullptr;
    }

    void Capes::comandoPPG(const string& codigo) {
        // Busca o programa correspondente ao codigo
        Programa* programa = getPrograma(codigo);
        // Se nao encontrou lanca um erro na pilha.
        if(programa == nullptr) throw invalid_argument("PPG nao encontrado.");
        // Imprime as informacoes.
        cout << "Programa: " << programa->getTitulo() << endl;
        cout << "Instituicoes:" << endl;
        programa->getInstituicoes(cout);
        cout << endl << "Quantidade de producoes por tipo:" << endl;
        programa->getRelatorio(cout);
        cout << endl;
    }

    void Capes::comandoIES(const string& sigla) {
        // Instancia as estruturas inicias.
        vector<Instituicao*> encontradas;
        bool achou = false;
        /* Percorre as instituicoes procurando as que possuam
        sigla igual a informada */
        for(auto& ies: instituicoes) {
            if(ies.second->getSigla() == sigla){
                /* Nao pode dar break, pois pode existir mais de uma
                instituicao com a mesma sigla */
                encontradas.push_back(ies.second);
                // Altera a flag
                achou = true;
            }
        }
        // Se nao achou nenhuma lanca um erro na pilha.
        if(!achou) throw invalid_argument("IES nao encontrada.");
        // Ordena e imprime as instituicoes
        sort(encontradas.begin(), encontradas.end(), ComparacoesCapes::ordenarIES);
        for(auto& ies : encontradas) {
            cout << ies->getNome() << " (" << ies->getSigla()
                 << "):" << endl;
            // Imprime na saida padrao o relatorio
            ies->getProducoes(cout);
        }
    }

    void Capes::comandoCSV(const string& codigo, const int& producao) {
        // Busca o programa com o codigo correspondente
        Programa* programa = getPrograma(codigo);
        // Se nao encontrou lanca uma excecao na pilha.
        if(programa == nullptr) throw invalid_argument("PPG nao encontrado.");
        // Pega as producoes com o id informado e as ordena.
        vector<Producao*> encontradas = programa->getProducoes(producao);
        sort(encontradas.begin(),encontradas.end(), ComparacoesProducoes::comparar);
        // Imprime na saida padrao os dados
        imprimirCabecalho(producao, cout);
        for(auto& prod : encontradas) {
            cout << *prod << endl;
        }
    }

    Capes::~Capes() {
        // Libera o indice (ponteiro)
        delete indice;
        // Percorre as instituicoes as liberando
        for(auto& instituicao : instituicoes)
            delete instituicao.second;
        // Percorre os programas os liberando
        for(auto& programa : programas)
            delete programa.second;
        // Percorre as producoes as liberando
        for(auto& producao : producoes)
            delete producao;
    }

}