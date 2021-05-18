/* Classe responsável por representar um ataque do pokemon, possuindo a 
   quantidade de ataques utilizáveis(pp, terminologia do jogo) e as
   informações necessárias para efetuar o dano nos pokemons. */
export default class Ataque {
    constructor(json) {
        this.nome = json.name;
        this.dano = json.damage;
        this.pp = json.pp;
        this.ppmax = json.pp;
        this.tipo = json.type;
    }

    /* Tenta utilizar o ataque e retorna se conseguiu */
    ataqueUtilizado() {
        if (this.pp === 0)
            return false;
        this.pp--;
        return true;
    }

    /* Retorna o texto da quantidade de ataque disponível */
    textoPP() {
        return `${this.pp}/${this.ppmax}`
    }
}