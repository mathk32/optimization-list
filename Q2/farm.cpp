#include <ilcplex/ilocplex.h>
#include <iostream>
#include <vector>

ILOSTLBEGIN

int main() {
    IloEnv env;
    try {
        IloModel model(env);

        int numFarms = 3;
        int numCrops = 3;

        double areaFarm[] = {400.0, 650.0, 350.0};
        double waterFarm[] = {1800.0, 2200.0, 950.0};

        double maxCropArea[] = {660.0, 880.0, 400.0};
        double waterCrop[] = {5.5, 4.0, 3.5};
        double profitCrop[] = {5000.0, 4000.0, 1800.0};

        IloNumVarArray x(env, numFarms * numCrops, 0.0, IloInfinity, ILOFLOAT);

        IloExpr objExpr(env);
        for (int i = 0; i < numFarms; i++) {
            for (int j = 0; j < numCrops; j++) {
                objExpr += profitCrop[j] * x[i * numCrops + j];
            }
        }
        model.add(IloMaximize(env, objExpr));
        objExpr.end();

        for (int i = 0; i < numFarms; i++) {
            IloExpr farmArea(env);
            IloExpr farmWater(env);

            for (int j = 0; j < numCrops; j++) {
                farmArea += x[i * numCrops + j];
                farmWater += waterCrop[j] * x[i * numCrops + j];
            }

            model.add(farmArea <= areaFarm[i]);
            model.add(farmWater <= waterFarm[i]);

            farmArea.end();
            farmWater.end();
        }

        for (int j = 0; j < numCrops; j++) {
            IloExpr cropTotalArea(env);

            for (int i = 0; i < numFarms; i++) {
                cropTotalArea += x[i * numCrops + j];
            }

            model.add(cropTotalArea <= maxCropArea[j]);

            cropTotalArea.end();
        }

        model.add(650.0 * x[0] - 400.0 * x[3] == 0);
        model.add(350.0 * x[0] - 400.0 * x[6] == 0);

        model.add(650.0 * x[1] - 400.0 * x[4] == 0);
        model.add(350.0 * x[1] - 400.0 * x[7] == 0);

        model.add(650.0 * x[2] - 400.0 * x[5] == 0);
        model.add(350.0 * x[2] - 400.0 * x[8] == 0);

        IloCplex cplex(model);
        cplex.setOut(env.getNullStream());

        std::cout << "------------------------------------------------" << std::endl;

        if (cplex.solve()) {

            std::cout << "Solution status: " << cplex.getStatus() << std::endl;
            std::cout << "Maximum Profit: R$ " << cplex.getObjValue() << std::endl;
            std::cout << "\n--- Planting Plan (Acres) ---" << std::endl;

            const char* cropNames[] = {"Corn", "Rice", "Beans"};

            for (int i = 0; i < numFarms; i++) {

                std::cout << "Farm " << (i + 1) << ":" << std::endl;

                double totalFarmArea = 0.0;

                for (int j = 0; j < numCrops; j++) {

                    double val = cplex.getValue(x[i * numCrops + j]);

                    if (val > 0.0001) {
                        std::cout << "  - " << cropNames[j]
                                  << ": " << val << std::endl;
                    }

                    totalFarmArea += val;
                }

                std::cout << "  (Total Area Used: "
                          << totalFarmArea
                          << " / "
                          << areaFarm[i]
                          << " acres)"
                          << std::endl;
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
