package Producoes;

/**
 * Producao que representa producoes do tipo Partituras
 * musicais.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class Partmu extends Producao {
	private String formacaoInstrumental;
	private String editora;

	/**
	 * Construtor da Classe
	 * 
	 * @param cidade String representando a cidade.
	 * @param paginas Inteiro representando a quantidade de paginas.
	 * @param natureza String representando a natureza do documento.
	 * @param formacaoInstrumental String representando a formacao instrumental.
	 * @param editora String representando a editora em que foi publicado.
	 */
	public Partmu(String cidade, int paginas, String natureza, String formacaoInstrumental, String editora) {
		super("", "", cidade, natureza, paginas);
		this.editora = editora;
		this.formacaoInstrumental = formacaoInstrumental;
	}

	/**
	 * Imprime o cabecalho da Classe partmu.
	 */
	public static void imprimirCabecalho() {
		System.out.println("Natureza;Editora;Cidade;Formacao;Paginas");
	}
	
	/**
	 * Sobreescreve o metodo toString, ou seja, representa
	 * a maneira usual de se exibir uma producao do tipo partmu.
	 */
	@Override
	public String toString() {
		String texto = this.getNatureza() + ";" + this.editora + ";" + this.getCidade() + 
				";" + this.formacaoInstrumental + ";";
		if(this.getPaginas() > 0) texto += this.getPaginas();
		return texto;
	}

	/**
	 * Sobreescreve o metodo equals.
	 */
	@Override
	public boolean equals(Object obj) {
		if(!(obj instanceof Partmu)) return false;
		Partmu referencia = (Partmu) obj;
		if(this.compareTo(referencia) == 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Sobreescreve o metodo compareTo, ou seja,  representa
	 * a maneira usual de se comparar producoes do tipo partmu.
	 */
	@Override
	public int compareTo(Producao o) {
		Partmu referencia = (Partmu) o;
		// Verifica as caracteristicas
		int cmp = this.getNatureza().compareTo(referencia.getNatureza());
		if (cmp != 0) return cmp;
		cmp = this.editora.compareTo(referencia.editora);
		if (cmp != 0) return cmp;
		cmp = this.getCidade().compareTo(referencia.getCidade());
		if (cmp != 0) return cmp;
		cmp = this.formacaoInstrumental.compareTo(referencia.formacaoInstrumental);
		if (cmp != 0) return cmp;
		return this.getPaginas() - referencia.getPaginas();
	}
	
}
