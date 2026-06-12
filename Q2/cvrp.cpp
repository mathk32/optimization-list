#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>
#include <cmath>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numNodes = 5;
        const int numVehicles = 2;
        const double capacity = 30.0;

        double demand[] = {0.0, 10.0, 15.0, 20.0, 10.0};

        double dist[] = {
              0.0,  10.0,  15.0,  20.0,  25.0,
             10.0,   0.0,  35.0,  25.0,  30.0,
             15.0,  35.0,   0.0,  30.0,  20.0,
             20.0,  25.0,  30.0,   0.0,  10.0,
             25.0,  30.0,  20.0,  10.0,   0.0
        };

        IloBoolVarArray x(env, numNodes * numNodes);

        IloNumVarArray u(env, numNodes, 0.0, capacity, ILOFLOAT);

        for (int i = 1; i < numNodes; i++) {
            u[i].setBounds(demand[i], capacity);
        }

        IloExpr objExpr(env);
        for (int i = 0; i < numNodes; i++) {
            for (int j = 0; j < numNodes; j++) {
                if (i != j) {
                    objExpr += dist[i * numNodes + j] * x[i * numNodes + j];
                }
            }
        }

        model.add(IloMinimize(env, objExpr));
        objExpr.end();

        for (int i = 1; i < numNodes; i++) {
            IloExpr outDegree(env);
            IloExpr inDegree(env);

            for (int j = 0; j < numNodes; j++) {
                if (i != j) {
                    outDegree += x[i * numNodes + j];
                    inDegree += x[j * numNodes + i];
                }
            }

            model.add(outDegree == 1);
            model.add(inDegree == 1);

            outDegree.end();
            inDegree.end();
        }

        double totalDemand = 0.0;
        for (int i = 1; i < numNodes; i++) {
            totalDemand += demand[i];
        }

        int minVehicles = (int)std::ceil(totalDemand / capacity);

        IloExpr leavesDepot(env);
        IloExpr entersDepot(env);

        for (int j = 1; j < numNodes; j++) {
            leavesDepot += x[0 * numNodes + j];
            entersDepot += x[j * numNodes + 0];
        }

        model.add(leavesDepot >= minVehicles);
        model.add(leavesDepot <= numVehicles);
        model.add(entersDepot == leavesDepot);

        leavesDepot.end();
        entersDepot.end();

        for (int i = 1; i < numNodes; i++) {
            for (int j = 1; j < numNodes; j++) {
                if (i != j) {
                    model.add(u[i] - u[j] + capacity * x[i * numNodes + j] <= capacity - demand[j]);
                }
            }
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Total Distance: " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Optimal Routes ---" << std::endl;

            int routeCount = 1;

            for (int j = 1; j < numNodes; j++) {
                if (cplex.getValue(x[0 * numNodes + j]) > 0.99) {
                    std::cout << "Vehicle " << routeCount << " Route: 0";

                    int currentNode = j;
                    double currentLoad = demand[j];

                    while (currentNode != 0) {
                        std::cout << " -> " << currentNode
                                  << " (load: " << currentLoad << ")";

                        for (int nextNode = 0; nextNode < numNodes; nextNode++) {
                            if (currentNode != nextNode &&
                                cplex.getValue(x[currentNode * numNodes + nextNode]) > 0.99) {
                                currentNode = nextNode;

                                if (currentNode != 0) {
                                    currentLoad += demand[currentNode];
                                }

                                break;
                            }
                        }
                    }

                    std::cout << " -> 0" << std::endl;
                    routeCount++;
                }
            }
        } else {
            std::cout << "No Solution Found." << std::endl;
        }

        std::cout << "------------------------------------------------" << std::endl;

    } catch (IloException& e) {
        std::cerr << "CPLEX Error: " << e << std::endl;
        env.end();
        return 1;
    }

    env.end();
    return 0;
}
