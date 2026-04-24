#ifndef TASK_H
#define TASK_H
#include <ctime>

enum class TaskType{
    SEND_READINGS,
    SEND_STATUS,
    RELAY_MESSAGE, 
    SEND_ERROR
};

class Task{
private:

TaskType task_type;
int priority;
int interval;
time_t last_executed;

public:
Task(TaskType task_type, int priority, int interval);
Task() = default;
int get_priority();
TaskType get_task_type();
int get_interval();
time_t get_last_executed();
void set_last_executed();
void print();
};


#endif 
