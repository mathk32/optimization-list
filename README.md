# Combinatorial Optimization - Exercise List

This repository contains solutions developed for a Combinatorial Optimization exercise list. The problems were solved using both a custom metaheuristic and exact optimization models implemented with IBM ILOG CPLEX.

## 📁 Repository Structure

```text
├── optimization_list.pdf
├── Q1/                      # Question 1: Bin Packing Metaheuristic
│   ├── bin_packing.cpp      # Main source code
│   ├── Makefile
│   └── instance.txt         # Input file
│
└── Q2/                      # Question 2: Exact Models with CPLEX
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

## 📦 Question 1 - Bin Packing Problem (Metaheuristic)

The Bin Packing problem was implemented in C++ using the **ILS (Iterated Local Search)** metaheuristic combined with a *First Improvement* local search strategy.

### Requirements Addressed

* **(a) Solution Representation:** Structure based on `std::vector`, allowing fast access to elements.
* **(b) Evaluation Function:** Maximization of $f(x) = \sum L_i^2$, encouraging bins to be more filled.
* **(c) Local Search:** Random *Relocate* and *Swap* moves, accepting the first improving solution found.
* **(d) Stopping Criterion:** Time-based control using the `<chrono>` library and a user-defined time limit.

### Compiling and Running Q1

Navigate to the `Q1` directory:

```bash
cd Q1
```

**Compile:**

```bash
make
```

**Run using the default input file:**

The script is configured to automatically read data from `instance.txt` using a time limit of 2 seconds.

```bash
make run
```

**Run manually:**

To change the time limit or use a different input file:

```bash
./bin_packing <time_limit_in_seconds> < <input_file>.txt
```

### Input File Format (`.txt`)

The `instance.txt` file must contain:

1. The total number of items on the first line.
2. The size of each item (between 0.0 and 1.0) on the following lines.

---

## 🧮 Question 2 - Exact Models with CPLEX

This directory contains several optimization problems modeled in C++ and solved using IBM ILOG CPLEX.

Implemented models include:

* Transportation Problem (`transport.cpp`)
* Traveling Salesman Problem (TSP) (`tsp.cpp`)
* Capacitated Vehicle Routing Problem (CVRP) (`cvrp.cpp`)
* Shortest Path Problem (`shortest.cpp`)
* Maximum Flow Problem (`maxflow.cpp`)
* Knapsack Problem (`knapsack.cpp`)
* Coverage Problem (`coverage.cpp`)
* Facility Location Problem (`facility.cpp`)
* Diet Problem (`diet.cpp`)
* Nurse Scheduling Problem (`nurses.cpp`)
* Frequency Assignment Problem (`frequency.cpp`)
* Farm Planning Problem (`farm.cpp`)
* Ration Problem (`ration.cpp`)
* Paint Problem (`paint.cpp`)
* Cans Problem (`cans.cpp`)
* Clique Problem (`click.cpp`)

### Compiling and Running Q2

Navigate to the `Q2` directory:

```bash
cd Q2
```

To compile a specific model:

```bash
make <file_name>
```

Then run the generated executable:

```bash
./<file_name>
```

### Examples

**Traveling Salesman Problem (TSP)**

```bash
make tsp
./tsp
```

**Capacitated Vehicle Routing Problem (CVRP)**

```bash
make cvrp
./cvrp
```

**Transportation Problem**

```bash
make transport
./transport
```

**Shortest Path Problem**

```bash
make shortest
./shortest
```

**Maximum Flow Problem**

```bash
make maxflow
./maxflow
```

### Cleaning Generated Files

To remove compiled executables and keep the directory organized:

```bash
make clean
```
