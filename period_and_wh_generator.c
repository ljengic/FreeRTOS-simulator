//THIS FILE NEEDS TO BE COMPILED SEPEATELY !!!!!
//gcc period_and_wh_generator.c -o period_and_wh_generator

#include "stdlib.h"
#include "stdio.h"

#include "period_and_wh_generator.h"

int period[MAX_TASK_CNT];
int weakly_hard[MAX_TASK_CNT];
int duration[MAX_TASK_CNT];

double rand_0_to_1()
{
    return (double)rand() / (double)RAND_MAX ;
}

int GCD(int a,int b)
{
  if (b == 0)
    return a;
  return GCD(b, a % b);
}

int LCM(int a, int b)
{
    return (a / GCD(a, b)) * b;
}

void calculateTaskDuration(int n){
	for(int i=0;i<gn;i++){
		double tmp = ((double) period[i] * u[i]);
		tmp+=0.5;
		//Task_Set[i].duration = (int) ((double) getTaskPeriod(i) * getTaskUtilization(i));
		duration[i] = (int) tmp;
		if(duration[i] == 0) duration[i]++;
	}
}

int calculateHiperperiod(int n){

	int hiperperiod;
	int temp = period[0];

	for(int i=1;i<n;i++){
		temp = LCM(period[i],temp);
	}
	return hiperperiod = temp;
}

double calculate 

void generatePeriods(int n){
	for(int i=0;i<n;i++){
		int range = HIGHEST_PERIOD - LOWEST_PERIOD + 1;
		int tmp = (rand_0_to_1() * range) + LOWEST_PERIOD;
		period[i]=tmp;
	}
}

void generateWeaklyHard(int n){
	for(int i=0;i<n;i++){
		int range = HIGHEST_WH - LOWEST_WH + 1;
		int tmp = (rand_0_to_1() * range) + LOWEST_WH;
		weakly_hard[i]=tmp;
	}
}

int main( int argc, char **argv )
{

    if(argc<=3) {
		printf("You did not feed me with right arguments, I will die now :( ...\n");
		exit(1);
	}

	int num_of_tasks = atoi(argv[1]);
    char * period_file_path = argv[2];
	char * wh_file_path = argv[3];

	srand(time(NULL));

	while(1){
		generatePeriods(num_of_tasks);
		if(calculateHiperperiod(num_of_tasks) < MAX_HIPERPERIOD) break;
	}

	generateWeaklyHard(num_of_tasks);

	//printf("%d\n",calculateHiperperiod(num_of_tasks));

	FILE *file;

	file = fopen(period_file_path,"w");

	for(int i=0;i<num_of_tasks;i++){
		fprintf(file,"%d\n",period[i]);
	}

	fclose(file);

	file = fopen(wh_file_path,"w");

	for(int i=0;i<num_of_tasks;i++){
		fprintf(file,"%d\n",weakly_hard[i]);
	}

	fclose(file);

return 0;
}