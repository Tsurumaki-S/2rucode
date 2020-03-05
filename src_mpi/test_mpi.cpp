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
	int id, proc_num ;
	MPI_Comm_rank(MPI_COMM_WORLD,&id) ;
	MPI_Comm_size(MPI_COMM_WORLD,&proc_num) ;

	FILE *fp_input  ;
	FILE *fp_output ;

	if( fp.set_Input_File(1,mr.get_argument(2)) && fp.set_Output_File(1,mr.get_argument(3)) )
	{
		fp_input  = fp.get_Input_File(1) ;
		fp_output = fp.get_Output_File(1) ;

		if( id == 0 )
		{
			char msg[BUFMAX_CHAR_LARGE] ;
			int n ;

			n = snprintf(msg,BUFMAX_CHAR_LARGE,"Total %d [process] detected.\n\n",proc_num) ;
			printf(msg) ; fprintf(fp_output,msg) ;

			n = snprintf(msg,BUFMAX_CHAR_LARGE,"Input  file = %s\n",fp.get_Input_FileName(1)) ;
			printf(msg) ; fprintf(fp_output,msg) ;

			n = snprintf(msg,BUFMAX_CHAR_LARGE,"Output file = %s\n\n",fp.get_Output_FileName(1)) ;
			printf(msg) ; fprintf(fp_output,msg) ;

			/* copy 1st line */
			for(int i=0;i<1;i++)
			{
				char one_line[BUFMAX_CHAR_LARGE] ;
				fgets(one_line,1024,fp_input) ;
				fprintf(fp_output,"Input file contents :\n%s\n",one_line ) ;
			}

			//for(int i = 0;i<10000000;i++)
			//{
			//	int dummy ;
			//	dummy = id + i ;
			//	dummy = id - i ;
			//	dummy = id * i ;
			//}
		}

		/* test calclation with MPI */
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

			printf("Now sleeping for 20 seconds.") ;
		}
		
		/* sleep for 20 seconds */
		MPI_Barrier(MPI_COMM_WORLD) ;
		sleep(20);
		//std::this_thread::sleep_for(std::chrono::seconds(20)); // c++11
	}

	MPI_Barrier(MPI_COMM_WORLD) ;

	return ;
}
