# Otimização Combinatória - Lista de Exercícios

Este repositório contém as soluções desenvolvidas para uma lista de exercícios de Otimização. Os problemas foram resolvidos utilizando uma meta-heurística própria e modelos exatos implementados com o solver IBM ILOG CPLEX.

## 📁 Estrutura do Repositório

```text
optimization_list/
├── optimization_list.pdf    # Lista dos exercícios 
├── Q1/                      # Questão 1: Meta-heurística para Bin Packing
│   ├── bin_packing.cpp      # Código principal
│   ├── Makefile
│   └── instance.txt         # Arquivo de entrada
│
└── Q2/                      # Questão 2: Modelos Exatos com CPLEX
    ├── cans.cpp
    ├── click.cpp
    ├── coverage.cpp
    ├── cvrp.cpp
    ├── diet.cpp
    ├── facility.cpp
    ├── farm.cpp
    ├── frequency.cpp
    ├── knapsack.cpp
    ├── maxflow.cpp
    ├── nurses.cpp
    ├── paint.cpp
    ├── ration.cpp
    ├── shortest.cpp
    ├── transport.cpp
    ├── tsp.cpp
    └── Makefile
```

---

## 📦 Questão 1 - Problema do Bin Packing (Meta-heurística)

A solução para o problema de Bin Packing foi implementada em C++ utilizando a meta-heurística **ILS (Iterated Local Search)** combinada com uma busca local baseada no critério *First Improvement*.

### Requisitos Atendidos

* **(a) Representação da Solução:** Estrutura baseada em `std::vector`, permitindo acesso rápido aos elementos.
* **(b) Função de Avaliação:** Maximização de $f(x) = \sum L_i^2$, favorecendo caixas mais preenchidas.
* **(c) Busca Local:** Movimentos aleatórios de *Relocate* e *Swap*, aceitando a primeira solução melhor encontrada.
* **(d) Critério de Parada:** Controle de tempo utilizando a biblioteca `<chrono>` e um limite definido pelo usuário.

### Como Compilar e Executar a Q1

Entre na pasta `Q1`:

```bash
cd Q1
```

**Compilar:**

```bash
make
```

**Executar utilizando o arquivo padrão:**

O script está configurado para ler os dados automaticamente a partir do arquivo `instance.txt`, utilizando um limite de 2 segundos.

```bash
make run
```

**Executar manualmente:**

Caso queira alterar o tempo limite ou utilizar outro arquivo de entrada:

```bash
./bin_packing <tempo_em_segundos> < <nome_do_arquivo>.txt
```

### Formato do Arquivo de Entrada (`.txt`)

O arquivo `instance.txt` deve conter:

1. O número total de itens na primeira linha.
2. O tamanho de cada item (entre 0.0 e 1.0) nas linhas seguintes.

---

## 🧮 Questão 2 - Modelos Exatos com CPLEX

Esta pasta reúne diferentes problemas de otimização modelados em C++ e resolvidos com o IBM ILOG CPLEX.

Os modelos implementados são:

* Problema de Transporte (`transport.cpp`)
* Caixeiro Viajante (`tsp.cpp`)
* Roteamento de Veículos Capacitado (`cvrp.cpp`)
* Caminho Mínimo (`shortest.cpp`)
* Fluxo Máximo (`maxflow.cpp`)
* Mochila (`knapsack.cpp`)
* Cobertura (`coverage.cpp`)
* Localização de Instalações (`facility.cpp`)
* Dieta (`diet.cpp`)
* Escalonamento de Enfermeiros (`nurses.cpp`)
* Frequência (`frequency.cpp`)
* Fazenda (`farm.cpp`)
* Racionamento (`ration.cpp`)
* Pintura (`paint.cpp`)
* Latas (`cans.cpp`)
* Cliques (`click.cpp`)

### Como Compilar e Executar a Q2

Entre na pasta `Q2`:

```bash
cd Q2
```

Para compilar um modelo específico:

```bash
make <nome_arquivo>
```

Depois execute:

```bash
./<nome_arquivo>
```

### Exemplos

**Caixeiro Viajante (TSP)**

```bash
make tsp
./tsp
```

**Roteamento de Veículos (CVRP)**

```bash
make cvrp
./cvrp
```

**Problema de Transporte**

```bash
make transport
./transport
```

**Caminho Mínimo**

```bash
make shortest
./shortest
```

**Fluxo Máximo**

```bash
make maxflow
./maxflow
```

### Limpeza dos Arquivos Compilados

Para remover os executáveis gerados e manter a pasta organizada:

```bash
make clean
```
