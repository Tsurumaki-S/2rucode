#include "../header/file.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FileIO::FileIO()
{
	for(int i=0; i<OPEN_FILE_NUM; i++)
	{
		input_check_open[i] = false ;
		output_check_open[i] = false ;
		input_line_num[i] = -1 ;
		output_line_num[i] = -1 ;
	}
}

FileIO::~FileIO()
{
	FileIO::close_Files() ;
}

bool FileIO::set_Input_File(int filenum, const char *file_name)
{
	input_line_num[filenum] = FileIO::setLine(file_name) ;

	if( fp_input[filenum]=fopen(file_name,"r") )
	{
		input_check_open[filenum] = 1 ;
		strcpy(input_filename[filenum],file_name) ;
		return true ;
	}
	else
	{
		fprintf(stderr,"\n  ERROR: Can not open the file \"%s\"\n\n",file_name) ;
		exit(1) ;
	}
}

char* FileIO::get_Input_FileName(int filenum)
{
	return input_filename[filenum] ;
}

bool FileIO::set_Output_File(int filenum, const char *file_name)
{
	if( fp_output[filenum]=fopen(file_name,"w") )
	{
		output_check_open[filenum] = 1 ;
		strcpy(output_filename[filenum],file_name) ;
		return true ;
	}
	else
	{
		fprintf(stderr,"\nERROR!\nCan not open the file \"%s\"\n",file_name) ;
		exit(1) ;
	}
}

char* FileIO::get_Output_FileName(int filenum)
{
	return output_filename[filenum] ;
}

bool FileIO::close_Files()
{
	for(int filenum=0; filenum<OPEN_FILE_NUM; filenum++)
	{
		if(input_check_open[filenum])
		{
			fclose(fp_input[filenum]) ;
			input_check_open[filenum] = false ;
		}

		if(output_check_open[filenum])
		{
			fclose(fp_output[filenum]) ;
			output_check_open[filenum] = false ;
		}
	}

	return true ;
}

FILE* FileIO::get_Input_File(int filenum)
{
	if(input_check_open[filenum])
	{
		return fp_input[filenum] ;
	}
	else
	{
		fprintf(stderr,"\nERROR!\nCan not open the file \"%s\"\n",input_filename[filenum]) ;
		exit(1) ;
	}
}

FILE* FileIO::get_Output_File(int filenum)
{
	if(output_check_open[filenum])
	{
		return fp_output[filenum] ;
	}
	else
	{
		fprintf(stderr,"\nERROR!\nCan not open the file \"%s\"\n",output_filename[filenum]) ;
		exit(1) ;
	}
}

int FileIO::setLine(const char *file_name)
{
	/////////////////////////////////////////////
	///                                       ///
	/// argument = file name                  ///
	/// return   = line number of the file    ///
	///                                       ///
	/// if 1 line has more than 2046 figures, ///
	/// this function can not work normally.  ///
	///                                       ///
	/////////////////////////////////////////////
	
	FILE *fp_get_line ;
	char buf[2048] ;
	int line = 0 ;

	if( (fp_get_line=fopen(file_name,"r")) == NULL )
	{
		return -1 ;
	}

	while( fgets(buf,2048,fp_get_line) != NULL )
	{
		line++ ;
	}

	fclose(fp_get_line) ;

	return line ;

} ;

int FileIO::get_LineNum_Input_File(int filenum)
{
	return input_line_num[filenum] ;
}

