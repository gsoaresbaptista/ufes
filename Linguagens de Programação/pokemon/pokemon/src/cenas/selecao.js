import Phaser from 'phaser';

/* Classe representando a cena de seleção de pokemons */
class Selecao extends Phaser.Scene {
    constructor() {
        super();

        /* pokemons selecionaveis */ 
        this.pokemons = [
            'blastoise',
            'charizard',
            'pikachu',
            'venusaur',
            'zapdos'
        ];

        /* Nome dos pokemons escolhidos */
        this.jogador = '';
        this.inimigo = '';

        this.selecionado = 0; // Índice do pokemon selecionado
        this.imagem_jogador = null;
        this.imagem_inimigo = null;
        
        this.frase = null;

        /* Configurações dos textos utilizados */
        this.config_nomes = {
            color: 'white',
            fontFamily: 'Pokemon',
            fontSize: 40
        };
        this.config_frase = {
            color: 'white',
            fontFamily: 'Joystix',
            fontSize: 12
        };
    }

    preload() {
        this.load.path = "src/assets/";
        this.load.image('background', 'backgrounds/menu.png');
        this.load.pack('pokemons', '../jsons/recursos.json');
        /* Carrega os audios da cena */
        this.load.audio('cursor', 'sons/cursor.wav');
        this.load.audio('select', 'sons/select.ogg');
    }

    create(musica) {
        /* Cria cena atual, onde a música é a que veio da cena anterior(principal) */
        this.musica = musica;
        this.background = this.add.image(0, 0, 'background');
        this.background.setOrigin(0, 0).setAlpha(.5);
        this.caixa = this.add.rectangle(250, 66, 300, 468, 0x1D1D1D);
        this.caixa.setOrigin(0, 0).setAlpha(.8);
        this.caixa.setStrokeStyle(3, 0xffffff);
        this.gerarNomes();

        /* Mapeia a tecla z e os cursores*/
        this.cursors = this.input.keyboard.createCursorKeys();
        this.z = this.input.keyboard.addKey('Z');
    }

     /* Gera os nomes de todos os pokemons selecionaveis e os exibe na tela */ 
    gerarNomes() {
        this.nomes = [
            this.add.text(400, 139, 'Blastoise', this.config_nomes),
            this.add.text(400, 215, 'Charizard', this.config_nomes),
            this.add.text(400, 296, 'Pikachu', this.config_nomes),
            this.add.text(400, 378, 'Venusaur', this.config_nomes),
            this.add.text(400, 453, 'Zapdos', this.config_nomes)
        ]
        this.nomes.forEach(nome => nome.setOrigin(.5, .5));
    }

    /* Desenha a imagem do pokemon que está em foco durante a seleção */ 
    desenharPokemon() {
        if (this.jogador === '') {
            if (this.imagem_jogador != null)
                this.imagem_jogador.destroy();

            this.imagem_jogador = this.add.image(
                125,
                300,
                `${this.pokemons[this.selecionado]}_frente`
            );

            this.imagem_jogador.displayWidth = 180;
            this.imagem_jogador.displayHeight = 180;
        } else {
            if (this.imagem_inimigo != null)
                this.imagem_inimigo.destroy();

            this.imagem_inimigo = this.add.image(
                675,
                300,
                `${this.pokemons[this.selecionado]}_frente`
            );

            this.imagem_inimigo.displayWidth = 180;
            this.imagem_inimigo.displayHeight = 180;
        }  
    }

    /* Verifica se o jogador apertou algum cursor */ 
    verificarBotoes() {
        if (this.cursors.up.isDown && this.selecionado > 0) {
            this.selecionado--;
            this.cursors.up.isDown = false;
            this.sound.play('cursor', {volume: 0.2});
        } else if (this.cursors.down.isDown && this.selecionado < 4) {
            this.selecionado++;
            this.cursors.down.isDown = false;
            this.sound.play('cursor', {volume: 0.2});
        }
    }

    /* Realiza a seleção dos pokemons individualmente, tanto para o jogador quanto
    para o inimigo */ 
    selecionar() {
        if (this.z.isDown) {
            this.sound.play('select', {volume: 0.2});
            
            if (this.jogador === '')
                this.jogador = this.pokemons[this.selecionado];
            else
                this.inimigo = this.pokemons[this.selecionado];

            this.z.isDown = false;
            this.selecionado = 0;
            
        }
    }

    /* Realiza a transição de cena para a entrada da batalha */ 
    batalhar() {
        if (this.inimigo !== '') {
            
            let data = {
                jogador: this.jogador,
                inimigo: this.inimigo
            };

            this.musica.pause();
            this.scene.start('Entrada', data);
        }
    }

    /* Escreve as frases de orientação no momento da escolha dos pokemons */ 
    escrever() {
        if (this.frase !== null)
            this.frase.destroy();

        if (this.jogador === '') {
            this.frase = this.add.text(
                10,
                10,
                'Selecione seu pokemon',
                this.config_frase
            );
        } else {
            this.frase = this.add.text(
                10,
                10,
                'Selecione o pokemon inimigo',
                this.config_frase
            );
        }
    }

    update() {
        /* Loop principal de execução da cena de seleção de pokemons */ 
        this.verificarBotoes();
        this.desenharPokemon();
        this.escrever();
        this.nomes.forEach(nome => nome.setColor('#ffffff')); 
        this.nomes[this.selecionado].setColor('#DE4242');
        this.selecionar();
        this.batalhar();
    }
}

export default Selecao;