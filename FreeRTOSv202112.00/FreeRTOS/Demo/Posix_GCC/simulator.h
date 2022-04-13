// declaration of functions used by FreeRTOS kernal and implemted in simulator

typedef struct tskTaskControlBlock * TaskHandle_t;

extern void incrementTimesKilled(int i);
extern void exit_function();
extern TaskHandle_t getTaskHandler(int i);
extern int getHiperPeriod();
extern int getStartTime(int i);
extern void setStartTime(int i,int x);

//extern  void console_print( const char * fmt,
//                        ... );
