module Classificador.Metricas
    ( acuracia
    , distancia
    , matrizConfusao
    ) where

import Utils.Lista ( encontrar, incrementarIndice, zeros, transpor )

{-|
Calcula a distancia euclidiana de duas lista de double
onde cada elemento das listas representam a coordenada
de um ponto, ou em caso de classificadores, a i-th
feature de um dado.

Entrada:
   * p1: [String] -> Primeiro ponto.
   * p2: [String] -> Segundo ponto.

Saida:
   * Double -> Distancia euclidiana entre os dois pontos.
-}
distancia :: [Double] -> [Double] -> Double
distancia p1 p2 = sqrt . sum $ zipWith (\a b -> (a-b)^2) p1 p2

{-|
Calcula acuracia dos dados de entrada comparando os dois
vetores de entrada, em suma, retorna a quantidade de
acertos dividido pelo total.

Entrada:
   * xs: [String] -> Primeiro conjunto de classes.
   * ys: [String] -> Segundo conjunto de classes.

Saida:
   * Double -> Acuracia dos dados xs e ys.
-}
acuracia :: [String] -> [String] -> Double
acuracia _ [] = 0.0
acuracia xs ys = foldl comparar 0 (zip xs ys) / nElementos
    where
        nElementos = fromIntegral . length $ ys
        comparar acc x = if uncurry (==) x then succ acc else acc

{-|
Conta quantas classes foram predita para cada classe
verdadeira, separando por linha, ou seja, caso a entrada
seja um classe1 seu retorno sera uma lista em que a 
primeira posicao representa quantos elementos da classe1
foram peditos como classe1, seguido de quantos elementos
da classe1 foram preditos como classe2 e assim por diante.

Entrada:
   * xs: [(String, String)] -> Lista contendo tuplas
   onde o primeiro elemento representa a classe verdadeira
   e o segundo a classe predita do dado.
   * classes: [String] -> Vetor contendo o nome de todas
   as classes.
   * classes: String -> Nome da classe que se deseja saber
   o resultado, ou seja, a classe que sera definida como
   a classe verdadeira.

Saida:
   * [Int] -> Lista representando uma linha onde cada
   elemento representa quantos dados da classe foram
   preditos como a classe naquela posicao(posicao
   retirada da ordem do vetor classes).
-}
contarClasses :: [(String, String)] -> [String] -> String -> [Int]
contarClasses xs classes classe = 
    let dados = filter ((==) classe . fst) xs
        pos l = encontrar (snd l) classes
        incrementar x l = incrementarIndice (pos l) x
    in  foldl incrementar (zeros $ length classes) dados

{-|
Retorna a matriz de confusao de um um determidado
conjunto de dados pelos dados preditos por algum
metodo(classificador). Onde a posicao Aij indica
a quantidade da classe i que foi predita como j,
caso i igual a j indica um verdadeiro positivo.

Entrada:
   * verdadeiros: [String] -> Vetor contendo o nome
   verdadeiro do dado.
   * preditos: [String] -> Vetor contendo o nome 
   predito para o dado, tendo a posicao correspondente
   com a lista `verdadeiros`.
   * classes: [String] -> Nome de todas as classes,
   utilizada 

Saida:
   * [[Int]] -> Matriz de confusao dos dados preditos.
-}
matrizConfusao :: [String] -> [String] -> [String] -> [[Int]]
matrizConfusao verdadeiros preditos classes =
    let pareados = zip verdadeiros preditos
        pegarLinha c xs = contarClasses pareados classes c:xs
    in  transpor $ foldr pegarLinha [] classes