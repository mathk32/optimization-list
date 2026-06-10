#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        const int numDays = 7;
        const char* dayNames[] = {"Seg", "Ter", "Qua", "Qui", "Sex", "Sab", "Dom"};

        double demand[] = {17.0, 13.0, 15.0, 19.0, 14.0, 16.0, 11.0};

        IloNumVarArray x(env, numDays, 0.0, IloInfinity, ILOINT);

        IloExpr objExpr(env);
        for (int j = 0; j < numDays; j++) {
            objExpr += x[j];
        }

        model.add(IloMinimize(env, objExpr));
        objExpr.end();

        for (int i = 0; i < numDays; i++) {
            IloExpr workersToday(env);

            for (int j = 0; j < numDays; j++) {
                if ((i - j + 7) % 7 < 4) {
                    workersToday += x[j];
                }
            }

            model.add(workersToday >= demand[i]);
            workersToday.end();
        }

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;
      
        if (cplex.solve()) {
            std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
            std::cout << "Minimum Nurses to Hire: " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Hiring and Shift Plan ---" << std::endl;

            for (int j = 0; j < numDays; j++) {
                if (cplex.getValue(x[j]) > 0.99) {
                    std::cout << "  -> Hire " << cplex.getValue(x[j])
                              << " nurses starting on " << dayNames[j] << std::endl;
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
