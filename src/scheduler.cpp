#include "scheduler.h"
#include <iostream>
#include "node.h" 
#include <ctime>
#include <unistd.h>

Scheduler::Scheduler(){
    task_count=0;
}

bool Scheduler::add_task(Task task){
    if(task_count==5) return false;
    tasks[task_count++]=task;
    return true;
}

void Scheduler::execute(Node& node){
    for(int i=0;i<task_count-1;i++){
        for(int j=0;j<task_count-1-i;j++){
            if(tasks[j].get_priority()>tasks[j+1].get_priority()){
                std::swap(tasks[j],tasks[j+1]);
            }
        }
    }

    for(int i=0; i<task_count; i++){
        if(time(nullptr) - tasks[i].get_last_executed() > tasks[i].get_interval()){
            switch(tasks[i].get_task_type()){
            case TaskType::SEND_READINGS:{
                SensorReadings readings = node.read_sensor();
                std::string payload = std::to_string(readings.temperature) + "|" + std::to_string(readings.humidity) + "|" + std::to_string(readings.pressure);
                Message m2(node.get_node_ID(), 3, 0, MessageType::SENSORREADING, payload);
                node.send_to(2, m2);
                break;
            }
            case TaskType::SEND_STATUS:
                node.broadcast(Message(node.get_node_ID(), 0, 0, MessageType::STATUS_PING, "Status"));
                break;
            case TaskType::SEND_ERROR:
                node.broadcast(Message(node.get_node_ID(), 0, 0, MessageType::ERROR, "error"));
                break;
            default:
                std::cout << "Unknown Tasktype\n";
                break;
            }
            tasks[i].set_last_executed();
        }
    }
}

void Scheduler::print(){
    for(int i=0;i<task_count;i++){
        tasks[i].print();
    }
}