module Classificador.Algoritmos
    ( centroides
    , calcularCentroides
    , knn
    ) where
    
import Classificador.Dados ( Dado, Dados, separarClasses )
import Classificador.Metricas ( distancia )

{-|
Prediz apenas um Dado pegando a mesma classe do
elemento com a menor distancia euclidiana no conjunto
base.

Entrada:
   * base: Dados -> Conjunto de dados utilizado como
   base de comparacao.

Saida:
   * String -> Classe predita para o dado.
-}
predizer' :: Dados -> Dado -> String
predizer' base x = snd (foldl menorDistancia (-1, "") base)
    where
        menorDistancia acc p
            | dist < fst acc || fst acc == -1 = (dist, snd p)
            | otherwise = acc
            where dist = distancia (fst x) (fst p)

{-|
Prediz as classes para os dados de teste utilizando
os dados de treino como criterio, ou seja, dado um dado
de teste, sua classe predita sera a mesma classe do dado
com a menor distancia nos dados de treino.

Entrada:
   * (treino, teste): (Dados, Dados)-> Tupla contendo
   dois conjuntos de dados, sendo o conjunto de treino
   e teste, respectivamente.

Saida:
   * [String] -> Classes preditas para os dados de 
   teste, na mesma ordem que a entrada.
-}
predizer :: (Dados, Dados) -> [String]
predizer (treino, teste) = 
    let f x acc = predizer' treino x:acc
    in  foldr f [] teste

{-|
Calcula o centroide para uma lista de lista de Double,
onde cada lista de Double pode representar um ponto em 
um espaco n-dimensional ou as features de um dado.

Entrada:
   * xs: [[Double]] -> Lista de lista de double,
   represendado o vetor de dados.

Saida:
   * [Double] -> Centroide para os dados fornecidos.
-}
calcularCentroide :: [[Double]] -> [Double]
calcularCentroide xs = 
    let soma = foldl1 (zipWith (+)) xs
        nPontos = fromIntegral . length $ xs
    in  map (/nPontos) soma

{-|
Executa o algoritmo de classificacao por centroides e 
retorna um lista com as classes preditas para utilizando
o centroide das classes de treino como criterio de comparacao.

Entrada:
   * xs: [Dados] -> Lista contendo conjuntos de dados(Dados),
   onde cada elemento(Dados) da lista representa os dados de uma
   mesma classe.

Saida:
   * Dados -> Retorna Dados, contendo apenas os centroides
   de cada uma das classes.
-}
calcularCentroides :: [Dados] -> Dados
calcularCentroides xs = 
    let classe = snd . head
        centroide' = calcularCentroide . map fst
    in  foldl (\acc x -> (centroide' x, classe x):acc) [] xs

{-|
Executa o algoritmo de classificacao por centroides e 
retorna um lista com as classes preditas para utilizando
o centroide das classes de treino como criterio de comparacao.

Entrada:
   * (treino, teste): (Dados, Dados)-> Tupla contendo
   dois conjuntos de dados, sendo o conjunto de treino
   e teste, respectivamente.

Saida:
   * [String] -> Lista com as classes preditas para
   cada elemento do conjunto de teste.
-}
centroides :: (Dados, Dados) -> [String]
centroides ([], _) = []
centroides (treino, teste) = 
    let centroides = calcularCentroides . separarClasses $ treino
    in  predizer (centroides, teste)

{-|
Executa o algoritmo de ordenacao de vizinho mais proximo
(knn com k igual a 1) e retorna um lista com as classes
preditas para o conjunto de dados de teste utilizando o 
conjunto de treino como criterio de comparacao.

Entrada:
   * (treino, teste): (Dados, Dados)-> Tupla contendo
   dois conjuntos de dados, sendo o conjunto de treino
   e teste, respectivamente.

Saida:
   * [String] -> Lista com as classes preditas para
   cada elemento do conjunto de teste.
-}
knn :: (Dados, Dados) -> [String]
knn (_, []) = []
knn (treino, teste) = predizer (treino, teste)
