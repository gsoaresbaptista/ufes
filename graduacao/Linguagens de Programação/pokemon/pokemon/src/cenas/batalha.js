import Phaser from 'phaser';
import Botao from './ui/botao';

/* Enumerate para os estados do menu em batalha  */
const MENU = Object.freeze({
    PRINCIPAL: 0,
    LUTAR: 1,
    MENSAGEM_JOGADOR: 2,
    MENSAGEM_INIMIGO: 3,
    ACABOU: 4
});

/* Classe representando a cena de batalha(jogo propriamente dito), 
   onde o jogador irá atacar e receber os ataques até a batalha acabar */
export default class Batalha extends Phaser.Scene {
    constructor() {
        super();

        this.menu = MENU.PRINCIPAL;
        this.botao_selecionado = 0;
        this.msg = '';
        this.textoPP = '';
        this.nomes_ataques = [];
        this.encerrou = false;

        /* Configurações dos textos utilizados */
        this.config_nomes = {
            color: 'black',
            fontFamily: 'Pokemon',
            fontSize: 30
        };
        this.config_vida = {
            color: 'black',
            fontFamily: 'Pokemon',
            fontSize: 25
        };
        this.config_mensagens = {
            fontFamily: 'Joystix',
            fontSize: '20px',
            color: '#303030',
        };
        this.config_botoes = {
            fontFamily: 'Joystix',
            fontSize: '20px',
            color: '#303030',
            wordWrap: {
                width: 330,
                useAdvancedWrap: true
            }
        };
        this.config_ataques = {
            fontFamily: 'Joystix',
            fontSize: '17px',
            color: '#303030'
        };
    }

    preload() {
        this.load.path = "src/assets/";
        this.load.image('fundo', 'backgrounds/batalha.png');
        this.load.image('base_i', 'batalha/base_inimigo.png');
        this.load.image('base_j', 'batalha/base_jogador.png');
        this.load.image('hud_j', 'batalha/hud_jogador.png');
        this.load.image('hud_i', 'batalha/hud_inimigo.png');
        this.load.image('hud_botoes', 'batalha/menu_botoes.png');
        this.load.image('hud_lutar', 'batalha/base_ataques.png');
        this.load.image('hud', 'batalha/text_bar.png');

        /* Carrega os áudios */
        this.load.audio('cursor', 'sons/cursor.wav');
        this.load.audio('select', 'sons/select.ogg');
        this.load.audio('dano', 'sons/damage.ogg');

        /* Carrega os sprites */
        this.load.spritesheet('botoes', 'outros/botoes.png', {
            frameWidth: 195,
            frameHeight: 69
        });
        this.load.spritesheet('ataques', 'batalha/ataques.png', {
            frameWidth: 300,
            frameHeight: 71
        });
        this.load.spritesheet('vida', 'batalha/barra.png', {
            frameWidth: 150,
            frameHeight: 10
        });
    }

    /* Carrega e retorna a imagem do pokemon na posição correta (depende se
        é o pokemon do jogador ou inimigo) */
    loadPokemon(pokemon) {
        return this.add.image(
            pokemon.x - 100,
            pokemon.y,
            pokemon.chave
        ).setOrigin(0, 1);
    }

    /* Escreve o PP de um ataque na tela */
    escreverPP() {
         /* Apaga o texto caso já tenha algo escrito */
        if (this.textoPP != '')
            this.textoPP.destroy();

        let pp = this.pokemon_j.ataques[this.botao_selecionado].textoPP(),
            tipo = this.pokemon_j.ataques[this.botao_selecionado].tipo;

        this.textoPP = this.add.text(
            635, 495,
            `PP ${pp}\n\n${tipo}`,
            this.config_mensagens
        );
    }

    create(dados) {
         /* Pega os pokemons que vieram da cena anterior */
        this.pokemon_j = dados.jogador;
        this.pokemon_i = dados.inimigo;
        this.musica = dados.musica;

         /* Denha a cena de batalha */
        this.add.image(0, 0, 'fundo').setOrigin(0, 0);
        this.add.image(220, 400, 'base_j');
        this.add.image(600, 230, 'base_i');
        this.add.image(400, 300, 'hud_j').setOrigin(0, 0);
        this.add.image(350, 30, 'hud_i').setOrigin(1, 0);

         /* Carrega os pokemons */
        this.jogador = this.loadPokemon(dados.jogador);
        this.inimigo = this.loadPokemon(dados.inimigo);

         /* Adiciona os textos de batalha */
        this.add.text(80, 45, this.pokemon_i.nome, this.config_nomes);
        this.add.text(460, 315, this.pokemon_j.nome, this.config_nomes);
        this.hp_jogador = this.add.text(
            670, 392,
            this.pokemon_j.textoVida(),
            this.config_vida
        ).setOrigin(0.5, 0.5);

        /* Adiciona as barras de vida do jogador e do inimigo */
        this.tamanho_original_jogador = 150;
        this.tamanho_original_inimigo = 156;
        this.barra_jogador = this.add.sprite(684, 366, 'vida', 0);
        this.barra_inimigo = this.add.sprite(199, 100, 'vida', 0);
        this.barra_inimigo.displayWidth = 156; 
        this.barra_inimigo.displayHeight = 11;
        this.barra_jogador.displayHeight = 11;

        /* Mapeia as teclas Z, X e os cursores */
        this.cursors = this.input.keyboard.createCursorKeys();
        this.z = this.input.keyboard.addKey('Z');
        this.x = this.input.keyboard.addKey('X');

        this.botoes();
        this.ataques();
        this.menuBotoes();
    }

    /* Adiciona os quatro botões selecionáveis durante a batalha */
    botoes() {
        this.botoes = [
            new Botao(0, this, 'botoes', 400, 458, () => {
                this.botoes.forEach(botao => botao.setVisible(false));
                this.menuLutar();
            }),
            new Botao(4, this, 'botoes', 598, 458, () =>
                alert('Não implementado!')
            ),
            new Botao(2, this, 'botoes', 400, 528, () => 
                alert('Não implementado!')
            ),
            new Botao(6, this, 'botoes', 598, 528, () =>
                alert('Não implementado!')
            ),
        ];
    }

    /* Adiciona os quatro botões de ataque selecionáveis durante a escolha do ataque */
    ataques() {
        this.ataques = [
            new Botao(16, this, 'ataques', 9, 460, () => this.atacar(0)),
            new Botao(16, this, 'ataques', 303, 460, () => this.atacar(1)),
            new Botao(16, this, 'ataques', 9, 529, () => this.atacar(2)),
            new Botao(16, this, 'ataques', 303, 529, () => this.atacar(3)),
        ];
    }

    /* Realiza a parte gráfica do ataque entre pokemons */
    atacar(id) {
        this.menu = MENU.MENSAGEM_JOGADOR;
        this.botao_selecionado = 0;
        this.ataques.forEach(ataque => ataque.setVisible(false));
        this.nomes_ataques.forEach(ataque => ataque.setVisible(false));
        /* Caso o ataque tenha PP suficiente, é acionado e executado */
        if (this.pokemon_j.ataques[id].pp > 0) {
            let nome_ataque = this.pokemon_j.ataques[id].nome;
            this.menuTexto(`${this.pokemon_j.nome} utilizou ${nome_ataque}!`);
            this.pokemon_j.atacar(id, this.pokemon_i);
            this.animarAtaque(this.jogador, this.inimigo, 50);
            this.sound.play('dano', {volume: 0.2, delay: 0.130});
        }
    }

    /* Bloqueia o input do teclado por tempo determinado */
    bloquearEntrada(delay) {
        this.input.keyboard.enabled = false;
        this.time.addEvent({
            delay: delay,
            callback: () => this.input.keyboard.enabled = true
        });
    }

    /* Realiza a animação do uso de um ataque */
    animarAtaque(atacante, receptor, x = 50) {
        this.bloquearEntrada(500);

        this.tweens.add({
            targets: atacante,
            x: atacante.x + x,
            yoyo: true,
            ease: 'Power1',
            duration: 300,
        });

        this.tweens.add({
            targets: receptor,
            alpha: 0,
            yoyo: true,
            ease: 'Power1',
            delay: 150,
            duration: 150,
            repeat: 1
        });
    }

    /* Exibe os botões e o texto do momento de escolha da ação do jogador */
    menuBotoes() {
        this.add.image(0, 450, 'hud_botoes').setOrigin(0, 0);
        this.botoes.forEach(botao => {
            botao.desenhar();
            botao.setVisible(true);
        });
        this.add.text(
            40, 490,
            `O quê ${this.pokemon_j.nome} fará?`,
            this.config_botoes
        );
    }

    /* Exibe os ataques para que possam ser escolhidos pelo jogador */
    menuLutar() {
        let xcoords = [48, 338, 48, 338],
            ycoords = [485, 485, 555, 555];

        this.add.image(0, 450, 'hud_lutar').setOrigin(0, 0);
        this.ataques.forEach(ataque => {
            ataque.desenhar();
            ataque.setVisible(true);
        });

        for (let i = 0; i < 4; i++) {
            this.nomes_ataques.push(
                this.add.text(
                    xcoords[i], ycoords[i],
                    this.pokemon_j.ataques[i].nome,
                    this.config_ataques
                ).setDepth(1)
            );
        }

        this.menu = MENU.LUTAR;
    }

    /* Escreve mensagens de texto durante a batalha */
    menuTexto(msg) {
        if (this.msg != '')
            this.msg.destroy();

        this.add.image(0, 450, 'hud').setOrigin(0, 0);
        this.msg = this.add.text(
            40, // x
            480, // y
            msg,
            this.config_mensagens
        ).setDepth(1);
    }

    /* Verifica se um cursor foi apertado e atualiza o botão selecionado */
    verificarBotoes() {
        if (this.cursors.right.isDown)
            this.atualizarBotao([0, 1, 2, 3]);
        else if (this.cursors.left.isDown)
            this.atualizarBotao([1, 0, 3, 2]);
        else if (this.cursors.up.isDown)
            this.atualizarBotao([2, 0, 3, 1]);
        else if (this.cursors.down.isDown)
            this.atualizarBotao([0, 2, 1, 3]);
    }

    /* Atualiza a posição do botão selecionado, utilizando os índices como chave
    de troca, pois a posição atual pode mover apenas para outras posicões fixas,
    por exemplo, o botão superior esquerdo só pode mover para direita ou para
    baixo */
    atualizarBotao(posicoes) {
        if (this.botao_selecionado == posicoes[0])
            this.botao_selecionado = posicoes[1];
        if (this.botao_selecionado == posicoes[2])
            this.botao_selecionado = posicoes[3];
        /* */
        if (!this.encerrou) this.sound.play('cursor', {volume: 0.2});

        this.cursors.right.isDown = false;
        this.cursors.left.isDown = false;
        this.cursors.up.isDown = false;
        this.cursors.down.isDown = false;
    }

    /* Verifica o encerramento da batalha e exibe as mensagens caso tenha acontecido */
    acabou() {
        if (this.pokemon_j.hp == 0 || this.pokemon_i.hp == 0) {
            this.menu = MENU.ACABOU;

            if (this.pokemon_j.hp === 0)
                this.menuTexto('Você perdeu!');
            else if (this.pokemon_i.hp === 0)
                this.menuTexto('Você ganhou!');
 
            setTimeout(() => alert('Jogo encerrado, obrigado por jogar!'), 1200);

            this.encerrou = true;
        }
    }

    /* Troca o sprite da barra de vida de acordo com o hp do pokemon inimigo */
    atualizarBarraInimigo() {
        if (this.barra_inimigo.displayWidth <= .3 * this.tamanho_original_inimigo)
            this.barra_inimigo.setFrame(2);
        else if (this.barra_inimigo.displayWidth <= .6 * this.tamanho_original_inimigo)
            this.barra_inimigo.setFrame(1);
    }

    /* Troca o sprite da barra de vida de acordo com o hp do pokemon do jogador */
    atualizarBarraJogador() {
        if (this.barra_jogador.displayWidth <= .3 * this.tamanho_original_jogador)
            this.barra_jogador.setFrame(2);
        else if (this.barra_jogador.displayWidth <= .6 * this.tamanho_original_jogador)
            this.barra_jogador.setFrame(1);
    }

    /* Realiza a animação de descrescimento da vida do pokemon inimigo após sofrer um golpe */ 
    animarVidaInimigo() {
        let vida_percentual = this.pokemon_i.hp/this.pokemon_i.hpmax,
            calculo = this.tamanho_original_inimigo * vida_percentual;

        if (this.barra_inimigo.displayWidth > 1 && 
            this.barra_inimigo.displayWidth > calculo) {
            this.barra_inimigo.displayWidth -= 1;
            this.barra_inimigo.x -= .5;
        }
    }

    /* Realiza a animação de descrescimento da vida do pokemon do jogador após sofrer um golpe */ 
    animarVidaJogador() {
        let vida_percentual = this.pokemon_j.hp/this.pokemon_j.hpmax,
            calculo = this.tamanho_original_jogador * vida_percentual;
            
        if (this.barra_jogador.displayWidth > 1 && 
            this.barra_jogador.displayWidth > calculo) {
            this.barra_jogador.displayWidth -= 1;
            this.barra_jogador.x -= .5;
        }
    }

    /* Realiza o ataque do inimigo, selecionando um dos ataques do pokemon de maneira aleatória */
    receberAtaque() {
        let id = Math.ceil(Math.random() * 3),
        nome = this.pokemon_i.ataques[id].nome;
        this.menuTexto(`${this.pokemon_i.nome} utilizou ${nome}!`);
        this.pokemon_i.atacar(id, this.pokemon_j);
        this.animarAtaque(this.inimigo, this.jogador, -50);
        this.sound.play('dano', {volume: 0.2, delay: 0.130});
        this.hp_jogador.destroy();
        this.hp_jogador = this.add.text(
            640, 378,
            this.pokemon_j.textoVida(),
            this.config_vida
        );

        this.acabou();
        if (!this.encerrou)
            this.menu = MENU.MENSAGEM_INIMIGO;
    }

    /* Sai do menu de ataques para o principal redefinindo os textos e imagens */
    voltarMenu() {
        this.menu = MENU.PRINCIPAL;
        this.botao_selecionado = 0;
        this.ataques.forEach(ataque => ataque.setVisible(false));
        this.nomes_ataques.forEach(ataque => ataque.setVisible(false));
        this.menuBotoes();
    }

    update() {
        /* Trocar a posicao atual do botao */
        this.verificarBotoes();

        if (this.menu === MENU.PRINCIPAL && this.z.isDown) {
            /* Botão apertado */
            this.sound.play('select', {volume: 0.2});
            this.botoes[this.botao_selecionado].funcao();

        } else if (this.menu === MENU.LUTAR && this.z.isDown) {
            /* Ataque apertado */
            this.sound.play('select', {volume: 0.2});
            this.ataques[this.botao_selecionado].funcao();

        } else if (this.menu == 1 && this.x.isDown) {
            /* Voltando do menu de ataques para o de botões */
            this.sound.play('select', {volume: 0.2});
            this.voltarMenu();

        } else if (this.menu === MENU.PRINCIPAL) {
            /* Deixando apenas o botão selecionado em foco */
            this.botoes.forEach(botao => botao.redefinir());
            this.botoes[this.botao_selecionado].foco();

        } else if (this.menu === MENU.LUTAR) {
            /* Deixando apenas o ataque selecionado em foco */
            this.escreverPP();
            this.ataques.forEach(ataque => ataque.redefinir());
            this.ataques[this.botao_selecionado].foco();

        } else if (this.menu === MENU.MENSAGEM_JOGADOR && this.z.isDown) {
            /* Mensagem de ataque */
            this.sound.play('select', {volume: 0.2});
            if (!this.pokemon_i.morto())
                this.receberAtaque();
            else
                this.acabou();

        } else if (this.menu === MENU.MENSAGEM_INIMIGO && this.z.isDown) {
            /* Termina o ataque e volta para o menu de botões */   
            this.sound.play('select', {volume: 0.2});
            this.menu = MENU.PRINCIPAL;
            this.msg.destroy();
            this.menuBotoes();
        }

        /* Diminui as barras de vida devagar("Animação") */
        this.animarVidaInimigo();
        this.animarVidaJogador();

        /* Troca o sprite das barras dependendo da porcentem de vida */
        this.atualizarBarraInimigo();
        this.atualizarBarraJogador();

        this.z.isDown = false;
        this.x.isDown = false;
    }
}
