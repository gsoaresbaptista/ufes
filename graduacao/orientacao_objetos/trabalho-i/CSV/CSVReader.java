package CSV;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

/**
 * Leitor de Arquivos CSV, classe usada para ler arquivos na
 * formatacao usual de arquivos CSV.
 * Obs.: Foi-se escolhido utilizar o bufferedReader por ter
 * demonstrado menor tempo de execucao durante os testes.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class CSVReader  {
	private String regex;
	private BufferedReader leitor;
	private String[] linhaAtual;
	
	/**
	 * Construtor da Classe
	 * 
	 * @param endereco String representando o caminho do arquivo.
	 * @param regex String representando a regex de quebra, usada
	 * no split.
	 * @throws IOException Caso o arquivo represente um arquivo
	 * inexistente.
	 */
	public CSVReader(String endereco, String regex) throws IOException {
		this.leitor = new BufferedReader(new FileReader(new File(endereco)));
		this.regex = regex;
	}
	
	/**
	 * Construtor da Classe
	 * 
	 * @param endereco String representando o caminho do arquivo.
	 * @throws IOException Caso o arquivo represente um arquivo
	 * inexistente.
	 */
	public CSVReader(String endereco) throws IOException {
		this(endereco, ";(?=([^\"]*\"[^\"]*\")*[^\"]*$)");
	}

	/**
	 * Retorna o conteudo da linha atual na posicao requisitada,
	 * ou seja, na posicao do indice, atente-se ao fato que
	 * nenhuma verificacao de validacao do indice e' feita.
	 * 
	 * @param indice Inteiro representado a coluna da qual deseja
	 * a informacao.
	 * @return String com o conteudo da coluna representando o
	 * indice informado.
	 */
	public String getColuna(int indice) {
		return this.linhaAtual[indice];
	}
	
	/**
	 * Le a proxima linha do arquivo CSV, atualizando
	 * a linha atual e a retornando.
	 * 
	 * @return vetor de String, com as informacoes da linha
	 * atual, se existir proxima linha, caso contrario,
	 * retorna null.
	 */
	public String[] proximaLinha(){
		try {
			String linha = this.leitor.readLine();
			this.linhaAtual = linha.split(regex);
			return this.linhaAtual;
		} catch (Exception e) {
			return null;
		}
	}
	
}
