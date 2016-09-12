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
public:
	sequentialLP(Pest &_pest_scenario, RunManagerAbstract* _run_mgr_ptr, 
		         TerminationController* _termination_ctl_ptr, Covariance &_parcov, 
				 FileManager &_file_mgr, OutputFileWriter* _out_wtr_ptr);
	void initialize();
	void solve();
	
	ModelRun get_optimum_run() { return optimum_run; }

private:
	vector<string> ctl_ord_dec_var_names;
	vector<string> ctl_ord_constraint_names;
	PriorInformation* null_prior = new PriorInformation();
	ModelRun optimum_run;
	ObjectiveFunc obj_func;
	Parameters decision_vars;
	Observations constraints_obs;
	Observations constraints_sim;
	Pest pest_scenario;
	Pest opt_scenario;
	RunManagerAbstract* run_mgr_ptr;
	TerminationController* termination_ctl_ptr;
	Covariance parcov;
	FileManager file_mgr;
	OutputFileWriter* out_wtr_ptr;
	ClpSimplex solve_lp_problem(Jacobian_1to1 &jco);
	void update(ClpSimplex &model);
	void update_decision_vars(ClpSimplex &model);
	void update_constraints(ClpSimplex &model);
	void separate_scenarios();
	void make_runs(Jacobian_1to1 &jco);
	CoinPackedMatrix jacobian_to_coinpackedmatrix(Jacobian_1to1 &jco);




};




#endif