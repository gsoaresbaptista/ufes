# Front-End do Projeto Themis

## Sobre

Este repositório contém o front-end do Projeto Themis, uma interface gráfica interativa desenvolvida para facilitar a interação com um modelo de linguagem de grande escala (LLM) aprimorado com dados jurídicos brasileiros. Utilizando React e TypeScript, esta interface oferece uma experiência de usuário simplificada e eficiente para formular questões jurídicas e receber respostas baseadas na legislação brasileira e nas provas da OAB.

## Características

- **Design Intuitivo**: Interface fácil de usar, adequada para profissionais e estudantes da área jurídica.
- **Tecnologia Moderna**: Desenvolvida com React e TypeScript, garantindo uma aplicação robusta e escalável.
- **Integração com o Back-End**: Comunicação fluida com o back-end para processamento de perguntas e geração de respostas.

## Pré-requisitos

Antes de começar, certifique-se de ter instalado:
- Node.js
- npm (Node Package Manager)

## Como Rodar o Projeto

O projeto necessita que a máquina tenha instalado o npm para executar o front-end. Esse trabalho utilizou a versão `10.2.5` do npm.

Para iniciar o front-end do Themis, siga estas etapas:

1. Clone o repositório:
```bash
git clone https://github.com/gsoaresbaptista/themis-frontend
```

2. Navegue até a pasta do projeto e instale as dependências:
```bash
cd themis-frontend
npm install
``` 

3. Inicie o servidor de desenvolvimento:
```bash
npm run dev
``` 

Isso abrirá a interface do Themis no seu navegador padrão.

## Estrutura do Projeto

O projeto está organizado da seguinte maneira:
- `src`: Contém todos os componentes React, stylesheets e lógica da aplicação.
- `public`: Arquivos públicos como ícones, imagens e o `index.html`.


# Processo de Login no Front-End do Projeto Themis

## Visão Geral

O processo de login no front-end do Projeto Themis é projetado para oferecer segurança e eficiência. Ele gerencia automaticamente os tokens de acesso (AccessToken) e de atualização (RefreshToken), garantindo uma autenticação segura e uma experiência de usuário contínua.

## Fluxo do Login

### 1. Página de Login
Os usuários são recebidos com uma interface de login, onde inserem suas credenciais (e-mail e senha).

### 2. Autenticação
As credenciais são enviadas ao back-end para verificação.

### 3. Geração de Tokens
Após a validação bem-sucedida das credenciais, o back-end gera dois tipos de tokens:
- **AccessToken**: Token de curta duração para autenticação da sessão.
- **RefreshToken**: Token de longa duração usado para renovar o AccessToken.

## Gestão de Tokens

- **Armazenamento Seguro**: Os tokens são armazenados de forma segura no cliente. O AccessToken é mantido na memória da aplicação para acesso rápido, enquanto o RefreshToken é armazenado em um local mais seguro, como cookies HTTP-only.
- **Uso do AccessToken**: Este token é utilizado para autenticar requisições do usuário ao back-end, sendo enviado no cabeçalho de autorização das requisições HTTP.
- **Renovação Automática**: Quando o AccessToken expira, o sistema solicita um novo usando o RefreshToken, de forma automática e transparente para o usuário.
- **Logout e Revogação de Tokens**: Ao fazer logout, ambos os tokens são revogados para encerrar a sessão de forma segura.

# Interação do Usuário com o Chat

## Funcionalidades do Chat

O chat interativo é uma das principais características do front-end do Projeto Themis. Ele permite que os usuários interajam diretamente com o modelo de linguagem aprimorado, enviando perguntas jurídicas e recebendo respostas baseadas na legislação brasileira e dados da OAB.

### Como Funciona

1. **Acesso ao Chat**: Após o login, os usuários acessam a interface do chat, onde podem começar a interagir com o sistema.
2. **Envio de Perguntas**: Os usuários digitam suas perguntas jurídicas na caixa de texto e as enviam.
3. **Processamento das Perguntas**: As perguntas são enviadas ao back-end, onde são processadas pelo modelo de linguagem.
4. **Recebimento de Respostas**: O modelo gera uma resposta com base nos dados com os quais foi treinado, e a resposta é exibida no chat para o usuário.

### Características do Chat

- **Interface Intuitiva**: A interface do chat é projetada para ser simples e intuitiva, permitindo que os usuários façam perguntas e vejam respostas facilmente.
- **Histórico de Conversas**: O histórico de conversas é mantido, permitindo que os usuários revisitem suas perguntas e respostas anteriores.
- **Respostas Contextualizadas**: As respostas fornecidas são baseadas em um modelo treinado especificamente em textos e questões jurídicas, garantindo respostas relevantes e contextualizadas.

## Segurança e Privacidade

- **Proteção de Dados**: Todas as interações no chat são protegidas e criptografadas para garantir a segurança e a privacidade dos usuários.
- **Conformidade com Normas de Privacidade**: O chat segue as normas e leis de privacidade, garantindo que as informações dos usuários sejam tratadas de forma responsável.

# Interação do Usuário com o Chat

## Funcionalidades do Chat

O chat interativo é uma das principais características do front-end do Projeto Themis. Ele permite que os usuários interajam diretamente com o modelo de linguagem aprimorado, enviando perguntas jurídicas e recebendo respostas baseadas na legislação brasileira e dados da OAB.

### Como Funciona

1. **Acesso ao Chat**: Após o login, os usuários acessam a interface do chat, onde podem começar a interagir com o sistema.
2. **Envio de Perguntas**: Os usuários digitam suas perguntas jurídicas na caixa de texto e as enviam.
3. **Processamento das Perguntas**: As perguntas são enviadas ao back-end, onde são processadas pelo modelo de linguagem.
4. **Recebimento de Respostas**: O modelo gera uma resposta com base nos dados com os quais foi treinado, e a resposta é exibida no chat para o usuário.

### Características do Chat

- **Interface Intuitiva**: A interface do chat é projetada para ser simples e intuitiva, permitindo que os usuários façam perguntas e vejam respostas facilmente.
- **Histórico de Conversas**: O histórico de conversas é mantido, permitindo que os usuários revisitem suas perguntas e respostas anteriores.
- **Respostas Contextualizadas**: As respostas fornecidas são baseadas em um modelo treinado especificamente em textos e questões jurídicas, garantindo respostas relevantes e contextualizadas.

## Contato

- Gabriel Soares Baptista - gsoaresbaptista@gmail.com
- Humberto Giuri Calente - humbertogiuri@gmail.com