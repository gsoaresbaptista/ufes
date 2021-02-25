package Producoes;

/**
 * Producao que representa producoes do tipo Outros.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class Outro extends Producao {
	private String editora;

	/**
	 * Construtor da Classe
	 * 
	 * @param idioma String representando o idioma.
	 * @param cidade String representando a cidade.
	 * @param paginas Inteiro representando a quantidade de paginas.
	 * @param natureza String representando a natureza do documento.
	 * @param editora String representando a editora em que foi publicado.
	 */
	public Outro(String idioma, String cidade, int paginas, String natureza, String editora) {
		super("", idioma, cidade, natureza, paginas);
		this.editora = editora;
	}
	
	/**
	 * Imprime o cabecalho da Classe outro.
	 */
	public static void imprimirCabecalho() {
		System.out.println("Natureza;Idioma;Editora;Cidade;Paginas");
	}
	
	/**
	 * Sobreescreve o metodo toString, ou seja, representa
	 * a maneira usual de se exibir uma producao do tipo outro.
	 */
	@Override
	public String toString() {
		String texto = this.getNatureza() + ";" + this.getIdioma() + ";" +
				this.editora + ";" + this.getCidade() + ";";
		if(this.getPaginas() > 0) texto += this.getPaginas();
		return texto;
	}

	/**
	 * Sobreescreve o metodo equals.
	 */
	@Override
	public boolean equals(Object obj) {
		if(!(obj instanceof Outro)) return false;
		Outro referencia = (Outro) obj;
		if(this.compareTo(referencia) == 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Sobreescreve o metodo compareTo, ou seja,  representa
	 * a maneira usual de se comparar producoes do tipo outro.
	 */
	@Override
	public int compareTo(Producao o) {
		Outro referencia = (Outro) o;
		// Verifica as caracteristicas
		int cmp = this.getNatureza().compareTo(referencia.getNatureza());
		if (cmp != 0) return cmp;
		cmp = this.getIdioma().compareTo(referencia.getIdioma());
		if (cmp != 0) return cmp;
		cmp = this.editora.compareTo(referencia.editora);
		if (cmp != 0) return cmp;
		cmp = this.getCidade().compareTo(referencia.getCidade());
		if (cmp != 0) return cmp;
		return this.getPaginas() - referencia.getPaginas();
	}
}
