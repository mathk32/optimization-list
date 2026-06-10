#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numVertices = 5;
        const int numFreqs = 5; 

        std::vector<std::pair<int, int>> edges = {
            {0, 1}, {0, 2}, {1, 2}, {3, 4}, {1, 3}
        };

        IloBoolVarArray y(env, numFreqs); 
        IloBoolVarArray x(env, numVertices * numFreqs);

        IloExpr objExpr(env);
        for (int c = 0; c < numFreqs; c++) {
            objExpr += y[c];
        }
        
        model.add(IloMinimize(env, objExpr));
        objExpr.end();

        for (int v = 0; v < numVertices; v++) {
            IloExpr sumFreq(env);
            for (int c = 0; c < numFreqs; c++) {
                sumFreq += x[v * numFreqs + c];
            }
            model.add(sumFreq == 1);
            sumFreq.end();
        }

        for (int v = 0; v < numVertices; v++) {
            for (int c = 0; c < numFreqs; c++) {
                model.add(x[v * numFreqs + c] <= y[c]);
            }
        }

        for (auto edge : edges) {
            int u = edge.first;
            int v = edge.second;
            for (int c = 0; c < numFreqs; c++) {
                model.add(x[u * numFreqs + c] + x[v * numFreqs + c] <= 1);
            }
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream()); 
        
        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Frequencies Needed: " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Frequency Assignment ---" << std::endl;
            
            for (int v = 0; v < numVertices; v++) {
                for (int c = 0; c < numFreqs; c++) {
                    if (cplex.getValue(x[v * numFreqs + c]) > 0.99) {
                        std::cout << "  Vertex " << (v + 1) << " -> Frequency " << (c + 1) << std::endl;
                    }
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
