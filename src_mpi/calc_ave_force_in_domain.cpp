#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <unistd.h>
#include "../header/const.h"
#include "../header/calc_ave_force_in_domain.h"
#include "../header/file.h"

void Calc_Ave_Force_In_Domain::do_Calc_Ave_Force_In_Domain(Run_Manager mr, FileIO fp)
{
	Calc_Ave_Force_In_Domain analysis ;

	/* MPI DEBUG */
	int id, proc_num ;
	MPI_Comm_rank(MPI_COMM_WORLD,&id) ;
	MPI_Comm_size(MPI_COMM_WORLD,&proc_num) ;
	printf("\n\n@do_Calc_Ave_Force_In_Domain-1 process ID = %d/%d\n\n",id,proc_num) ;
	/* MPI DEBUG */

	
	analysis.set_Dt(atof(mr.get_argument(6))) ;

	/* MPI DEBUG */
	printf("\n\n@do_Calc_Ave_Force_In_Domain-2 process ID = %d/%d\n\n",id,proc_num) ;
	/* MPI DEBUG */

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

	if(id==0)
	{
		if( fp.set_Output_File(1,mr.get_argument(4)) )
		{
			printf("\nOutput file1 = %s\n",mr.get_argument(4)) ;
		}
	}
	else
	{
		if( fp.set_Output_File(1,"temp.dummy.dat") )
		{
			printf("\nOutput file1 = %s\n","temp.dummy.dat") ;
		}
	}


//	printf("\n\n\nframe_max=%d\n\n\n",fp.get_LineNum_Input_File1()-1) ; //DEBUG
	int frame_max = fp.get_LineNum_Input_File(1) - 1 ;
	printf("\n@bofore Check_Domain  proc:%d/%d\n",id,proc_num) ;
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
	printf("\n@bofore do_calc  proc:%d/%d\n",id,proc_num) ;
	analysis.do_calc(fp.get_Input_File(1),fp.get_Input_File(2),fp.get_Output_File(1),frame_max,100) ;
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

	/* added for MPI (start) */
	//const char input_name[]="input_ascii.dat" ;
	//int line=getLine(input_name) ;

	int id, proc_num ;
	MPI_Comm_rank(MPI_COMM_WORLD,&id) ;
	MPI_Comm_size(MPI_COMM_WORLD,&proc_num) ;
	/* MPI DEBUG */
	printf("\n\n@Check_Domain process ID = %d/%d\n\n",id,proc_num) ;
	/* MPI DEBUG */

	char output_name[256] ;
	sprintf(output_name,"temp.dat.proc%d",id) ;
	FILE *fp_output ;
	fp_output = fopen(output_name,"w") ;

	int data_length_per_one_proc=(frame_max+1)/proc_num ; // Because frame_max start from 0.
	int start_frame = (id  )*data_length_per_one_proc     ;
	int end_frame   = (id+1)*data_length_per_one_proc - 1 ;
	if(id==(proc_num-1))
	{
		end_frame   = frame_max;
	}

	for(int i=0;i<start_frame;i++)
	{
		/* skip data */
		while(fgetc(fp_input_pos) != '\n');
		frame++ ;
	}
	/* added for MPI (end) */

	while(frame<=end_frame)
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
				fprintf(fp_output,"%d\t",start_frame_in_domain) ;
				continue_in_domain = true ;
			}

			in_domain_num++ ;

		}
		else if(continue_in_domain)
		{
			fprintf(fp_output,"%d\n",frame-start_frame_in_domain) ;
			continue_in_domain = false ;
		}

		frame++ ;
	}

	if(in_domain_check && continue_in_domain)
	{
		fprintf(fp_output,"%d\n",frame-start_frame_in_domain) ;
	}

	fclose(fp_output) ;

	//sleep(20) ; // MPI DEBUG
	MPI_Barrier(MPI_COMM_WORLD) ;
	printf("\n\nproc:%d finish MPI_Barrier\n",id) ; // MPI DEBUG

	FILE *fp_input ;
	if(id==0)
	{
		int data[2] ;
		char file_name[256] ;
		for(int i=0;i<proc_num;i++)
		{
			sprintf(file_name,"temp.dat.proc%d",i) ;
			if( (fp_input = fopen(file_name,"r")) != NULL )
			{
				printf("\n\nread %s\n\n",file_name) ; // MPI DEBUG
				while(fscanf(fp_input  ,"%d %d"  ,&data[0],&data[1]) != EOF )
				{
					fprintf(fp_indomain,"%d %d\n", data[0], data[1]) ;
					printf("%s %d %d\n",file_name, data[0], data[1]) ;
				}
				fclose(fp_input) ;
			}
			else
			{
				printf("\n\nCan not read %s\n\n",file_name) ; // MPI DEBUG
			}

		}

		printf("\nframe num in the domain = %d\t[frame] (%.1f\%)",in_domain_num,((double)in_domain_num)/((double)frame_max)*100.0) ;
		printf("\n                        = %f\t[ps]",in_domain_num*dt) ;
		printf("\n                        = %f\t[ns]",in_domain_num*dt/1000.0) ;
	}

	/* MPI DEBUG */
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

	/* added for MPI (start) */
	//const char input_name[]="input_ascii.dat" ;
	//int line=getLine(input_name) ;

	int id, proc_num ;
	MPI_Comm_rank(MPI_COMM_WORLD,&id) ;
	MPI_Comm_size(MPI_COMM_WORLD,&proc_num) ;
	/* MPI DEBUG */
	printf("\n\n@do_calc process ID = %d/%d\n\n",id,proc_num) ;
	/* MPI DEBUG */

	char output_name[256] ;
	sprintf(output_name,"temp.frc.dat.proc%d",id) ;
	FILE *fp_output ;
	fp_output = fopen(output_name,"w") ;

	int data_length_per_one_proc=(frame_max+1)/proc_num ; // "+1" because frame_max start from 0.
	int start_frame = (id  )*data_length_per_one_proc     ;
	int end_frame   = (id+1)*data_length_per_one_proc - 1 ;
	if(id==(proc_num-1))
	{
		end_frame   = frame_max ;
	}

	for(int i=0;i<start_frame;i++)
	{
		/* skip data */
		while(fgetc(fp_input_force) != '\n');
		frame++ ;
	}

	// /* not use rewind ver. (start) */
	//
	// FILE *fp_check_indomain_mpi ;
	// if(fp_check_indomain_mpi = fopen("indomain.dat","r"))
	// {
	// 	int data_indomain ;
	// 	int data_indomain_small=0 ;
	// 	int data_indomain_large ;
	// 	int start_line_indomain=0 ;
	//
	// 	while( ( fscanf(fp_check_indomain_mpi,"%d %d",&data_indomain_large,&data_indomain) ) != EOF )
	// 	{
	// 		start_line_indomain++ ;
	// 		
	// 		if( (data_indomain_small<=start_frame) && (start_frame<data_indomain_large) )
	// 		{
	// 			break ;
	// 		}
	// 		data_indomain_large = data_indomain_small ;
	// 	}
	// }
	// fclose(fp_check_indomain_mpi) ;
	//
	// /* not use rewind ver. (end) */
		
	// /* use rewind ver. (start) */
	// int data_indomain ;
	// int data_indomain_small=0 ;
	// int data_indomain_large ;
	// int start_line_indomain=0 ;
	// while( ( fscanf(fp_indomain,"%d %d",&data_indomain_large,&data_indomain) ) != EOF )
	// {
	// 	if( (data_indomain_small<=start_frame) && (start_frame<data_indomain_large) )
	// 	{
	// 		break ;
	// 	}
	// 	start_line_indomain++ ;
	// 	data_indomain_large = data_indomain_small ;
	// }
	// rewind(fp_indomain) ;
	// /* use rewind ver. (end) */

	
	/* added for MPI (end) */
	while(frame<=end_frame)
	{
		fscanf(fp_input_force,"%lf %lf %lf %lf",&dummy_dbl, &f[X], &f[Y], &f[Z] ) ;
//		//printf("in_domain_num=%d, f[%d]=%f, f[%d]=%f, f[%d]=%f\n",in_domain_num,X,f[X],Y,f[Y],Z,f[Z]) ; //DEBUG


		if( end_frame_in_domain < frame )
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
			fprintf(fp_output,"%d\t",in_domain_num) ;

			for(int dim=0;dim<XYZ;dim++)
			{
				f_sum_all[dim] += f_sum_tmp[dim] ;
				f_ave[dim] = f_sum_all[dim] / in_domain_num ;
				fprintf(fp_output," %g",f_ave[dim]) ;
//				printf(" %g",f_ave[dim]) ; //DEBUG
				f_sum_tmp[dim] = 0.0 ;
			}

			fprintf(fp_output,"\n") ;
//			printf("\n") ; //DEBUG
			output_check = in_domain_num ;
		}
		frame++ ;
	}
	fclose(fp_output) ;

	/* put all temp files' average force in one file */
	MPI_Barrier(MPI_COMM_WORLD) ;
	if(id==0)
	{
		FILE *fp_input ;
		int first_have_data_proc = -1 ;
		int data_num ;
		int data_num_sofar;
		double data[XYZ] ;
		double data_sofar[XYZ] ;

		for(int i=0;i<proc_num;i++)
		{
			char file_name[256] ;
			sprintf(file_name,"temp.frc.dat.proc%d",i) ;
			if( ( (fp_input = fopen(file_name,"r")) != NULL ) && (first_have_data_proc==(-1)) )
			{
				printf("\nchecking %s\n",file_name) ;
				while(fscanf(fp_input  ,"%d %lf %lf %lf",&data_num,&data[X],&data[Y],&data[Z]) != EOF )
				{
					fprintf(fp_aveforce,"%d %f %f %f\n",data_num,data[X],data[Y],data[Z] ) ;
					first_have_data_proc = i ;
				}
			}
		}

		/* copy data */
		for(int dim=0;dim<XYZ;dim++)
		{
			data_sofar[dim] = data[dim] ;
		}
		data_num_sofar = data_num ;

		/* put all temp files' average force in one file */
		double new_avefrc[XYZ] ;
		for(int i=first_have_data_proc+1;i<proc_num;i++)
		{
			char file_name[256] ;
			sprintf(file_name,"temp.frc.dat.proc%d",i) ;
			if( (fp_input = fopen(file_name,"r")) != NULL )
			{
				while(fscanf(fp_input  ,"%d %lf %lf %lf",&data_num,&data[X],&data[Y],&data[Z]) != EOF )
				{
					for(int dim=0;dim<XYZ;dim++)
					{
						new_avefrc[dim] = (data_num_sofar*data_sofar[dim]+nstout*data[dim])/(data_num_sofar+nstout) ;
						data_sofar[dim] = new_avefrc[dim] ;
					}
					fprintf(fp_aveforce,"%d %f %f %f\n", data_num_sofar+nstout,new_avefrc[X],new_avefrc[Y],new_avefrc[Z]) ;
					data_num_sofar += nstout ;
				}
				fclose(fp_input) ;
			}
		}

	}// if(id==0)
	
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
