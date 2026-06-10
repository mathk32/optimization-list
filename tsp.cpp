#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numNodes = 5;

        double dist[] = {
              0.0,  10.0,  15.0,  20.0,  25.0,
             10.0,   0.0,  35.0,  25.0,  30.0,
             15.0,  35.0,   0.0,  30.0,  20.0,
             20.0,  25.0,  30.0,   0.0,  10.0,
             25.0,  30.0,  20.0,  10.0,   0.0
        };

        IloBoolVarArray x(env, numNodes * numNodes);

        IloNumVarArray u(env, numNodes, 1.0, (double)(numNodes - 1), ILOFLOAT);

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

        for (int i = 0; i < numNodes; i++) {
            IloExpr outDegree(env);
            for (int j = 0; j < numNodes; j++) {
                if (i != j) outDegree += x[i * numNodes + j];
            }
            model.add(outDegree == 1);
            outDegree.end();
        }

        for (int j = 0; j < numNodes; j++) {
            IloExpr inDegree(env);
            for (int i = 0; i < numNodes; i++) {
                if (i != j) inDegree += x[i * numNodes + j];
            }
            model.add(inDegree == 1);
            inDegree.end();
        }

        for (int i = 1; i < numNodes; i++) {
            for (int j = 1; j < numNodes; j++) {
                if (i != j) {
                    model.add(u[i] - u[j] + numNodes * x[i * numNodes + j] <= numNodes - 1);
                }
            }
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;
       
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Tour Distance: " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Optimal Tour ---" << std::endl;

            int currentNode = 0;
            std::cout << "Path: 0";

            for (int step = 0; step < numNodes - 1; step++) {
                for (int nextNode = 0; nextNode < numNodes; nextNode++) {
                    if (currentNode != nextNode && cplex.getValue(x[currentNode * numNodes + nextNode]) > 0.99) {
                        std::cout << " -> " << nextNode;
                        currentNode = nextNode;
                        break;
                    }
                }
            }

            std::cout << " -> 0" << std::endl;

        } else {
            std::cout << "No feasible solution found." << std::endl;
        }

        std::cout << "------------------------------------------------" << std::endl;

    } catch (IloException& e) {
        std::cerr << "CPLEX Error: " << e << std::endl;
        env.end();
    }

    env.end();
    return 0;
}
