#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

int main() {
    IloEnv env;
    
    try {
        IloModel model(env);
        
        int numIngredients = 6;
        
        IloNumArray price(env, numIngredients, 35.0, 30.0, 60.0, 50.0, 27.0, 22.0);

        IloNumArray vitA(env, numIngredients, 1.0, 0.0, 2.0, 2.0, 1.0, 2.0);
        IloNumArray vitC(env, numIngredients, 0.0, 1.0, 3.0, 1.0, 3.0, 2.0);
        
        double minVitA = 9.0;
        double minVitC = 19.0;

        IloNumVarArray x(env, numIngredients, 0.0, IloInfinity, ILOFLOAT);

        IloExpr objExpr(env);
        for (int i = 0; i < numIngredients; i++) {
            objExpr += price[i] * x[i];
        }
       
        model.add(IloMinimize(env, objExpr));
        objExpr.end();

        IloExpr exprA(env);
        IloExpr exprC(env);
        
        for (int i = 0; i < numIngredients; i++) {
            exprA += vitA[i] * x[i];
            exprC += vitC[i] * x[i];
        }
        
        model.add(exprA >= minVitA);
        model.add(exprC >= minVitC);
        
        exprA.end();
        exprC.end();

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream()); 
        
        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum cost: R$ " << cplex.getObjValue() << std::endl;
            
            std::cout << "Optimal diet composition:" << std::endl;
            for (int i = 0; i < numIngredients; i++) {
                
                if (cplex.getValue(x[i]) > 0.0001) { 
                    std::cout << "- Ingredient " << (i + 1) << ": " << cplex.getValue(x[i]) << " units" << std::endl;
                }
            }
        } else {
            std::cout << "No solution found." << std::endl;
        }
        std::cout << "------------------------------------------------" << std::endl;
        
    } catch (IloException& e) {
        std::cerr << "CPLEX Error: " << e << std::endl;
    }
    
    env.end();
    return 0;
}
