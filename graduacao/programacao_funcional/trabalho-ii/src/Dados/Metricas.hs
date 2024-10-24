module Dados.Metricas 
    ( media
    , desvioPadrao
    , distancia
    , acuracia
    , matrizConfusao
    ) where

import Utils.Lista ( encontrar, incrementarIndice, transpor, zeros )

{-|
Dado uma lista do R^n, calcula a média dos valores
retornando um double.

Entrada:
    * dados: [Double] -> Lista de Double que se deseja
    calcular a media.
Saida:
    * Double -> Média dos valores.
-}
media :: [Double] -> Double
media dados =
    let n = fromIntegral $ length dados
    in  (/ n) $ sum dados

{-|
Calcula o desvio padrão de uma lista  de tamanho R^n
sendo R^n o espaco dimensional do vetor, retornando um
double.

Entrada:
    * dados: [Double] -> Lista de Double que se deseja
    calcular o desvio padrão.
Saida:
    * Double -> Desvio padrão dos valores da lista.
-}
desvioPadrao :: [Double] -> Double
desvioPadrao dados = 
    let mediaDados = media dados
        -- Faz a diferenca ao quadrado para cada dado
        quadradoDiferenca x = map ((^2) . flip (-) mediaDados) x
        n = fromIntegral $ length dados
    -- Soma todas as diferencas, divide pelo tamanho e calcula
    -- a raiz
    in sqrt . (/ n) . sum $ quadradoDiferenca dados

{-|
Calcula a distância euclidiana entre dois pontos, onde ambos os 
pontos têm suas coordenadas representadas por vetores do R^n.

Entrada:
    * a: [Double] -> Vetor do R^n.
    * b: [Double] -> Vetor do R^n.
Saida:
    * Double -> Número representando a distância euclidiana
    entre os pontos a e b.
-}
distancia :: [Double] -> [Double] -> Double
distancia a b = sqrt . sum $ map (^2) (zipWith (-) a b)

{-|
Calcula acurácia dos dados de entrada comparando os dois
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
Conta quantas classes foram preditas para cada classe
verdadeira, separando por linha, ou seja, caso a entrada
seja um classe1 seu retorno sera uma lista em que a 
primeira posição representa quantos elementos da classe1
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
Retorna a matriz de confusão de um um determidado
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