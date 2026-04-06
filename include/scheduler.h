#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "task.h"

class Scheduler{
private:
Task tasks [5];
int task_count;

public:
Scheduler();
bool add_task(Task task_added);
void execute();
void print();

};

#endif