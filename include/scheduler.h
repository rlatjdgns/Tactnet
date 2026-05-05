#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "task.h"

// Forward declaration to avoid circular dependency with node.h
class Node;

class Scheduler {
private:
    Task tasks[5];   // fixed-size task queue
    int task_count;

public:
    Scheduler();
    bool add_task(Task task);
    void execute(Node& node);  // sorts by priority then executes due tasks
    void print();
};

#endif