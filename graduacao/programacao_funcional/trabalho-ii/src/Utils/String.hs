module Utils.String
    ( separar
    , formatarMatriz
    , intercalar
    ) where

import Utils.Lista ( comprimento )

{-|
Informado um delimitador e um texto, separa uma string em
uma lista de strings onde o delimitador é o caractere
utilizado como ponto de quebra.

Entrada:
   * sep: [String] -> Caractere utilizado como delimitador.
   * "texto": String -> String a ser separada, note que esta
   subtendida na definicao devido ao retorna se tratar de
   uma função curried.
Saida:
   * [String] -> Lista de strings contendo cada string
   antes do caractere delimitador(ou fim da string).
-}
separar :: Char -> String -> [String]
separar delim = foldr separar' [[]]
    where 
        separar' caractere resultado@(r:rs)
            | caractere == delim = []: resultado
            | otherwise = (caractere: r): rs

{-|
Formata uma matriz de dados numéricos para uma
representação semelhante a utilizada na matemática.

Entrada:
   * xs: [[Num a]] -> Matriz numérica que será
   formatada;
Saida:
   * String -> String contendo a representação da
   matriz em um formato apresentável, semelhante
   ao utilizado na matemática.
-}
formatarMatriz :: (Num a, Show a) => [[a]] -> String
formatarMatriz = init . concatMap escreverLinha
    where
        escreverEl x = replicate (3 - comprimento x) ' ' ++ show x
        escreverLinha x = intercalar ',' (map escreverEl x) ++ "\n"

{-|
Intercala as strings da lista utilizando um
caractere de união(função oposta a separar).

Entrada:
   * uni: Char -> Caractere de uniao;
   * xs: [String] -> Lista com strings que serao
   unidas.
Saida:
   * String -> String resultante da uniao das
   strings da lista com o caracetere de uniao.
-}
intercalar :: Char -> [String] -> String
intercalar uni xs = tail $ foldr (\x acc -> uni: x ++ acc) [] xs