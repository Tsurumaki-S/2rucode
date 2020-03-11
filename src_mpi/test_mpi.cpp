#include <stdio.h>
#include <mpi.h>
#include <vector>
//#include <thread>
//#include <chrono>
#include <unistd.h>
#include <omp.h>
#include "../header/const.h"
#include "../header/file.h"
#include "../header/common_function.h"
#include "../header/test_mpi.h"

void Test_Mpi::do_test(Run_Manager mr, FileIO fp)
{
	/* file set */
	FILE *fp_input  ;
	FILE *fp_output ;
	fp.set_Input_File(1,mr.get_argument(2)) ;
	fp.set_Output_File(1,mr.get_argument(3)) ;
	fp_input  = fp.get_Input_File(1) ;
	fp_output = fp.get_Output_File(1) ;

	/* MPI process check */
	int id, proc_num ;

	/* show job information ( process and thread ) */
	show_job_info(&fp, &id, &proc_num) ;

	/* test calclation with MPI */
	test_calc_mpi(fp_output,id,proc_num);
	MPI_Barrier(MPI_COMM_WORLD) ;

	/* test calclation with hybrid parallel (MPI and OpenMP) */
	test_calc_mpi_openmp_hybrid(fp_output,id,proc_num);
	MPI_Barrier(MPI_COMM_WORLD) ;
	
	/* sleep for 20 seconds */
	if(id==0) printf("Now sleeping for 20 seconds.") ;
	MPI_Barrier(MPI_COMM_WORLD) ;
	//sleep(20);
	//std::this_thread::sleep_for(std::chrono::seconds(20)); // c++11

	MPI_Barrier(MPI_COMM_WORLD) ;

	return ;
}

void Test_Mpi::show_job_info(FileIO *fp, int *id, int *proc_num)
{
	MPI_Comm_rank(MPI_COMM_WORLD,id) ;
	MPI_Comm_size(MPI_COMM_WORLD,proc_num) ;

	if( *id == 0 )
	{
		show_proc_info(fp, *proc_num) ;
		show_thread_info(fp) ;
		show_thread_per_proc(fp) ;
		show_file_info(fp) ;
	}

	show_host_info(fp,*id,*proc_num) ;

	#pragma omp parallel for
	for(int i=0;i<4;i++)
	{
		printf("\nproc# %3d\nthread# %3d\n",*id,omp_get_thread_num());
	}

	return ;
}

void Test_Mpi::show_proc_info(FileIO *fp, int proc_num)
{
	int n;
	char msg[BUFMAX_CHAR_LARGE] ;

	FILE *fp_output = fp->get_Output_File(1) ;

	n = snprintf(msg,BUFMAX_CHAR_LARGE,"Total %4d [process] detected.\n\n",proc_num) ;
	print_std_and_fp(msg,fp_output);

	return ;
}

void Test_Mpi::show_thread_info(FileIO *fp)
{
	int n;
	char msg[BUFMAX_CHAR_LARGE] ;

	FILE *fp_output = fp->get_Output_File(1) ;

	n = snprintf(msg,BUFMAX_CHAR_LARGE,"Total %4d [thread] detected.\n\n",omp_get_max_threads()) ;
	print_std_and_fp(msg,fp_output);

	return ;
}

void Test_Mpi::show_thread_per_proc(FileIO *fp)
{
	FILE *fp_output = fp->get_Output_File(1) ;

	int proc_num ;
	MPI_Comm_size(MPI_COMM_WORLD,&proc_num) ;
	int thread_num = omp_get_max_threads() ;
	int thread_pre_proc = thread_num / proc_num ;

	char msg[BUFMAX_CHAR_LARGE] ;
	int n = snprintf(msg,BUFMAX_CHAR_LARGE,"  ==> %4d [thread/process]\n\n",thread_pre_proc) ;
	print_std_and_fp(msg,fp_output);

	return ;
}


void Test_Mpi::show_file_info(FileIO *fp)
{
	int n;
	char msg[BUFMAX_CHAR_LARGE] ;

	FILE *fp_output = fp->get_Output_File(1) ;
	FILE *fp_input  = fp->get_Input_File(1) ;

	n = snprintf(msg,BUFMAX_CHAR_LARGE,"Input  file = %s\n",fp->get_Input_FileName(1)) ;
	print_std_and_fp(msg,fp_output);

	n = snprintf(msg,BUFMAX_CHAR_LARGE,"Output file = %s\n\n",fp->get_Output_FileName(1)) ;
	print_std_and_fp(msg,fp_output);

	/* copy 1st line */
	for(int i=0;i<1;i++)
	{
		char one_line[BUFMAX_CHAR_LARGE] ;
		fgets(one_line,1024,fp_input) ;
		fprintf(fp_output,"Input file contents :\n%s\n",one_line ) ;
	}
}

void Test_Mpi::show_host_info(FileIO *fp, int id, int proc_num)
{
	/* make temporary file name */
	//char tmp_name[L_tmpnam+1];
	//tmpnam(tmp_name);
	char tmp_name[]={"tmp.XXXXXXXXXX"} ;
	mkstemp(tmp_name) ;

	int n ;
	char tmp_command[BUFMAX_CHAR_LARGE] ;

	/* search hostname */
	n = snprintf(tmp_command,BUFMAX_CHAR_LARGE,"hostname > %s",tmp_name) ;
	system(tmp_command) ;

	/* search IP address */
	n = snprintf(tmp_command,BUFMAX_CHAR_LARGE,"host $(hostname) >> %s",tmp_name) ;
	system(tmp_command) ;

	/* set info from temporary files to arrays */
	FILE *fp_tmp;
	if( (fp_tmp=fopen(tmp_name,"r")) == NULL )
	{
		fprintf(stderr,"\nMaking temporary files failed.\n") ;
		exit(EXIT_FAILURE);
	}
	char **hostnames     = ALLOC_MATRIX(char,proc_num,BUFMAX_CHAR_LARGE) ;
	char **tmp_hostname  = ALLOC_MATRIX(char,proc_num,BUFMAX_CHAR_LARGE) ;
	char **ipaddress     = ALLOC_MATRIX(char,proc_num,BUFMAX_CHAR_LARGE) ;
	char **tmp_ipaddress = ALLOC_MATRIX(char,proc_num,BUFMAX_CHAR_LARGE) ;
	fgets(tmp_hostname[id] ,BUFMAX_CHAR_LARGE,fp_tmp) ;
	fgets(tmp_ipaddress[id],BUFMAX_CHAR_LARGE,fp_tmp) ;
	fclose(fp_tmp) ;
	remove(tmp_name) ;

	/* gather data from ench proc to proc No.0 */
	MPI_Gather(&tmp_hostname[id][0],BUFMAX_CHAR_LARGE,MPI_CHAR,&hostnames[id][0],BUFMAX_CHAR_LARGE,MPI_CHAR,0,MPI_COMM_WORLD) ;
	MPI_Gather(&tmp_ipaddress[id][0],BUFMAX_CHAR_LARGE,MPI_CHAR,&ipaddress[id][0],BUFMAX_CHAR_LARGE,MPI_CHAR,0,MPI_COMM_WORLD) ;

	/* output */
	if(id==0)
	{
		FILE *fp_output = fp->get_Output_File(1) ;
		char hname[BUFMAX_CHAR_LARGE] ;
		for(int i=0;i<proc_num;i++)
		{
			int n = snprintf(hname,BUFMAX_CHAR_LARGE,"Proc No.%4d\nHostname  : %sIP address: %s\n\n",i,hostnames[i],ipaddress[i]) ;
			fprintf(fp_output,hname) ;
		}
	}

	return ;
}

/* test calclation with MPI */
void Test_Mpi::test_calc_mpi(FILE *fp_output, int id, int proc_num)
{
	int test_calc     = id+1 ;
	int test_calc_sum = -1 ;
	MPI_Reduce(&test_calc,&test_calc_sum,1,MPI_INT,MPI_SUM ,0,MPI_COMM_WORLD) ;

	int test_calc_sum__true_value = proc_num*(proc_num+1)/2 ;

	if( id == 0 )
	{
		char msg[BUFMAX_CHAR_LARGE] ;
		int n ;

		n = snprintf(msg,BUFMAX_CHAR_LARGE,"\nMPI TEST : ") ;
		print_std_and_fp(msg,fp_output);

		if( test_calc_sum == test_calc_sum__true_value )
		{
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"OK\n") ;
			print_std_and_fp(msg,fp_output);
		}
		else if( test_calc_sum == -1 )
		{
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"ERROR!\n") ;
			print_std_and_fp(msg,fp_output);
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"MPI function \"MPI_Reduce\" can't execute normally.\n") ;
			print_std_and_fp(msg,fp_output);
		}
		else
		{
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"ERROR!\n") ;
			print_std_and_fp(msg,fp_output);
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"MPI function \"MPI_Reduce\" can't execute normally.\n") ;
			print_std_and_fp(msg,fp_output);
		}

	}

	return ;
}

/* test calclation with hybrid parallel (MPI and OpenMP) */
void Test_Mpi::test_calc_mpi_openmp_hybrid(FILE *fp_output, int id, int proc_num)
{
	return ;
}
