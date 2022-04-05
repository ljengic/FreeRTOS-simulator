#include "stdlib.h"
#include "stdio.h"


int main( int argc, char **argv )
{

    if(argc<=2) {
		printf("You did not feed me with right arguments, I will die now :( ...\n");
		exit(1);
	}

	int num_of_tasks = atoi(argv[1]);
    char * file_path = argv[2];

	FILE *file;

	file = fopen(file_path,"a");
	
	fprintf(file,"Utilization,Number_of_tasks,Total missed");

	for(int i=0;i<num_of_tasks;i++){
		fprintf(file,",Task_%d missed",i+1);
	}

	fprintf(file,"\n");

	fclose(file);

return 0;
}