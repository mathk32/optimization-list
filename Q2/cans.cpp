#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        int numPatterns = 4;

        double sheet1Limit = 200.0;
        double sheet2Limit = 90.0;
        double timeLimit = 100.0;

        double sheet1Use[] = {1.0, 0.0, 1.0, 1.0};
        double sheet2Use[] = {0.0, 1.0, 0.0, 0.0};
        double timeUse[]   = {2.0, 3.0, 2.0, 1.0};

        double bodiesGen[] = {1.0, 2.0, 0.0, 4.0};
        double lidsGen[]   = {7.0, 3.0, 9.0, 4.0};

        IloNumVarArray x(env, numPatterns, 0.0, IloInfinity, ILOINT);
        IloNumVar y(env, 0.0, IloInfinity, ILOINT);

        IloExpr bodies(env);
        IloExpr lids(env);

        for (int i = 0; i < numPatterns; i++) {
            bodies += bodiesGen[i] * x[i];
            lids += lidsGen[i] * x[i];
        }

        IloExpr objExpr(env);
        
        objExpr = 50 * y - 5 * (bodies - y) - 3 * (lids - 2 * y);
        
        model.add(IloMaximize(env, objExpr));
        objExpr.end();
        IloExpr sheet1Expr(env);

        for (int i = 0; i < numPatterns; i++) {
            sheet1Expr += sheet1Use[i] * x[i];
        }

        model.add(sheet1Expr <= sheet1Limit);
        sheet1Expr.end();

        IloExpr sheet2Expr(env);

        for (int i = 0; i < numPatterns; i++) {
            sheet2Expr += sheet2Use[i] * x[i];
        }

        model.add(sheet2Expr <= sheet2Limit);
        sheet2Expr.end();

        IloExpr timeExpr(env);

        for (int i = 0; i < numPatterns; i++) {
            timeExpr += timeUse[i] * x[i];
        }

        model.add(timeExpr <= timeLimit);
        timeExpr.end();

        model.add(y <= bodies);
        model.add(2 * y <= lids);

        bodies.end();
        lids.end();

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream()); 
        
        std::cout << "------------------------------------------------" << std::endl;
        
        if (cplex.solve()) {
            std::cout << "Solution status: " << cplex.getStatus() << std::endl;
            std::cout << "Maximum Profit: R$ " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Production Plan ---" << std::endl;
            
            for (int i = 0; i < numPatterns; i++) {
                std::cout << "  Pattern " << (i + 1) << " impressions: " << cplex.getValue(x[i]) << std::endl;
            }
            std::cout << "  Total Cans Produced (y): " << cplex.getValue(y) << std::endl;
            
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
