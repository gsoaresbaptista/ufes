package Producoes;
import java.text.DateFormat;
import java.text.SimpleDateFormat;
import java.util.Date;

/**
 * Producao que representa producoes do tipo Artigo em
 * jornais e revistas.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class Artjr extends Producao {
	private Date dataDePublicacao;
	private String issn;

	/**
	 * Construtor da Classe
	 * 
	 * @param titulo String representando o titulo.
	 * @param idioma String representando o idioma.
	 * @param cidade String representando a cidade.
	 * @param paginas Inteiro representando a quantidade de paginas.
	 * @param data Date representando a data da producao.
	 * @param issn String representando o issn do documento.
	 */
	public Artjr(String titulo, String idioma, String cidade, int paginas, Date data, String issn) {
		super(titulo, idioma, cidade, "", paginas);
		this.dataDePublicacao = data;
		this.issn = issn;
	}

	/**
	 * Imprime o cabecalho da Classe artjr.
	 */
	public static void imprimirCabecalho() {
		System.out.println("Titulo;Idioma;Cidade;Data;ISSN;Paginas");
	}
	
	/**
	 * Sobreescreve o metodo toString, ou seja, representa
	 * a maneira usual de se exibir uma producao do tipo artjr.
	 */
	@Override
	public String toString() {
		String texto = this.getTitulo() + ";" + this.getIdioma() + ";" + this.getCidade() + ";" + this.getData() + ";"
				+ this.issn + ";";
		if (this.getPaginas() > 0)
			texto += this.getPaginas();
		return texto;
	}

	/**
	 * Retorna a data do artigo em jornais e revistas atual.
	 * 
	 * @return data do documento atual em string formatado na saida brasileira.
	 */
	private String getData() {
		DateFormat formater = new SimpleDateFormat("dd/MM/yyyy");
		return formater.format(this.dataDePublicacao);
	}

	/**
	 * Sobreescreve o metodo equals.
	 */
	@Override
	public boolean equals(Object obj) {
		if (!(obj instanceof Artjr))
			return false;
		Artjr referencia = (Artjr) obj;
		if (this.compareTo(referencia) == 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * Sobreescreve o metodo compareTo, ou seja,  representa
	 * a maneira usual de se comparar producoes do tipo artrj.
	 */
	@Override
	public int compareTo(Producao o) {
		Artjr referencia = (Artjr) o;
		// Verifica as caracteristicas
		int cmp = this.getTitulo().compareTo(referencia.getTitulo());
		if (cmp != 0) return cmp;
		cmp = this.getIdioma().compareTo(referencia.getIdioma());
		if (cmp != 0) return cmp;
		cmp = this.getCidade().compareTo(referencia.getCidade());
		if (cmp != 0) return cmp;
		cmp = this.dataDePublicacao.compareTo(referencia.dataDePublicacao);
		if (cmp != 0) return cmp;
		cmp = this.issn.compareTo(referencia.issn);
		if (cmp != 0) return cmp;
		return this.getPaginas() - referencia.getPaginas();
	}

}
