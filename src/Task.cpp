#include "task.h"
#include <iostream>

Task::Task(TaskType task_type, int priority, int interval){
    this->task_type = task_type;
    this->priority = priority;
    this->interval = interval;
}
int Task::get_priority(){
    return priority; 
}

void Task::print(){
    switch(task_type){
    case TaskType::SEND_TEMPERATURE:
        std::cout<<"Task type: "<<"Send Temperature\n";
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