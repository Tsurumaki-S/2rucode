#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include "../header/file.h"
//#include "../header/format_gro.h"
#include "../header/run_manager.h"
//#include "../header/array.h"


int main(int argc, char *argv[])
{

	MPI_Init(&argc,&argv) ;
	int id, proc_num ;
	MPI_Comm_rank(MPI_COMM_WORLD,&id) ;
	MPI_Comm_size(MPI_COMM_WORLD,&proc_num) ;

	/* get start time */
	clock_t time_start_main ;
	if(id==0) time_start_main = clock() ;

	Run_Manager manager(argc,argv) ;

	FileIO fp ;

	manager.loadAnalyMode(manager.get_argument(1)) ;

	/* MPI DEBUG */
	//printf("\n\n@main-2 process ID = %d/%d\n\n",id,proc_num) ;
	/* MPI DEBUG */

	manager.doAnaly(manager,fp) ;
	//manager.enum_test_print() ;
	
	/* get finish time and calculate processing time */
	clock_t time_finish_main ;
	if(id==0)
	{
		time_finish_main = clock() ;
		const double all_time = static_cast<double>(time_finish_main-time_start_main) / CLOCKS_PER_SEC ;
		printf("\nprocessing time: %f [second] (%f [minute], %f [hour])\n\n",all_time,all_time/60.0,all_time/3600.0) ;
	}

	MPI_Finalize() ;
	
	return 0;
}
