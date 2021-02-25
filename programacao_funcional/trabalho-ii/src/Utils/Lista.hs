module Utils.Lista 
    ( escolher
    , dividir
    , removerPosicao
    , pegar
    , fst3
    , snd3
    , trd3
    , transpor
    , encontrar
    , incrementarIndice
    , zeros
    , unicos
    , somarMatrizes
    , comprimento
    ) where

import System.Random ( mkStdGen, Random(randomRs) )

{-|
Pega os elementos únicos de uma lista de dados,
ignorando elementos repetidos que aparacem
posteriormente.

Entrada:
   * xs: [a] -> Lista que se deseja eliminar
   elementos repetidos.
Saida:
   * [a] -> Lista da entrada, contendo apenas
   elementos únicos.
-}
unicos :: Eq a => [a] -> [a]
unicos xs = unicos' xs []
    where
        unicos' [] _ = []
        unicos' (x:xs) ls
            | x `elem` ls = unicos' xs ls
            | otherwise = x: unicos' xs (x:ls)

{-|
Escolhe n números unicos aleatórios de zero a M.

Entrada:
   * (qtd, max, semente) -> Onde qtd é a quantidade
   de números que serão gerados, max e o maior valor
   que pode ser gerado e semente e a seed utilizada
   pelo gerador.
Saida:
   * [Int] -> Lista contendo qtd números aleatórios
    no intervalo de zero a max.
-}
escolher :: (Int, Int, Int) -> [Int]
escolher (qtd, max, semente) = escolher' qtd
    where
        escolher' qtd = take qtd $ unicos aleatorios
        aleatorios = randomRs (0, max) gerador
        gerador = mkStdGen semente

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
dividirResto :: [[Int]] -> [Int] -> [[Int]]
dividirResto div [] = div
dividirResto (d:ds) (r:rs) = (d ++ [r]): dividirResto ds rs

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
dividir :: Int -> [Int] -> [[Int]]
dividir qtd xs = 
    let tam = length xs `div` qtd
        lista = take (qtd * tam) xs
        resto = drop (qtd * tam) xs
        -- Divide a quantidade inteira igualmente
        dividido = foldr f [[]] lista
        f x res@(r:rs) = 
            if length r == tam
            then [x]: res
            else (x: r): rs
        -- Por fim, divide o resto
    in  dividirResto dividido resto

{-|
Pega os valores da segunda lista utilizando as
os valores da primeira lista como índices.

Entrada:
   * indices: [a] -> Indices que serao pegos;
   * dados: [a] -> Lista de elementos.
Saida:
   * [a] -> Lista contendo os elementos nas
   posicoes da primeira lista.
-}
pegar :: [Int] -> [a] -> [a]
pegar indices dados = [dados !! i | i <- indices]

{-|
Remove o elemento na posição informada pelo índice,
em caso de lista vazia retorna a própria lista.

Entrada:
   * i: Int -> Posição do elemento a ser removido,
   sendo indexado em zero;
   * (x:xs): [a] -> Lista composta de elementos quaisquer.
Saida:
   * [a] -> Lista da entrada sem o elemento na posição
   informada.
-}
removerPosicao :: Int -> [a] -> [a]
removerPosicao _ [] = []
removerPosicao i (x:xs)
    | i /= 0 = x: removerPosicao (i - 1) xs
    | otherwise = xs

{-|
Retorna o primeiro elemento de um trio.

Entrada:
   * (a, b, c) -> Trio onde cada elemento pode
   representar um tipo diferente, mesmo que os
   três elementos possam ser do mesmo tipo.
Saida:
   * a -> Primeiro elemento do trio.
-}
fst3 :: (a, b, c) -> a
fst3 (a, _, _) = a

{-|
Retorna o segundo elemento de um trio.

Entrada:
   * (a, b, c) -> Trio onde cada elemento pode
   representar um tipo diferente, mesmo que os
   três elementos possam ser do mesmo tipo.
Saida:
   * b -> Segundo elemento do trio.
-}
snd3 :: (a, b, c) -> b
snd3 (_, b, _) = b

{-|
Retorna o terceiro elemento de um trio.

Entrada:
   * (a, b, c) -> Trio onde cada elemento pode
   representar um tipo diferente, mesmo que os
   três elementos possam ser do mesmo tipo.
Saida:
   * c -> Terceiro elemento do trio.
-}
trd3 :: (a, b, c) -> c
trd3 (_, _, c) = c

{-|
Transpoem uma matriz, ou seja, cada
linha passa a representar uma coluna da matriz
resultante.

Entrada:
   * xs: [[a]] -> Matriz de qualquer tipo de
   elemento.
Saida:
   * [[a]] -> Matriz da entrada transposta.
-}
transpor :: [[a]] -> [[a]]
transpor ([]:_) = []
transpor xs = map head xs: transpor (map tail xs)

{-|
Incrementa um elemento da lista na posição
requerida.

Entrada:
   * i: Int -> Índice do elemento a ser
   incrementado;
   * xs: [a] -> Lista de numeros.
Saida:
   * [Int] -> Lista idem a xs, contudo, com
   o elemento na posicao i incrementado.
-}
incrementarIndice :: (Num a, Enum a) => Int -> [a] -> [a]
incrementarIndice i xs = incrementar' i xs
    where
        incrementar' _ [] = []
        incrementar' i (x:xs)
            | i == 0 = succ x:xs
            | otherwise = x:incrementar' (pred i) xs

{-|
Encontra e retorna a posição(índice iniciando em zero),
de um elemento da lista.

Entrada:
   * id: a -> Elemento que se deseja a posicao na
   lista;
   * id: [a] -> Lista de elementos. 
Saida:
   * [Int] -> Índice do elemento na lista.
-}
encontrar :: Eq a => a -> [a] -> Int
encontrar id = length . takeWhile (/= id)

{-|
Gera uma lista de tamanho N contendo apenas zeros.

Entrada:
   * x: Int -> Tamanho da lista de saida.
Saida:
   * [Int] -> Lista de tamanho x contendo apenas
   zeros.
-}
zeros :: Int -> [Int]
zeros x = replicate x 0

{-|
Soma, célula por célula de todos as matrizes na
lista, retornando apenas uma matriz resultante.

Entrada:
   * matrizes: [[[a]]] -> Lista de matrizes
   numéricas que serão somadas.
Saida:
   * [[a]] -> Matriz resultante da soma de todas as
   matrizes da lista.
-}
somarMatrizes :: (Num a) => [[[a]]] -> [[a]]
somarMatrizes matrizes = foldr1 (zipWith (zipWith (+))) matrizes

{-|
Retorna o número de elementos de um certo dado,
podendo ser o número de letras de uma string ou
o número de dígito em um número.

Entrada:
   * x: a -> Elemento que se deseja o tamanho.
Saida:
   * Int -> Comprimento do elemento.
-}
comprimento :: Show a => a -> Int
comprimento x = length $ show x