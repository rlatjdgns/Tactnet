#include "scheduler.h"
#include <iostream>

Scheduler::Scheduler(){
    task_count=0;

}

bool Scheduler::add_task(Task task_added){
    if(task_count==5){
        return false;
    } 
    else{
        tasks[task_count]=task_added;
        task_count++;
    }
    return true;
}

void Scheduler::execute(){
    for(int i=0;i<task_count-1;i++){
        for(int j=0;j<task_count-1-i;j++){
            if(tasks[j].get_priority()>tasks[j+1].get_priority()){
                std::swap(tasks[j],tasks[j+1]);
            }
        }
    }
}

void Scheduler::print(){
    for(int i=0;i<task_count;i++){
        tasks[i].print();
    }
}