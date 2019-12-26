#include "../header/file.h"
#include <stdlib.h>
#include <stdio.h>

FileIO::FileIO()
{
	input1_check  = 0 ;
	input2_check  = 0 ;
	output1_check = 0 ;
	output2_check = 0 ;
	input1_line_num = -1 ;
	input2_line_num = -1 ;
}

bool FileIO::set_Input_File1(const char *file_name)
{
	input1_line_num = FileIO::setLine(file_name) ;

	if( input1=fopen(file_name,"r") )
	{
		input1_check = 1 ;
		return true ;
	}
	else
	{
		fprintf(stderr,"\nERROR!\nCan not open the file \"%s\"\n",file_name) ;
		exit(1) ;
	}
}

bool FileIO::set_Input_File2(const char *file_name)
{
	input2_line_num = setLine(file_name) ;

	if( input2=fopen(file_name,"r") )
	{
		input2_check = 1 ;
		return true ;
	}
	else
	{
		fprintf(stderr,"\nERROR!\nCan not open the file \"%s\"\n",file_name) ;
		exit(1) ;
	}
}

bool FileIO::set_Output_File1(const char *file_name)
{
	if( output1=fopen(file_name,"w") )
	{
		output1_check = 1 ;
		return true ;
	}
	else
	{
		fprintf(stderr,"\nERROR!\nCan not open the file \"%s\"\n",file_name) ;
		exit(1) ;
	}
}
bool FileIO::set_Output_File2(const char *file_name)
{
	if( output2=fopen(file_name,"w") )
	{
		output2_check = 1 ;
		return true ;
	}
	else
	{
		fprintf(stderr,"\nERROR!\nCan not open the file \"%s\"\n",file_name) ;
		exit(1) ;
	}
}

bool FileIO::close_Files()
{
	if(input1_check != 0)
	{
		fclose(input1) ;
		input1_check = 0 ;
	}
	if(input2_check != 0)
	{
		fclose(input2) ;
		input2_check = 0 ;
	}
	if(output1_check != 0)
	{
		fclose(output1) ;
		output1_check = 0 ;
	}
	if(output2_check != 0)
	{
		fclose(output2) ;
		output2_check = 0 ;
	}
	return true ;
}

FILE* FileIO::get_Input_File1()
{
	if(input1_check != 0)
	{
		return input1 ;
	}
}

FILE* FileIO::get_Input_File2()
{
	if(input2_check != 0)
	{
		return input2 ;
	}
}

FILE* FileIO::get_Output_File1()
{
	if(output1_check != 0)
	{
		return output1 ;
	}
}

FILE* FileIO::get_Output_File2()
{
	if(output2_check != 0)
	{
		return output2 ;
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

int FileIO::get_LineNum_Input_File1()
{
	return input1_line_num ;
}

int FileIO::get_LineNum_Input_File2()
{
	return input2_line_num ;
}
