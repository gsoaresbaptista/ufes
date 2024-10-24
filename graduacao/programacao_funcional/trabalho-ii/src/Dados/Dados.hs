module Dados.Dados
    ( Dados (..)
    , lerCsv
    , pegarDados
    , padronizar
    , kFolds
    , concatenarDados
    , separarClasses
    , nomesClasses
    ) where

import Utils.Lista ( escolher, dividir, unicos )
import Utils.String ( separar )

{-|
Definição do tipo Dados, representando um conjunto
com todas as informaçoes obtidas do csv.

Atributos:
    * valores: [[Double]] -> Lista contendo os valores
    de cada uma das caracteristicas.
    * classes: [String] -> Lista contendo as classes
    de cada uma das caracteristicas, pareado com valores,
    ou seja, o índice zero de valores corresponde ao índice
    zero das classes.
    * tamanho: Int -> Número de dados contudos no conjunto.
-}
data Dados = Dados
    { valores :: [[Double]]
    , classes :: [String]
    , tamanho :: Int
    } deriving ( Show )

{-|
Lê um arquivo pelo endereço fornecido retornado um
conjunto de dados.

Entrada:
   * csv: FilePath -> Endereço completo do arquivo csv.
Saida:
   * IO Dados -> Ação IO contendo um conjunto de dados
   (classe Dados).
-}
lerCsv :: FilePath -> IO Dados
lerCsv caminho = do
    csv <- readFile caminho
    let linhas = map (separar ',') $ lines csv -- Separa as linhas do csv
    -- Separa e formata os valores
    let f linha (xs, ys, tam) = (init linha: xs, last linha: ys, tam + 1)
    let (valoresTexto, classes, tamanho) = foldr f ([], [], 0) linhas
    let valores = map (map (read::String->Double)) valoresTexto
    return $ Dados valores classes tamanho

{-|
Pega os dados de índices informados de um conjunto já existente
os juntando e formando um novo conjunto de dados.

Entrada:
    * is: [Int] -> Lista de indices que serão pegos do conjunto
    de dados, note que a lista deve ser indexada em zero.
    * dados: Dados -> Conjunto que será retirado os elementos
    de índices requeridos.
Saida:
    * Dados -> Novo conjunto de dados contendo apenas os elementos
    informados em is.
-}
pegarDados :: [Int] -> Dados -> Dados
pegarDados is dados = uncurry Dados separados $ length is
        where
            pegarValores i = valores dados !! i
            pegarClasse i = classes dados !! i
            pegar' i (ss, rs) = (pegarValores i: ss, pegarClasse i: rs)
            separados = foldr pegar' ([], []) is

{-|
Padroniza um conjunto de dados(Dados), utilizando a média e o 
desvio padrão fornecidos, isto é, utiliza-se o z-score,
    Z = (x - M) / sig
Onde, Z é o valor padronizado, x o valor a ser padroniado,
M a média e sig o desvio padrão dos dados que se deseja
utilizar como base de padronização.

Entrada:
    * dados: Dados -> Conjunto de elementos a ser padronizado.
    * media: [Double] -> Lista contendo as médias de cada índice.
    * desvioPadrao: [Double] -> Lista contendo os desvios padrões
    de cada índice.
Saida:
    * Dados -> Conjunto de dados da entrada padronizado.
-}
padronizar :: Dados -> [Double] -> [Double] -> Dados
-- padronizar dados media desvioPadrao = dados
padronizar dados media desvioPadrao =
    let val = valores dados
        -- Aplica a subtracao e depois a divisao
        sub =  map (zipWith subtract media) val
        div = map (zipWith (flip (/)) desvioPadrao) sub
    -- Retorna em formato de Dados
    in Dados div (classes dados) (tamanho dados)

{-|
Separa um conjunto de dados em k subconjuntos aleatórios,
comumente utilizado para métodos de avaliação de algoritmos.

Entrada:
    * k: Int -> Número de subdivisões que serão feitas.
    * semente: Int -> Semente utilizada na geração dos
    números aleatórios.
    * dados: Dados -> Conjunto de dados que será dividido.
Saida:
    * [Dados] -> Lista contendo k conjuntos de dados, onde
    cada conjunto possui (M / k) elementos, sendo M o tamanho
    do conjunto original e k o número de divisões feitas.
-}
kFolds :: Int -> Int -> Dados -> [Dados]
kFolds k semente dados = 
    let n = tamanho dados
        aleatorios = escolher (n, n - 1, semente)
        folds = dividir k aleatorios
    in map (`pegarDados` dados) folds

{-|
Separa os dados fornecidos em conjunto de classes,
agrupando elementos de mesma classe.

Entrada:
   * xs: Dados -> Conjunto de dados, contendo os elementos
    que se deseja agrupar por classe.
Saida:
   * [Dados] -> Lista de Dados, onde cada posicao da Lista
   contem um conjunto de dados que possuem apenas elementos
   de mesma classe.
-}
separarClasses :: Dados -> [Dados]
separarClasses dados = map (\x -> Dados (map fst x) (map snd x) (length x)) $ foldr separar' [[]] (zip (valores dados) (classes dados))
    where
        separar' x [] = [[x]]
        separar' x (l:ls)
            | null l = (x: l): ls
            | snd (head l) == snd x = (x: l): ls
            | otherwise = l: separar' x ls
        
{-|
Concatena uma lista de Dados, unindo os vetores de valores
e classes de cada Dados e somando o o tamanho de cada um 
destes conjuntos.

Entrada:
   * xs: [Dados] -> Lista contendo conjuntos de dados(Dados).
Saida:
   * Dados -> Conjunto de dados contendo todos elementos dos
   conjuntos dentro da lista.
-}
concatenarDados :: [Dados] -> Dados
concatenarDados xs = 
    let val = concatMap valores xs
        cla = concatMap classes xs
        tam = sum $ map tamanho xs
    in  Dados val cla tam

{-|
Pega, em ordem, o nome de cada classe que aparece
nos dados informados, retornando como uma lista de
nome de classes, ou seja, caso o conjunto possua
150 dados de 3 classes diferentes, retornará uma 
lista de tamanho 3 contendo o nome das 3 classes.

Entrada:
   * xs: Dados -> Conjunto de dados.
Saida:
   * [String] -> Lista com o nome das classes contida no
   conjunto.
-}
nomesClasses :: Dados -> [String]
nomesClasses xs = (unicos . classes) xs
