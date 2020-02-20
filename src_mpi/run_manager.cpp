#include <stdio.h>
#include <string.h>
#include <mpi.h>
#include "../header/run_manager.h"
#include "../header/const.h"
#include "../header/calc_ave_force_in_domain.h"
#include "../header/format_gro.h"
#include "../header/test_mpi.h"

Run_Manager::Run_Manager(int argc, char **argv)
{
	argument = ALLOC_MATRIX(char,argc,BUFMAX_CHAR) ;
	for(int i=0;i<argc;i++)
	{
		strcpy(argument[i],argv[i]) ;
	}
}

bool Run_Manager::loadAnalyMode(const char *mode_char) {

    if (analy_tag.converter.find(mode_char) != analy_tag.converter.end()) {
        mode = analy_tag.converter[mode_char];
    } else {
        printf("\n  Error: Incorrect argument.\n\n");
		exit(1) ;
    }

    return true;
}

bool Run_Manager::doAnaly(Run_Manager mr, FileIO fp)
{
	int id, proc_num ;
	MPI_Comm_rank(MPI_COMM_WORLD,&id) ;
	MPI_Comm_size(MPI_COMM_WORLD,&proc_num) ;

	if(mode==analy_tag.converter["avefrc_indomain"])
	{
		printf("\nmode = avefrc_indomain (%d)\n",mode) ;

		printf("\n\n@doAnaly-1 process ID = %d/%d\n\n",id,proc_num) ;
		/* MPI DEBUG */

		Calc_Ave_Force_In_Domain analy ;

		/* MPI DEBUG */
		printf("\n\n@doAnaly-2 process ID = %d/%d\n\n",id,proc_num) ;
		/* MPI DEBUG */

		analy.do_Calc_Ave_Force_In_Domain(mr,fp) ;
	}
	else if (mode==analy_tag.converter["avefrc_gather"])
	{
		printf("\nmode = avefrc_gather (%d)\n"     ,mode) ;
		Calc_Ave_Force_In_Domain analy ;
		analy.do_Gather_Data(mr,fp) ;
	}
	else if (mode==analy_tag.converter["format_gro"])
	{
		printf("\nmode = format_gro (%d)\n"     ,mode) ;
		Format_Gro analy ;
		analy.format_Gro(mr,fp) ;
	}
	else if (mode==analy_tag.converter["test_mpi"])
	{
		if(id==0) printf("\nmode = test_mpi (%d)\n",mode) ;
		Test_Mpi analy ;
		analy.do_test(mr,fp) ;
	}

	return true ;
}

bool Run_Manager::enum_test_print()
{

	if(mode==analy_tag.converter["avefrc_indomain"])
	{
		printf("\nmode = avefrc_indomain (%d)\n",mode) ;
	}
	else if (mode==analy_tag.converter["format_gro"])
	{
		printf("\nmode = format_gro (%d)\n"     ,mode) ;
	}

	return true ;
}

char *Run_Manager::get_argument(int num)
{
	return argument[num] ;
}
