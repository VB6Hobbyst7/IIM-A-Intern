/************************************************************************************
 *Classic Travelling Salesman Problem with imporved SECs
 *6 employees, 1 Office, minimum Car-Pool distance.
 *Author : meetshah1995
 * Min Sum[i,j](Cij*Xij) s.t.
 *		Sum[i,i!=j](Xij)=1
 *		Sum[j,i!=j](Xij)
 *		Ui-Uj+n*Xij <= n-1
 *		0<=Xij<=1
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
ifstream in;

IloArray<IloNumArray> distance(env);
for (int i = 0; i < 7; i++)
{
	distance[i] = IloNumVarArray(env,7,0,1,ILOINT);
}

void takeData(String file)
{
	String line;
	in.open(file,ios:in);
	for(int i = 0; i < 7;i++)
	{
		in >> line;
		for(int j =0; j < 14; j = j+2)
		{	
			distance[i][j/2] = int(line[j]);
			cout << distance[i][j/2] << endl;
		}
	}
	in.close();
}

int main(int argc, char **argv) 
{
	IloEnv env;
	try 
	{
		//Model Initialization
		IloModel model(env);

		//Taking Data from Distance Data File
		takeData("carpool-data.txt");

		//2D Array for Xij
		IloArray<IloNumVarArray> X(env, 7);
		for (int i = 0; i < 7; i++)
		{
			X[i] = IloNumVarArray(env,7,0,1,ILOINT);
		}

		// 1D array for Ui
		IloNumVarArray U(env,7,0,1,ILOINT);

		// Defining Obj to be equal to sum-product(Dij*Xij)
 		IloExpr Obj(env);
 		for (int i = 0; i < 7; ++i)
 		{
 			for (int j = 0; j < 7; j++)
 			{
 				Obj += distance[i][j]*X[i][j];
 			}
 		}
		//model.add(IloMaximize(env,Obj)); // IloMinimize is used for minimization problems
		//Alternatively, model.add(IloMaximize(env,Obj)); can be replaced by the following three lines.
		//This is useful while iterating in Decomposition Techniques where the objective function is redefined at each iteration
		IloObjective Objective = IloMinimize(env); 
		model.add(Objective); 
		Objective.setExpr(Obj);
		Obj.end();

		//Constraints for the model

		//outcoming
		for (int i = 0; i < 7; i++)
		{
			IloExpr Sum(env);
			for (int j = 0; j < 7; j++)
			{
				if(i != j)
					Sum += X[i][j];
			}
			model.add(Sum == 1);
			Sum.end();	
		}

		//outgoing
		for (int j = 0; j < 7; j++)
		{
			IloExpr Sum(env);
			for (int i = 0; i < 7; i++)
			{
				if(i != j)
					Sum += X[i][j];
			}
			model.add(Sum == 1);
			Sum.end();	
		}

		//SECs
		for (int i = 0; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				IloExpr sec(env);
				sec = U[i] - U[j] +nX[i][j];
				model.add(sec <= 6);
				sec.end();
			}
		}

		// Optimize
		IloCplex cplex(model);
		//cplex.setOut(env.getNullStream()); // if we get an empty stream in return
		//cplex.setWarning(env.getNullStream());
		cplex.solve();//solving the MODEL
		if (cplex.getStatus() == IloAlgorithm::Infeasible) // if the problem is infeasible
		{
			env.out() << "Problem Infeasible" << endl; 
		}
		cplex.getValues(X, U);
		// Print results
		cout<< "Objective Value = " << cplex.getObjValue() << endl;
		//cout<<"X = "<<X_val<<endl;
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