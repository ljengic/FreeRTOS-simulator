#define MAX_TASK_CNT	50

void startTaskSetGenerator(double utilization,int n);
int getTaskCnt();
int getTaskPeriod(int i);
int getTaskDuration(int i);
char * getTaskName(int i);
