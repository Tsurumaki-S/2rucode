#include <stdio.h>
#include <mpi.h>
#include "../header/file.h"
#include "../header/run_manager.h"
#include "../header/timer.h"


int main(int argc, char *argv[])
{

	MPI_Init(&argc,&argv) ;
	int id, proc_num ;
	MPI_Comm_rank(MPI_COMM_WORLD,&id) ;
	MPI_Comm_size(MPI_COMM_WORLD,&proc_num) ;

	/* timer start */
	Timer tr ;

	Run_Manager mr(argc,argv) ;

	FileIO fp ;

	mr.loadAnalyMode(mr.get_argument(1)) ;

	mr.doAnaly(mr,fp) ;
	
	/* timer stop */
	tr.stop() ;
	if(id==0) printf("\nprocessing %s\n",tr.display_time()) ;

	MPI_Finalize() ;
	
	return 0;
}
