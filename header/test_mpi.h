#ifndef __TEST_MPI__
#define __TEST_MPI__
#include "const.h"
#include "run_manager.h"


class Test_Mpi
{
	public:
		void do_test(Run_Manager mr, FileIO fp) ;

	private:
		void show_job_info(FileIO *fp, int *id, int *proc_num) ;
		void show_proc_info(FileIO *fp, int proc_num) ;
		void show_thread_info(FileIO *fp) ;
		void show_thread_per_proc(FileIO *fp) ;
		void show_file_info(FileIO *fp) ;
		void show_host_info(FileIO *fp, int id, int proc_num) ;
		void test_calc_mpi(FILE *fp_output, int id, int proc_num) ;
		void test_calc_mpi_openmp_hybrid(FILE *fp_output, int id, int proc_num) ;
} ;

#endif
