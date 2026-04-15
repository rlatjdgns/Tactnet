#include "include/node.h"
#include "include/scheduler.h"
#include "include/LoRaDriver.h"
#include <iostream>
#include <string>
#include <cstdlib>

void printMessage(const Message& m){
    m.print();
}


int main(int argc, char* argv[]){
    int nodeID = std::atoi(argv[1]); 
    
    if(nodeID == 1){
        Node n1(1);
        n1.add_neighbor(2);
        Message m(1, 2, 1, MessageType::TEMPERATURE, 23.5);
        bool result = n1.broadcast(m);
        std::cout << "Broadcast result: " << result << "\n";
    }

    else if(nodeID == 2){
        Node n2(2);
        if(n2.receive()){
            std::cout << "Message received\n";
            n2.print_node();
        } else {
            std::cout << "No message\n";
        }
    }
    return 0;
}

