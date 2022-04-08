#include <stdbool.h>

#define MAX_TASK_CNT	100
#define MAX_PERIOD_CNT	1000
#define MAX_NAME_LENGHT	8

typedef struct tskTaskControlBlock * TaskHandle_t;

void startTaskSetGenerator(double utilization,int n, char * report_file);

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
void setTaskHandler(int i,TaskHandle_t x);
TaskHandle_t getTaskHandler(int i);
TaskHandle_t * getPointerToHandler(int i);
