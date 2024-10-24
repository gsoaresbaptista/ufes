module Classificador.Dados 
    ( Dado
    , Dados
    , lerDados
    , treinoTeste
    , separarClasses
    , classes
    ) where

import Utils.Lista ( remover, pegar, escolher, unicos )
import Utils.String ( separar )

-- Definicao de um tipo sinonimo para Dado e Dados
type Dado = ([Double], String)
type Dados = [Dado]

{-|
Le um arquivo pelo endereco do arquivo .csv dos dados
retornado um conjunto de dados(Dados).

Entrada:
   * csv: FilePath -> Endereco do completo do arquivo(
    /pasta1/past2/../nome.tipo, usualmente .csv).

Saida:
   * IO Dados -> Acao IO contendo um conjunto de dados(Dados).
-}
lerDados :: FilePath -> IO Dados
lerDados csv = do 
    csv <- readFile csv
    let linhas = map (separar ',') . lines $ csv
    let converter = map (read::String->Double)
    return $ map (\x-> (converter . init $ x, last x)) linhas

{-|
Separa um conjunto de dados em dois conjuntos sendo 
o primeiro o conjunto de treino e o segundo o conjunto
de teste, utilizando numeros pseudo-aleatorios advindos
de uma semente de geracao.

Entrada:
   * dataset: Dados -> Conjunto de dados que sera dividido
   entre treino e teste.
   * semente: Int -> Inteiro representando a semente de geracao
   utilizada nos numeros aleatorios.
   * percentual: Int -> Inteiro que apos a divisao por cem, 
   indica a porcentagem dos dados dos dados de treino.

Saida:
   * [String] -> Tupla contendo os dados de treino e 
   teste, respectivamente.
-}
treinoTeste :: Dados -> Int -> Int -> (Dados, Dados)
treinoTeste dataset semente percentual  = (treino, teste)
    where
        treino = pegar (remover escolhidos [0..pred nTotal]) dataset
        teste = pegar escolhidos dataset
        nTotal = length dataset
        -- `div` Pega somente a parte inteira da divisao
        nTeste = (nTotal * percentual) `div` 100 
        escolhidos = escolher (nTeste, pred nTotal, semente)

{-|
Separa os dados fornecidos em conjunto de classes,
agrupando elementos de mesma classe.

Entrada:
   * xs: Dados -> Conjunto de dados([([Double], String)]),
   contendo os elementos que se deseja agrupar por classe.

Saida:
   * [Dados] -> Lista de Dados, onde cada posicao da Lista
   contem um conjunto de dados que possuem apenas elementos
   de mesma classe.
-}
separarClasses :: Dados -> [Dados]
separarClasses = foldl f [[]]
    where
        f [] x = [[x]]
        f (l:ls) x
            | null l = (x: l): ls
            | snd (head l) == snd x = (x: l): ls
            | otherwise = l: f ls x

{-|
Pega, em ordem, o nome de cada classe que aparece
nos dados informados, retornando como uma lista de
nome de classes, ou seja, caso o conjunto possua
150 dados de 3 classes diferentes, retornara uma 
lista de tamanho 3 contendo o nome das 3 classes.

Entrada:
   * xs: Dados -> Conjunto de dados([([Double], String)]),
   contendo todos exemplos do conjunto do problema.

Saida:
   * [String] -> Lista com o nome das classes contidas no
   conjunto.
-}
classes :: Dados -> [String]
classes xs = unicos $ map snd xs
