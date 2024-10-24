# Back-End do Projeto Themis

## Sobre

Este repositório contém o back-end do Projeto Themis, responsável por gerenciar a lógica de negócios, processamento de dados e comunicação com o modelo de linguagem de grande escala (LLM) aprimorado. Desenvolvido com Python e utilizando o framework Starlette, este back-end oferece uma base robusta e escalável para o projeto.

## Características

- **Arquitetura Limpa**: Segue os princípios da Clean Architecture para uma estrutura modular e manutenível.
- **Integração com LLM**: Utiliza a biblioteca `ctransformers` para integrar o modelo GPT quantizado.
- **Gerenciamento de Dados**: Responsável por processar e responder às consultas feitas através do front-end.

## Pré-requisitos

Antes de começar, é necessário ter instalado na máquina:
- Python >= `3.11`
- PostgreSQL >= `16.1`
- Poetry >= `1.7.1`

## Como Rodar o Projeto

Para executar o back-end do Themis, siga estas etapas:

1. Clone o repositório:
```bash
git clone https://github.com/gsoaresbaptista/themis-backend
```

2. Navegue até a pasta do projeto e instale as dependências:
```bash
cd themis-backend
poetry shell
poetry install
pip install torch
```

3. Configure o banco de dados PostgreSQL e ajuste o arquivo `.env` para as configurações do seu ambiente de execução.

4. Inicie o servidor:
```bash
task run
```


## Estrutura do Projeto

O projeto está organizado da seguinte maneira:
- `app`: Contém a lógica principal da aplicação e os endpoints da API.
- `models`: Define os modelos de dados e as interações com o banco de dados.
- `services`: Inclui os serviços para integração com o modelo LLM e outras funcionalidades.

## Contato

- Gabriel Soares Baptista - gsoaresbaptista@gmail.com
- Humberto Giuri Calente - humbertogiuri@gmail.com