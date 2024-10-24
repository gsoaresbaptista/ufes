import java.io.IOException;
import java.util.Locale;
import java.util.Scanner;

import Capes.Capes;

public class T1_2019_1_Etapa01 {
	public static void main(String[] args) {
		Locale.setDefault(new Locale("pt", "BR"));
		Scanner entrada = new Scanner(System.in);

		String pasta = entrada.nextLine();
		String arquivo = entrada.nextLine();
		
		try {
			Capes capes = new Capes(pasta + arquivo);
			capes.lerArquivo();
			// Imprime os dados requisitados
			System.out.printf("Instituicoes que publicaram em anais: %d\n", capes.getNumeroDeInstituicoes());
			System.out.printf("PPGs que publicaram em anais: %d\n", capes.getNumeroDePPGS());
			System.out.printf("Quantidade de producoes em anais: %d\n", capes.getNumeroDeProducoes());
			System.out.printf("Quantidade de paginas publicadas em anais: %d\n", capes.getPaginasPublicadas());
			System.out.printf("Media de paginas por publicacao: %.1f\n", (double)capes.getPaginasPublicadas()/(double)capes.getNumeroDeProducoesValidas());
		} catch (IOException e) {
			System.out.println("Erro de I/O");
		}  catch (Exception e) {
			System.out.println(e.getMessage());
		} finally {
			entrada.close();
		}
	}
}
