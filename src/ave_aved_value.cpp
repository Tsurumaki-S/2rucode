#include <stdio.h>
#include <stdlib.h>
#include "../header/const.h"
#include "../header/ave_aved_value.h"
#include "../header/file.h"


#define ARRAY_NUM 1

/* calclate average of averaged value */

void Ave_Aved_Value::do_Ave_Aved_Value(Run_Manager mr, FileIO fp)
{
	FILE *fp_input ;
	FILE *fp_output ;
	double value[ARRAY_NUM] ;
	double sum_value[ARRAY_NUM] = {} ;
	double ave_value[ARRAY_NUM] = {} ;
	double dummy_dbl ;
	int frame = 0 ;
	int     num_of_used_for_average = 0 ;
	int sum_num_of_used_for_average = 0 ;

	if( fp.set_Input_File(1,mr.get_argument(2)) )
	{
		fp_input = fp.get_Input_File(1) ;
	}

	if( fp.set_Output_File(1,mr.get_argument(3)) )
	{
		fp_output = fp.get_Output_File(1) ;
	}

	int frame_max = fp.get_LineNum_Input_File(1) - 1 ;

	while(frame<=frame_max)
	{
		fscanf(fp_input,"%d %lf",&num_of_used_for_average, &value[0] ) ;

		sum_num_of_used_for_average += num_of_used_for_average ;

		for(int dim=0;dim<ARRAY_NUM;dim++)
		{
			sum_value[dim] += value[dim] * num_of_used_for_average ;
		}

		fprintf(fp_output,"%d\t%d\t",frame+1,sum_num_of_used_for_average) ;

		for(int dim=0;dim<ARRAY_NUM;dim++)
		{
			ave_value[dim] = sum_value[dim] / sum_num_of_used_for_average ;
			fprintf(fp_output," %g",ave_value[dim]) ;
		}

		fprintf(fp_output,"\n") ;

		frame++ ;

	}


	return ;
}
