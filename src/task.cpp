#include "task.h"
#include <iostream>

Task::Task(TaskType task_type, int priority, int interval): task_type(task_type), priority(priority), interval(interval), last_executed(0) {}

int Task::get_priority(){return priority;}
TaskType Task::get_task_type(){return task_type;}
int Task::get_interval(){return interval;}
time_t Task::get_last_executed(){return last_executed;}

void Task::set_last_executed(){last_executed = time(nullptr);}

void Task::print() {
    switch (task_type) {
        case TaskType::SEND_READINGS: std::cout << "Task type: Send Readings\n"; break;
        case TaskType::SEND_STATUS: std::cout << "Task type: Send Status\n"; break;
        case TaskType::SEND_ERROR: std::cout << "Task type: Send Error\n"; break;
        default: std::cout << "Task type: Unknown\n"; break;
    }
    std::cout << "Priority: " << priority << "\n";
    std::cout << "Interval: " << interval << "s\n";
}