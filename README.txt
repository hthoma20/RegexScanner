Riley Snook and Harry Thoma

Our project is a regular expression scanner with capturing capabilities.
Our API (accessed by including scanre.h) includes:
	one type : cRegex, a capturable regular expression
	
	a function for making a capturable regex with a regex string such as <a*>b,
		signifying that the a* should be captured
		
	a function to free a cRegex
	
	functions to scan a file based on a given cRegex
	
To run the examples demonstrated in class, use:
	make
	./main
	
To run all unit tests, use:
	make test
	./test