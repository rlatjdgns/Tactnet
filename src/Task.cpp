#include "task.h"
#include <iostream>

Task::Task(TaskType task_type, int priority, int interval):last_executed(0){
    this->task_type = task_type;
    this->priority = priority;
    this->interval = interval;
}

int Task::get_priority(){
    return priority; 
}

TaskType Task::get_task_type(){
    return task_type;
}

int Task::get_interval(){
    return interval;
}

time_t Task::get_last_executed(){
    return last_executed;
}

void Task::set_last_executed(){
    last_executed = time(nullptr);
}

void Task::print(){
    switch(task_type){
    case TaskType::SEND_READINGS:
        std::cout<<"Task type: "<<"Send Readings\n";
        break;
    case TaskType::SEND_STATUS:
        std::cout<<"Task type: "<<"Send Status\n";
        break;
    case TaskType::RELAY_MESSAGE: 
        std::cout<<"Task type: "<<"Send Relay message\n";
        break;
    case TaskType::SEND_ERROR:
        std::cout<<"Task type: "<<"Send Error\n";
        break;
    default:
        std::cout<<"Invalid Message Type\n";
        break;
    }
    std::cout<<"Priority: "<<priority<<"\n";
    std::cout<<"Interval: "<<interval<<"\n"; 
}