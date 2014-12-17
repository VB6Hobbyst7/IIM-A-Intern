/************************************************************************************
 *Vehicle Routing Problem
 *6 employees, 1 Office, minimum 2 tours for 3-seater cars.
 *Author : meetshah1995
 * Min Sum[i,j](Cij*Xijk) s.t.
 *		Sum[i,i!=j](Xijk)=1
 *		Sum[j,i!=j](Xijk)=1
 *		Sum[k,i!=j](Xijk)=1
 *		Sum[i]Sum[k](Xi7k)=2
 * 		Sum[j]Sum[k](X7jk)=2
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

int main(int argc, char **argv) 
{
	IloEnv env;	
	try 
	{
		
		IloArray<IloNumArray> distance(env);
		
		//Model Initialization
		IloModel model(env);

		//Taking Data from Distance Data File
		ifstream in;
		in.open("carpool-data.txt",ios::in);
		in >> distance;
		in.close();
		cout<<distance<<endl;
		
		//2D Array for Results
		IloArray<IloNumArray> X_val(env, 7);
		for (int i = 0; i < 7; i++)
		{
			X_val[i] = IloNumArray(env,7);
		}

		IloArray<IloNumArray> Y_val(env, 7);
		for (int i = 0; i < 7; i++)
		{
			Y_val[i] = IloNumArray(env,7);
		}

		//2D Array for Xij
		IloArray<IloNumVarArray> X(env, 7);
		for (int i = 0; i < 7; i++)
		{
			X[i] = IloNumVarArray(env,7,0,1,ILOINT);
		}

		//2D Array for Yij
		IloArray<IloNumVarArray> Y(env, 7);
		for (int i = 0; i < 7; i++)
		{
			Y[i] = IloNumVarArray(env,7,0,1,ILOINT);
		}

		// 1D array for Ui and U-val
		//IloNumVarArray U(env,7,0,7,ILOINT);
		//IloNumArray U_val(env,7);

		// Defining Obj to be equal to sum-product(Dij*Xij + Dij*Yij)
 		IloExpr Obj(env);
 		for (int i = 0; i < 7; ++i)
 		{
 			for (int j = 0; j < 7; j++)
 			{
				if(i != j)
				{
 				Obj += distance[i][j]*X[i][j] + distance[i][j]*Y[i][j];
				}
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

		//One Input per Node except Office
		IloExpr Input(env);
		for (int i = 0; i < 6; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if(i != j )
				{
				Input += X[i][j] + Y[i][j];
				}
			}
			model.add(Input == 1);
		}
		Input.end();
		
		//One Output per Node except Office
		IloExpr Output(env);
		for (int j = 0; j < 6; j++)
		{
			for (int i = 0; i < 7; i++)
			{
				if(i != j )
				{
				Output += X[i][j] + Y[i][j];
				}
			}
			model.add(Output == 1);
		}
		Output.end();
	
		//Ensuring 2 entries and exits for the Office node
		IloExpr Entryx(env);
		for (int i = 0; i <6; i++)
		{
			Entryx = Entryx + X[i][6];		
		}
		model.add(Entryx == 1);
		Entryx.end();

		IloExpr Exitx(env);
		for (int i = 0; i <6; i++)
		{
			Exitx = Exitx + X[6][i];
		}
		model.add(Exitx == 1);
		Exitx.end();

		IloExpr Entryy(env);
		for (int i = 0; i <6; i++)
		{
			Entryy = Entryy + Y[i][6];		
		}
		model.add(Entryy == 1);
		Entryy.end();

		IloExpr Exity(env);
		for (int i = 0; i <6; i++)
		{
			Exity = Exity + Y[6][i];
		}
		model.add(Exity == 1);
		Exity.end();

		// Optimize
		IloCplex cplex(model);
		cplex.setOut(env.getNullStream()); // if we get an empty stream in return
		//cplex.setWarning(env.getNullStream());
		cplex.solve();//solving the MODEL
		//cout << "hey there I reached the breakpoint" << endl;
		if (cplex.getStatus() == IloAlgorithm::Infeasible) // if the problem is infeasible
		{
			env.out() << "Problem Infeasible" << endl; 
		}
		for(int i = 0; i < 7 ; i++)
		{
			cplex.getValues(X_val[i], X[i]);
		}
		cout<<X_val<<endl;
		for(int i = 0; i < 7 ; i++)
		{
			cplex.getValues(Y_val[i], Y[i]);
		}
		cout<<Y_val<<endl;
		//	cplex.getValues(U_val,U);
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