module Classificador.Classificador 
    ( Classificador (..)
    ) where

import Dados.Dados ( Dados )

{-|
Definição da interface utilizada pelos elementos
denominados de classificadores, onde cada um destes
deve implementar as funções "ajustar" e "predizer",
buscando manter coesão e padrão no uso dos classificadores.
-}
class Classificador cl where
    {-|
    Dado um classificador base(um classificador que apenas
    foi inicialiazado e não possui conjunto de treino definido),
    retorna um classificador ajustado para o conjunto de treino
    fornecido, deixando-o pronto para predizer novas entradas.
    -}
    ajustar :: cl -> Dados -> cl

    {-|
    Através de um classificador já ajustado para uma base de 
    treino, retorna a predição dos dados contidos no conjunto
    informado.
    -}
    predizer :: cl -> Dados -> [String]
