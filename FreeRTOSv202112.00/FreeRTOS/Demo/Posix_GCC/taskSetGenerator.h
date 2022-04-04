#include <stdbool.h>

#define MAX_TASK_CNT	100
#define MAX_PERIOD_CNT	1000
#define MAX_NAME_LENGHT	8

void startTaskSetGenerator(double utilization,int n);

int getTaskPeriod(int i);
int getTaskDuration(int i);
double getTaskUtilization(int i);
int getTaskNumberOfPeriods(int i);
char * getTaskName(int i);
int getTaskCnt();
int getHiperPeriod();

void printInfo(int x);
bool * getReport(int i);
bool setReport(int i,int x);
void countMissedDeadlines();
void writeReportInFile();
int getMissedDeadlines(int i);
