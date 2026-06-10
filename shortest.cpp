#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>
#include <cmath>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numNodes = 6;
        const int numEdges = 8;

        const int sourceNode = 0;
        const int targetNode = 5;

        int fromNode[] = {0, 0, 1, 1, 2, 3, 4, 4};
        int toNode[]   = {1, 2, 2, 3, 4, 5, 3, 5};
        double cost[]  = {2.0, 4.0, 1.0, 7.0, 3.0, 1.0, 2.0, 5.0};

        IloBoolVarArray x(env, numEdges);

        IloExpr objExpr(env);
        for (int e = 0; e < numEdges; e++) {
            objExpr += cost[e] * x[e];
        }

        model.add(IloMinimize(env, objExpr));
        objExpr.end();

        for (int i = 0; i < numNodes; i++) {
            IloExpr flowOut(env);
            IloExpr flowIn(env);

            for (int e = 0; e < numEdges; e++) {
                if (fromNode[e] == i) flowOut += x[e];
                if (toNode[e] == i) flowIn += x[e];
            }

            int rhs = 0;

            if (i == sourceNode) {
                rhs = 1;
            } else if (i == targetNode) {
                rhs = -1;
            }

            model.add(flowOut - flowIn == rhs);

            flowOut.end();
            flowIn.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Distance: " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Optimal Path ---" << std::endl;

            int currentNode = sourceNode;
            std::cout << "Path: " << currentNode;

            while (currentNode != targetNode) {
                for (int e = 0; e < numEdges; e++) {
                    if (fromNode[e] == currentNode && cplex.getValue(x[e]) > 0.99) {
                        currentNode = toNode[e];
                        std::cout << " -> " << currentNode << " (cost: " << cost[e] << ")";
                        break;
                    }
                }
            }

            std::cout << std::endl;

        } else {
            std::cout << "No Path Found." << std::endl;
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
