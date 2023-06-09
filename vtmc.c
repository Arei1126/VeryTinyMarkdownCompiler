#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INPUT_BUFFER_SIZE 1024
#define OUTPUT_BUFFER_SIZE 2048

#define DEBUG 0


int single_variable_overwritten_sprintf(char *str, char *command){
	char buf[OUTPUT_BUFFER_SIZE];
	sprintf(buf,command, str);
	strcpy(str,buf);
	return strlen(str);
}

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
	int br_flag;  // \n

	int blockquote_flag;  // >
};

int main(int argc, char **argv) {
	int title_flag = 0;
	switch (argc) {
		case 2:
			title_flag = 1;
			printf(	"<!DOCTYPE html>\n"
		 		"<html>\n"
				"<head>\n"
				"<title>\n"
				"%s\n"
				"</title>\n"
				"</head>\n"
				"<body>\n",argv[1]);
				break;
		case 3:
			title_flag = 1;
			printf(	"<!DOCTYPE html>\n"
		 		"<html>\n"
				"<head>\n"
				"<title>\n"
				"%s\n"
				"</title>\n"
				"<STYLE TYPE=\"text/css\">\n"
				"<!--\n"
				"BODY {\n"
				"background-color:#EFEFEF;\n"
				"FONT-STYLE:normal;\n"
				"FONT-FAMILY:\"MS ゴシック\",serif,system-ui;\n"
				"margin-left: 2%;\n"
				"margin-right: 2%;\n"
				"}\n"
				"-->\n"
				"</STYLE>\n"
				"</head>\n"
				"<body>\n",argv[1]);
				break;
		case 1:
			break;
		default:
	       		printf("Usage: %s [DOUCMENT_TITLE]\n",argv[0]);
	      		exit(1);
	}
	struct line input;

	input.list_flag = 0;
	input.ordered_list_flag = 0;

	while (1) {
		// Line Initialization
		if(fgets(input.strings, INPUT_BUFFER_SIZE,  stdin) == NULL) {
			break;  // EOF -> finalization
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

		input.link_title[0] = '\0';
		input.link_src[0] = '\0';

		input.blockquote_flag = 0;

		// end of initialization
		
		// Command interpretation
		for(int i=0; i <= (int)strlen(input.strings); i++) {
			char c = input.strings[i];

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
			}else if(c == '>'){
				input.blockquote_flag++;
			}else if(c == '\n'){
				input.br_flag++;
			}else if(input.link_flag >= 1 && input.bracket_end_index == 0){
				continue;
			}else {
				break;
			}
		}
		// end of command interpretation

		// Generate Tags
		char output_strings[OUTPUT_BUFFER_SIZE];

		int offset = input.header_counter + input.strong_flag + input.hr_flag + input.list_flag + input.ordered_list_flag + input.blockquote_flag;
	
		//  generate link title and link url
		if(input.link_flag > 0){
			strncpy(input.link_title, &input.strings[input.bracket_start_index + 1], input.bracket_end_index - input.bracket_start_index - 1 );
			input.link_title[input.bracket_end_index - input.bracket_start_index -1 ] = '\0';
#if DEBUG
			printf("linke title: %s\n 文字数: %d",input.link_title,(int)strlen(input.link_title));
#endif
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
		// other tag processing
		
		if(input.header_counter > 0){
			if (input.header_counter > 6){
				input.header_counter = 6;
			}
			char buf[OUTPUT_BUFFER_SIZE];
			sprintf(buf,"<h%d>%s</h%d>",input.header_counter,output_strings,input.header_counter);
			strcpy(output_strings,buf);
		}

		if(input.strong_flag > 0){
			single_variable_overwritten_sprintf(output_strings, "<strong>%s</strong>");
		}

		if(input.list_flag + input.ordered_list_flag > 0){
			single_variable_overwritten_sprintf(output_strings,"<li>%s</li>");
			}

		if(input.hr_flag > 0){
			single_variable_overwritten_sprintf(output_strings,"<hr>%s");
			}

		
		if(input.br_flag > 0){
			single_variable_overwritten_sprintf(output_strings,"<br>%s");
		}

		if(input.blockquote_flag > 0){
			single_variable_overwritten_sprintf(output_strings,"<blockquotes>%s</blockquotes>");
		}

		// generate ol or ul 
		if((!input.prev_line_is_list) && input.list_flag){
			single_variable_overwritten_sprintf(output_strings,"<ul>%s");
			}


		if((!input.prev_line_is_olist) && input.ordered_list_flag){
			single_variable_overwritten_sprintf(output_strings,"<ol>%s");
			}
		
		if(input.prev_line_is_list && (!input.list_flag)){
			single_variable_overwritten_sprintf(output_strings,"%s</ul>");
			}
		

		if(input.prev_line_is_olist && (!input.ordered_list_flag)){
			single_variable_overwritten_sprintf(output_strings,"%s</ol>");
			}

		// printing
		printf("%s\n",output_strings);
	}

	// Finalization
	printf("<!-- Document generated by VeryTinyMarkDownCompiler https://github.com/Arei1126/VeryTinyMarkdownCompiler  -->");

	if(title_flag){
		printf("</body>\n</html>\n");
	}
	return 0;
}
