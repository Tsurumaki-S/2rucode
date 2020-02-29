#include <stdio.h>
#include "../header/file.h"
//#include "../header/format_gro.h"
#include "../header/run_manager.h"
//#include "../header/array.h"
#include "../header/timer.h"

int main(int argc, char *argv[])
{
	/* timer start */
	Timer tr ;

	Run_Manager manager(argc,argv) ;

	FileIO fp ;

	manager.loadAnalyMode(manager.get_argument(1)) ;
	manager.doAnaly(manager,fp) ;
	//manager.enum_test_print() ;

	/* timer stop and display */
	tr.stop() ;
	printf("\nprocessing %s\n",tr.display_time()) ;
	
	return 0;
}
