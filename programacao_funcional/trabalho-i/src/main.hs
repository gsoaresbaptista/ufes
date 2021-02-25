import System.IO ( hFlush, stdout )
import Utils.String ( formatarMatriz )
import Classificador.Dados ( lerDados, treinoTeste, classes )
import Classificador.Algoritmos ( centroides, knn )
import Classificador.Metricas ( acuracia, matrizConfusao )
import Text.Printf ( printf )

-- Requisitar uma entrada
lerInformacao :: String -> IO String
lerInformacao prompt = do
    putStr prompt
    hFlush stdout
    getLine

-- Entradas especificas do problema
lerEntradas :: IO [String]
lerEntradas = sequence
    [ lerInformacao "Forneca o nome do arquivo de entrada: "
    , lerInformacao "Forneca o nome do arquivo de saida: "
    , lerInformacao "Forneca o percentual de exemplos de teste: "
    , lerInformacao "Forneca o valor da semente para geracao randomizada: "
    ]

-- Funcao para padronizar as entradas
descompactar :: [String] -> (String, String, Int, Int)
descompactar [x1, x2, x3, x4] = (x1, x2, read x3, read x4)

main :: IO ()
main = do
    -- Lendo os dados e passando para os classificadores
    entradas <- lerEntradas
    let (csv, saida, percentual, semente) = descompactar entradas
    dataset <- lerDados csv
    let (treino, teste) = treinoTeste dataset semente percentual
    
    let preditoK = knn (treino, teste) -- Predito pelo KNN
    let preditoC = centroides (treino, teste) -- Predito pelo centroide

    -- Resultados
    let verdadeiros = map snd teste -- Valores verdadeiros (base de teste)
    let nomesClasses = classes dataset -- Nome de todas classes do csv

    let acuraciaK = acuracia preditoK verdadeiros
    let acuraciaC = acuracia preditoC verdadeiros
    let matrizK = matrizConfusao verdadeiros preditoK nomesClasses
    let matrizC = matrizConfusao verdadeiros preditoC nomesClasses

    -- Printa os resultados na saida
    putStrLn $ "Acuracia(vizinho): " ++ printf "%.2f" (acuraciaK * 100) ++ "%"
    putStrLn $ "Acuracia(centroide): " ++ printf "%.2f" (acuraciaC * 100) ++ "%"

    -- Salva as matrizes de confusao no arquivo de saida
    let texto1 = "vizinho mais próximo:\n" ++ formatarMatriz matrizK
    let texto2 = "\ncentroides:\n" ++ formatarMatriz matrizC
    writeFile saida (texto1 ++ "\n" ++ texto2)

    return ()