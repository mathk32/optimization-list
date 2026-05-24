#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

int main() {

    IloEnv env; 
    
    try {

        IloModel model(env);
        
        int numItems = 5;
        double capacity = 10.0;
        
        IloNumArray weights(env, numItems, 8.0, 3.0, 6.0, 4.0, 2.0);
        IloNumArray values(env, numItems, 100.0, 60.0, 70.0, 15.0, 15.0);
        
        std::cout << "Structure created." << std::endl;
        std::cout << "Backpack capacity: " << capacity << std::endl;

	IloBoolVarArray x(env, numItems);

	std::cout << "Decision variables created." << std::endl;

        IloExpr objExpr(env);

        for (int i = 0; i < numItems; i++) {
            objExpr += values[i] * x[i];
        }

        model.add(IloMaximize(env, objExpr));

        objExpr.end();

        std::cout << "Objective function added." << std::endl;

        IloExpr weightExpr(env);

        for (int i = 0; i < numItems; i++) {
            weightExpr += weights[i] * x[i];
        }
      
        model.add(weightExpr <= capacity);
        weightExpr.end();

	std::cout << "Capacity restriction added" << std::endl;

        IloCplex cplex(model);

        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;

        if (cplex.solve()) {
		
	    std::cout << "Sulution status: " << cplex.getStatus() << std::endl;
            std::cout << "Maximum value (Total profit): " << cplex.getObjValue() << std::endl;

            std::cout << "Added items: ";

            for (int i = 0; i < numItems; i++) {
      
                if (cplex.getValue(x[i]) > 0.99) {
                    std::cout << "[" << i << "] ";
                }
            }
            std::cout << std::endl;
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
