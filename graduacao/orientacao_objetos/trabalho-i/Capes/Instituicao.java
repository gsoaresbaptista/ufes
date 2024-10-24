package Capes;
import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

/**
 * Instituicao de Ensino Superior, manuseia e detem
 * os metodos relacionados as ies.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class Instituicao implements Comparable<Instituicao> {
	private String nome;
	private String sigla;
	private Map<String, PPG> ppgs;

	/**
	 * Construtor da classe.
	 * 
	 * @param nome String representando o nome da ies.
	 * @param sigla String representando a sigla da ies.
	 */
	public Instituicao(String nome, String sigla) {
		this.nome = nome;
		this.sigla = sigla;
		this.ppgs = new HashMap<>();
	}
	
	/**
	 * Rorna o nome da instituicao em questao.
	 * 
	 * @return String representando o nome da ies.
	 */
	public String getNome() {
		return this.nome;
	}
	
	/**
	 * Rorna a sigla da instituicao em questao.
	 * 
	 * @return String representando a sigla da ies.
	 */
	public String getSigla() {
		return this.sigla;
	}
	
	/**
	 * Itera para todos programas de ensino superior,
	 * ppg, relacionados a instituicao em questao, 
	 * acumulando o valor das paginas de cada producao.
	 * 
	 * @return Numero de paginas publicadas por todas ppgs
	 * relacionadas a instituicao.
	 */
	public int getPaginasPublicadas() {
		int paginas = 0;
		for (PPG ppg : this.ppgs.values()) {
			paginas += ppg.getPaginasPublicadas();
		}
		return paginas;
	}
	
	/**
	 * Adiciona um programa de pos graduacao no mapa
	 * da instituicao.
	 * 
	 * @param ppg Referencia da instituicao que se deseja
	 * adicionar.
	 */
	public void addPPG(PPG ppg) {
		this.ppgs.put(ppg.getCodigo(), ppg);
	}

	/**
	 * Sobreescreve o metodo toString, representando,
	 * portanto, a maneira usual de se exibir uma ies.
	 */
	@Override
	public String toString() {
		return this.sigla + " " + "(" + this.nome + ")";
	}

	/**
	 * Sobreescreve o compareTo, representando a maneira
	 * usual de se comparar duas ies.
	 */
	@Override
	public int compareTo(Instituicao referencia) {
		int sigla = this.sigla.compareTo(referencia.sigla);
		if (sigla == 0) {
			return this.nome.compareTo(referencia.nome);
		} else {
			return sigla;
		}
	}

	/**
	 * Imprime um relatorio da instituicao, ordenando suas ppgs,
	 * e, posteriormente, iterando entre elas e as exibindo na saida.
	 */
	public void imprimirRelatorio() {
		List<PPG> ppgs = new ArrayList<>(this.ppgs.values());
		Collections.sort(ppgs, new NomePPGComparador());
		for (PPG ppg : ppgs) {
			System.out.printf("\t- %s: %d producoes\n", ppg.getNome(), ppg.getNumeroDeProducoes());
		}
	}

}

