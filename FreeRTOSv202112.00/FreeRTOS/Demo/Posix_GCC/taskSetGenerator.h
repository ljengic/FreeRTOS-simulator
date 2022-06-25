#include <stdbool.h>

#define MAX_TASK_CNT	100
#define MAX_PERIOD_CNT	10000
#define MAX_NAME_LENGHT	8

#define LOWEST_PERIOD	20
#define HIGHEST_PERIOD	100
#define MAX_HIPERPERIOD	10000
#define MAX_L           1000000

#define LOWEST_WH	0
#define HIGHEST_WH	5

typedef struct tskTaskControlBlock * TaskHandle_t;

void startTaskSetGenerator(double utilization,int n, char * report_file);

int getTaskPeriod(int i);
int getTaskDuration(int i);
double getTaskUtilization(int i);
int getTaskNumberOfPeriods(int i);
char * getTaskName(int i);
int getTaskCnt();
int getHiperPeriod();
void countWeaklyHardBroken();
int getWeaklyHardBroken(int i);

void printInfo(int x);
bool * getReport(int i);
void setReport(int i,int x);
void countMissedDeadlines();
void writeReportInFile();
void setTaskHandler(int i,TaskHandle_t x);
TaskHandle_t getTaskHandler(int i);
TaskHandle_t * getPointerToHandler(int i);
void incrementTimesKilled(int i);
int getTimesKilled(int i);;
void weaklyHardBroken();