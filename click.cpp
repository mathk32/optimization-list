#include <ilcplex/ilocplex.h>
#include <iostream>

ILOSTLBEGIN

int main(){

	IloEnv env;

	try{
		IloModel model(env);

		int numVertices = 5;

		IloBoolVarArray x(env, numVertices);

		IloExpr objExpr(env);
		for (int i = 0; i < numVertices; i++){
			objExpr += x[i];
		
		}
		model.add(IloMaximize(env,objExpr));
		objExpr.end();


		model.add(x[0] + x[3] <= 1);
		model.add(x[0] + x[4] <= 1);
		model.add(x[1] + x[4] <= 1);
		model.add(x[2] + x[3] <= 1);


		IloCplex cplex(model);
		cplex.setOut(env.getNullStream());

		std::cout << "------------------------------------------" << std::endl;

		if(cplex.solve()){
			
			std::cout << "Solution Status: " << cplex.getStatus() << std::endl;
			std::cout << "Maximum Click Size: " << cplex.getObjValue() << " vertices" << std::endl;

			std::cout << "Vertices in the Maximum Click: ";
			for (int i = 0; i < numVertices; i++){
				if (cplex.getValue(x[i]) > 0.99) {
					std::cout << "[" << (i + 1) << "]";

				
				}
			
			}
			std::cout << std::endl;
		
		}else{
			std::cout << "No Solution Found." << std::endl;
		
		}
		std::cout << "-----------------------------------------------" << std::endl;

	}catch (IloException& e){
		std::cerr << "CPLEX Error" << e << std::endl;
	}

	env.end();
	return 0;



}
