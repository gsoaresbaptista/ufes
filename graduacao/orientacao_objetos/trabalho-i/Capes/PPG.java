package Capes;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.Set;
import java.util.TreeSet;

// Producoes existentes no programa.
import Producoes.*;

/**
 * Programa de Pos Graduacao, manusea todo que tem referencia a ppgs, ou seja,
 * todas operacoes com producoes passam por dentro desta classe, pois so nela
 * existe armazenamento destas. Obs.: Deve ser comparavel para que possa ser
 * ordenada, caso necessario na exibicao de listas de ppg.
 * 
 * @see {@link #Comparable}, {@link #Instituicao}
 * @author Gabriel S. Baptista
 *
 */
public class PPG implements Comparable<PPG> {
	private String codigo;
	private String nome;
	private List<Instituicao> instituicoes;
	private List<Producao> producoes;

	/**
	 * Construtor da classe.
	 * 
	 * @param codigo String representando o codigo da PPG.
	 * @param nome   String representando o nome da PPG.
	 */
	public PPG(String codigo, String nome) {
		this.codigo = codigo;
		this.nome = nome;
		this.instituicoes = new ArrayList<>();
		this.producoes = new ArrayList<>();
	}

	/**
	 * Numero de producoes armazenadas pela ppg, no caso, 
	 * o tamanho da lista de producoes.
	 *
	 * @return Numero de producoes da ppg.
	 */
	public int getNumeroDeProducoes() {
		return this.producoes.size();
	}

	/**
	 * Retorna o nome da ppg em questao.
	 * 
	 * @return Nome da ppg.
	 */
	public String getNome() {
		return this.nome;
	}
	
	/**
	 * Retorna o codigo da ppg em questao.
	 * 
	 * @return String representando o codigo da ppg.
	 */
	public String getCodigo() {
		return this.codigo;
	}

	/**
	 * Numero de total de paginas publicadas pela ppg,
	 * ou seja, a somatoria de paginas de todas producoes
	 * armazenadas no ppg.
	 * 
	 * @return Numero de paginas publicadas.
	 */
	public int getPaginasPublicadas() {
		int paginas = 0;
		for (Producao producao : this.producoes) {
			paginas += producao.getPaginas();
		}
		return paginas;
	}

	/**
	 * Numero de producoes "validas", ou seja, producoes que
	 * possuem o numero de paginas validadas corretamente,
	 * maior que zero.
	 * 
	 * @return Numero de producoes "validas".
	 */
	public int getNumeroDeProducoesValidas() {
		int validas = 0;
		for (Producao producao : this.producoes) {
			if (producao.getPaginas() > 0)
				validas++;
		}
		return validas;
	}

	/**
	 * Adiciona uma instituicao no ppg em questao.
	 * 
	 * @param instituicao Referencia para a instituicao que se deseja adicionar.
	 */
	public void addInstituicao(Instituicao instituicao) {
		if (!this.instituicoes.contains(instituicao)) {
			this.instituicoes.add(instituicao);
		}
	}

	/**
	 * Adiciona uma producao no ppg em questao.
	 * 
	 * @param producao Referencia para a producao que se deseja adicionar.
	 */
	public Producao addProducao(Producao producao) {
//		if (!this.producoes.contains(producao)) {
			this.producoes.add(producao);
//		}
		return producao;
	}

	/**
	 * Verifica se o programa esta em rede, ou seja, se possui mais de uma
	 * instituicao de ensino relacionada a esta.
	 * 
	 * @return True, caso esteja em rede, caso contrario retorna False.
	 */
	public boolean emRede() {
		if (this.instituicoes.size() > 1) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Modo padrao de ser exibido uma ppg.
	 */
	@Override
	public String toString() {
		return this.codigo + ": " + this.nome;
	}

	/**
	 * Compara a ppg pelo seu codigo, maneira usual de se comparar dois programas
	 * diferentes.
	 */
	@Override
	public int compareTo(PPG referencia) {
		return this.codigo.compareTo(referencia.codigo);
	}

	/**
	 * Imprime uma lista de instituicoes que estao relacionadas ao programa em
	 * questao.
	 */
	public void imprimirInstituicoes() {
		for (Instituicao instituicao : this.instituicoes) {
			System.out.println("\t- " + instituicao);
		}
	}

	/**
	 * Ordena as insituicoes da ppg.
	 */
	public void ordenarInstituicoes() {
		Collections.sort(this.instituicoes);
	}

	/**
	 * Imprime um relatorio geral acerca da ppg em questao, metodo utilizado
	 * primariamente pelo comando PPG.
	 */
	public void imprimirRelatorio() {
		int[] tipos = this.apurarProducoes();
		System.out.println("Quantidade de producoes por tipo:");
		System.out.println("\t- Artigos em anais de eventos: " + tipos[0]);
		System.out.println("\t- Artigos em jornais e revistas: " + tipos[1]);
		System.out.println("\t- Artigos em periodicos cientificos: " + tipos[2]);
		System.out.println("\t- Livros: " + tipos[3]);
		System.out.println("\t- Partituras musicais: " + tipos[4]);
		System.out.println("\t- Traducoes: " + tipos[5]);
		System.out.println("\t- Outros: " + tipos[6]);
	}

	/**
	 * Apura a quantidade de producoes de cada tipo para o programa em questao.
	 * 
	 * @return Vetor de inteiros representando a quantidade de cada producao no ppg.
	 */
	private int[] apurarProducoes() {
		int[] contagem = new int[7];
		for (Producao producao : this.producoes) {
			if (producao instanceof Anais)
				contagem[0]++;
			else if (producao instanceof Artjr)
				contagem[1]++;
			else if (producao instanceof Artpe)
				contagem[2]++;
			else if (producao instanceof Livro)
				contagem[3]++;
			else if (producao instanceof Partmu)
				contagem[4]++;
			else if (producao instanceof Tradu)
				contagem[5]++;
			else
				contagem[6]++;
		}
		return contagem;
	}

	/**
	 * Imprime uma lista de producoes que tenham o mesmo tipo da producao
	 * correspondente a string passada no parametro, observar que mesmo
	 * que existe na assinatura um aviso de excessao, nesta etapa da execucao
	 * o problema ja foi tratato na captura do comando.
	 * 
	 * @param tipoDaProducao String representando o tipo da producao.
	 * @throws ClassNotFoundException Caso nao exista a producao informada.
	 */
	public void imprimirProducoes(String tipoDaProducao) throws ClassNotFoundException {
		Class<?> tipo = this.capturarTipo(tipoDaProducao);
		Set<Producao> producoes = new TreeSet<>();
		for (Producao producao : this.producoes) {
			if (tipo.isInstance(producao)) {
				producoes.add(producao);
			}
		}
		for (Object producao : producoes) {
			System.out.printf("%s\n", producao);
		}
	}

	/**
	 * Pega a String representando o o tipo de producao desejada e
	 * a formata adequadamente para ser achada e retornada pelo comando
	 * "Class.forName()".
	 * 
	 * @param tipoDaProducao String com o nome da producao desejada.
	 * @return Class da producao desejada.
	 * @throws ClassNotFoundException Caso nao exista a producao informada.
	 */
	private Class<?> capturarTipo(String tipoDaProducao) throws ClassNotFoundException {
		String tipo = "Producoes." + tipoDaProducao.substring(0, 1).toUpperCase() + tipoDaProducao.substring(1);
		return Class.forName(tipo);
	}
}
