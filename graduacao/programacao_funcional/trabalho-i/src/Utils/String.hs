module Utils.String
    ( separar
    , formatarMatriz
    , intercalar
    ) where

import Utils.Lista ( comprimento )

{-|
Separa uma string em varias substrings utilizando
o caractere divisor como ponto de quebra.

Entrada:
   * divisor: Char -> Caractere de quebra;
   * xs: String -> String que sera separada.

Saida:
   * [String] -> Strings resultantes da separacao
   pelo caractere indicado.
-}
separar :: Char -> String -> [String]
separar divisor xs = foldr separar' [[]] xs
    where
        separar' c lista@(x:xs)
            | c == divisor = []: lista
            | otherwise = (c: x): xs

{-|
Intercala as strings da lista utilizando um
caractere de uniao(funcao oposta a separar).

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

{-|
Formata uma matriz de dados numericos para uma
representacao semelhante a utilizada na matematica.

Entrada:
   * xs: [[Num a]] -> Matriz numerica que sera
   formatada;

Saida:
   * String -> String contendo a representacao da
   matriz em um formato apresentavel, semelhante
   ao utilizado na matematica.
-}
formatarMatriz :: (Num a, Show a) => [[a]] -> String
formatarMatriz = init . concatMap escreverLinha
    where
        escreverEl x = replicate (3 - comprimento x) ' ' ++ show x
        escreverLinha x = intercalar ',' (map escreverEl x) ++ "\n"