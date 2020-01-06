#ifndef __MY_FILE__
#define __MY_FILE__

#include <stdlib.h>
#include <stdio.h>
class FileIO
{
	private:
		FILE *input1  ;
		FILE *input2  ;
		FILE *output1 ;
		FILE *output2 ;
		int input1_check ;
		int input2_check ;
		int output1_check ;
		int output2_check ;
		int input1_line_num ;
		int input2_line_num ;
	public:
		FileIO() ;
		bool set_Input_File1(const char *file_name) ;
		bool set_Input_File2(const char *file_name) ;
		char *get_Input_FileName() ;
		bool set_Output_File1(const char *file_name) ; 
		bool set_Output_File2(const char *file_name) ; 
		char *get_Output_FileName() ;
		bool close_Files() ; 
		FILE *get_Input_File1() ;
		FILE *get_Input_File2() ;
		FILE *get_Output_File1() ;
		FILE *get_Output_File2() ;
		int  setLine(const char *file_name) ;
		int  get_LineNum_Input_File1() ;
		int  get_LineNum_Input_File2() ;
} ;

#endif
