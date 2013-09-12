#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <gmp.h>
#include <time.h>
#include <float.h>
#include <limits.h>
#include <mpfr.h>
#include <mpf2mpfr.h>


#ifndef SOLVER_MIDPOINT_H
#define SOLVER_MIDPOINT_H

#include "missing_bertini_headers.hpp"



#include "solver.hpp"
#include "fileops.hpp"
#include "data_type.hpp"

#include "determinant_derivative.hpp"
#include "programConfiguration.hpp"
#include "postProcessing.hpp"








//
class surface_decomposition;
//
class midpoint_config
{
public:
	
	
	SLP_global_pointers crit_memory;
	SLP_global_pointers mid_memory;
	

	// these are all merely pointers, and should only be assigned to memory set by the SLP creation routine inside of setupProg(), by the midpoint_config::setup() call
	
	prog_t *SLP_crit;
	prog_t *SLP_face; // will hold a pointer to the SLP. will pass this pointer to the BED class member
	
	
	mat_mp randomizer_matrix_crit, randomizer_matrix;
	
	int num_mid_vars;  //the number of variables (incl homogenizing) of midpoint.  set by setup
	int num_crit_vars;//the number of variables (incl homogenizing) of each edge point.  set by setup
	int num_variables;//the number of variables (incl homogenizing) of midpoint.  set by setup

	//patch already lives in the base class.
	
	comp_mp v_target; // set during the loop in connect the dots
	comp_mp u_target;// set during the loop in connect the dots
	
	comp_mp crit_val_left;// set during the loop in connect the dots
	comp_mp crit_val_right;// set during the loop in connect the dots
	
	int MPType;
	
	
	midpoint_config(){
		init();
	}
	
	~midpoint_config(){
		clear();
	}
	
	
	midpoint_config & operator=(const midpoint_config & other)
	{
		init();
		
		copy(other);
		return *this;
	}
	
	midpoint_config(const midpoint_config & other)
	{
		init();
		
		copy(other);
		
	}
	
	

	void copy(const midpoint_config & other){
		
		this->MPType = other.MPType;
		
		this->crit_memory = other.crit_memory;
		this->mid_memory = other.mid_memory;
		
		
		
		mat_cp_mp(randomizer_matrix, other.randomizer_matrix);
		mat_cp_mp(randomizer_matrix_crit, other.randomizer_matrix_crit);
		
		num_mid_vars = other.num_mid_vars;
		num_crit_vars = other.num_crit_vars;
		num_variables = other.num_variables;
		
		set_mp(v_target,other.v_target);
		set_mp(u_target,other.u_target);
		set_mp(crit_val_left,other.crit_val_left);
		set_mp(crit_val_right,other.crit_val_right);
		
		SLP_face = other.SLP_face;
		SLP_crit = other.SLP_crit;
	}
	
	
	
	void init();
	
	void clear()
	{
		std::cout << "clearing md_confug" << std::endl;
		clear_mat_mp(randomizer_matrix_crit);
		clear_mat_mp(randomizer_matrix);
		
		clear_mp(v_target);
		clear_mp(u_target);
		clear_mp(crit_val_left);
		clear_mp(crit_val_right);
		
		//also put in clearing stuff for the SLP's here.
		clearProg(this->SLP_face, this->MPType, 1); // 1 means call freeprogeval()
		clearProg(this->SLP_crit, this->MPType, 1); // 1 means call freeprogeval()
	}
	

	
	void print()
	{
		print_comp_matlab(u_target,"u_target");
		print_comp_matlab(v_target,"v_target");
		
		print_comp_matlab(crit_val_right,"crit_val_right");
		print_comp_matlab(crit_val_left,"crit_val_left");
		
		print_matrix_to_screen_matlab(randomizer_matrix,"R");
		print_matrix_to_screen_matlab(randomizer_matrix_crit,"R_crit");
	}
	
	void setup(const surface_decomposition & surf,
						 solver_configuration & solve_options);
	
};






// the mp version
// this must be defined before the double version, because double has mp.
class midpoint_eval_data_mp : public solver_mp
{
public:
	
	int num_mid_vars;
	int num_crit_vars;
	
	SLP_global_pointers crit_memory;
	SLP_global_pointers mid_memory;
	
	// these are all merely pointers, and should only be assigned to memory set by the SLP creation routine inside of setupProg(), by the midpoint_config::setup() call
	
	
	prog_t *SLP_crit;
	prog_t *SLP_face; // these are to be freed by the midpoint_setup object.
	
	vec_mp *pi;
	int num_projections;
	
	//patch already lives in the base class.
	
	comp_mp v_target;
	comp_mp u_target;
	
	mat_mp randomizer_matrix_crit;
	
	comp_mp crit_val_left;
	comp_mp crit_val_right;
	
	comp_mp half;
	comp_mp u_start;
	comp_mp v_start;
	comp_mp one;
	comp_mp zero;
	
	
	
	vec_mp *pi_full_prec;
	comp_mp v_target_full_prec;
	comp_mp u_target_full_prec;
	
	mat_mp randomizer_matrix_crit_full_prec;
	
	comp_mp crit_val_left_full_prec;
	comp_mp crit_val_right_full_prec;
	
	comp_mp half_full_prec;
	comp_mp u_start_full_prec;
	comp_mp v_start_full_prec;
	comp_mp one_full_prec;
	comp_mp zero_full_prec;
	
	
	
	
	// default initializer
	midpoint_eval_data_mp() : solver_mp(){
		
		reset_counters();
		
		init();
	}
	
	midpoint_eval_data_mp(int mp) : solver_mp(mp){
		
		this->MPType = mp;
		reset_counters();
		
		init();
	}
	
	~midpoint_eval_data_mp(){
		
		clear();
		// no need to reset the counters.
	}
	
	void print()
	{

	};
	
	void reset_counters()
	{
		this->num_projections = 0;
	} // re: reset_counters
	
	
	midpoint_eval_data_mp & operator=(const midpoint_eval_data_mp & other)
	{
		init();
		
		copy(other);
		return *this;
	}
	
	midpoint_eval_data_mp(const midpoint_eval_data_mp & other)
	{
		init();
		
		//no need to clear or reset counters, as this is a new object.
		copy(other);
	}
	
	
	// MPI SENDS AND RECEIVES
	
	int send(parallelism_config & mpi_config);
	
	int receive(parallelism_config & mpi_config);
	
	
	
	int setup(const midpoint_config & md_config,
						surface_decomposition & surf,
						const witness_set & W,
						solver_configuration & solve_options);
	
	
	
	
	void add_projection(vec_mp proj)
	{ // this is wrong for mptype 2
		if (this->num_projections==0) {
			this->pi = (vec_mp *) br_malloc(sizeof(vec_mp));
		}
		else {
			this->pi = (vec_mp *) br_realloc(pi,(this->num_projections+1) *sizeof(vec_mp));
		}
		
		init_vec_mp(pi[num_projections],0);
		vec_cp_mp(pi[num_projections], proj);
		
		if (this->num_projections==0) {
			this->pi_full_prec = (vec_mp *) br_malloc(sizeof(vec_mp));
		}
		else {
			this->pi_full_prec = (vec_mp *) br_realloc(pi_full_prec,(this->num_projections+1) *sizeof(vec_mp));
		}
		
		init_vec_mp2(pi_full_prec[num_projections],0,1024);
		vec_cp_mp(pi_full_prec[num_projections], proj);
		
		num_projections++;
		
	}
	
	
	
	
	
	void clear()
	{
		
		
		if (this->num_projections>0) {
			for (int ii=0; ii<num_projections; ii++) {
				clear_vec_mp(pi[ii]);
			}
			free(pi);
		}
		
	} // re: clear
	
	void init(); // in the .cpp file
		
	
	void copy(const midpoint_eval_data_mp & other)
	{
		solver_mp::copy(other);
		
		this->num_mid_vars = other.num_mid_vars;
		this->num_crit_vars = other.num_crit_vars;
		
		this->crit_memory = other.crit_memory;
		this->mid_memory = other.mid_memory;
		
		this->SLP_face = other.SLP_face;
		this->SLP_crit = other.SLP_crit;
		
		for (int ii=0; ii<other.num_projections; ii++) {
			if (other.MPType==2) {
				add_projection(other.pi_full_prec[ii]);
			}
			else{
				add_projection(other.pi[ii]);
			}
		}
		
		//patch already lives in the base class.
		
		set_mp(v_target,other.v_target);
		set_mp(u_target,other.u_target);
		
		mat_cp_mp(randomizer_matrix_crit,other.randomizer_matrix_crit);
		
		set_mp(crit_val_left,other.crit_val_left);
		set_mp(crit_val_right,other.crit_val_right);
		
		set_mp(u_start, other.u_start);
		set_mp(v_start, other.v_start);
		
		
		if (this->MPType==2) {
			set_mp(v_target_full_prec,other.v_target_full_prec);
			set_mp(u_target_full_prec,other.u_target_full_prec);
			
			mat_cp_mp(randomizer_matrix_crit_full_prec,other.randomizer_matrix_crit_full_prec);
			
			set_mp(crit_val_left_full_prec,other.crit_val_left_full_prec);
			set_mp(crit_val_right_full_prec,other.crit_val_right_full_prec);
			
			set_mp(u_start_full_prec, other.u_start_full_prec);
			set_mp(v_start_full_prec, other.v_start_full_prec);
		}
		//copy the mp one?
	} // re: copy
	
	
	
	
}; // re: class nullspace_eval_data_mp










// the double version
// this must be defined after the mp version, because double has mp.
class midpoint_eval_data_d : public solver_d
{
public:
	
	midpoint_eval_data_mp *BED_mp; // used only for AMP
	
	int num_mid_vars;
	int num_crit_vars;
	
	
	SLP_global_pointers crit_memory;
	SLP_global_pointers mid_memory;
	
	prog_t *SLP_crit;
	prog_t *SLP_face;
	
	vec_d *pi;
	int num_projections;
	
	//patch already lives in the base class.
	
	comp_d v_target;
	comp_d u_target;
	
	mat_d randomizer_matrix_crit;
	
	comp_d crit_val_left;
	comp_d crit_val_right;
	
	comp_d half;
	comp_d u_start;
	comp_d v_start;
	comp_d one;
	comp_d zero;
	
	
	// default initializer
	midpoint_eval_data_d() : solver_d(){
		
		reset_counters();
		
		init();
	}
	
	midpoint_eval_data_d(int mp) : solver_d(mp){
		
		this->MPType = mp;
		reset_counters();
		
		init();
	}
	
	~midpoint_eval_data_d(){
		
		midpoint_eval_data_d::clear();
		// no need to reset the counters.
	}
	
	void print()
	{
		std::cout << "num_variables " << num_variables << std::endl;
		std::cout << "num_mid_vars " << num_mid_vars << std::endl;
		std::cout << "SLP_crit " << SLP_crit->size << std::endl;
		std::cout << "SLP_face " << SLP_face->size << std::endl;

		std::cout << num_projections << " projections: " << std::endl;
		for (int ii=0; ii<2; ii++) {
			print_point_to_screen_matlab(this->pi[ii],"pi");
		}
		
	};
	
	
	
	void reset_counters()
	{
		
	} // re: reset_counters
	
	
	midpoint_eval_data_d & operator=(const midpoint_eval_data_d & other)
	{
		this->MPType = other.MPType;
		init();
		
		copy(other);
		return *this;
	}
	
	midpoint_eval_data_d(const midpoint_eval_data_d & other)
	{
		init();
		solver_d();
		midpoint_eval_data_d();
		
		//no need to clear or reset counters, as this is a new object.
		copy(other);
	}
	
	
	// MPI SENDS AND RECEIVES
	
	int send(parallelism_config & mpi_config);
	
	int receive(parallelism_config & mpi_config);
	
	
	
	int setup(const midpoint_config & md_config,
						surface_decomposition & surf,
						const witness_set & W,
						solver_configuration & solve_options);
	
	
	void add_projection(vec_mp proj)
	{
		if (this->num_projections==0) {
			this->pi = (vec_d *) br_malloc(sizeof(vec_d));
		}
		else {
			this->pi = (vec_d *) br_realloc(pi,(this->num_projections+1) *sizeof(vec_d));
		}
		
		init_vec_d(pi[num_projections],0);
		vec_mp_to_d(pi[num_projections], proj);
		
		num_projections++;
	}
	
	
	void add_projection(vec_d proj)
	{
		if (this->num_projections==0) {
			this->pi = (vec_d *) br_malloc(sizeof(vec_d));
		}
		else {
			this->pi = (vec_d *) br_realloc(pi,(this->num_projections+1) *sizeof(vec_d));
		}
		
		init_vec_d(pi[num_projections],0);
		vec_cp_d(pi[num_projections], proj);
		
		num_projections++;
	}
private:
	
	
	void clear()
	{
		
		free(SLP_crit);
		free(SLP_face);
		
		//yeah, there's some stuff to clear here.
		if (this->MPType==2) {
			
			delete this->BED_mp;
		}
	} // re: clear
	
	void init();
	
	void copy(const midpoint_eval_data_d & other)
	{
		
		
		solver_d::copy(other);
		
		this->num_mid_vars = other.num_mid_vars;
		this->num_crit_vars = other.num_crit_vars;
		
		this->crit_memory = other.crit_memory;
		this->mid_memory = other.mid_memory;
		
		
		this->SLP_face = other.SLP_face;
		this->SLP_crit = other.SLP_crit;

		
		for (int ii=0; ii<other.num_projections; ii++) {
			add_projection(other.pi[ii]);
		}
		
		//patch already lives in the base class.
		
		set_d(v_target,other.v_target);
		set_d(u_target,other.u_target);
		
		mat_cp_d(randomizer_matrix_crit,other.randomizer_matrix_crit);
		
		set_d(crit_val_left,other.crit_val_left);
		set_d(crit_val_right,other.crit_val_right);
		
		set_d(u_start, other.u_start);
		set_d(v_start, other.v_start);
	}
};







/** the main function for finding critical conditions WRT a projection
 */

int midpoint_solver_master_entry_point(const witness_set						& W, // carries with it the start points, and the linears.
																			 witness_set						*W_new, // new data goes in here
																			 surface_decomposition & surf,
																			 midpoint_config & md_config,
																			 solver_configuration		& solve_options);






int midpoint_eval_d(point_d funcVals, point_d parVals, vec_d parDer, mat_d Jv, mat_d Jp, point_d current_variable_values, comp_d pathVars, void const *ED);

int midpoint_eval_mp(point_mp funcVals, point_mp parVals, vec_mp parDer, mat_mp Jv, mat_mp Jp, point_mp current_variable_values, comp_mp pathVars, void const *ED);





int midpoint_dehom(point_d out_d, point_mp out_mp, int *out_prec, point_d in_d, point_mp in_mp, int in_prec, void const *ED_d, void const *ED_mp);



int change_midpoint_eval_prec(void const *ED, int new_prec);



int check_issoln_midpoint_d(endgame_data_t *EG,
																tracker_config_t *T,
																void const *ED);
int check_issoln_midpoint_mp(endgame_data_t *EG,
																 tracker_config_t *T,
																 void const *ED);

int check_isstart_midpoint_d(point_d testpoint,
																 tracker_config_t *T,
																 void const *ED);


void check_midpoint_evaluator(point_mp current_values,
															 void const *ED);




void midpoint_slave_entry_point(solver_configuration & solve_options);










#endif


