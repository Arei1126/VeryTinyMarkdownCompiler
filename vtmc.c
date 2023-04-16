#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INPUT_BUFFER_SIZE 1024
#define OUTPUT_BUFFER_SIZE 2048

#define ADD(Dest,Expr) Dest = Dest + (Expr) 

#define DEBUG 0

struct line {
	char strings[INPUT_BUFFER_SIZE];

	int header_counter;  // #
	int strong_flag;  // .

	int img_flag;  //!
	int link_flag;  //[

	int bracket_start_index;  // [
	int bracket_end_index;  // ]
	
	char link_title[INPUT_BUFFER_SIZE];
	char link_src[INPUT_BUFFER_SIZE];

	int list_flag;  // -
	int ordered_list_flag;  // .
	
	int prev_line_is_list;
	int prev_line_is_olist;

	int hr_flag;  // _
	int br_flag;
};

int oneLineCommandCouneter(char input_word, char key) {
	if (input_word == key) return 1;
	else return 0;
}

int main(int argc, char **argv) {
	if (argc > 2){
	       printf("Usage: %s [DOUCMENT_TITLE]\n",argv[0]);
	       exit(1);
	}
	
	if (argc == 2 ){
		printf(	"<!DOCTYPE html>\n"
		 	"<html>\n"
			"<head>\n"
			"<title>\n"
			"%s\n"
			"</title>\n"
			"</head>\n"
			"<body>\n",argv[1]);
			}

	struct line input;

	input.list_flag = 0;
	input.ordered_list_flag = 0;

	while (1) {
		// Line Initialization
		if(fgets(input.strings, INPUT_BUFFER_SIZE,  stdin) == NULL) {
			break;
		}
	
		input.prev_line_is_list = input.list_flag;
		input.prev_line_is_olist = input.ordered_list_flag;
			

		input.header_counter = 0;
		input.strong_flag = 0;

		input.img_flag = 0;
		input.link_flag = 0;

		input.bracket_end_index = 0;
		input.bracket_end_index = 0;

		input.list_flag = 0;
		input.ordered_list_flag = 0;

		input.hr_flag = 0;
		input.br_flag = 0;
		// end of initialization
		
		// Command interpretation
		for(int i=0; i <= (int)strlen(input.strings); i++) {
			char c = input.strings[i];

			//ADD(input.header_counter, c == '#');
			//ADD(input.strong_counter, c == '*');
			//ADD(input.img_flag, c == '!');
			//ADD(input.link_flag, c == '[');
			
			if(c == '#'){
				input.header_counter++;
			}else if(c == '*'){
				input.strong_flag++;
			}else if(c == '!'){
				input.img_flag++;
			}else if(c == '['){
				input.link_flag++;
				input.bracket_start_index = i;
			}else if(c == ']'){
				input.bracket_end_index = i;
			}else if(c == '-'){
				input.list_flag++;
			}else if(c == '.'){
				input.ordered_list_flag++;
			}else if(c == '_'){
				input.hr_flag++;
			}else if(c == '\n'){
				input.br_flag++;
			}else if(input.link_flag >= 1){
				continue;
			}else {
				break;
			}
		}
		// end of command interpretation

		// Generate Tags
		char output_strings[OUTPUT_BUFFER_SIZE];

		int offset = input.header_counter + input.strong_flag + input.hr_flag + input.list_flag + input.ordered_list_flag;
	
		
			if(input.link_flag > 0){
			strncpy(input.link_title, &input.strings[input.bracket_start_index + 1], input.bracket_end_index - input.bracket_start_index - 1 );
			
			strcpy(input.link_src, &input.strings[input.bracket_end_index + 1]);

			// generate img src.. or a hred... tag
			if(input.img_flag){
				sprintf(output_strings,"<img src = \"%s\" alt = \"%s\">",input.link_title,input.link_src);
			}else{
				sprintf(output_strings,"<a href = \"%s\">%s</a>",input.link_src,input.link_title);
			}
		}else{
			strcpy(output_strings,&input.strings[offset]);
		}

		// link procecced and command words deleated
		
		if(input.header_counter > 0){
			if (input.header_counter > 6){
				input.header_counter == 6;
			}
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"<h%d>%s</h%d>",input.header_counter,output_strings,input.header_counter);
			strcpy(output_strings,buf);
		}

		if(input.strong_flag > 0){
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"<strong>%s</strong>",output_strings);
			strcpy(output_strings,buf);
		}

		if(input.list_flag + input.ordered_list_flag > 0){
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"<li>%s</li>",output_strings);
			strcpy(output_strings,buf);
		}

		if(input.hr_flag > 0){
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"<hr>%s",output_strings);
			strcpy(output_strings,buf);
		}
		
		if(input.br_flag > 0){
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"<br>%s",output_strings);
			strcpy(output_strings,buf);
		}

		// generate ol or ul 
		if((!input.prev_line_is_list) && input.list_flag){
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"<ul>%s",output_strings);
			strcpy(output_strings,buf);
		}

		if((!input.prev_line_is_olist) && input.ordered_list_flag){
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"<ol>%s",output_strings);
			strcpy(output_strings,buf);
			}
		
		if(input.prev_line_is_list && (!input.list_flag)){
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"%s</ul>",output_strings);
			strcpy(output_strings,buf);
			}
		

		if(input.prev_line_is_olist && (!input.ordered_list_flag)){
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"%s</ol>",output_strings);
			strcpy(output_strings,buf);
			}

		// printing
		printf("%s\n",output_strings);
	}
	return 0;
}
