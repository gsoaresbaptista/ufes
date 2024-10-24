module Classificador.Knn
    ( Knn (..)
    , novoKnn
    ) where

import Data.List ( sortBy )
import Data.Ord ( comparing )

import Classificador.Classificador ( Classificador(..) )
import Dados.Dados ( Dados(Dados, classes, valores) )
import Dados.Metricas ( distancia )
import Utils.Lista

{-|
Definicao do classificador KNN. Classificador que
classifica um novo dado utilizando os K valores na
base de treino mais próximas ao valor informado,
predizendo como a classe moda ou, em caso de empate,
a classe do dado mais próximo.

Atributos:
    * conjunto: Dados -> Conjunto de dados de treino,
    valores utilizados para comparação com os novos dados.
    * k: Int -> Número de vizinhos, valor que representa
    a quantida de dados comparados que serão pegos no
    momento de determinar a classe de um novo dado.
-}
data Knn = Knn
    { conjunto :: Dados
    , k :: Int
    } deriving ( Show )

{-|
Cria um classificador Knn não ajustado, ou seja, apenas
encapsula o número de vizinhos dentro do classificador,
para que possa ser ajustado em seguida.

Entrada:
   * k: Int -> Número de vizinhos utilizado, observe que
   esta omitido na implementação devido as funções curried.
Saida:
   * Knn -> Classificador Knn não ajustado.
-}
novoKnn :: Int -> Knn
novoKnn = Knn (Dados [] [] 0)

{-|
Prediz um elemento percorendo a base de treino e encontrando
os k vizinhos mais próximos e retorna a moda das classes dentro
deste grupo, caso o haja empate, é retornada a classe do ponto
mais próximo.

Entrada:
   * k: Int -> Número de vizinhos utilizado.
   * base: [([Double], String)] -> Lista de tuplas onde o primeiro
   elemento representa as coordenadas dos valores de treino e o
   segundo a classe.
   * x: [Double] -> Coordenadas do ponto que se deseja predizer.
Saida:
   * String -> Classe prevista para o ponto x.
-}
predizer' :: Int -> [([Double], String)] -> [Double] -> String
predizer' k base x = classe $ sortBy (comparing fst) $ foldl calcularDistancias [] base
    where
        classe = definirClasse . contarVizinhos . take k
        calcularDistancias acc a = (dist, snd a): acc
            where dist = distancia x (fst a)

{-|
Pelo vetor de contagem, vetor obtido pela função contarVizinhos,
retorna a classe que mais apareceu, classe moda, nos k pontos
mais próximos, ou, em caso de empate, retorna o primeiro ponto,
sendo este o ponto mais próximo.

Entrada:
   * xs: [(Int, String)] -> O primeiro elemento de cada tupla
   indica quantas ocorrências da classe tiveram e o segundo
   o nome da classe.
Saida:
   * String -> Nome da classe prevista para o ponto de referência.
-}
definirClasse :: [(Int, Double, String)] -> String
definirClasse xs = trd3 $ foldr1 escolher' xs
    where
        escolher' x a
            | fst3 x > fst3 a = x
            | fst3 x == fst3 a && snd3 x < snd3 a = x
            | otherwise = a

{-|
Pela lista de distâncias, ou seja, uma lista que contêm
as K menores distâncias do dado de referências, conta-se
a quantidade de cada calsse, retornando uma nova lista
no formato [(quantidade, classe)].

Entrada:
   * xs: [(Double, String)] -> Lista de tuplas, onde o
   primeiro elemento da dupla indica a distância da referência
   ao ponto e o segundo elemento indica a classe do ponto.
Saida:
   * [(Int, String)] -> O primeiro elemento de cada tupla
   indica quantas ocorrências da classe tiveram nos k
   pontos mais próximos e o segundo elemento a classe
   destes k pontos.
-}
contarVizinhos :: [(Double, String)] -> [(Int, Double, String)]
contarVizinhos xs = contar' xs []
    where
        contar' [] freq = freq
        contar' (y:ys) [] = contar' ys [(1, fst y, snd y)] 
        contar' (y:ys) (a:as)
            | snd y == trd3 a = contar' ys ((fst3 a + 1, snd3 a + fst y, trd3 a): as)
            | otherwise = contar' ys (a: contar' [y] as)

-- Métodos definidos pela interface
instance Classificador Knn where
    -- Retorna um novo Knn com o valor de k do primeiro e
    -- com o conjunto de dados informado.
    ajustar knn dados = Knn dados $ k knn

    -- Prediz os dados informados pelo novo conjunto.
    predizer knn dados =
        let treino = conjunto knn 
            -- Zipa os valores da base de treino (Dados do Knn)
            base = zip (valores treino) (classes treino)
            -- Para cada elemento do conjunto que será previsto
            -- prediz a classe percorrendo a base de treino
            f x acc = predizer' (k knn) base x: acc
        in  foldr f [] (valores dados)
