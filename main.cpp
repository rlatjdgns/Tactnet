#include "include/node.h"
#include "include/scheduler.h"
#include "include/LoRaDriver.h"
#include "include/BME280Driver.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <unistd.h>
#include <ctime>

void printMessage(const Message& m){
    m.print();
}

int main(int argc, char* argv[]){
    int nodeID = std::atoi(argv[1]); 
    
    if(nodeID == 1){
        BME280Driver bme;
        bme.begin();
        Node n1(1);
        n1.add_neighbor(2);
        n1.add_neighbor(3);
        time_t last_broadcast = time(nullptr);

        while(true){
            n1.receive(); 
            if((time(nullptr) - last_broadcast) >= 5){
                SensorReadings readings = bme.read();
                std::string payload = std::to_string(readings.temperature) + "|" + 
                            std::to_string(readings.humidity) + "|" + 
                            std::to_string(readings.pressure);
                Message m(1, 0, 1, MessageType::SENSORREADING, payload);
                bool result = n1.broadcast(m);
                last_broadcast = time(nullptr);
                std::cout << "Broadcasted: " << payload << "\n";
                n1.check_neighbors();
            }
        }
    }

    else if(nodeID == 2){
        Node n2(2);
        n2.add_neighbor(1);
        std::cout << "Node 2 listening...\n";
        std::cout<<"-----------------------------------\n";
        while(true){
            if(n2.receive()){
                n2.print_node();
                Message m(2, 1, 1, MessageType::STATUS_PING, "Ping Test");
                n2.broadcast(m);
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
                Message m(3, 1, 1, MessageType::STATUS_PING, "Ping Test");
                n3.broadcast(m);
            }
            n3.check_neighbors();
        }
    }
    return 0;
}

