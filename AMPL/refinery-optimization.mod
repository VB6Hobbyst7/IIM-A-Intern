#######################################
# Refinery Optimization Problem Solution
# Objective : Maxmimise Revenue
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
#Total Crude Barrels

var CRA >=0;
var CRB >=0;


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


#Revenue Maximization
maximize revenue: 7*PMF + 6*RMF + 4*JF + 3.5*FO + 1.5*LBO;

# PART 3 CONSTRAINTS: names and corresponding mathematical expressions


########################################
#                 Constraints
########################################
#Total Capacity
subject to max_CRA: CRA <=20000;
subject to max_CRB: CRB <=30000;

#Total Distillation Capacity
subject to max_CRA_plus_CRB: CRB + CRA <=45000;

#Total Reforming and Cracking Capacity
subject to max_reform: LNRG + MNRG + HNRG <= 10000;
subject to max_cracking: LOCGO + HOCGO <= 8000;

#Lube Oil
subject to max_loil: LBO <= 1000;
subject to max_loil: LBO <= 1000;

#Continuities
subject to lightN: LN -.15*CRB - .1*CRA := 0;
#subject to meduimN: MN -.15*CRB - .1*CRA := 0;
#subject to heavyN: LN -.15*CRB - .1*CRA := 0;
#subject to lightO: LN -.15*CRB - .1*CRA := 0;
#subject to heavyO: LN -.15*CRB - .1*CRA := 0;
#subject to residuum: LN -.15*CRB - .1*CRA := 0;

#Reformed Gasoline
subject to ref_gasoline: RG - .45*HNRG - .52*MNRG - .6*LNRG := 0;

#Cracked Gasoline
subject to cracked_gas: CG -.2*HOCGO - .28*LOCGO := 0;

#Cracked Oil
subject to cracked_oil: CO - .68*LOCGO - .75*HOCGO :=0;

#Lube Oil & Residuum
subject to lo_res: LBO - .5*RLBO :=0;

#Light Naptha Belnding and Reforming
subject to : blend_crack: LNRG + LNPMF + LNRMF - LN :=0; 
#subject to : blend_crack: LNRG + LNPMF + LNRMF - LN :=0; 
#subject to : blend_crack: LNRG + LNPMF + LNRMF - LN :=0; 

#Blending Fuel oil
subject to fo_blend: LOCGO + LOJF + .55*FO - LO :=0;
#subject to fo_blend: LOCGO + LOJF + .55*FO - LO :=0;
#subject to fo_blend: LOCGO + LOJF + .55*FO - LO :=0;
#subject to fo_blend: LOCGO + LOJF + .55*FO - LO :=0;

#Blending Reformed Gasoline and Cracked Gasoline
#subject to fo_blend: LOCGO + LOJF + .55*FO - LO :=0;
#subject to fo_blend: LOCGO + LOJF + .55*FO - LO :=0;

#Premium Motor Fuel
subject to pre_mot_fuel: PMF -CGPMF -RGPMF -HNPMF-MNPMF-LNPMF :=0; 

#Regular Motor Fuel
#subject to pre_mot_fuel: PMF -CGPMF -RGPMF -HNPMF-MNPMF-LNPMF :=0;

#Jet Fuel
#subject to pre_mot_fuel: PMF -CGPMF -RGPMF -HNPMF-MNPMF-LNPMF :=0;

#Premium Regular
subject to pre_reg: PMF -.4*RMF >=0;

#Quality
subject to pre_qua: 94*PMF -90*LNPMF -80*MNPMF - 70*HNPMF -115*RGPMF - 105*CGPMF +94*PMF <=0;
#subject to pre_qua: 94*PMF -90*LNPMF -80*MNPMF - 70*HNPMF -115*RGPMF - 105*CGPMF +94*PMF <=0;
subject to jf_qua: JF -.05*RJF -1.5*COJF - .6*HOJF - LOJF <=0;