package Producoes;

/**
 * Producao que representa producoes do tipo Traducao.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class Tradu extends Producao {
	private String editora;
	private String idiomaTraducao;

	/**
	 * Construtor da Classe
	 * 
	 * @param titulo String representando o titulo.
	 * @param idioma String representando o idioma.
	 * @param cidade String representando a cidade.
	 * @param paginas Inteiro representando a quantidade de paginas.
	 * @param natureza String representando a natureza do documento.
	 * @param editora String representando a editora em que foi publicado.
	 * @param idiomaTraducao String representando o idioma de traducao.
	 */
	public Tradu(String titulo, String idioma, String cidade, int paginas, String natureza, String editora,
			String idiomaTraducao) {
		super(titulo, idioma, cidade, natureza, paginas);
		this.editora = editora;
		this.editora = editora;
		this.idiomaTraducao = idiomaTraducao;
	}

	/**
	 * Imprime o cabecalho da Classe tradu.
	 */
	public static void imprimirCabecalho() {
		System.out.println("Natureza;Titulo;Idioma;Editora;Cidade;Traducao;Paginas");
	}
	
	/**
	 * Sobreescreve o metodo toString, ou seja, representa
	 * a maneira usual de se exibir uma producao do tipo tradu.
	 */
	@Override
	public String toString() {
		String texto = this.getNatureza() + ";" + this.getTitulo() + ";" + this.getIdioma() + ";" + this.editora + ";"
				+ this.getCidade() + ";" + this.idiomaTraducao + ";";
		if (this.getPaginas() > 0)
			texto += this.getPaginas();
		return texto;
	}

	/**
	 * Sobreescreve o metodo equals.
	 */
	@Override
	public boolean equals(Object obj) {
		if (!(obj instanceof Tradu))
			return false;
		Tradu referencia = (Tradu) obj;
		if (this.compareTo(referencia) == 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Sobreescreve o metodo compareTo, ou seja,  representa
	 * a maneira usual de se comparar producoes do tipo tradu.
	 */
	@Override
	public int compareTo(Producao o) {
		Tradu referencia = (Tradu) o;
		// Verifica as caracteristicas
		int cmp = this.getNatureza().compareTo(referencia.getNatureza());
		if (cmp != 0) return cmp;
		cmp = this.getTitulo().compareTo(referencia.getTitulo());
		if (cmp != 0) return cmp;
		cmp = this.editora.compareTo(referencia.editora);
		if (cmp != 0) return cmp;
		cmp = this.getCidade().compareTo(referencia.getCidade());
		if (cmp != 0) return cmp;
		cmp = this.idiomaTraducao.compareTo(referencia.idiomaTraducao);
		if (cmp != 0) return cmp;
		return this.getPaginas() - referencia.getPaginas();
	}
}
