package Producoes;

/**
 * Classe abstrata que representa uma producao em geral.
 * Implementa a interface comparable, exigindo que subclasses
 * a implementem, garantindo que as mesmas tambem seram comparaveis,
 * e portanto, ordenaveis.
 * 
 * @author Gabriel S. Baptista
 *
 */
public abstract class Producao implements Comparable<Producao> {
	private String titulo;
	private String idioma;
	private String cidade;
	private String natureza;
	private int paginas;
	
	/**
	 * Construtor da Classe
	 * 
	 * @param titulo String representando o titulo.
	 * @param idioma String representando o idioma.
	 * @param cidade String representando a cidade.
	 * @param paginas Inteiro representando a quantidade de paginas.
	 */
	public Producao(String titulo, String idioma, String cidade, String natureza, int paginas) {
		this.titulo = titulo;
		this.idioma = idioma;
		this.cidade = cidade;
		this.natureza = natureza;
		this.paginas = paginas;
	}
	
	/**
	 * Retorna o titulo do documento.
	 * 
	 * @return String representando o titulo do documento.
	 */
	public String getTitulo() {
		return this.titulo;
	}
	
	/**
	 * Retorna o idioma do documento.
	 * 
	 * @return String representando o idioma do documento.
	 */
	public String getIdioma() {
		return this.idioma;
	}
	
	/**
	 * Retorna a cidade do documento.
	 * 
	 * @return String representando a cidade do documento.
	 */
	public String getCidade() {
		return this.cidade;
	}
	
	/**
	 * Retorna a natureza do documento.
	 * 
	 * @return String representando a natureza do documento.
	 */
	public String getNatureza() {
		return this.natureza;
	}
	
	/**
	 * Retorna a quantidade de paginas do documento.
	 * 
	 * @return inteiro representando a qunatidade de paginas do documento.
	 */
	public int getPaginas() {
		return this.paginas;
	}
	
}
