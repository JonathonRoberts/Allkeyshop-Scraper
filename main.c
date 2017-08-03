#include <stdio.h>
#include <string.h>
#include <regex.h>
#include "ezXPath.c"

 /*
 * Using XPath to return when games on http://www.allkeyshop.com/ are below a certain price
 */

#define MAX_CHAR 300
int scrape(char * url, float thresh);

int main(int argc, char *argv[]){
	if(argc != 2){
		fprintf(stderr,"Usage: %s <configfile>\n",argv[0]);
		return 0;
	}

	char string[MAX_CHAR];
	FILE *fp;
	char url[MAX_CHAR];
	float price;
	float thresh;
	fp=fopen(argv[1], "r");

	while(fgets(string, MAX_CHAR, fp)!=NULL) {
		strlcpy(url,  string,strlen(string));
		if(fgets(string, MAX_CHAR, fp)==NULL){
			fclose(fp);
			return 0;
		}
		thresh = atof(string);
		scrape(url,thresh);
	}

	fclose(fp);
	return 1;

}
int scrape(char * url,float thresh){
	char *output[6];
	int size;
	float price;
	char *xpath = "/html/body/div/div/div/div/div/p/span/@content";
	regex_t regex;
	int validurl = regcomp(&regex, "^http://www.allkeyshop.com/[^[:space:]]+$",REG_EXTENDED);
	if((validurl = regexec(&regex, url,0,NULL,0))==0){
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
	}
	else{
		printf("Invalid URL\n");
	}
}
