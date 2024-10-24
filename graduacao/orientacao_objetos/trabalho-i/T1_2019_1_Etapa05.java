import java.io.IOException;
import java.util.Scanner;

import Capes.Capes;
import Capes.Comando;

public class T1_2019_1_Etapa05 {
	public static void main(String[] args) {
		Scanner entrada = new Scanner(System.in);

		String pasta = entrada.nextLine();
		String arquivo = entrada.nextLine();

		try {
			Capes capes = new Capes(pasta + arquivo);
			capes.lerArquivo();
			for (int i = 0; i < 6; i++) {
				arquivo = entrada.nextLine();
				capes.mudarArquivoDeEntrada(pasta + arquivo);
				capes.lerArquivo();
			}
			Comando comando = Comando.obtemComando(entrada.nextLine());
			capes.executarComando(comando);
		} catch (IOException e) {
			System.out.println("Erro de I/O");
		} catch (Exception e) {
			System.out.println(e.getMessage());
		} finally {
			entrada.close();
		}

	}
}
