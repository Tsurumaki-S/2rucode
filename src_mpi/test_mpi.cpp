#include <stdio.h>
#include <mpi.h>
#include <vector>
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
		}
	}


//	/* copy the last one line */
//	for(int i=0;i<1;i++)
//	{
//		while(fgetc(fp_input) != '\n');
//		fgets(one_line,1024,fp_input) ;
//		fprintf(fp_output,"%s",one_line ) ;
//	}

	MPI_Barrier(MPI_COMM_WORLD) ;

	return ;
}
