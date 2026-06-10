#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numFacilities = 3;
	const int numClients = 4;   

        double fixedCost[] = {1000.0, 1500.0, 1200.0};
        double transportCost[] = {
            10.0, 20.0, 30.0, 40.0, 
	    50.0, 10.0, 15.0, 25.0, 
	    20.0, 30.0, 10.0, 15.0};

        IloBoolVarArray y(env, numFacilities);
        IloBoolVarArray x(env, numFacilities * numClients);
        IloExpr objExpr(env);
        
        for (int i = 0; i < numFacilities; i++) {
            objExpr += fixedCost[i] * y[i];
        }
        
        for (int i = 0; i < numFacilities; i++) {
            for (int j = 0; j < numClients; j++) {
                objExpr += transportCost[i * numClients + j] * x[i * numClients + j];
            }
        }
        
        model.add(IloMinimize(env, objExpr));
        objExpr.end();

        for (int j = 0; j < numClients; j++) {
            IloExpr assignSum(env);
            for (int i = 0; i < numFacilities; i++) {
                assignSum += x[i * numClients + j];
            }
            model.add(assignSum == 1);
            assignSum.end();
        }

        for (int i = 0; i < numFacilities; i++) {
            for (int j = 0; j < numClients; j++) {
                model.add(x[i * numClients + j] <= y[i]);
            }
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream()); 
        
        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Total Cost: R$ " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Optimal Infrastructure Plan ---" << std::endl;
            
            for (int i = 0; i < numFacilities; i++) {
                if (cplex.getValue(y[i]) > 0.99) {
                    std::cout << "[OPEN] Facility " << i << " is active. Serving clients:" << std::endl;
                    for (int j = 0; j < numClients; j++) {
                        if (cplex.getValue(x[i * numClients + j]) > 0.99) {
                            std::cout << "  -> Client " << j << " (Transport Cost: R$ " 
                                      << transportCost[i * numClients + j] << ")" << std::endl;
                        }
                    }
                } else {
                    std::cout << "[CLOSED] Facility " << i << " remains closed." << std::endl;
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
