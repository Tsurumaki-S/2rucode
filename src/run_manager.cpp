#include <stdio.h>
#include <string.h>
#include "../header/run_manager.h"
#include "../header/const.h"
#include "../header/calc_ave_force_in_domain.h"
#include "../header/format_gro.h"

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
        printf("\nError: Incorrect argument.\n");
        return false;
    }

    return true;
}

bool Run_Manager::doAnaly(Run_Manager mr, FileIO fp)
{
	if(mode==analy_tag.converter["avefrc_indomain"])
	{
		printf("\nmode = avefrc_indomain (%d)\n",mode) ;
		Calc_Ave_Force_In_Domain analy ;
		analy.do_Calc_Ave_Force_In_Domain(mr,fp) ;
	}
	else if (mode==analy_tag.converter["format_gro"])
	{
		printf("\nmode = format_gro (%d)\n"     ,mode) ;
		Format_Gro analy ;
		analy.format_Gro(mr,fp) ;
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
