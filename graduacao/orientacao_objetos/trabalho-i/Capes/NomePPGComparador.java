package Capes;
import java.util.Comparator;

/**
 * Classe para comparar dois Programas de Pos
 * Graduacao, PPG, pelo seu nome, meneira 
 * nao convencional, por codigo, por este motivo,
 * implementada utilizando a interface Comparator
 * e nao Comparable na propria classe.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class NomePPGComparador implements Comparator<PPG> {
	@Override
	public int compare(PPG o1, PPG o2) {
		return o1.getNome().compareTo(o2.getNome());
	}
}
