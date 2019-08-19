#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define N 500

struct scale{
	int high_level;
	int low_level;
	char appeal[N];
};

int checkFile (FILE *f, char name[N]);
int checkDataCriteria (FILE *f, char name[N]);
void inputNameOfEmployer (char *employer);
int countScore (char criteria[N], char name[N]);
void findDouble (double *score, char criteria[N], char name[N]);
char makeChoice (int count);
void printfRecommend (double result, char recommend[N]);

	
int main()
{
	char criteria[N]={}, recommend[N]={}, employer[N]={};
	char *p, answer;
	double *score, result=0;
	int i=0, count=0;
	FILE *f_criteria, *f_recommend, *f_employer;
		
	f_criteria = fopen("criteria.txt", "r+");
	if (checkFile (f_criteria, "criteria.txt")) return 0;
	if (checkDataCriteria(f_criteria, "criteria.txt")) return 0;
	f_employer = fopen("employer.txt", "a+");
	if (checkFile (f_employer, "employer.txt")) return 0;
	
	printf ("\n--------------------IT employer criteria appeal for the jobseeker--------------------\n\n");
	inputNameOfEmployer (employer);
	fprintf (f_employer, "%s: ", employer);
		
	while (fgets (criteria, N, f_criteria) != NULL){
		p = strstr(criteria, "Total score:");
			if (p==NULL)
				printf ("%s", criteria); 
			else {
				count=countScore (criteria, "Total score:");
				score=(double*)calloc((count),sizeof(double));
				findDouble (score, criteria, "Total score:");
				answer=makeChoice (count);
				result+=score[answer-'a'];
				count=0;
				free(score);
			}
		}
	fclose(f_criteria);
	printf ("\nTotal: %0.1f score\n\n", result);
	fprintf (f_employer, "%0.1f score\n", result);
	
	f_recommend = fopen("recommend.txt", "r");
	if (checkFile (f_recommend, "recommend.txt")) return 0;
	while (1){
		p=fgets (recommend, N, f_recommend);	
		if (p==NULL) break;
		printfRecommend (result, recommend);
	}
	fclose(f_recommend);
	fclose(f_employer);	
	getchar();
	return 0;
}


int checkFile (FILE *f, char name[N]){
	if (f==NULL ) {
		printf("The file %s is unavailable", name);
		getchar();
		return 1;
	}
	else return 0;
}

int checkDataCriteria (FILE *f, char name[N]){
	char data[N]={};
	int count_answer=0, count_score=0;
	while (fgets (data, N, f) != NULL){
		if (data[0]>96 && data[0]<123 && data[1]==')'){
				count_answer++;
			}
		if (strstr(data, "Total score: ")!=NULL){
				count_score=countScore (data, "Total score: ");
			}
		if (data[0]=='-'&&data[1]=='-'){
			if (count_answer!=count_score) {
				printf ("\nError! The number of answers and scores in file %s do not match!\n", name);
				fclose(f);
				return 1;	
			}
			count_answer=0;
			count_score=0;
		}
	}
	fseek(f, 0, SEEK_SET);
	return 0;
}

void inputNameOfEmployer (char *employer){
	printf ("Enter the name of employer: ");
	scanf ("%s", employer);
	fflush(stdin);
	printf ("\n");
}

int countScore (char criteria[N], char name[N]){
	int count_score=0;
	char *end, *start;
	
	start=(criteria+strlen(name));
	end = start;
	while(*start) {
		strtod(start, &end);
		count_score++;
		start = end;
		while(!isdigit(*start) && *start) start++;
		}	
	return count_score;		
}

void findDouble (double *score, char criteria[N], char name[N]){
	double score_temp=0;
	int i=0;
	char *end, *start = (criteria+strlen(name));
	
	end = start;
	while(*start) {
		score_temp=strtod(start, &end);
		score[i++]=score_temp;
		start = end;
		while(!isdigit(*start) && *start) start++;
		}	
}

char makeChoice (int count){
	int i=0;
	char answer;
	printf ("Make your choice:");
			for (i=0; i<count; i++)
				printf (" %c", 'a'+i);
				printf ("\n");
			
			do{
				scanf ("%c", &answer);
				fflush(stdin);
				if (answer<'a'||answer>('a'+count-1))
				printf ("Wrong number of answer! Try again!\n");
			} while (answer<'a'||answer>('a'+count-1));
	return answer;
}

void printfRecommend (double result, char recommend[N]){
	struct scale employer[3];
	employer[0].high_level=100; employer[0].low_level=80; strcpy (employer[0].appeal, "The level of appeal of employer is high");
	employer[1].high_level=79; employer[1].low_level=40; strcpy (employer[1].appeal, "The level of appeal of employer is medium");
	employer[2].high_level=39; employer[2].low_level=0; strcpy (employer[2].appeal, "The level of appeal of employer is low");
	
	int i;
	for (i=0; i<3; i++){
		if (result>=employer[i].low_level && result<=employer[i].high_level){
			if (strstr (recommend, employer[i].appeal)!=NULL)
			printf ("%s", recommend);
		}
	}
}
