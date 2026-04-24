#include "scheduler.h"
#include <iostream>
#include "node.h" 
#include <ctime>
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

void Scheduler::execute(Node& node){
    
    for(int i=0;i<task_count-1;i++){
        for(int j=0;j<task_count-1-i;j++){
            if(tasks[j].get_priority()>tasks[j+1].get_priority()){
                std::swap(tasks[j],tasks[j+1]);
            }
        }
    }


    for(int i=0; i<task_count; i++){
        if(time(nullptr) -tasks[i].get_last_executed() > tasks[i].get_interval()){
            switch(tasks[i].get_task_type()){
            case TaskType::SEND_READINGS:{
                std::cout << "Broadcasting now\n";
                SensorReadings readings = node.read_sensor();
                std::string payload = std::to_string(readings.temperature) + "|" + std::to_string(readings.humidity) + "|" + std::to_string(readings.pressure);
                for(int j=0;j<node.get_neighbor_count();j++){
                    node.broadcast(Message(node.get_node_ID(), node.get_neighbor_address(j), 0, MessageType::SENSORREADING, payload));    
                }
                break;
            }
            case TaskType::SEND_STATUS:
                std::cout << "Executing: SEND_STATUS\n";
                node.broadcast(Message(node.get_node_ID(), 0, 0, MessageType::STATUS_PING, "Status"));
                break;
            case TaskType::SEND_ERROR:
                std::cout << "Executing: SEND_ERROR\n";
                node.broadcast(Message(node.get_node_ID(), 0, 0, MessageType::ERROR, "error"));
                break;
            case TaskType::RELAY_MESSAGE:
                std::cout << "Executing: RELAY_MESSAGE\n";
                node.broadcast(Message(node.get_node_ID(), 0, 0, MessageType::RELAY, "relay"));
                break;
            default:
                std::cout << "Execution Failed\n";
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