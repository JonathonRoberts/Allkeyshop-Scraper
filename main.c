#include <stdio.h>
#include <string.h>
#include <regex.h>
#include <unistd.h>
#include <time.h>
#include "ezXPath.c"

#define MAXELEMENTS 3000 /* Maximum number of results to return */

 /*
 * Using XPath to return the best odds for an event
 */

float setreturn(int noods, float odds[]);
int scrape(char * url, float thresh);
int footballquick();
int findraces();
int crawlall(char *search);
time_t getdatetime(char *datestring);
static int cmp (const void *p1, const void *p2);
void list();
void printstruct(int i);
void extracturldata(char *website);

void help(char *progname){
	fprintf(stderr,"\ Usage: %s <file>\n",progname);
}

int main(int argc, char *argv[]){
	if(argc != 2){
		help(argv[0]);
		return 1;
	}

	int MAX_CHAR = 500;

	char string[MAX_CHAR];
	FILE *fp;
	char url[300];
	float price;
	float thresh;
	fp=fopen(argv[1], "r");
	while(fgets(string, MAX_CHAR, fp)!=NULL) {
		strlcpy(url,  string,strlen(string));
		if(fgets(string, MAX_CHAR, fp)==NULL)
			return 0;
		thresh = atof(string);
		scrape(url,thresh);

	}
	return 1;

}
int scrape(char * url,float thresh){
	char *output[MAXELEMENTS];
	int size;
	float price;
	char *xpath = "/html/body/div/div/div/div/div//p/span/@content";
	/*
	regex_t regex;
	int datecheck = regcomp(&regex, ".+ [[:digit:]][[:digit:]]:",REG_EXTENDED);
	*/

	size = ezXPathHTML(url,xpath,output);
	if(size==2){
		if(output[0]!=NULL){
			price = atof(output[0]);
			if(thresh>=price)
				printf("%s\nis on offer for %.2f\n",url,price);
			free(output[0]);
		}
		if(output[1]!=NULL)
			free(output[1]);
	}
	//}
}



