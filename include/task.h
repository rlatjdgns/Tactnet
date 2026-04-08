#ifndef TASK_H
#define TASK_H

enum class TaskType{
    SEND_TEMPERATURE,
    SEND_STATUS,
    RELAY_MESSAGE, 
    SEND_ERROR
};

class Task{
private:

TaskType task_type;
int priority;
int interval;

public:
Task(TaskType task_type, int priority, int interval);
Task() = default;
int get_priority();
TaskType get_task_type();
void print();
};


#endif 
