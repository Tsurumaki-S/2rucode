#ifndef __CALC_AVE_FORCE_IN_DOMAIN__
#define __CALC_AVE_FORCE_IN_DOMAIN__
#include "const.h"
#include "run_manager.h"
#include "file.h"
class Calc_Ave_Force_In_Domain
{
	private:
		double r_min[XYZ] ;
		double r_max[XYZ] ;
		double dt ;

	public:
		void do_Calc_Ave_Force_In_Domain(Run_Manager mr, FileIO fp) ;
		void Check_Domain(FILE *fp_input_pos, FILE *fp_indomain, int frame_max) ;
		void set_Domain(double x_min,double y_min,double z_min,double x_max,double y_max,double z_max) ;
		void set_Dt(double time) ;
		void set_Domain_From_File(FILE *input) ;
		void do_calc(FILE *fp_indomain, FILE *fp_input_force, FILE *fp_aveforce, int frame_max, int nstout) ;
		void gather_Data(FILE *fp_input, FILE *fp_output,int frame_max) ;

} ;

#endif
