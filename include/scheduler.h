#ifndef SCHEDULER_H
#define SCHEDULER_H
#include "task.h"
class Node;

class Scheduler{
private:
Task tasks [5];
int task_count;

public:
Scheduler();
void execute(Node& node);
bool add_task(Task task_added);
void print();

};

#endif