#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define INPUT_BUFFER_SIZE 1024
#define OUTPUT_BUFFER_SIZE 2048

#define DEBUG 0

int oneLineCommandCouneter(char input_word, char key) {
	if (input_word == key) return 1;
	else return 0;
}


int main(int argc, char **argv) {
	int title_flag = 0;

	char img_title[INPUT_BUFFER_SIZE];
	char img_url[INPUT_BUFFER_SIZE];
	char link_title[INPUT_BUFFER_SIZE];
	char link_url[INPUT_BUFFER_SIZE];

	if (argc > 2){
	       printf("Usage: %s [DOUCMENT_TITLE]\n",argv[0]);
	       exit(1);
	}
	
	if (argc == 2 ){
		title_flag = 1;
		printf(	"<!DOCTYPE html>\n"
		 	"<html>\n"
			"<head>\n"
			"<title>\n"
			"%s\n"
			"</title>\n"
			"</head>\n"
			"<body>\n",argv[1]);
			}
	

	
	int ul_prev_flag = 0;
	int ol_prev_flag = 0;

	int first_flag = 1;
	
	while (1) {

	int ul_flag = 0;
	int ol_flag = 0;

	char input_strings[INPUT_BUFFER_SIZE];
	char output_strings[OUTPUT_BUFFER_SIZE];
	int head_counter = 0;
	int bold_flag = 0;
	int hr_flag = 0;

	int img_flag = 0;
	int img_title_start = 0;
	int img_title_end = 0;
	
	int link_flag = 0;
	int link_title_start = 0;
	int link_title_end = 0;
	
	char img_title[INPUT_BUFFER_SIZE];
	char link_title[INPUT_BUFFER_SIZE];


	if(fgets(input_strings, INPUT_BUFFER_SIZE,  stdin) == NULL){
		break;
	}
	
	for(int i=0; i <= (int)strlen(input_strings); i++){

#if DEBUG
		printf("%d,%d,%d\n",head_counter,bold_flag,hr_flag);
#endif 	
		
		head_counter = head_counter + oneLineCommandCouneter(input_strings[i], '#');
		
		bold_flag = bold_flag + oneLineCommandCouneter(input_strings[i], '*');

		hr_flag = hr_flag + oneLineCommandCouneter(input_strings[i], '_');

// img
	img_flag = img_flag + oneLineCommandCouneter(input_strings[i], '!');

	

	if(img_title_start == 0 && img_flag == 1) {
		img_title_start = i;
	}

	if(img_title_end == 0 && img_flag == 2) {
		img_title_end = i;
	}
//img 

//link/

	if(oneLineCommandCouneter(input_strings[i],'[')){
		link_title_start = i;
	}


	if( oneLineCommandCouneter(input_strings[i],']')){
		link_title_end = i;
	}
///link

//ul
	ul_flag = ul_flag + oneLineCommandCouneter(input_strings[i],'-');

//ol

	ol_flag = ol_flag + oneLineCommandCouneter(input_strings[i],'.');

	}
////////////////////////////////////////////////
// ここからが、行ごとの処理

	strcpy(output_strings,input_strings);

//ula
	if(ul_flag == 0 && ul_prev_flag == 1) {
		printf("</ul>\n");
		ul_prev_flag = 0;
	}


	if (ul_flag == 1 && ul_prev_flag == 0){
		printf("<ul>\n<li>");
		strcpy(output_strings,&input_strings[1]);
		printf("%s",output_strings);
		strcpy(output_strings,"");
		printf("</li>\n");
		ul_prev_flag = 1;
		
	}
	
	if (ul_flag == 1 && ul_prev_flag == 1){
		strcpy(output_strings,&input_strings[1]);
		printf("<li>%s</li>\n",output_strings);
		ul_prev_flag = 1;
		strcpy(output_strings,"");
	}

///ol

	if(ol_flag == 0 && ol_prev_flag == 1) {
		printf("</ol>\n");
		ol_prev_flag = 0;
	}


	if (ol_flag == 1 && ol_prev_flag == 0){
		printf("<ol>\n<li>");
		strcpy(output_strings,&input_strings[1]);
		printf("%s",output_strings);
		strcpy(output_strings,"");
		printf("</li>\n");
		ol_prev_flag = 1;
		
	}
	
	if (ol_flag == 1 && ol_prev_flag == 1){
		strcpy(output_strings,&input_strings[1]);
		printf("<li>%s</li>\n",output_strings);
		ol_prev_flag = 1;
		strcpy(output_strings,"");
	}


//ol




// img処理

	
	if(img_flag == 2){
		printf("<img alt = \"");
		for (int i = img_title_start;i < img_title_end;i++) {
			fputc(input_strings[i],stdout);
	}
		printf("\" src = \"");
		for (int i = img_title_end + 1;  i < (int)strlen(input_strings);i++){
			fputc(input_strings[i],stdout);
		}
		printf("\">\n");
		strcpy(output_strings,"");
	}

// img処理ここまで
// link
	
	else if(link_title_end){
		printf("<a href = \"");
		for (int i = link_title_end + 1;  i < (int)strlen(input_strings);i++){
			fputc(input_strings[i],stdout);
		}
		printf("\">");

		for (int i = link_title_start + 1;i < link_title_end;i++) {
			fputc(input_strings[i],stdout);
	}
		printf("</a>\n");
		strcpy(output_strings,"");
	}

// link処理ここまでse 

	else if ((head_counter != 0) && head_counter < 7 ) {
		strcpy(output_strings,&input_strings[head_counter+1]);
		printf("<h%d>\n",head_counter);
	}

	else if (bold_flag != 0) {
		printf("<bold>\n");
		strcpy(output_strings,&input_strings[1]);
	}

	else if (hr_flag != 0) {
		printf("<hr>\n");
		strcpy(output_strings,"");
	}

	else if(head_counter + bold_flag + link_title_end + img_flag + ul_flag +ol_flag + first_flag == 0){
		printf("<br>\n");
	}

	printf("%s\n",output_strings);



	if ((head_counter != 0) && head_counter < 7 ) {
		printf("</h%d>\n",head_counter);
	}

	if (bold_flag != 0) {
		printf("</bold>\n");
	}

	first_flag = 0;

	}
	if (title_flag){
		printf("</body>\n</html>");
	}
	

	return 0;
}
