#pragma once
#ifndef SEQUENTIAL_LP_H
#define SEQUENTIAL_LP_H

#include "pest.h"
#include "Jacobian_1to1.h"
#include "ClpSimplex.hpp"
#include "RunManagerAbstract.h"
#include "TerminationController.h"
#include "covariance.h"
#include "FileManager.h"
#include "OutputFileWriter.h"
#include "Transformable.h"
#include "ModelRunPP.h"

class sequentialLP
{
	enum ConstraintSense {less_than,greater_than,equal_to,undefined};
public:
	sequentialLP(Pest &_pest_scenario, RunManagerAbstract* _run_mgr_ptr, 
		         Covariance &_parcov, FileManager* _file_mgr);
	void initialize_and_check();
	void solve();

	
	//ModelRun get_optimum_run() { return optimum_run; }

private:
	string obj_func_str;
	bool use_chance;
	int slp_iter;
	
	double* dec_var_lb;
	double* dec_var_ub;
	double* constraint_lb;
	double* constraint_ub;
	double* ctl_ord_obj_func_coefs;
	double risk;

	ClpSimplex model;
	Jacobian_1to1 jco;

	map<string, double> obj_func_coef_map;
	map<string, ConstraintSense> constraint_sense_map;
	map <string, string> constraint_sense_name;
	map<string, double> prior_constraint_stdev;
	map<string, double> post_constraint_stdev;
	map<string, double> prior_constraint_offset;
	map<string, double> post_constraint_offset;
	//map<string, map<string, double>> pi_constraint_factors;
	//map<string, double> pi_constraint_rhs;

	vector<double> iter_obj_values;
	vector<string> ctl_ord_dec_var_names;
	vector<string> ctl_ord_obs_constraint_names;
	vector<string> ctl_ord_pi_constraint_names;
	vector<string> ctl_ord_ext_var_names;
	vector<string> nz_obs_names;
	vector<string> adj_par_names;

	PriorInformation* null_prior = new PriorInformation();
	Parameters all_pars_and_dec_vars;
	Parameters all_pars_and_dec_vars_initial;
	ParamTransformSeq par_trans;
	Observations constraints_obs;
	Observations constraints_sim;
	Observations constraints_fosm;
	Observations constraints_sim_initial;
	PriorInformation constraints_pi;
	Observations obj_func_obs;
	ObservationInfo obj_func_info;
	Pest pest_scenario;
	RunManagerAbstract* run_mgr_ptr;
	Covariance parcov;
	Covariance obscov;
	FileManager* file_mgr;
	//OutputFileWriter* out_wtr_ptr;
		
	int num_dec_vars() { return ctl_ord_dec_var_names.size(); }
	int num_obs_constraints() { return ctl_ord_obs_constraint_names.size(); }
	int num_pi_constraints() { return ctl_ord_pi_constraint_names.size(); }
	int num_constraints() { return num_obs_constraints() + num_pi_constraints(); }
	int num_adj_pars() { return adj_par_names.size(); }
	int num_nz_obs() { return nz_obs_names.size(); }

	//get the number of non zero elements in the prior information constraints
	int num_nz_pi_constraint_elements();

	//parse the obs or pi group name to get the constraint sense
	pair<ConstraintSense,string> get_sense_from_group_name(string &name);

	//solve the current LP problem
	void iter_solve();
	
	//report initial conditions to rec file
	void initial_report();

	//report the constraint info before the solving the current LP problem
	void presolve_constraint_report();

	//report the fosm chance constraint info before solving the current LP problem
	void presolve_fosm_report();

	//report dec var info the newly solved LP solution.  returns the current and new obj func
	pair<double,double> postsolve_decision_var_report(Parameters &upgrade_pars);
	
	//report the current and newly solved LP constraint info
	void postsolve_constraint_report(Observations &upgrade_obs, Parameters &upgrade_pars);
	
	//prepare for LP solution, including filling response matrix
	void iter_presolve();
	
	//run the model with dec var values from the newly solved LP problem
	bool make_upgrade_run(Parameters &upgrade_pars, Observations &upgrade_obs);
	
	//process the LP solve, including check for convergence
	void iter_postsolve();

	//convert the jacobian to a coin packed matrix instance
	CoinPackedMatrix jacobian_to_coinpackedmatrix();

	//convert the constraint info from Transformable to double*
	void build_constraint_bound_arrays();

	//error handlers
	void throw_sequentialLP_error(string message);
	void throw_sequentialLP_error(string message,const vector<string> &messages);
	void throw_sequentialLP_error(string message, const set<string> &messages);

	//get the current constraint residual vector 
	vector<double> get_constraint_residual_vec();
	
	//get a residual vector comparing constraints_obs and sim_vals
	vector<double> get_constraint_residual_vec(Observations &sim_vals);
	
	//set the double* obj_func array
	void build_obj_func_coef_array();

	//calc FOSM-based chance constraint offsets
	void calc_chance_constraint_offsets();
};




#endif