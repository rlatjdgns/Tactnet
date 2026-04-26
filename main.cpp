#include "include/node.h"
#include "include/scheduler.h"
#include "include/LoRaDriver.h"
#include "include/BME280Driver.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>


void printMessage(const Message& m){
    m.print();
}

int main(int argc, char* argv[]){
    int nodeID = std::atoi(argv[1]); 
    
    if(nodeID == 1){
        Node n1(1);
        n1.add_neighbor(2);
        n1.add_task(Task(TaskType::SEND_READINGS, 1, 10));

        while(true){
            n1.run_task();
            if(n1.receive()){
                n1.print_node();
            }
            n1.check_neighbors();
        }
    }

    else if(nodeID == 2){
        Node n2(2);    
        n2.add_neighbor(1);
        n2.add_neighbor(3);
        std::cout << "Node 2 listening...\n";
        std::cout<<"-----------------------------------\n";
        while(true){
            if(n2.receive()){
                n2.print_node();
                SensorReadings r = n2.read_sensor();
                if(r.temperature > 35.0 || r.humidity > 85.0 || r.temperature < 10.0){
                    std::string error_payload = std::to_string(r.temperature) + "|" + 
                                                std::to_string(r.humidity) + "|" + 
                                                std::to_string(r.pressure);
                    Message m(2, 1, 1, MessageType::ERROR, error_payload);
                    n2.send_to(1, m);
                } else {
                    std::string ping_payload = std::to_string(r.temperature) + "|" + 
                                            std::to_string(r.humidity) + "|" + 
                                            std::to_string(r.pressure);
                    Message m(2, 1, 1, MessageType::STATUS_PING, ping_payload);
                    n2.send_to(1, m);
                }
            }
            n2.check_neighbors();
        }
    }
    
    else if(nodeID == 3){
        Node n3(3);
        n3.add_neighbor(1);
        std::cout << "Node 3 listening...\n";
        std::cout<<"-----------------------------------\n";
        while(true){
            if(n3.receive()){
                n3.print_node();
                SensorReadings r = n3.read_sensor();
                if(r.temperature > 35.0 || r.humidity > 85.0 || r.temperature < 10.0){
                    std::string error_payload = std::to_string(r.temperature) + "|" + 
                                                std::to_string(r.humidity) + "|" + 
                                                std::to_string(r.pressure);
                    Message m(3, 1, 1, MessageType::ERROR, error_payload);
                    n2.send_to(1, m);
                } else {
                    std::string ping_payload = std::to_string(r.temperature) + "|" + 
                                            std::to_string(r.humidity) + "|" + 
                                            std::to_string(r.pressure);
                    Message m(3, 1, 1, MessageType::STATUS_PING, ping_payload);
                    n3.send_to(1, m);
                }
            }
            n3.check_neighbors();
        }
    }
    return 0;
}

