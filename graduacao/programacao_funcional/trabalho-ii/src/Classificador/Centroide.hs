module Classificador.Centroide
    ( Centroide (..)
    , novoCentroide
    ) where

import Classificador.Classificador ( Classificador(..) )
import Dados.Dados ( Dados(..), separarClasses, concatenarDados )
import Dados.Metricas ( distancia )

{-|
Definicao do classificador Centroide. Classificador que
classifica um novo dado predizendo a classe deste valor 
atravéz do centroide mais próximo do dado, sendo os
centroides calculádos no ajusto do classificador,
reduzindo o conjunto de treino a um conjunto com apenas
um elemento de cada classe(centroide da classe).

Atributos:
    * conjunto: Dados -> Conjunto de dados de treino
    apóis o ajuste, ou seja, os centroides dos dados.
-}
newtype Centroide = Centroide
    { conjunto :: Dados
    } deriving ( Show )

{-|
Cria um classificador Centroide não ajustado, ou seja, contem
apenas um conjunto de dados vazio, mesmo esta definição de função
sendo totalmente desnecessária vale ressaltar que seu uso é
atribuido ao fato da consistência na utilização dos classificadores.

Entrada:
   -
Saida:
   * Centroide -> Classificador Centroide não ajustado.
-}
novoCentroide :: Centroide
novoCentroide = Centroide (Dados [] [] 0)

{-|
Calcula o centroide para uma lista de lista de Double,
onde cada lista de Double pode representar um ponto em
um espaco n-dimensional ou as features de um dado.

Entrada:
   * xs: [[Double]] -> Lista de lista de double,
   represendado a matriz de dados de treino.
Saida:
   * [Double] -> Centroide para os dados fornecidos.
-}
calcularCentroide :: [[Double]] -> [Double]
calcularCentroide xs = 
    let soma = foldl1 (zipWith (+)) xs
        nPontos = fromIntegral . length $ xs
    in  map (/nPontos) soma

{-|
Calcula todos os centroides dos dados de entrada, retornando
uma um conjunto de dados que contem apenas um elemento de cada
classe, sendo esses os centroides.

Entrada:
   * xs: Dados -> Conjunto de dados de onde deve ser calculado
   os centroides.
Saida:
   * Dados -> Conjunto de Dados, contendo apenas os centroides
   de cada uma das classes.
-}
calcularCentroides :: Dados -> Dados
calcularCentroides xs = 
    let datasets = separarClasses xs
        centroide' = calcularCentroide . valores
        centroideDados x = Dados [centroide' x] [head (classes x)] 1
    in  concatenarDados $ foldl (\acc x -> centroideDados x: acc) [] datasets

{-|
Prediz um elemento percorendo a base de treino e encontrando
o centroide mais próximos e define a classe deste dado como
a mesma do centroide.

Entrada:
   * base: [([Double], String)] -> Lista de tuplas onde o primeiro
   elemento representa as coordenadas dos valores de treino e o
   segundo a classe.
   * x: [Double] -> Coordenadas do ponto que se deseja predizer.
Saida:
   * String -> Classe prevista para o ponto x.
-}
predizer' :: [([Double], String)] -> [Double] -> String
predizer' base x = snd (foldl menorDistancia (-1, "") base)
    where
        menorDistancia acc p
            | dist < fst acc || fst acc == -1 = (dist, snd p)
            | otherwise = acc
            where dist = distancia x (fst p)

-- Métodos definidos pela interface
instance Classificador Centroide where
    ajustar _ dados = 
        let centroides = calcularCentroides dados
        in  Centroide centroides
    
    predizer centroide dados =
        let treino = conjunto centroide 
            -- Zipa os valores da base de treino (Dados do Knn)
            base = zip (valores treino) (classes treino)
            -- Para cada elemento do conjunto que será previsto
            -- prediz a classe percorrendo a base de treino
            f x acc = predizer' base x: acc
        in  foldr f [] (valores dados)