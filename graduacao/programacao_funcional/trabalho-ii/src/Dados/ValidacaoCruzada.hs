module Dados.ValidacaoCruzada
    ( Resultado (..)
    , validacaoCruzada
    ) where

import Classificador.Classificador ( Classificador(..) )
import Dados.Dados ( concatenarDados, padronizar, Dados(..) )
import Dados.Metricas ( acuracia, desvioPadrao, media, matrizConfusao )
import Utils.Lista ( pegar, removerPosicao, transpor )

data Resultado = Resultado
    { matriz :: [[[Int]]]
    , acuracias :: [Double]
    } deriving ( Show )

{-|
Separa os folds em dois conjuntos, um conjunto para 
testes e outro para treino, sendo o conjunto de
teste constituido de apenas um elemento vindo dos
folds(id) e o conjunto de treino contendo todos os
outros folds(ids).

Entrada:
   * id: Int -> Índice do fold de treino, indexado em
   zero;
   * ids -> [Int] -> Lista contendo os índices dos folds
   utilizados no treino, indexado em zero;
   * folds: [Dados] -> Lista contendo conjunto de dados,
   representando os folds.
Saida:
   * (Dados, Dados) -> Tupla com dois conjuntos de dados,
   sendo o primeiro o conjunto de treino(k-1 folds) e o 
   segundo o de teste(1 fold).
-}
treinoTeste :: Int -> [Int] -> [Dados] -> (Dados, Dados)
treinoTeste id ids folds = 
    let treino = concatenarDados $ pegar ids folds
        teste = head $ pegar [id] folds
        desvioP = map desvioPadrao (transpor $ valores treino)
        mediaV = map media (transpor $ valores treino)
        padronizar' x = padronizar x mediaV desvioP
    in (padronizar' treino, padronizar' teste)

{-|
Faz a validação de um subconjunto de teste, sendo que
a cada validaçao de um subconjunto é trocado o fold de
treino e é utilizado os restantes para treino, retornando
no final a acurácia do subconjunto de teste.

Entrada:
   * (treino, teste): (Dados, Dados) -> Tupla contendo os
   conjuntos de treino e teste, respectivamente;
   * cl: Classificador -> Instância de classificador ainda
   não ajustado(inicialização padrão da classe).
Saida:
   * Double -> Acurácia da predição do subconjunto de teste
   utilizado.
-}
validacaoCruzada' :: Classificador cl => [String] -> (Dados, Dados) -> cl-> (Double, [[Int]])
validacaoCruzada' nomes (treino, teste) cl = 
    let verdadeiros = classes teste
        ajustado = ajustar cl treino
        predito = predizer ajustado teste
    in  (acuracia predito verdadeiros, matrizConfusao verdadeiros predito nomes)

{-|
Aplica a validação cruzada utilizando o classificador
informado sobre os folds informados, note que os folds
não devem estar padronizados.

Entrada:
   * cl: Classificador -> Instância de classificador ainda
   não ajustado(inicialização padrão da classe);
   * folds: [Dados] -> Lista contendo conjunto de dados, ou
   seja, uma lista com os folds a serem utilizados.
Saida:
   * [Double] -> Lista contendo as acurácias de cada subconjunto
   de teste(fixa-se um fold como treino e o restante é utilizado
   no teste, trocando o fold de treino até utilizar todos os folds).
-}
validacaoCruzada :: Classificador cl => cl -> [String] -> [Dados] -> Resultado
validacaoCruzada cl nomes folds =
    let k = length folds
        separados x = treinoTeste x (removerPosicao x [0..k-1]) folds
        validar x = validacaoCruzada' nomes (separados x) cl
        resultado = map validar [0..k-1]
    in  Resultado (map snd resultado) (map fst resultado)
