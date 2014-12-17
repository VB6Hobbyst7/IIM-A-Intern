/************************************************************************************
 *Vehicle Routing Problem
 *6 employees, 1 Office, minimum 2 tours for 3-seater cars.
 *Author : meetshah1995
 * Min Sum[i,j](Cij*Xijk) s.t. [ k = 1,2 ]
 * Office at 0 and Employees from 1 to 7
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
#include <ilcplex/ilocplex.h>
#include <ilconcert/ilosys.h>

using namespace std;

ILOSTLBEGIN

int main(int argc, char **argv) 
{
	IloEnv env;	
	try 
	{	
		IloArray<IloIntArray> distance(env);
		
		//Model Initialization
		IloModel model(env);

		//Taking Data from Distance Data File
		ifstream in;
		in.open("carpool-office-0.txt",ios::in);
		in >> distance;
		in.close();
		//cout<<distance<<endl;
		
		//2D Array for Results
		IloArray<IloIntArray> X_val(env, 7);
		for (int i = 0; i < 7; i++)
		{
			X_val[i] = IloIntArray(env,7);
		}

		//2D Array for Xij
		IloArray<IloIntVarArray> X(env, 7);
		for (int i = 0; i < 7; i++)
		{
			X[i] = IloIntVarArray(env,7,0,1);
		}

		// 1D array for Ui and U-val
		IloIntVarArray U(env,7,0,7);
		IloIntArray U_val(env);

		//Variables for L(Lower bound of the no of nodes a salesman should visit) and K(upper bound for the same) 
		IloInt L = 7;
		//IloInt K  = ;

		// Defining Obj to be equal to sum-product(Dij*Xij)
 		IloExpr Obj(env);
 		for (int i = 0; i < 7; ++i)
 		{
			model.add(X[i][i]==0);
 			for (int j = 0; j < 7; j++)
 			{
				if(i != j)
				{
 				Obj += distance[i][j]*X[i][j];
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
		for (int i = 1; i < 7; i++)
		{
			for (int j = 0; j < 7; j++)
			{
				if(i != j )
				{
				Input += X[i][j];
				}
			}
			model.add(Input == 1);
		}
		Input.end();
		
		//One Output per Node except Office
		IloExpr Output(env);
		for (int j = 1; j < 7; j++)
		{
			
			for (int i = 0; i < 7; i++)
			{
				if(i != j )
				{
				Output += X[i][j];
				}
			}
			model.add(Output == 1);
		}
		Output.end();
	
		//Ensuring 2 entries and exits for the Office node
		IloExpr Entryx(env);
		for (int i = 1; i <7; i++)
		{
			Entryx = Entryx + X[i][0];		
		}
		model.add(Entryx == 2);
		Entryx.end();

		IloExpr Exitx(env);
		
		for (int i = 1; i <7; i++)
		{
			Exitx = Exitx + X[0][i];
		}
		model.add(Exitx == 2);
		Exitx.end();

		//SECs
		for(int i = 2; i < 7; i++)
		{
			for(int j = 2; j < 7; j++)
			{
				if(i!=j)
				{
					IloExpr sec(env);
					sec = U[i] - U[j] +L*X[i][j]; //+ (L-2)*X[j][i];
					model.add(sec <= L-1);
					sec.end();
				}	
			}
		}

		/*for(int j = 1; j < 7; j++)
			{
					IloExpr sec2(env);
					sec2 = X[0][j] + X[j][0];
					model.add(sec2 <= 1);
					sec2.end();
			}*/

		for(int i = 1; i < 7; i++)
			{
					IloExpr sec3(env);
					sec3 = U[i] + (L-2)*X[0][i] - X[i][0];
					model.add(sec3 <= L-1);
					sec3.end();
			}	
		
		/*for(int i = 1; i < 7; i++)
			{
					IloExpr sec4(env);
					sec4 = U[i] + X[0][i] + (2-K)*X[i][0];
					model.add(sec4 >= 2);
					sec4.end();
			}*/		
			
		// Optimize
		IloCplex cplex(model);
		cplex.setOut(env.getNullStream()); // if we get an empty stream in return
		//cplex.setWarning(env.getNullStream());
		cplex.solve();//solving the MODEL
		if (cplex.getStatus() == IloAlgorithm::Infeasible) // if the problem is infeasible
		{
			env.out() << "Problem Infeasible" << endl; 
		}
		for(int i = 0; i < 7 ; i++)
		{
			for(int j= 0; j < 7 ; j++)
			{	
			X_val[i][j] = cplex.getValue(X[i][j]);
			}
		}
		/*
		for(int i = 1; i < 7 ; i++)
		{
			U_val[i] = cplex.getValue(U[i]);
		}*/
		// Print results
		cout<<X_val<<endl;
		//cout<<U_val<<endl;
		cout<< "Objective Value = " << cplex.getObjValue() << endl;
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