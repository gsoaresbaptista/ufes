module Utils.Lista 
    ( escolher
    , pegar
    , remover
    , agrupar
    , encontrar
    , zeros
    , incrementarIndice
    , comprimento
    , unicos
    , transpor
    ) where

import System.Random ( mkStdGen, Random(randomRs) )

{-|
Pega os elementos unicos de uma lista de dados,
ignorando elementos repetidos que aparacem
posteriormente.

Entrada:
   * xs: [a] -> Lista que se deseja eliminar
   elementos repetidos.

Saida:
   * [a] -> Lista idem a da entrada, contudo,
   contendo apenas elementos unicos.
-}
unicos :: Eq a => [a] -> [a]
unicos xs = unicos' xs []
    where
        unicos' [] _ = []
        unicos' (x:xs) ls
            | x `elem` ls = unicos' xs ls
            | otherwise = x: unicos' xs (x:ls)

{-|
Remove os valores da segunda lista caso este
pertenca a primeira lista.

Entrada:
   * xs: [a] -> Lista com elementos que se
   deseja remover;
   * lista: [a] -> Lista contendo todos elementos.

Saida:
   * [a] -> Lista contendo todos elementos de `lista`,
   menos os valores contidos em xs.
-}
remover :: Eq a => [a] -> [a] -> [a]
remover xs lista = 
    let pegarElemento x acc = if x `elem` xs then acc else x: acc
    in  foldr pegarElemento [] lista

{-|
Pega os valores da segunda lista utilizando as
os valores da primeira lista como posicoes.

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
Escolhe n numeros unicos aleatorios de zero a M.

Entrada:
   * (qtd, max, semenete) -> Onde qtd e a quantidade
   de numeros que serao gerados, max e o maior valor
   que pode ser gerado e semente e a seed utilizada
   no gerador.

Saida:
   * [Int] -> Lista contendo qtd numeros aleatorios
   onde cada numero esta no intervalo de zero a max.
-}
escolher :: (Int, Int, Int) -> [Int]
escolher (qtd, max, semente) = escolher' qtd
    where
        escolher' qtd = take qtd $ unicos aleatorios
        aleatorios = randomRs (0, max) gerador
        gerador = mkStdGen semente

{-|
Agrupa os elementos iguais de uma lista.

Entrada:
   * xs: [a] -> Lista de elementos que se deseja
   agrupar.

Saida:
   * [[a]] -> Lista contendo agrupamentos de
   elementos iguais.
-}
agrupar :: (Eq a) => [a] -> [[a]]
agrupar = foldl f [[]]
    where
        f [] x = [[x]]
        f (l:ls) x
            | null l = (x:l):ls
            | head l == x = (x:l):ls
            | otherwise = l: f ls x

{-|
Encontra e retorna a posicao(indice iniciando em zero),
de um elemento da lista.

Entrada:
   * id: a -> Elemento que se deseja a posicao na
   lista;
   * id: [a] -> Lista de elementos. 

Saida:
   * [Int] -> Indice do elemento na lista.
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
Incrementa um elemento da lista na posicao
requerida.

Entrada:
   * i: Int -> Indice do elemento a ser
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
Retorna o numero de elementos de um certo dado,
podendo ser o numero de letras de uma string ou
o numero de digitos em um numero.

Entrada:
   * x: a -> Elemento que se deseja o tamanho.

Saida:
   * Int -> Comprimento do elemento.
-}
comprimento :: Show a => a -> Int
comprimento x = length $ show x

{-|
Transpoem uma matriz, ou seja, cada
linha passa a representar uma coluna da matriz
resultante.

Entrada:
   * xs: [[a]] -> Matriz de qualquer tipo de
   elemento.

Saida:
   * [[a]] -> Matriz de entrada transposta.
-}
transpor :: [[a]] -> [[a]]
transpor ([]:_) = []
transpor xs = map head xs: transpor (map tail xs)