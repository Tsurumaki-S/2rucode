#ifndef __RUN_MANAGER__
#define __RUN_MANAGER__
#include "enum_manager.h"
#include "array.h"
#include "file.h"

class Run_Manager
{
	private:
		/* add new commandline argument this line. */
		ENUM(analy_tag, avefrc_indomain, avefrc_gather, format_gro, ave_aved_value, test_mpi)
		analy_tag::type mode ;
		char **argument ;

	public:
		Run_Manager(int argc, char **argv);
		bool loadAnalyMode(const char *mode_char);
		bool doAnaly(Run_Manager mr, FileIO fp);
		bool enum_test_print();
		char *get_argument(int num) ;
};

#endif

