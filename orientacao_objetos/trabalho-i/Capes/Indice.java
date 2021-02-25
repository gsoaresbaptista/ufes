package Capes;
import java.util.HashMap;
import java.util.Map;

/**
 * Cabecalho do arquivo CSV da CAPES, ou seja, tem registrado todas
 * os nomes necessario dos arquivos de cada tipo de producao, usados
 * para saber os indices de cada informacao, tornando-se desnecessario
 * utilizar numeros fixos para recolher dados dos vetores de string.
 * 
 * @author Gabriel S. Baptista
 *
 */
public class Indice {
	private String[] cabecalho;
	private Map<String, Integer> indices;
	
	public Indice(String[] cabecalho) {
        this.cabecalho = cabecalho;
		this.indices = new HashMap<>();
		/*
		 * Percorre o vetor de Strings, neste caso o proprio cabecalho,
		 * adicionando no mapa com o indice que este representa, onde a 
		 * chave e' o proprio nome do espaco do cabecalho, estes que foram
		 * fixados nos metodos abaixo. 
		 */
		for (int indice = 0; indice < cabecalho.length; indice++) {
			this.indices.put(cabecalho[indice], indice);
		}
	}
	
	/**
	 * @return String com todos as informacoes do cabecalho.
	 */
	public String[] getCabecalho() {
		return this.cabecalho;
	}
	
	public int codigoPPG() {
		return this.indices.get("CD_PROGRAMA_IES");
	}
	
	public int dataDePublicacao() {
		return this.indices.get("DT_PUBLICACAO");
	}

	public int editora() {
		return this.indices.get("NM_EDITORA");
	}
	
	public int editoraTradu() {
		return this.indices.get("NM_EDITORA_TRADUCAO");
	}
	
	public int idioma() {
		return this.indices.get("DS_IDIOMA");
	}
	
	public int issn() {
		return this.indices.get("DS_ISSN");
	}
	
	public int isbn() {
		return this.indices.get("DS_ISBN");
	}
	
	public int natureza() {
		return this.indices.get("DS_NATUREZA");
	}

	public int cidade() {
		return this.indices.get("NM_CIDADE");
	}
	
	public int cidadePais() {
		return this.indices.get("NM_CIDADE_PAIS");
	}

	public int paginasContribuicao() {
		return this.indices.get("NR_PAGINAS_CONTRIBUICAO");
	}
	
	public int nomePPG() {
		return this.indices.get("NM_PROGRAMA_IES");
	}

	public int evento() {
		return this.indices.get("DS_EVENTO");
	}

	public int nomeIES() {
		return this.indices.get("NM_ENTIDADE_ENSINO");
	}

	public int tituloProducao() {
		return this.indices.get("NM_TITULO");
	}

	public int paginaFinal() {
		return this.indices.get("NR_PAGINA_FINAL");
	}

	public int paginaInicial() {
		return this.indices.get("NR_PAGINA_INICIAL");
	}
	
	public int paginas() {
		return this.indices.get("NR_PAGINAS");
	}

	public int siglaIES() {
		return this.indices.get("SG_ENTIDADE_ENSINO");
	}

	public int subTipoDeProducao() {
		return this.indices.get("ID_SUBTIPO_PRODUCAO");
	}
	
	public int tipoDeProducao() {
		return this.indices.get("ID_TIPO_PRODUCAO");
	}
	
	public int volume() {
		return this.indices.get("NR_VOLUME");
	}

	public int fasciculo() {
		return this.indices.get("DS_FASCICULO");
	}

	public int serie() {
		return this.indices.get("NR_SERIE");
	}

	public int formacaoInstrumental() {
		return this.indices.get("DS_FORMACAO_INSTRUMENTAL");
	}

	public int idiomaTraducao() {
		return this.indices.get("DS_IDIOMA_TRADUCAO");
	}

}
