#include <stdio.h>
#include "../header/file.h"
//#include "../header/format_gro.h"
#include "../header/run_manager.h"
//#include "../header/array.h"

int main(int argc, char *argv[])
{

	Run_Manager manager(argc,argv) ;

	FileIO fp ;

	manager.loadAnalyMode(manager.get_argument(1)) ;
	manager.doAnaly(manager,fp) ;
	//manager.enum_test_print() ;
	
	return 0;
}
