#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

int main() {
    IloEnv env;
    
    try {
        IloModel model(env);
        
        int numProducts = 2;
        
        IloNumArray profit(env, numProducts, 11.0, 12.0);
        
        IloNumArray cerealUsage(env, numProducts, 5.0, 2.0);
        double cerealLimit = 30000.0;
        
        IloNumArray meatUsage(env, numProducts, 1.0, 4.0);
        double meatLimit = 10000.0;

        IloNumVarArray x(env, numProducts, 0.0, IloInfinity, ILOFLOAT);

        IloExpr objExpr(env);
        for (int i = 0; i < numProducts; i++) {
            objExpr += profit[i] * x[i];
        }
        model.add(IloMaximize(env, objExpr));
        objExpr.end();

        IloExpr cerealExpr(env);
        IloExpr meatExpr(env);
        
        for (int i = 0; i < numProducts; i++) {
            cerealExpr += cerealUsage[i] * x[i];
            meatExpr += meatUsage[i] * x[i];
        }
        
        model.add(cerealExpr <= cerealLimit);
        model.add(meatExpr <= meatLimit);
        
        cerealExpr.end();
	meatExpr.end();
     
        IloCplex cplex(model);
        cplex.setOut(env.getNullStream()); 
        
        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution status: " << cplex.getStatus() << std::endl;
            std::cout << "Maximum profit: R$ " << cplex.getObjValue() << std::endl;
            
            std::cout << "Production plan:" << std::endl;
            std::cout << "- AMGS: " << cplex.getValue(x[0]) << " units" << std::endl;
            std::cout << "- RE: " << cplex.getValue(x[1]) << " units" << std::endl;
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
