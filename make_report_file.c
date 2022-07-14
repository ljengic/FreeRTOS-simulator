//THIS FILE NEEDS TO BE COMPILED SEPEATELY !!!!!
//gcc make_report_file.c -o make_report_file

#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"

int main( int argc, char **argv )
{

    if(argc<=2) {
		printf("You did not feed me with right arguments, I will die now :( ...\n");
		exit(1);
	}

	int num_of_tasks = atoi(argv[1]);
    char * file_path = argv[2];


	if (access(file_path, F_OK) == 0) {

		return 0;

	} else {
		FILE *file;

		file = fopen(file_path,"a");

		fprintf(file,"Utilization,Number_of_tasks,Total skipped,Total broken,Weakly hard,QoS,Qos2,QoS3");

		for(int i=0;i<num_of_tasks;i++){
			fprintf(file,",Task_%d skipped",i+1);
		}

		fprintf(file,"\n");

		fclose(file);
	}

	


return 0;
}