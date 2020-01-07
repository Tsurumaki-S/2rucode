#ifndef __MY_FILE__
#define __MY_FILE__

#include <stdlib.h>
#include <stdio.h>
#include "const.h"

class FileIO
{
	private:
		FILE *fp_input[OPEN_FILE_NUM] ;
		FILE *fp_output[OPEN_FILE_NUM] ;
		bool input_check_open[OPEN_FILE_NUM] ;
		bool output_check_open[OPEN_FILE_NUM] ;
		int input_line_num[OPEN_FILE_NUM] ;
		int output_line_num[OPEN_FILE_NUM] ;
		char input_filename[OPEN_FILE_NUM][BUFMAX_CHAR_LARGE] ;
		char output_filename[OPEN_FILE_NUM][BUFMAX_CHAR_LARGE] ;
	public:
		FileIO() ;
		~FileIO() ;
		bool set_Input_File(int filenum, const char *file_name) ;
		char *get_Input_FileName(int filenum) ;
		bool set_Output_File(int filenum, const char *file_name) ;
		char *get_Output_FileName(int filenum) ;
		bool close_Files() ; 
		FILE *get_Input_File(int filenum) ;
		FILE *get_Output_File(int filenum) ;
		int  setLine(const char *file_name) ;
		int  get_LineNum_Input_File(int filenum) ;
} ;

#endif
