# Jogo de Truco em C

Este projeto é uma implementação em linguagem C do clássico jogo de cartas Truco. Ele simula as principais mecânicas do jogo, permitindo que os jogadores interajam, joguem cartas, e utilizem a funcionalidade de "pedir truco" para aumentar as apostas da rodada.

## Funcionalidades

O jogo oferece as seguintes funcionalidades principais:

- **Configuração de Partida**: Suporta partidas com 2, 4 ou 6 jogadores, organizados em duas equipes.

- **Gerenciamento de Cartas**: Realiza o embaralhamento e a distribuição aleatória das cartas, garantindo que não haja repetições. A carta "vira" da rodada também é definida.

- **Interação do Jogador**: Permite que os jogadores escolham qual carta de sua mão jogar e acompanhem as cartas que ainda possuem.

- **Mecânica de Truco**: Implementa as ações de "pedir truco", "aceitar truco" e "aumentar o valor do truco" (para 6, 9 ou 12 pontos).

- **Cálculo de Pontuação**: Acompanha e exibe as pontuações de cada time ao longo das rodadas e da partida completa.

- **Determinação do Vencedor**: Define o time vencedor de cada rodada e da partida final, aplicando as regras de comparação de cartas e as manilhas.

- **Interface de Texto**: Toda a interação é feita através de uma interface de linha de comando, exibindo a mão dos jogadores, a carta "vira" e as pontuações.

## Estrutura do Código

O projeto é modular e bem organizado, utilizando `structs` para representar `carta` e `jogador`, e diversas funções para gerenciar as diferentes etapas do jogo. As principais seções do código incluem:

- `main`: A função principal que coordena o fluxo do jogo.

- **Geração de Cartas:** Funções como `gerar_numero_aleatorio`, `sorteio_numero_carta`, `sorteio_naipe` e `criar_carta_aleatoria` lidam com a criação e atribuição de valores às cartas.

- **Configuração de Jogadores e Equipes:** `adicionar_nome_jogador` e `adicionar_equipe` são responsáveis por configurar os participantes.

- **Exibição de Informações:** `exibir_carta`, `exibir_mao`, `exibir_time`, `exibir_ganhador`, `exibir_pontuacao_final` e `exibir_pontuacao_partida` cuidam da apresentação visual do jogo.

- **Lógica de Rodada:** A função `rodada_truco` encapsula a lógica central de cada rodada do jogo.

- **Interação com Cartas:** `pedir_carta_jogar`, `retirar_carta_jogada` e `jogar_carta` gerenciam as ações dos jogadores relacionadas às suas cartas.

- **Ações de Truco:** `pedir_truco`, `aceitar_truco` e `aumentar_truco` implementam a dinâmica de aposta do jogo.

- **Tomada de Decisão do Jogador:** `escolher_acao` permite que o jogador selecione entre jogar uma carta ou pedir truco.

- **Distribuição e Manilha:** `embaralhar` distribui as cartas e define a carta "vira", enquanto `comparar_cartas` define a hierarquia das cartas, incluindo as manilhas.

- **Auxiliares:** Funções como `troca_repetida`, `eh_repetida`, `zerar_cartas_em_jogo` e `cartas_iguais` garantem a integridade do baralho e a lógica de jogo.

## Como Compilar e Rodar

Para compilar e executar o jogo, siga os passos abaixo:

1. **Salve o código:** Salve o código fornecido em um arquivo `.c` (ex: `truco.c`).

2. **Compile:** Utilize um compilador C (como GCC) no seu terminal:

```markdown
gcc truco.c -o truco
```

3. **Execute:** Após a compilação, execute o programa:

```markdown
./truco
```

O jogo irá então solicitar o número de jogadores e os nomes para iniciar a partida.

## Autores

Este projeto foi desenvolvido por:

- **Gabriel Veríssimo**

- **Hugo Aparecido Gregorio de Almeida (RA: 844917)**

- **João Vitor Antoni**

- **Rafael Massaro Dias**
