#######################################
# Manpower Planning Problem Solution
# Objective : Minimize Redundancy
# Author : github.com/meetshah1995
#######################################

# PART 1 DECLARATION OF VARIABLES (variables, parameters, sets etc)

########################################
#                Parameters
########################################
param years := 3;
param years_total := 4;

########################################
#                 Variables
########################################
#Total Employed

var skilled_employed{i in 1..years_total}>= 0;
var semi_skilled_employed{i in 1..years_total}>= 0; 
var unskilled_employed{i in 1..years_total}>= 0;

#Total Recruited

var skilled_recruit{i in 1..years}>= 0;
var semi_skilled_recruit{i in 1..years}>= 0;
var unskilled_recruit{i in 1..years}>= 0;

#Total Downgraded

var skilled_to_semi_skilled_downgrade{i in 1..years}>= 0;
var skilled_to_unskilled_downgrade{i in 1..years}>= 0;
var semi_skilled_to_unskilled_downgrade{i in 1..years}>= 0;

#Total Retrained

var semi_skilled_to_skilled_retrained{i in 1..years}>= 0;
var unskilled_to_semi_skilled_retrained{i in 1..years}>= 0;

#Total Redundant

var skilled_redundant{i in 1..years}>= 0;
var semi_skilled_redundant{i in 1..years}>= 0;
var unskilled_redundant{i in 1..years}>= 0;

#Total Short-Term

var skilled_shortterm{i in 1..years}>= 0;
var semi_skilled_shortterm{i in 1..years}>= 0;
var unskilled_shortterm{i in 1..years}>= 0;

#Total Overmanning

var skilled_overmanning{i in 1..years} >= 0;
var semi_skilled_overmanning{i in 1..years} >= 0;
var unskilled_overmanning{i in 1..years} >= 0;


# PART 2 OBJECTIVE FUNCTION: name and mathematical expression


#Part 1
minimize redundancy: sum {i in 1..years} skilled_redundant[i]+semi_skilled_redundant[i]+unskilled_redundant[i];

# PART 3 CONSTRAINTS: names and corresponding mathematical expressions


########################################
#                 Constraints
########################################
#Initial Conditions
subject to i_skilled: skilled_employed[1]:=1000;
subject to i_semi_skilled: semi_skilled_employed[1]:=1500;
subject to i_unskilled: unskilled_employed[1]:=2000;

#Continuity
subject to skilled_cont {i in 1..years}: skilled_employed[i+1]:= .95*skilled_employed[i]+.9*skilled_recruit[i]+.95*semi_skilled_to_skilled_retrained[i]-skilled_to_semi_skilled_downgrade[i]-skilled_to_unskilled_downgrade[i]-skilled_redundant[i];     
subject to semi_skilled_cont {i in 1..years}: semi_skilled_employed[i+1]:=.95*semi_skilled_employed[i]+.8*semi_skilled_recruit[i]+.95*unskilled_to_semi_skilled_retrained[i]-semi_skilled_to_skilled_retrained[i]+.5*skilled_to_semi_skilled_downgrade[i]-semi_skilled_to_unskilled_downgrade[i]-semi_skilled_redundant[i];
subject to unskilled_cont {i in 1..years}: unskilled_employed[i+1]:=.9*unskilled_employed[i]+.75*unskilled_recruit[i]-unskilled_to_semi_skilled_retrained[i]+.5*skilled_to_unskilled_downgrade[i]+.5*semi_skilled_to_unskilled_downgrade[i]-unskilled_redundant[i];

#Retraining 
subject to retrain_limit {i in 1..years}: semi_skilled_to_skilled_retrained[i] -.25*skilled_employed[i] <= 0;
subject to retrain_cap {i in 1..years}: unskilled_to_semi_skilled_retrained[i] <=200;

#Overmanning
subject to overman_cap {i in 1..years}: unskilled_overmanning[i]+skilled_overmanning[i]+semi_skilled_overmanning[i] <= 150;

#Needs
subject to skilled_needs: skilled_employed[1]-skilled_overmanning[1]-.5*skilled_shortterm[1]:=1000;
subject to skilled_needs: skilled_employed[2]-skilled_overmanning[2]-.5*skilled_shortterm[2]:=1500;
subject to skilled_needs: skilled_employed[3]-skilled_overmanning[3]-.5*skilled_shortterm[3]:=2000;

subject to semi_skilled_needs: semi_skilled_employed[1]-semi_skilled_overmanning[1]-.5*semi_skilled_shortterm[1]:=1400;
subject to semi_skilled_needs: semi_skilled_employed[2]-semi_skilled_overmanning[2]-.5*semi_skilled_shortterm[2]:=2000;
subject to semi_skilled_needs: semi_skilled_employed[3]-semi_skilled_overmanning[3]-.5*semi_skilled_shortterm[3]:=2500;

subject to unskilled_needs: unskilled_employed[1]-unskilled_overmanning[1]-.5*unskilled_shortterm[1]:=1000;
subject to unskilled_needs: unskilled_employed[2]-unskilled_overmanning[2]-.5*unskilled_shortterm[2]:=500;
subject to unskilled_needs: unskilled_employed[3]-unskilled_overmanning[3]-.5*unskilled_shortterm[3]:=0;

#Bounds
subject to skilled_intake {i in 1..years}: skilled_recruit[i]<=500;
subject to semi_skilled_intake {i in 1..years}: semi_skilled_recruit[i]<=800;
subject to semi_skilled_intake {i in 1..years}: semi_skilled_recruit[i]<=500;

#Shortterm Bounds
subject to skilled_shortcap {i in 1..years}: skilled_shortterm[i]<=50;
subject to semi_skilled_shortcap {i in 1..years}: semi_skilled_shortterm[i]<=50;
subject to unskilled_shortcap {i in 1..years}: unskilled_shortterm[i]<=50;