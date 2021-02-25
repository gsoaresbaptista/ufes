package Capes;

/**
 * Enum representando os comandos do programa, ou seja,
 * corresponde aos 4 comandos:
 * - REDE
 * - CSV
 * - IES
 * - PPG
 * E caso seja inserido um comando inexistente, retorna o "comando" 
 * DESCONHECIDO, permitindo a apuracao de erros posteriormente.
 * 
 * @author Gabriel S. Baptista
 *
 */
public enum Comando {
	REDE("rede"), CSV("csv"), IES("ies"), PPG("ppg"), DESCONHECIDO("");
	
	private String nome;
	private String info;
	
	Comando(String nome){
		this.nome = nome;
	}
	
	/**
	 * Captura o comando corresponte a String passada, capturada
	 * pela entrada do programa, caso nao seja encontrada nenhum comando
	 * retorna-se o "comando" DESCONHECIDO.
	 * 
	 * @param linha String com todo o conteudo da linha inserida, importante
	 * ser passado integralmente, visto que alguns comandos tem a necessidade
	 * de dados extras, estes que sao armazenados no proprio comando.
	 * 
	 * @return Comando correspondente caso exista, caso contrario, retorna o comando
	 * DESCONHECIDO.
	 */
	public static Comando obtemComando(String linha) {
		String[] info = linha.split("\\s");
		for(Comando comando : Comando.values()) {
			if (comando.nome.equals(info[0])) {
				if(comando.nome.equals("rede")) {
					return REDE;
				} else if (comando.nome.equals("csv")) {
					Comando csv = CSV;
					csv.info = info[1] + " " + info[2];
					return csv;
				} else if(comando.nome.equals("ies")) {
					Comando ies = IES;
					ies.info = info[1];
					return ies;
				} else {
					// Comando PPG
					Comando ppg = PPG;
					ppg.info = info[1];
					return ppg;
				}
			}	
		}
		return DESCONHECIDO;
	}
	
	/**
	 * Retorna a informacao grafada no comando, atente-se no fato
	 * que este comando nao distingue os comandos, portanto, nao faz
	 * nenhuma verificacao do comando possuir ou nao uma informacao.
	 * 
	 * @param indice Indice onde e' salva a informacao. 
	 * Ex.: "csv 30001013007P0 anais", o  codigo da ppg(30001013007P0),
	 * corresponde ao indice 0, enquanto que o tipo de producao corresponde
	 * ao indice 1.
	 * @return String com a informacao correspondente ao indice passado.
	 */
	public String getInfo(int indice) {
		return this.info.split("\\s")[indice];
	}
	
	/**
	 * Idem ao comando com parametro, contanto e' usado em casos que o comando
	 * possui apenas um espaco para informacao.
	 * 
	 * @return Informacao do comando.
	 */
	public String getInfo() {
		return this.getInfo(0);
	}
}
