#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

//alloc memory for and populate each given char**
//based on the given string
void populate(char* str, ...){
	//create and start the va_list
	va_list list;
	va_start(list, str);
	
	int i;
	//track where the string to capture began
	int open= -1;
	
	//walk the string
	for(i=0; i < strlen(str); i++){
		//this is the beginning of the capture
		if(str[i] == '<'){
			open= i;
		}
		//we have found the end of the capture
		else if(str[i] == '>'){
			//get the next element in the va_list
			//we are expecting char** types
			char** strptr= va_arg(list, char**);
			
			//allocate memory for the string
			*strptr= malloc(sizeof(char) * (i-open));
			//copy the contents into the string and terminate with \0
			strncpy(*strptr, str+open+1, i-open-1);
			(*strptr)[i-open-1]= '\0';
		}
	}
	
	//end list iteration
	va_end(list);
}

void main(){
	//create a string with things to capture
	char* str= "<hello> brave new <world>";
	
	//create the variable we want to capture the things into
	char* hello;
	char* world;
	
	//capture them --note the &, we need to pass in unallocated char**
	populate(str, &hello, &world);
	
	printf("%s %s\n", hello, world);
}