package Producoes;

/**
 * Producao que representa producoes do tipo Artigo em
 * periodicos.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class Artpe extends Producao {
	private String editora;
	private int volume;
	private int fasciculo;
	private int serie;
	private String issn;

	/**
	 * Construtor da Classe
	 * 
	 * @param idioma String representando o idioma.
	 * @param cidade String representando a cidade.
	 * @param paginas Inteiro representando a quantidade de paginas.
	 * @param natureza String representando a natureza do documento.
	 * @param editora String representando a editora em que foi publicado.
	 * @param volume inteiro representando o volume da producao.
	 * @param fasciculo inteiro representando o fasciculo do documento.
	 * @param serie inteiro representando a serie do documento.
	 * @param issn String representando o issn do documento.
	 */
	public Artpe(String idioma, String cidade, int paginas, String natureza, String editora,
			int volume, int fasciculo, int serie, String issn) {
		super("", idioma, cidade, natureza, paginas);
		this.editora = editora;
		if (volume >= 0) this.volume = volume; else this.volume = 0;
		if (fasciculo >= 0) this.fasciculo = fasciculo; else this.fasciculo = 0;
		if (serie >= 0) this.serie = serie; else this.serie = 0;
		this.issn = issn;
	}
	
	/**
	 * Imprime o cabecalho da Classe artpe
	 */
	public static void imprimirCabecalho() {
		System.out.println("Natureza;Idioma;Editora;Cidade;Volume;Fasciulo;Serie;ISSN;Paginas");
	}
	
	/**
	 * Retorna o volume do documento.
	 * 
	 * @return String representando o volume do documento.
	 */
	public String getVolume() {
		if(this.volume == 0) {
			return "";
		} else {
			return "" + this.volume;
		}
	}
	
	/**
	 * Retorna o fasciculo do documento.
	 * 
	 * @return String representando o fasciculo do documento.
	 */
	public String getFasciculo() {
		if(this.fasciculo == 0) {
			return "";
		} else {
			return "" + this.fasciculo;
		}
	}
	
	/**
	 * Retorna a serie do documento.
	 * 
	 * @return Strinng representando a serie do documento.
	 */
	public String getSerie() {
		if(this.serie == 0) {
			return "";
		} else {
			return "" + this.serie;
		}
	}

	/**
	 * Sobreescreve o metodo toString, ou seja, representa
	 * a maneira usual de se exibir uma producao do tipo artpe.
	 */
	@Override
	public String toString() {
		String texto = this.getNatureza() + ";" + this.getIdioma() + ";" + this.editora + 
				";" + this.getCidade()+ ";" + this.getVolume() + ";" + this.getFasciculo() + 
				";" + this.getSerie() + ";" + this.issn + ";";
		if(this.getPaginas() > 0) texto += this.getPaginas();
		return texto;
	}

	/**
	 * Sobreescreve o metodo equals.
	 */
	@Override
	public boolean equals(Object obj) {
		if(!(obj instanceof Artpe)) return false;
		Artpe referencia = (Artpe) obj;
		if(this.compareTo(referencia) == 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Sobreescreve o metodo compareTo, ou seja,  representa
	 * a maneira usual de se comparar producoes do tipo artpe.
	 */
	@Override
	public int compareTo(Producao o) {
		Artpe referencia = (Artpe) o;
		// Verifica as caracteristicas
		int cmp = this.getNatureza().compareTo(referencia.getNatureza());
		if (cmp != 0) return cmp;
		cmp = this.getIdioma().compareTo(referencia.getIdioma());
		if (cmp != 0) return cmp;
		cmp = this.editora.compareTo(referencia.editora);
		if (cmp != 0) return cmp;
		cmp = this.getCidade().compareTo(referencia.getCidade());
		if (cmp != 0) return cmp;
		cmp = this.volume - referencia.volume;
		if (cmp != 0) return cmp;
		cmp = this.fasciculo - referencia.fasciculo;
		if (cmp != 0) return cmp;
		cmp = this.serie - referencia.serie;
		if (cmp != 0) return cmp;
		cmp = this.getPaginas() - referencia.getPaginas();
		if (cmp != 0) return cmp;
		return this.issn.compareTo(referencia.issn);
	}

}
