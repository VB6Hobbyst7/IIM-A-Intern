/************************************************************************************
 * This example is an example on IP from Application oriented Guide to LR by Fisher, Interfaces, 1985.
 * Maximize 16X1 + 10X2 +      4X4
 * s.t.: 
 *			8X1 + 2X2   + X3 + 4X4 <= 10
 *			 X1 +  X2              <= 1
 *				          X3 +  X4 <= 1
 * 0 <= X1, X2, X3, X4 <= 1 integers (binaries)
 ***********************************************************************************/
#include<stdio.h>
#include<conio.h>
#include<iostream>
#include<fstream>
#include<iosfwd>
#include<string>
#include <deque>
#include <sstream>
#include <time.h>
//#include <iomanip.h>
#include <stdlib.h>
#include <vector>//for vectors
#include <math.h>

//#include <cplex.h>
#include <ilcplex/ilocplex.h>
//#include <iloconcert/ilomodel.h>
//#include <iloconcert/iloenv.h>
#include <ilconcert/ilosys.h>

using namespace std;

ILOSTLBEGIN

//typedef IloArray<IloNumArray> TwoDMatrix;

int main(int argc, char **argv) 
{
	IloEnv env;
	try 
	{
		IloNumVarArray X(env, 4, 0, 1, ILOINT);
		IloNumArray X_val(env, 4);
		IloModel model(env);
		IloExpr Obj(env);
		Obj = 16*X[0]+10*X[1]+4*X[3];
		//model.add(IloMaximize(env,Obj)); // IloMinimize is used for minimization problems
		//Alternatively, model.add(IloMaximize(env,Obj)); can be replaced by the following three lines.
		//This is useful while iterating in Decomposition Techniques where the objective function is redefined at each iteration
		IloObjective Objective = IloMaximize(env); 
		model.add(Objective); 
		Objective.setExpr(Obj);

		Obj.end();
		model.add(8*X[0]+2*X[1]+X[2]+4*X[3] <= 10);
		model.add(X[0]+X[1] <= 1);
		model.add(X[2]+X[3] <= 1);
		// Optimize
		IloCplex cplex(model);
		//cplex.setOut(env.getNullStream()); // if we get an empty stream in return
		//cplex.setWarning(env.getNullStream());
		cplex.solve();//solving the MODEL
		if (cplex.getStatus() == IloAlgorithm::Infeasible) // if the problem is infeasible
		{
			env.out() << "Problem Infeasible" << endl; 
		}
		cplex.getValues(X_val, X);
		// Print results
		cout<< "Objective Value = " << cplex.getObjValue() << endl;
		cout<<"X = "<<X_val<<endl;
	}
	catch(IloException &e) 
	{
		env.out() << "ERROR: " << e << endl;
	}
	catch(...)
	{
		env.out() << "Unknown exception" << endl;
	}
	env.end();
	return 0;
}