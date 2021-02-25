package Capes;

// Dependencias da biblioteca Java
import java.io.IOException;
import java.security.InvalidParameterException;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

// Dependencias das Producoes e leitor de CSV
import CSV.CSVReader;
import Producoes.*;

/**
 * <h1>Processador de dados da Capes</h1>
 * 
 * <p>Classe responsavel por fazer o manuseio dos dados retirados
 * dos arquivos csv da CAPES, ou seja, utiliza as classes estruturadas
 * e, de acordo com o requisitado, as popula e insere corretamente as
 * informacoes.</p>
 * 
 * @see {@link #CSVReader}, {@link #Indice}, {@link #Map}
 * @author Gabriel S. Baptista
 * @version 1.0
 */
public class Capes {
	private CSVReader leitor;
	private Indice indice;
	private Map<String, Instituicao> instituicoes;
	private Map<String, PPG> ppgs;
	
	/**
	 * <h2>Construtor da classe Capes</h2>
	 * 
	 * @param endereco Endereco do arquivo CSV da CAPES.
	 * @throws IOException.
	 */
	public Capes(String endereco) throws IOException {
		this.leitor = new CSVReader(endereco);
		this.indice = new Indice(this.leitor.proximaLinha());
		this.instituicoes = new HashMap<>();
		this.ppgs = new HashMap<>();
	}

	// Getters
	
	/**
	 * Retorna o numero de instituicoes registradas no programa.
	 * 
	 * @return Numero de Instituicoes.
	 */
	public int getNumeroDeInstituicoes() {
		return this.instituicoes.size();
	}

	/**
	 * Retorna o numero de PPGs registradas no programa.
	 * 
	 * @return Numero de PPGs no programa.
	 */
	public int getNumeroDePPGS() {
		return this.ppgs.size();
	}

	/**
	 * Retorna o numero de producoes registradas no programa,
	 * onde e' necessario entrada em cada ppg individualmente
	 * para fazer a contabilidade.
	 * 
	 * @return Numero de producoes no programa.
	 */
	public int getNumeroDeProducoes() {
		int numeroDeProducoes = 0;
		for (PPG ppg : this.ppgs.values()) {
			numeroDeProducoes += ppg.getNumeroDeProducoes();
		}
		return numeroDeProducoes;
	}

	/**
	 * Retorna o nnumero de paginas publicadas no total,
	 * ou seja, faz uma contabilidade de todas paginas
	 * para cada producao interando-as.
	 * 
	 * @return Numero de paginas publicadas por todas as
	 * ppgs registradas.
	 */
	public int getPaginasPublicadas() {
		int paginasPublicadas = 0;
		for (PPG ppg : this.ppgs.values()) {
			paginasPublicadas += ppg.getPaginasPublicadas();
		}
		return paginasPublicadas;
	}

	/**
	 * Retorna o numero de producoes validas, ou seja, 
	 * possuem paginas maiores que zero(valor dado para
	 * paginas caso ocorra erro na conversao).
	 * 
	 * @return Numero de producoes que sao consideradas validas.
	 */
	public double getNumeroDeProducoesValidas() {
		int producoesValidas = 0;
		for (PPG ppg : this.ppgs.values()) {
			producoesValidas += ppg.getNumeroDeProducoesValidas();
		}
		return producoesValidas;
	}
	
	/**
	 * Le o arquivo de entrada, processando os dados
	 * e populando as tabelas de dispersao com as 
	 * informacoes contidas no arquivo.
	 * 
	 * @throws IOException 
	 */
	public void lerArquivo() throws IOException {
		String[] linha = this.leitor.proximaLinha();
		while (linha != null) {
			this.processarLinha(linha);
			linha = this.leitor.proximaLinha();
		}
	}

	/**
	 * Retira os dados da linha e efetua as operacoes
	 * necessarias para iniciar as estruturas de instituicao,
	 * programa de pos graduacao e, por fim, producao.
	 * 
	 * @param linha Linha qualquer do arquivo CSV.
	 */
	private void processarLinha(String[] linha) {
		Instituicao instituicao = this.addInstituicao();
		PPG ppg = this.addPPG();
		instituicao.addPPG(ppg);
		ppg.addInstituicao(instituicao);
		this.addProducao(ppg);
	}

	/**
	 * Se instituicao nao existir na tabela de disperssao
	 * de instituicoes a insere e posteriormente a retorna,
	 * caso contrario, busca a instituicao na tabela e a retorna.
	 * 
	 * @return Referencia para a instituicao na tabela de instituicao.
	 */
	private Instituicao addInstituicao() {
		String nome = this.leitor.getColuna(this.indice.nomeIES());
		String sigla = this.leitor.getColuna(this.indice.siglaIES());
		String chave = nome + sigla;
		if (!this.instituicoes.containsKey(chave)) {
			Instituicao instituicao = new Instituicao(nome, sigla);
			this.instituicoes.put(chave, instituicao);
			return instituicao;
		} else {
			return this.instituicoes.get(chave);
		}
	}

	/**
	 * Se o programa de pos graduacao nao existir na tabela de
	 * disperssao de PPGs o insere e posteriormente o retorna,
	 * caso contrario, busca o PPG na tabela e o retorna.
	 * 
	 * @return Referencia para o PPG na tabela de programas.
	 */
	private PPG addPPG() {
		String codigo = this.leitor.getColuna(this.indice.codigoPPG());
		String nome = this.leitor.getColuna(this.indice.nomePPG());
		String chave = codigo;
		if (!this.ppgs.containsKey(chave)) {
			PPG ppg = new PPG(codigo, nome);
			this.ppgs.put(chave, ppg);
			return ppg;
		} else {
			return this.ppgs.get(chave);
		}
	}
	
	/**
	 * Adiciona a producao no programa de pos graduacao informado.
	 * 
	 * @param ppg Referencia para um programa de pos graduacao<b>(PPG)</b>.
	 * @return Referencia para a producao adicionada.
	 */
	private Producao addProducao(PPG ppg) {
		Producao producao = this.iniciarProducao();	
		return ppg.addProducao(producao);
	}
	
	/**
	 * Instancia o tipo de producao dependendo do sub tipo obtido
	 * na linha do arquivo CSV.
	 * 
	 * @return Producao instanciada correspondente ao seu tipo.
	 */
	private Producao iniciarProducao() {
		String subTipoTexto = this.leitor.getColuna(this.indice.subTipoDeProducao());
		int subTipo = this.converterInteiro(subTipoTexto);
		
		Producao producao = null;
		
		switch (subTipo) {
		case 8: 
			producao = this.iniciarAnais();
			break;
		case 9: 
			producao = this.iniciarArtjr();
			break;
		case 10: 
			producao = this.iniciarOutro();
			break;
		case 21: 
			producao = this.iniciarTradu();
			break;
		case 25: 
			producao = this.iniciarArtpe();
			break;
		case 26: 
			producao = this.iniciarLivro();
			break;
		case 28: 
			producao = this.iniciarPartmu();
			break;
		default: 
			throw new InvalidParameterException("Tipo invalido.");
		}
		
		return producao;
	}

	/**
	 * Instancia uma producao do tipo anais.
	 * 
	 * @return Producao do tipo anais com os dados retirados da linha atual.
	 */
	private Producao iniciarAnais() {
		String cidade = this.leitor.getColuna(this.indice.cidade());
		String evento = this.leitor.getColuna(this.indice.evento());
		String idioma = this.leitor.getColuna(this.indice.idioma());
		String natureza = this.leitor.getColuna(this.indice.natureza());
		String paginaFinal = this.leitor.getColuna(this.indice.paginaFinal());
		String paginaInicial = this.leitor.getColuna(this.indice.paginaInicial());
		String titulo = this.leitor.getColuna(this.indice.tituloProducao());
		int paginas = this.calcularPaginas(paginaInicial, paginaFinal);
		Producao producao = new Anais(titulo, idioma, cidade, paginas, natureza, evento);
		return producao;
	}

	/**
	 * Instancia uma producao do tipo Artigos em jornais ou revistas.
	 * 
	 * @return Producao do tipo Artigos em jornais ou revistas com os
	 * dados retirados da linha atual.
	 */
	private Producao iniciarArtjr() {
		String cidade = this.leitor.getColuna(this.indice.cidade());
		Date data = this.iniciarData(this.leitor.getColuna(this.indice.dataDePublicacao()));
		String idioma = this.leitor.getColuna(this.indice.idioma());
		String issn = this.leitor.getColuna(this.indice.issn());
		String paginaFinal = this.leitor.getColuna(this.indice.paginaFinal());
		String paginaInicial = this.leitor.getColuna(this.indice.paginaInicial());
		String titulo = this.leitor.getColuna(this.indice.tituloProducao());
		int paginas = this.calcularPaginas(paginaInicial, paginaFinal);
		Producao producao = new Artjr(titulo, idioma, cidade, paginas, data, issn);
		return producao;
	}
	
	/**
	 * Instancia um objeto do tipo Date, se for passado
	 * uma String com valor valido.
	 * 
	 * @see Date
	 * @param textoData String representando uma data no parametro brasileiro
	 * @return Date data iniciada se a String passada for valida, caso contrario,
	 * retorna null.
	 */
	private Date iniciarData(String textoData) {
		DateFormat format = new SimpleDateFormat("dd/MM/yyyy");
		java.util.Date data;
		try {
			data = format.parse(textoData);
		} catch (Exception e) {
			data = null;
		}
		return data;
	}
	/**
	 * Instancia uma producao do tipo Artigos em periodicos.
	 * 
	 * @return Producao do tipo Artigos em periodicos com os
	 * dados retirados da linha atual.
	 */
	private Producao iniciarArtpe() {
		String cidade = this.leitor.getColuna(this.indice.cidade());
		String editora = this.leitor.getColuna(this.indice.editora());
		int fasciculo = this.converterInteiro(this.leitor.getColuna(this.indice.fasciculo()));
		String idioma = this.leitor.getColuna(this.indice.idioma());
		String issn = this.leitor.getColuna(this.indice.issn());
		String natureza = this.leitor.getColuna(this.indice.natureza());
		String paginaFinal = this.leitor.getColuna(this.indice.paginaFinal());
		String paginaInicial = this.leitor.getColuna(this.indice.paginaInicial());
		int serie = this.converterInteiro(this.leitor.getColuna(this.indice.serie()));
		int volume = this.converterInteiro(this.leitor.getColuna(this.indice.volume()));
		int paginas = this.calcularPaginas(paginaInicial, paginaFinal);
		Producao producao = new Artpe(idioma, cidade, paginas, natureza, editora, volume, fasciculo, serie, issn);
		return producao;
	}
	
	/**
	 * Instancia uma producao do tipo Livro.
	 * 
	 * @return Producao do tipo Livro com os dados retirados da linha atual.
	 */
	private Producao iniciarLivro() {
		String cidade = this.leitor.getColuna(this.indice.cidadePais());
		String editora = this.leitor.getColuna(this.indice.editora());
		String idioma = this.leitor.getColuna(this.indice.idioma());
		String isbn = this.leitor.getColuna(this.indice.isbn());
		String natureza = this.leitor.getColuna(this.indice.natureza());
		String titulo = this.leitor.getColuna(this.indice.tituloProducao());
		int paginas = this.calcularPaginas(this.leitor.getColuna(this.indice.paginasContribuicao()));
		Producao producao = new Livro(titulo, idioma, cidade, paginas, natureza, editora, isbn);
		return producao;
	}
	
	/**
	 * Instancia uma producao do tipo Partitura musical.
	 * 
	 * @return Producao do tipo partitura musical com os dados retirados da linha atual.
	 */
	private Producao iniciarPartmu() {
		String cidade = this.leitor.getColuna(this.indice.cidade());
		String editora = this.leitor.getColuna(this.indice.editora());
		String formacaoInstrumental = this.leitor.getColuna(this.indice.formacaoInstrumental());
		String natureza = this.leitor.getColuna(this.indice.natureza());
		int paginas = this.calcularPaginas(this.leitor.getColuna(this.indice.paginas()));
		Producao producao = new Partmu(cidade, paginas, natureza, formacaoInstrumental, editora);
		return producao;
	}
	
	/**
	 * Instancia uma producao do tipo Traducao.
	 * 
	 * @return Producao do tipo Traducao com os dados retirados da linha atual.
	 */
	private Producao iniciarTradu() {
		String cidade = this.leitor.getColuna(this.indice.cidade());
		String editora = this.leitor.getColuna(this.indice.editoraTradu());
		String idioma = this.leitor.getColuna(this.indice.idioma());
		String idiomaTraducao = this.leitor.getColuna(this.indice.idiomaTraducao());
		String natureza = this.leitor.getColuna(this.indice.natureza());
		String titulo = this.leitor.getColuna(this.indice.tituloProducao());
		int paginas = this.calcularPaginas(this.leitor.getColuna(this.indice.paginas()));
		Producao producao = new Tradu(titulo, idioma, cidade, paginas, natureza, editora, idiomaTraducao);
		return producao;
	}
	
	/**
	 * Instancia uma producao do tipo Outros.
	 * 
	 * @return Producao do tipo Outros com os dados retirados da linha atual.
	 */
	private Producao iniciarOutro() {
		String cidade = this.leitor.getColuna(this.indice.cidade());
		String editora = this.leitor.getColuna(this.indice.editora());
		String idioma = this.leitor.getColuna(this.indice.idioma());
		String natureza = this.leitor.getColuna(this.indice.natureza());
		int paginas = this.calcularPaginas(this.leitor.getColuna(this.indice.paginas()));
		Producao producao = new Outro(idioma, cidade, paginas, natureza, editora);
		return producao;
	}
	
	/**
	 * Calcula a quantidade de paginas, retornando zero caso
	 * o valor passado nao esteja presente dentro das regras
	 * estabelecidas previamente.
	 * 
	 * @param numero String representando o numero de paginas.
	 * @return O numero de paginas caso esteja correto, e zero
	 * caso a String informada nao seja compativel com as regras.
	 */
	private int calcularPaginas(String numero) {
		int paginas = this.converterInteiro(numero);
		if (paginas > 0) {
			return paginas;
		} else {
			return 0;
		}
	}
	
	/**
	 * Calcula a quantidade de paginas, retornando zero caso
	 * os valor passado nao estejam presentes dentro das regras
	 * estabelecidas previamente.
	 * 
	 * @param paginaInicial String representando a primeira pagina da producao.
	 * @param paginaFinal String representando a ultima pagina da producao.
	 * @return O numero de paginas caso esteja correto, e zero
	 * caso as Strings informadas nao sejam compativeis com as regras.
	 */
	private int calcularPaginas(String paginaInicial, String paginaFinal) {
		int primeiraPagina = this.converterInteiro(paginaInicial);
		int ultimaPagina = this.converterInteiro(paginaFinal);
		int conta = ultimaPagina - primeiraPagina + 1;
		if (primeiraPagina >= 0 && ultimaPagina >= 0 && primeiraPagina <= ultimaPagina && conta <= 2000) {
			return conta;
		} else {
			return 0;
		}
	}

	/**
	 * Converte a String informada para o valor numero
	 * que a representa, caso seja uma String invalida para
	 * ser convertida em numero, o valor retornado e -1.
	 * 
	 * @param numero String representando um numero inteiro;
	 * @return O numero <b>inteiro</b> que a string representa 
	 * caso seja valido, caso contrario, retorna -1;
	 */
	private int converterInteiro(String numero) {
		try {
			return Integer.parseInt(numero);
		} catch (Exception e) {
			return -1;
		}
	}

	/**
	 * Muda o arquivo de entrada do programa para um novo
	 * fazendo a mesma verificiacao da existencia do arquivo
	 * que a na inicializacao do tipo Capes.
	 * 
	 * @param enderecoNovoArquivo String representando o caminho
	 * do novo arquivo.
	 * @throws IOException
	 */
	public void mudarArquivoDeEntrada(String enderecoNovoArquivo) throws IOException {
		this.leitor =  new CSVReader(enderecoNovoArquivo);
		this.indice = new Indice(this.leitor.proximaLinha());
	}
	
	/**
	 * Identifica o comando passado e chama o metodo 
	 * correspondente para o comando, caso nao exista
	 * o comando passado(Comando.DESCONHECIDO), lanca
	 * uma excessao na pilha de execucao.
	 * 
	 * @see Comando
	 * @param comando Comando que deseja iniciar
	 * @throws InvalidParameterException
	 * @throws ClassNotFoundException
	 */
	public void executarComando(Comando comando) throws InvalidParameterException, ClassNotFoundException {
		switch (comando) {
		case REDE:
			this.executarComandoRede();
			break;
		case CSV:
			this.executarComandoCSV(comando);
			break;
		case IES:
			this.executarComandoIES(comando);
			break;
		case PPG:
			this.executarComandoPPG(comando);
			break;
		case DESCONHECIDO:
			throw new InvalidParameterException("Comando desconhecido.");
		}
	}
	
	/**
	 * Executa o comando CSV, imprime o cabecalho do tipo 
	 * de producao informada no comando, caso nao exita lanca 
	 * uma excessao na pilha de execussao. Posteriormente, pega
	 * todas as producoes do tipo e as imprime ordenadas.
	 * 
	 * @param comando
	 * @throws InvalidParameterException
	 * @throws ClassNotFoundException
	 */
	private void executarComandoCSV(Comando comando) throws InvalidParameterException, ClassNotFoundException {
		PPG ppg = this.ppgs.get(comando.getInfo(0));
		if (ppg != null) {
			this.imprimirCabecalho(comando.getInfo(1));
			ppg.imprimirProducoes(comando.getInfo(1));
		} else {
			throw new InvalidParameterException("PPG nao encontrado.");
		}
	}

	/**
	 * Chama a funcao de imprimir cabecalhode acordo com
	 * o tipo informado, o ultimo caso foi adicionado como
	 * forma de seguranca, visto que este erro nao existe mais
	 * nesta parte da execussao do programa.
	 * 
	 * @param tipoDeProducao String com o nome da producao
	 */
	private void imprimirCabecalho(String tipoDeProducao) {
		if (tipoDeProducao.equals("anais")) {
			Anais.imprimirCabecalho();
		} else if (tipoDeProducao.equals("artjr")) {
			Artjr.imprimirCabecalho();
		} else if (tipoDeProducao.equals("artpe")) {
			Artpe.imprimirCabecalho();
		} else if (tipoDeProducao.equals("livro")) {
			Livro.imprimirCabecalho();
		} else if (tipoDeProducao.equals("partmu")) {
			Partmu.imprimirCabecalho();
		} else if (tipoDeProducao.equals("tradu")) {
			Tradu.imprimirCabecalho();
		} else if (tipoDeProducao.equals("outro")){
			Outro.imprimirCabecalho();
		} else {
			throw new InvalidParameterException("Tipo invalido.");
		}
	}

	/**
	 * Executa o comando IES, busca a instituicoes que
	 * sao representadas com a sigla passada, caso nao exista
	 * nenhuma lanca um erro na pilha de execussao, caso contrario,
	 * imprime cada instituicao e um relatorio da mesma.
	 * 
	 * @param comando Comando IES, necessario para pegar os dados
	 * como a sigla da instituicao.
	 */
	private void executarComandoIES(Comando comando) {
		List<Instituicao> instituicoes = this.buscarPorSigla(comando.getInfo());
		if (instituicoes.size() == 0) {
			System.out.println("IES nao encontrada.");
		} else {
			for (Instituicao instituicao : instituicoes) {
				System.out.printf("%s (%s):\n", instituicao.getNome(), instituicao.getSigla());
				instituicao.imprimirRelatorio();
			}
		}
	}

	/**
	 * Busca uma instituicao no mapa de instituicoes
	 * a partir da sigla informada.
	 * 
	 * @param sigla String representando a sigla da instituicao
	 * @return Lista das instituicoes representadas pela sigla informada
	 */
	private List<Instituicao> buscarPorSigla(String sigla) {
		List<Instituicao> lista = new ArrayList<>();
		for (Instituicao instituicao : this.instituicoes.values()) {
			if (instituicao.getSigla().equals(sigla)) {
				lista.add(instituicao);
			}
		}
		Collections.sort(lista);
		return lista;
	}

	/**
	 * Executa o comando PPG, busca um programa de pos graduacao
	 * a partir do codigo informado, caso nao exista lanca um 
	 * erro na pilha de execussao, caso contrario, imprime o programa
	 * assim como as instituicoes a qual este pertence e algumas informacoes
	 * acerca da ppg.
	 * 
	 * @param comando Comando de execussao de PPG, necessario para pegar os dados como
	 * o codigo da ppg.
	 */
	private void executarComandoPPG(Comando comando) {
		if (this.ppgs.containsKey(comando.getInfo())) {
			PPG ppg = this.ppgs.get(comando.getInfo());
			this.imprimirPPG(ppg);
		} else {
			System.out.println("PPG nao encontrado.");
		}
	}

	/**
	 * Imprime uma ppg como requisitado no comando PPG, formatando 
	 * e posteriormente exibindo as informacoes requisitadas.
	 * 
	 * @param ppg Referencia para uma ppg existente.
	 */
	private void imprimirPPG(PPG ppg) {
		System.out.println("Programa: " + ppg.getNome());
		System.out.println("Instituicoes:");
		ppg.ordenarInstituicoes();
		ppg.imprimirInstituicoes();
		System.out.println();
		ppg.imprimirRelatorio();
		System.out.println();
		System.out.println("Total de paginas produzidas pelo PPG: " + ppg.getPaginasPublicadas());
	}

	/**
	 * Executa o comando Rede, busca no mapa de PPG programas
	 * que estejam presentes em mais de uma instituicao, e,
	 * posteriormente, as imprime.
	 */
	private void executarComandoRede() {
		System.out.println("Programas em rede:");
		List<PPG> ppgsEmRede = this.criarListaPPGsEmRede();
		for (PPG ppg: ppgsEmRede) {
			System.out.println(ppg);
			ppg.imprimirInstituicoes();
		}	
	}

	/**
	 * Instancia uma lista e verifica se cada instituicao no mapa 
	 * esta em rede, se estiver adiciona na lista e no final retorna
	 * a lista.
	 * 
	 * @return Lista contendo todas ppgs que estam em rede.
	 */
	private List<PPG> criarListaPPGsEmRede() {
		List<PPG> ppgsEmRede = new ArrayList<>();
		for (PPG ppg : this.ppgs.values()) {
			if (ppg.emRede()) {
				ppgsEmRede.add(ppg);
				ppg.ordenarInstituicoes();
			}
		}
		Collections.sort(ppgsEmRede);
		return ppgsEmRede;
	}

}
