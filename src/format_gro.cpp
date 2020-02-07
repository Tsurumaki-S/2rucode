#include <stdio.h>
#include "../header/const.h"
#include "../header/file.h"
#include "../header/format_gro.h"

/* modify gro file format */

void Format_Gro::format_Gro(Run_Manager mr, FileIO fp)
{
	if( fp.set_Input_File(1,mr.get_argument(2)) )
	{
		printf("\nInput file1 = %s\n",mr.get_argument(2)) ;
	}

	if( fp.set_Output_File(1,mr.get_argument(3)) )
	{
		printf("\nOutput file1 = %s\n",mr.get_argument(3)) ;
	}

	char one_line[1024] ;
	int  residue_num ;
	char residue_name[6] ;
	char residue_name_new[6] ;
	char atom_name[6] ;
	char atom_name_new[6] ;
	int  atom_num ;
	double r[XYZ] ;

	FILE *fp_input = fp.get_Input_File(1) ;
	FILE *fp_output = fp.get_Output_File(1) ;

	int atom_num_modified = 1 ;
	int frame = 1 ;

	int atom_num_max = fp.get_LineNum_Input_File(1) - 3 ;

//	/* skip 1st and 2nd line */
//	while(fgetc(fp_input) != '\n');
//	while(fgetc(fp_input) != '\n');

	/* copy 1st and 2nd line */
	for(int i=0;i<2;i++)
	{
		fgets(one_line,1024,fp_input) ;
		fprintf(fp_output,"%s",one_line ) ;
	}

	while(frame<=atom_num_max)
	{
//		fscanf(fp_input,  "%5d%5s%5s%5d %lf %lf %lf",&residue_num, residue_name, atom_name, &atom_num, &r[X], &r[Y], &r[Z] ) ;

		fscanf(fp_input,"%d",&residue_num) ;
		fgets(residue_name,6,fp_input) ;
		fgets(atom_name,6,fp_input) ;
		fscanf(fp_input,  "%d %lf %lf %lf",&atom_num, &r[X], &r[Y], &r[Z] ) ;
		int j ;

		j=0 ;
		for(int i=0;i<6;i++)
		{
			if(residue_name[i]!=' ')
			{
				residue_name_new[j]=residue_name[i] ;
				j++ ;
			}
		}

		j=0 ;
		for(int i=0;i<6;i++)
		{
			if(atom_name[i]!=' ')
			{
				atom_name_new[j]=atom_name[i] ;
				j++ ;
			}
		}

		if(atom_num_modified==100000)
		{
			atom_num_modified = 0 ;
		}

		atom_num = atom_num_modified ;
		fprintf(fp_output,"%5d%-5s%5s%5d%8.3f%8.3f%8.3f\n",residue_num, residue_name_new, atom_name_new, atom_num, r[X], r[Y], r[Z] ) ;

		atom_num_modified++ ;
		frame++ ;
	}

	/* copy the last one line */
	for(int i=0;i<1;i++)
	{
		while(fgetc(fp_input) != '\n');
		fgets(one_line,1024,fp_input) ;
		fprintf(fp_output,"%s",one_line ) ;
	}

	return ;
}
