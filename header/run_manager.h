#ifndef __RUN_NAMAGER__
#define __RUN_NAMAGER__
#include "enum_manager.h"
#include "array.h"
#include "file.h"

class Run_Manager
{
	private:
		/* add new commandline argument this line. */
		ENUM(analy_tag, avefrc_indomain, avefrc_gather, format_gro)
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

