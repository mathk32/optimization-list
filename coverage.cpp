#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numNeighborhoods = 6;

        std::vector<std::vector<int>> adj(numNeighborhoods);

        adj[0] = {1, 2};
        adj[1] = {0, 3};
        adj[2] = {0, 3};
        adj[3] = {1, 2, 4};
        adj[4] = {3, 5};
        adj[5] = {4};

        IloBoolVarArray x(env, numNeighborhoods);

        IloExpr objExpr(env);
        for (int i = 0; i < numNeighborhoods; i++) {
            objExpr += x[i];
        }

        model.add(IloMinimize(env, objExpr));
        objExpr.end();

        for (int i = 0; i < numNeighborhoods; i++) {
            IloExpr coverExpr(env);

            coverExpr += x[i];

            for (int neighbor : adj[i]) {
                coverExpr += x[neighbor];
            }

            model.add(coverExpr >= 1);
            coverExpr.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;
      
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Schools to Build: " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Construction Plan ---" << std::endl;

            for (int i = 0; i < numNeighborhoods; i++) {
                if (cplex.getValue(x[i]) > 0.99) {
                    std::cout << "  -> Build school in Neighborhood " << i << std::endl;
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
