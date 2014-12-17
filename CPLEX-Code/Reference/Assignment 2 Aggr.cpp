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
#include <ilconcert/ilocsvreader.h>

ILOSTLBEGIN

int main(int argc, char **argv)
{
	IloEnv env;
	IloModel model_master(env);
	IloNumVar theta(env);
	IloNum thetasol =0;
	IloNumVar u(env);IloNum usol = 0.5;
	model_master.add(IloMinimize(env,theta));
	model_master.add(u >= 0);
	IloBoolVar x1(env),x2(env),x3(env),x4(env);
	IloBool xsol1 =0 ,xsol2 =0 ,xsol3 =0 ,xsol4 =0;
	
	IloModel model_sub(env);
	model_sub.add(x1 + x2 <= 1);
	model_sub.add(x3 + x4 <= 1);
	IloObjective obj = IloMaximize(env);
	model_sub.add(obj);
	
	IloNum UB=IloInfinity,LB = -IloInfinity;
	IloInt Iteration = 0;
	while ((UB-LB) > 0.0001)
	{
		Iteration++;
		obj.setExpr((16-8*usol)*x1+(10-2*usol)*x2-usol*x3+(4-4*usol)*x4+10*usol);
		IloCplex cplex_sub(model_sub);
		cplex_sub.setOut(env.getNullStream());
		cplex_sub.solve();
		xsol1=cplex_sub.getValue(x1);
		xsol2=cplex_sub.getValue(x2);
		xsol3=cplex_sub.getValue(x3);
		xsol4=cplex_sub.getValue(x4);
		UB = IloMin (UB,cplex_sub.getObjValue());
		model_master.add(theta >= (16-8*u)*xsol1+(10-2*u)*xsol2+(4-4*u)*xsol4-u*xsol3+10*u);
		IloCplex cplex_master(model_master);
		cplex_master.setOut(env.getNullStream());
		cplex_master.solve();
		usol=cplex_master.getValue(u);
		LB = cplex_master.getObjValue();
		cout << "\n**********************************************\n";
		cout << "Iteration     = "<<Iteration<<endl;
		cout << "UB            = "<<UB<<endl;
		cout << "LB            = "<<LB<<endl;
		cout << "Gap (%)       = "<<(UB-LB)*100/LB<<endl;
		cout << "u             = "<<usol<<endl;
		cout << "(x1,x2,x3,x4) = "<<"("<<xsol1<<","<<xsol2<<","<<xsol3<<","<<xsol4<<")"<<endl; 
		cout << "Cut           = theta >= "<<xsol1<<"(16-8u)+"<<xsol2<<"(10-2u)-"<<xsol3<<"u+"<<xsol4<<"(4-4u)+10u\n";
		cout << "**********************************************";
	}
}