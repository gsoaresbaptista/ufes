import System.IO ( hFlush, stdout )
import Text.Printf ( printf )
import Dados.Dados ( kFolds, lerCsv, nomesClasses, Dados ) 
import Dados.Metricas ( media, desvioPadrao )
import Dados.ValidacaoCruzada ( validacaoCruzada, Resultado(..) )
import Classificador.Classificador ( Classificador )
import Classificador.Knn ( novoKnn )
import Classificador.Centroide ( novoCentroide ) 
import Utils.Lista ( somarMatrizes )
import Utils.String ( formatarMatriz )

-- Formata a saida no formata requisitado
formatar :: String-> String -> Double -> IO ()
formatar tipo prompt valor = do
    let valorF = printf "%.2f" (valor * 100)
    let pre = tipo ++ "(" ++ prompt ++ "): "
    putStrLn $ pre ++ valorF ++ "%"
    return ()

-- Faz validacao cruzada e retorna a matriz de confusao somada
validar :: (Classificador cl) => cl -> [Dados] -> [String] -> String -> IO [[Int]]
validar cl folds classes prompt = do
    -- Pega as informacoes do resultado
    let resultado = validacaoCruzada cl classes folds
    let matrizConfusao = somarMatrizes $ matriz resultado
    let mediaAcuracia = media $ acuracias resultado
    let desvioAcuracia = desvioPadrao $ acuracias resultado
    -- Printa os resultados formatados
    formatar "Acuracia" prompt mediaAcuracia
    formatar "Desvio-Padrao" prompt desvioAcuracia
    return matrizConfusao

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
    , lerInformacao "Forneca o número de folds: "
    , lerInformacao "Forneca o número de vizinhos: "
    , lerInformacao "Forneca o valor da semente para geracao randomizada: "
    ]

-- Funcao para padronizar as entradas
descompactar :: [String] -> (String, String, Int, Int, Int)
descompactar [x1, x2, x3, x4, x5] = (x1, x2, read x3, read x4, read x5)

main :: IO ()
main = do
    -- Ler os dados de entrada
    entradas <- lerEntradas
    let (csv, saida, nFolds, vizinhos, semente) = descompactar entradas

    -- Ler os dados e forma os folds
    dados <- lerCsv csv
    let nomes = nomesClasses dados
    let folds = kFolds nFolds semente dados 

    -- Printa os resultados e pegando a matriz de confusao
    resultadoNN <- validar (novoKnn 1) folds nomes "vizinho"
    resultadoCentroide <- validar novoCentroide folds nomes "centroide"
    resultadoKNN <- validar (novoKnn vizinhos) folds nomes "k-vizinhos"

    -- Salva as matrizes de confusao no arquivo de saida
    let texto1 = "vizinho mais próximo:\n" ++ formatarMatriz resultadoNN
    let texto2 = "\ncentroides:\n" ++ formatarMatriz resultadoCentroide
    let texto3 = "\nk-vizinhos mais próximos:\n" ++ formatarMatriz resultadoKNN
    writeFile saida (texto1 ++ "\n" ++ texto2 ++ "\n" ++ texto3)

    return ()
