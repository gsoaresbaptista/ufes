import Phaser from 'phaser';

/* Classe responsável por todos os botões do jogo, cuidado do clique e de
sprites especiais (caso sejam clicados ou estejam em opção de serem 
selecionados) */
export default class Botao extends Phaser.GameObjects.Sprite {
    constructor(frame, cena, nome, x, y, clicado) {
        super(cena, 0, 0, nome, frame);
        this.x = x;
        this.y = y;
        this.cena = cena;
        this.spritePadrao = frame;
        this.spriteFoco = frame + 1;
        this.clicado = clicado;
        this.ativo = false;
    }

    /* Desenha um botão no jogo */
    desenhar() {
        this.setOrigin(0, 0);
        this.cena.add.existing(this);
        this.setDepth(1);
    }

    /* Redefine um botão como inativo (retorna para o sprite básico) */
    redefinir() {
        this.ativo = false;
        this.setFrame(this.spritePadrao);
    }

    /* Define o botão como ativo (e troca para um sprite especial) */
    foco() {
        this.ativo = true;
        this.setFrame(this.spriteFoco);
    }

    /* Função de callback utilizada no construtor */
    funcao() {
        this.clicado();
    }
}