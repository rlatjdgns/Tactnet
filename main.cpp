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
        BME280Driver bme;
        bme.begin();
        SensorReadings readings = bme.read();
        std::cout << "Temperature: " << readings.temperature << "C\n";
        std::cout << "Humidity: " << readings.humidity << "%\n";
        std::cout << "Pressure: " << readings.pressure << " hPa\n";
        Node n1(1);
        n1.add_neighbor(2);
        Message m(1, 2, 1, MessageType::TEMPERATURE, readings.temperature);
        while(true){
            SensorReadings readings = bme.read();
            Message m(1, 2, 1, MessageType::TEMPERATURE, readings.temperature);
            n1.broadcast(m);
            std::cout << "Broadcasted: " << readings.temperature << "C\n";
            ::sleep(5);
        }
    }

    else if(nodeID == 2){
        Node n2(2);
        std::cout << "Node 2 listening...\n";
        while(true){
            if(n2.receive()){
                std::cout << "Message received!\n";
                n2.print_node();
            }
        }
    }
    return 0;
}

