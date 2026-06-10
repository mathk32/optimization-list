#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numFactories = 3;
        const int numDepots = 3;

        double supply[] = {120.0, 80.0, 80.0};

        double demand[] = {150.0, 70.0, 60.0};

        double transportCost[] = {
             8.0,  5.0,  6.0,
            15.0, 10.0, 12.0,
             3.0,  9.0, 10.0
        };

        IloNumVarArray x(env, numFactories * numDepots, 0.0, IloInfinity, ILOFLOAT);

        IloExpr objExpr(env);
        for (int i = 0; i < numFactories; i++) {
            for (int j = 0; j < numDepots; j++) {
                objExpr += transportCost[i * numDepots + j] * x[i * numDepots + j];
            }
        }

        model.add(IloMinimize(env, objExpr));
        objExpr.end();

        for (int i = 0; i < numFactories; i++) {
            IloExpr supplyExpr(env);
            for (int j = 0; j < numDepots; j++) {
                supplyExpr += x[i * numDepots + j];
            }
            model.add(supplyExpr <= supply[i]);
            supplyExpr.end();
        }

        for (int j = 0; j < numDepots; j++) {
            IloExpr demandExpr(env);
            for (int i = 0; i < numFactories; i++) {
                demandExpr += x[i * numDepots + j];
            }
            model.add(demandExpr >= demand[j]);
            demandExpr.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Transportation Cost: R$ " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Optimal Routing Plan ---" << std::endl;

            for (int i = 0; i < numFactories; i++) {
                for (int j = 0; j < numDepots; j++) {
                    double amount = cplex.getValue(x[i * numDepots + j]);
                    if (amount > 0.0001) {
                        std::cout << "  Factory " << (i + 1) << " -> Depot " << (j + 1)
                                  << " : " << amount << " units (Cost: R$ "
                                  << (amount * transportCost[i * numDepots + j]) << ")" << std::endl;
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
