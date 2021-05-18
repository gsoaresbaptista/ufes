import Phaser from 'phaser';

/* Classe representando a cena inicial do jogo, onde temos uma cena apenas com
   algumas informações do jogo e o título. */
export default class Inicial extends Phaser.Scene {
    constructor() {
        super();
        /* Configuração dos textos utilizados */
        this.config_frase = {
            color: 'white',
            fontFamily: 'Joystix',
            fontSize: 12
        };
        this.config_titulo = {
            fontFamily: 'Pokemon',
            fontSize: '42px',
            color: '#fefefe',
            strokeThickness: 2,
            stroke: 'black'
        };
    }

    preload() {
        this.load.path = "src/assets/";
        this.load.image('fundo_inicial', 'backgrounds/inicial.png');
        this.load.image('jogar', 'outros/start.png');
        this.load.image('z', 'outros/z.png');
        this.load.image('x', 'outros/x.png');
        this.load.image('setas', 'outros/setas.png');
        this.load.audio('abertura', 'sons/opening.mp3');
        this.load.audio('ok', 'sons/select.ogg');
    }

    create() {
        /* Imagem de fundo */
        this.inicial = this.add.image(0, 0, 'fundo_inicial').setOrigin(0, 0);        
        
        /* Música do jogo */
        if (this.sound.context.state === 'suspended')
            this.sound.context.resume();
        this.musica = this.sound.add('abertura', {loop: true, volume: 0.1});
        this.musica.play();

        /* Criando textos */
        this.criarPressEnter();
        this.criarTitulo();
        this.comoJogar();

        /* Associa a tecla z com a função para trocar a cena */
        this.input.keyboard.on('keydown-Z', () => {
            this.sound.play('ok', {volume: 0.2});
            this.batalhar();
        });
    }

    /* Cria a imagem com o PRESS ENTER na tela */
    criarPressEnter() {
        this.jogar = this.add.image(0, 600, 'jogar').setOrigin(0, 1);

        /* Efeito do PRESS ENTER */
        this.tweens.add({
            targets: this.jogar,
            alpha: 0,
            ease: 'Cubic.easeIn',
            duration: 500,
            repeat: -1,
            yoyo: true
        })
    }

    /* Cria o texto de título */
    criarTitulo() {
        this.titulo = this.add.text(
            400, 200, // Coordenadas
            'POKEMON BATTLE SYSTEM',
            this.config_titulo
        ).setOrigin(.5, .5);

        /* Efeito do Título */
        this.tweens.add({
            targets: this.titulo,
            y: this.titulo.y - 30,
            ease: 'Power1',
            duration: 500,
            repeat: -1,
            yoyo: true
        })
    }

    /* Realiza a transição entre a tela inicial e a seleção dos pokemons */
    batalhar() {
        this.scene.transition({
            target: 'Selecao',
            data: this.musica,
            duration: 0
        });
    }

    /* Exibe as instruções básicas do jogo na tela inicial do jogo */
    comoJogar() {
        /* Adiciona as imagens das teclas */
        this.add.image(10, 10, 'setas').setOrigin(0, 0).setScale(.5);
        this.add.image(17, 40, 'z').setOrigin(0, 0).setScale(.5);
        this.add.image(17, 70, 'x').setOrigin(0, 0).setScale(.5);

        /* Adiciona os textos dos comandos */
        this.frase = this.add.text(
            60,
            12,
            'Mover',
            this.config_frase
        );
        this.frase = this.add.text(
            60,
            42,
            'Confirmar',
            this.config_frase
        );
        this.frase = this.add.text(
            60,
            72,
            'Voltar',
            this.config_frase
        );
    }
}
