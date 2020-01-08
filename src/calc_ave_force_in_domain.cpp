#include <stdio.h>
#include <stdlib.h>
#include "../header/const.h"
#include "../header/calc_ave_force_in_domain.h"
#include "../header/file.h"

void Calc_Ave_Force_In_Domain::do_Calc_Ave_Force_In_Domain(Run_Manager mr, FileIO fp)
{
	Calc_Ave_Force_In_Domain analysis ;

	
	analysis.set_Dt(atof(mr.get_argument(6))) ;

	if( fp.set_Input_File(1,"domain.dat") )
	{
		printf("\nInput file1 = domain.dat\n") ;
	}
	analysis.set_Domain_From_File(fp.get_Input_File(1)) ;
	fp.close_Files() ;

	//if( fp.set_Input_File1(argv[1]) )
	if( fp.set_Input_File(1,mr.get_argument(2)) )
	{
		printf("\nInput file1 = %s\n",mr.get_argument(2)) ;
	}

	if( fp.set_Input_File(2,mr.get_argument(3)) )
	{
		printf("\nInput file2= %s\n",mr.get_argument(3)) ;
	}

	if( fp.set_Output_File(1,mr.get_argument(4)) )
	{
		printf("\nOutput file1 = %s\n",mr.get_argument(4)) ;
	}

//	printf("\n\n\nframe_max=%d\n\n\n",fp.get_LineNum_Input_File1()-1) ; //DEBUG
	int frame_max = fp.get_LineNum_Input_File(1) - 1 ;
	analysis.Check_Domain(fp.get_Input_File(1),fp.get_Output_File(1),frame_max) ;
	fp.close_Files() ;

	if( fp.set_Input_File(1,mr.get_argument(4)) )
	{
		printf("\nInput file1 = %s\n",mr.get_argument(4)) ;
	}

	if( fp.set_Input_File(2,mr.get_argument(3)) )
	{
		printf("\nInput file2= %s\n",mr.get_argument(3)) ;
	}

	if( fp.set_Output_File(1,mr.get_argument(5)) )
	{
		printf("\nOutput file1 = %s\n\n",mr.get_argument(5)) ;
	}
	analysis.do_calc(fp.get_Input_File(1),fp.get_Input_File(2),fp.get_Output_File(1),frame_max,1000) ;
	fp.close_Files() ;

	return ;
}

void Calc_Ave_Force_In_Domain::Check_Domain(FILE *fp_input_pos, FILE *fp_indomain, int frame_max)
{
	double r[XYZ] ;
	int frame = 0 ;
	int start_frame_in_domain ;
	double dummy_dbl ;
	bool continue_in_domain = false ;
	int in_domain_check = 1 ;
	int in_domain_num = 0 ;

	printf("\nNow calculating.\n\n") ;

	while(frame<=frame_max)
	{
		fscanf(fp_input_pos,"%lf %lf %lf %lf",&dummy_dbl, &r[X], &r[Y], &r[Z] ) ;
		in_domain_check = 1 ;
		
		/* check if the COM position is in the domain or not */
		for(int dim=0;dim<XYZ;dim++)
		{
			if( (r[dim]<r_min[dim]) || (r_max[dim]<r[dim]) )
			{
				in_domain_check *= 0 ;
			}
		}
		
		/* if the COM position is in the domain, save the frame number */
		if(in_domain_check)
		{
			if(!continue_in_domain)
			{
				start_frame_in_domain = frame ;
				fprintf(fp_indomain,"%d\t",start_frame_in_domain) ;
				continue_in_domain = true ;
			}

			in_domain_num++ ;

		}
		else if(continue_in_domain)
		{
			fprintf(fp_indomain,"%d\n",frame-start_frame_in_domain) ;
			continue_in_domain = false ;
		}

		frame++ ;
	}

	if(in_domain_check && continue_in_domain)
	{
		fprintf(fp_indomain,"%d\n",frame-start_frame_in_domain) ;
	}

	printf("\nframe num in the domain = %d\t[frame] (%.1f\%)",in_domain_num,((double)in_domain_num)/((double)frame_max)*100.0) ;
	printf("\n                        = %f\t[ps]",in_domain_num*dt) ;
	printf("\n                        = %f\t[ns]",in_domain_num*dt/1000.0) ;

	return ;
}

void Calc_Ave_Force_In_Domain::set_Domain(double x_min,double y_min,double z_min,double x_max,double y_max,double z_max)
{
	r_min[X] = x_min ;
	r_min[Y] = y_min ;
	r_min[Z] = z_min ;

	r_max[X] = x_max ;
	r_max[Y] = y_max ;
	r_max[Z] = z_max ;

	return ;
}

void Calc_Ave_Force_In_Domain::set_Dt(double time)
{
	dt = time ;
	return ;
}

void Calc_Ave_Force_In_Domain::set_Domain_From_File(FILE *input)
{
	for(int dim=0;dim<XYZ;dim++)
	{
		fscanf(input,"%lf %lf",&r_min[dim],&r_max[dim]) ;
	}

	return ;
}

void Calc_Ave_Force_In_Domain::do_calc(FILE *fp_indomain, FILE *fp_input_force, FILE *fp_aveforce, int frame_max, int nstout)
{
	double f[XYZ] ;
	double f_sum_tmp[XYZ] = {} ;
	double f_sum_all[XYZ] = {} ;
	double f_ave[XYZ] = {} ;
	double dummy_dbl ;
	int frame = 0 ;
	int start_frame_in_domain ;
	int period_in_domain ;
	int end_frame_in_domain = -1 ;
	int in_domain_num = 0 ;
	int output_check = 0 ;

	while(frame<=frame_max)
	{
		fscanf(fp_input_force,"%lf %lf %lf %lf",&dummy_dbl, &f[X], &f[Y], &f[Z] ) ;
//		//printf("in_domain_num=%d, f[%d]=%f, f[%d]=%f, f[%d]=%f\n",in_domain_num,X,f[X],Y,f[Y],Z,f[Z]) ; //DEBUG


		if(frame==end_frame_in_domain+1)
		{
			fscanf(fp_indomain,"%d %d",&start_frame_in_domain,&period_in_domain) ;
			end_frame_in_domain = start_frame_in_domain + period_in_domain - 1 ;
		}

		if( (start_frame_in_domain<=frame) && (frame<=end_frame_in_domain) )
		{
			for(int dim=0;dim<XYZ;dim++)
			{
				f_sum_tmp[dim] += f[dim] ;
			}
			in_domain_num++ ;
		}
	
		if( (in_domain_num%nstout==0) && (in_domain_num!=output_check) )
		{
			fprintf(fp_aveforce,"%d\t",in_domain_num) ;

			for(int dim=0;dim<XYZ;dim++)
			{
				f_sum_all[dim] += f_sum_tmp[dim] ;
				f_ave[dim] = f_sum_all[dim] / in_domain_num ;
				fprintf(fp_aveforce," %g",f_ave[dim]) ;
//				printf(" %g",f_ave[dim]) ; //DEBUG
				f_sum_tmp[dim] = 0.0 ;
			}

			fprintf(fp_aveforce,"\n") ;
//			printf("\n") ; //DEBUG
			output_check = in_domain_num ;
		}
		frame++ ;
	}
	
	return ;
}

void Calc_Ave_Force_In_Domain::do_Gather_Data(Run_Manager mr, FileIO fp)
{
	FILE *fp_input ;
	FILE *fp_output ;
	double f[XYZ] ;
	double f_sum[XYZ] = {} ;
	double f_ave[XYZ] = {} ;
	double dummy_dbl ;
	int frame = 0 ;
	int in_domain_num = 0 ;
	int in_domain_num_sum = 0 ;

	if( fp.set_Input_File(1,mr.get_argument(2)) )
	{
		fp_input = fp.get_Input_File(1) ;
	}

	if( fp.set_Output_File(1,mr.get_argument(3)) )
	{
		fp_output = fp.get_Output_File(1) ;
	}

	int frame_max = fp.get_LineNum_Input_File(1) - 1 ;

	while(frame<=frame_max)
	{
		fscanf(fp_input,"%d %lf %lf %lf",&in_domain_num, &f[X], &f[Y], &f[Z] ) ;

		in_domain_num_sum += in_domain_num ;

		for(int dim=0;dim<XYZ;dim++)
		{
			f_sum[dim] += f[dim] * in_domain_num ;
		}

		fprintf(fp_output,"%d\t%d\t",frame+1,in_domain_num_sum) ;

		for(int dim=0;dim<XYZ;dim++)
		{
			f_ave[dim] = f_sum[dim] / in_domain_num_sum ;
			fprintf(fp_output," %g",f_ave[dim]) ;
		}

		fprintf(fp_output,"\n") ;

		frame++ ;

	}


	return ;
}
