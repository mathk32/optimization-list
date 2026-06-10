#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numNodes = 8;
        const int numEdges = 15;
        const char* nodeNames[] = {"s", "v1", "v2", "v3", "v4", "v5", "v6", "t"};

        int fromNode[] = {0, 0, 0, 1, 1, 1, 2, 2, 3, 3, 4, 5, 5, 6, 6};
        int toNode[] = {1, 2, 3, 2, 4, 5, 3, 5, 5, 6, 7, 4, 7, 5, 7};
        double capacity[] = {5.0, 4.0, 6.0, 4.0, 6.0, 5.0, 3.0, 4.0, 6.0, 5.0, 5.0, 5.0, 3.0, 7.0, 6.0};

        int sourceNode = 0;
        int sinkNode = 7;

        IloNumVarArray x(env, numEdges, 0.0, IloInfinity, ILOFLOAT);

        IloExpr objExpr(env);
        for (int e = 0; e < numEdges; e++) {
            if (fromNode[e] == sourceNode) {
                objExpr += x[e];
            }
        }

        model.add(IloMaximize(env, objExpr));
        objExpr.end();

        for (int e = 0; e < numEdges; e++) {
            model.add(x[e] <= capacity[e]);
        }

        for (int i = 0; i < numNodes; i++) {
            if (i == sourceNode || i == sinkNode) continue;

            IloExpr flowIn(env);
            IloExpr flowOut(env);

            for (int e = 0; e < numEdges; e++) {
                if (toNode[e] == i) flowIn += x[e];
                if (fromNode[e] == i) flowOut += x[e];
            }

            model.add(flowIn == flowOut);

            flowIn.end();
            flowOut.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Maximum Gas Flow to Factory 't': " << cplex.getObjValue() << " units" << std::endl;
            std::cout << "\n--- Optimal Pipeline Routing ---" << std::endl;

            for (int e = 0; e < numEdges; e++) {
                double flow = cplex.getValue(x[e]);
                if (flow > 0.0001) {
                    std::cout << "  " << nodeNames[fromNode[e]] << " -> " << nodeNames[toNode[e]]
                              << " : " << flow << " / " << capacity[e] << " units" << std::endl;
                }
            }
        } else {
            std::cout << "No Solution Found." << std::endl;
        }

        std::cout << "------------------------------------------------" << std::endl;

    } catch (IloException& e) {
        std::cerr << "CPLEX Error: " << e << std::endl;
    }

    env.end();
    return 0;
}
