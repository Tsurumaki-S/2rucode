#include <stdio.h>
#include <mpi.h>
#include <vector>
//#include <thread>
//#include <chrono>
#include <unistd.h>
#include "../header/const.h"
#include "../header/file.h"
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

	/* show status */
	detect_and_show_proc(&fp, &id, &proc_num) ;

	/* test calclation with MPI */
	test_mpi_reduce(fp_output,id,proc_num);
	
	/* sleep for 20 seconds */
	if(id==0) printf("Now sleeping for 20 seconds.") ;
	MPI_Barrier(MPI_COMM_WORLD) ;
	//sleep(20);
	//std::this_thread::sleep_for(std::chrono::seconds(20)); // c++11

	MPI_Barrier(MPI_COMM_WORLD) ;

	return ;
}

void Test_Mpi::detect_and_show_proc(FileIO *fp, int *id, int *proc_num)
{
	MPI_Comm_rank(MPI_COMM_WORLD,id) ;
	MPI_Comm_size(MPI_COMM_WORLD,proc_num) ;

	if( *id == 0 )
	{
		show_proc_info(fp, *proc_num) ;
		show_file_info(fp) ;
	}

	show_host_info(fp,*id,*proc_num) ;

	return ;
}

void Test_Mpi::show_proc_info(FileIO *fp, int proc_num)
{
	int n;
	char msg[BUFMAX_CHAR_LARGE] ;

	FILE *fp_output = fp->get_Output_File(1) ;

	n = snprintf(msg,BUFMAX_CHAR_LARGE,"Total %d [process] detected.\n\n",proc_num) ;
	printf(msg) ; fprintf(fp_output,msg) ;
}

void Test_Mpi::show_file_info(FileIO *fp)
{
	int n;
	char msg[BUFMAX_CHAR_LARGE] ;

	FILE *fp_output = fp->get_Output_File(1) ;
	FILE *fp_input  = fp->get_Input_File(1) ;

	n = snprintf(msg,BUFMAX_CHAR_LARGE,"Input  file = %s\n",fp->get_Input_FileName(1)) ;
	printf(msg) ; fprintf(fp_output,msg) ;

	n = snprintf(msg,BUFMAX_CHAR_LARGE,"Output file = %s\n\n",fp->get_Output_FileName(1)) ;
	printf(msg) ; fprintf(fp_output,msg) ;

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
	char **hostnames      = ALLOC_MATRIX(char,proc_num,BUFMAX_CHAR_LARGE) ;
	char **temp_hostname  = ALLOC_MATRIX(char,proc_num,BUFMAX_CHAR_LARGE) ;
	char **ipaddress      = ALLOC_MATRIX(char,proc_num,BUFMAX_CHAR_LARGE) ;
	char **temp_ipaddress = ALLOC_MATRIX(char,proc_num,BUFMAX_CHAR_LARGE) ;

	char temp_name[BUFMAX_CHAR_LARGE] ;
	char temp_command[BUFMAX_CHAR_LARGE] ;
	int n ;
	n = snprintf(temp_name,BUFMAX_CHAR_LARGE,"temp_%05d.txt",id) ;

	/* search hostname */
	n = snprintf(temp_command,BUFMAX_CHAR_LARGE,"hostname > temp_%05d.txt",id) ;
	system(temp_command) ;

	/* search IP address */
	n = snprintf(temp_command,BUFMAX_CHAR_LARGE,"host $(hostname) >> temp_%05d.txt",id) ;
	system(temp_command) ;

	/* get hostname and IP address */
	FILE *fp_temp=fopen(temp_name,"r") ;
	fgets(temp_hostname[id] ,BUFMAX_CHAR_LARGE,fp_temp) ;
	fgets(temp_ipaddress[id],BUFMAX_CHAR_LARGE,fp_temp) ;
	fclose(fp_temp) ;
	remove(temp_name) ;

	/* gather data from ench proc to proc No.0 */
	MPI_Gather(&temp_hostname[id][0],BUFMAX_CHAR_LARGE,MPI_CHAR,&hostnames[id][0],BUFMAX_CHAR_LARGE,MPI_CHAR,0,MPI_COMM_WORLD) ;
	MPI_Gather(&temp_ipaddress[id][0],BUFMAX_CHAR_LARGE,MPI_CHAR,&ipaddress[id][0],BUFMAX_CHAR_LARGE,MPI_CHAR,0,MPI_COMM_WORLD) ;

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
void Test_Mpi::test_mpi_reduce(FILE *fp_output, int id, int proc_num)
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
		printf(msg) ; fprintf(fp_output,msg) ;

		if( test_calc_sum == test_calc_sum__true_value )
		{
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"OK\n") ;
			printf(msg) ; fprintf(fp_output,msg) ;
		}
		else if( test_calc_sum == -1 )
		{
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"ERROR!\n") ;
			printf(msg) ; fprintf(fp_output,msg) ;
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"MPI function \"MPI_Reduce\" can't execute normally.\n") ;
			printf(msg) ; fprintf(fp_output,msg) ;
		}
		else
		{
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"ERROR!\n") ;
			printf(msg) ; fprintf(fp_output,msg) ;
			n = snprintf(msg,BUFMAX_CHAR_LARGE,"MPI function \"MPI_Reduce\" can't execute normally.\n") ;
			printf(msg) ; fprintf(fp_output,msg) ;
		}

	}

	return ;
}

