#ifndef __TEST_MPI__
#define __TEST_MPI__
#include "const.h"
#include "run_manager.h"


class Test_Mpi
{
	public:
		void do_test(Run_Manager mr, FileIO fp) ;

	private:
		void test_mpi_reduce(FILE *fp_output, int id, int proc_num) ;
} ;

#endif
