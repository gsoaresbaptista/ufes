import Phaser from 'phaser';
import Inicial from './cenas/inicial';
import Selecao from './cenas/selecao';
import Entrada from './cenas/entrada';
import Batalha from './cenas/batalha';

/* Inicialização do jogo base */
const jogo = new Phaser.Game({
    type: Phaser.AUTO,
    parent: 'jogo',
    width: 800,
    height: 600,
})

/* Adicionando as cenas ao jogo */
jogo.scene.add('Inicial', Inicial, true);
jogo.scene.add('Selecao', Selecao, false);
jogo.scene.add('Entrada', Entrada, false);
jogo.scene.add('Batalha', Batalha, false);