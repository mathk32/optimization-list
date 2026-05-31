#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        IloNumVarArray x(env, 8, 0.0, IloInfinity, ILOFLOAT);

        IloExpr cost(env);
        
        cost += 1.5 * (x[0] + x[4]); 
        cost += 1.0 * (x[1] + x[5]);
        cost += 4.0 * (x[2] + x[6]);
        cost += 6.0 * (x[3] + x[7]);
        model.add(IloMinimize(env, cost));
        cost.end();

        model.add(x[0] + x[1] + x[2] + x[3] == 1000.0);
        model.add(x[4] + x[5] + x[6] + x[7] == 250.0);  

        model.add(0.30 * x[0] + 0.60 * x[1] + 1.0 * x[2] >= 250.0);
        model.add(0.70 * x[0] + 0.40 * x[1] + 1.0 * x[3] >= 500.0); 

        model.add(0.30 * x[4] + 0.60 * x[5] + 1.0 * x[6] >= 50.0);
        model.add(0.70 * x[4] + 0.40 * x[5] + 1.0 * x[7] >= 125.0);

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "-------------------------------------------" << std::endl;
  
        if (cplex.solve()) {
            std::cout << "Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Cost: R$ " << cplex.getObjValue() << std::endl;
            
            std::cout << "\nTotal Purchases Required:" << std::endl;
            std::cout << "SolA: " << cplex.getValue(x[0]) + cplex.getValue(x[4]) << " liters" << std::endl;
            std::cout << "SolB: " << cplex.getValue(x[1]) + cplex.getValue(x[5]) << " liters" << std::endl;
            std::cout << "SEC puro: " << cplex.getValue(x[2]) + cplex.getValue(x[6]) << " liters" << std::endl;
            std::cout << "COR puro: " << cplex.getValue(x[3]) + cplex.getValue(x[7]) << " liters" << std::endl;
        } else {
            std::cout << "No Sotution Found." << std::endl;
        }
        std::cout << "------------------------------------------------" << std::endl;

    } catch (IloException& e) {
        std::cerr << "CPLEX Error: " << e << std::endl;
    }
    env.end();
    return 0;
}
