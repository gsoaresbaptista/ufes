package Producoes;

/**
 * Producao que representa producoes do tipo Anais.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class Anais extends Producao {
	private String evento;
	
	/**
	 * Construtor da Classe
	 * 
	 * @param titulo String representando o titulo.
	 * @param idioma String representando o idioma.
	 * @param cidade String representando a cidade.
	 * @param paginas Inteiro representando a quantidade de paginas.
	 * @param natureza String representando a natureza do documento.
	 * @param evento String representando o evento em que foi apresentado.
	 */
	public Anais(String titulo, String idioma, String cidade, int paginas, String natureza, String evento) {
		super(titulo, idioma, cidade, natureza, paginas);
		this.evento = evento;
	}
	
	/**
	 * Imprime o cabecalho da Classe anais.
	 */
	public static void imprimirCabecalho() {
		System.out.println("Natureza;Titulo;Idioma;Evento;Cidade;Paginas");
	}
	
	/**
	 * Sobreescreve o metodo toString, ou seja, representa
	 * a maneira usual de se exibir uma producao do tipo anais.
	 */
	@Override
	public String toString() {
		String texto = this.getNatureza() + ";" + this.getTitulo() + ";" +
				this.getIdioma() + ";" + this.evento + ";" + this.getCidade() + ";";
		if (this.getPaginas() > 0) texto += this.getPaginas();
		return texto;
	}

	/**
	 * Sobreescreve o metodo equals.
	 */
	@Override
	public boolean equals(Object obj) {
		if (!(obj instanceof Anais)) return false;
		Anais referencia = (Anais) obj;
		if (this.compareTo(referencia) == 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Sobreescreve o metodo compareTo, ou seja,  representa
	 * a maneira usual de se comparar producoes do tipo anais.
	 */
	@Override
	public int compareTo(Producao o) {
		Anais referencia = (Anais) o;
		// Verifica as caracteristicas
		int cmp = this.getNatureza().compareTo(referencia.getNatureza());
		if (cmp != 0) return cmp;
		cmp = this.getTitulo().compareTo(referencia.getTitulo());
		if (cmp != 0) return cmp;
		cmp = this.getIdioma().compareTo(referencia.getIdioma());
		if (cmp != 0) return cmp;
		cmp = this.evento.compareTo(referencia.evento);
		if (cmp != 0) return cmp;
		cmp = this.getCidade().compareTo(referencia.getCidade());
		if (cmp != 0) return cmp;
		return this.getPaginas() - referencia.getPaginas();
	}
}
