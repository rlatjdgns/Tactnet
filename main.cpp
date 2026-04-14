#include "include/node.h"
#include "include/scheduler.h"
#include "include/LoRaDriver.h"
#include <iostream>
#include <string>

void printMessage(const Message& m){
    m.print();
}


int main(){
    /*
    std::cout << "Program started\n";
    Message m = Message(1,2,1,MessageType::ERROR, 1.5);
    std::cout << m.toString();


    */
    
    // Node n1 = Node(1);
    // Node n2 = Node(2);
    // Node n3 = Node(3);

    // n1.add_neighbor(&n2); n1.add_neighbor(&n3);
    // n2.add_neighbor(&n1); n2.add_neighbor(&n3);
    // n3.add_neighbor(&n1); n3.add_neighbor(&n2);

    // Task t1 = Task(TaskType::SEND_TEMPERATURE, 3, 10);
    // Task t2 = Task(TaskType::SEND_STATUS, 2, 5);
    // Task t3 = Task(TaskType::SEND_ERROR, 1, 2);
    
    // n1.add_task(t1);n1.add_task(t2); n1.add_task(t3);
    // n1.run_task();

    // n1.print_node(); n2.print_node(); n3.print_node();
        


    // LoRaDriver LD1 = LoRaDriver(1);
    // LD1.begin();
    // LD1.send(2,"Hello");
    // std::cout<<LD1.recieve()<<"\n";
    // LD1.disconnect();

    

    LoRaDriver LD1(1);
    LD1.begin();
    ReceivedMessage msg = LD1.recieve();
    std::cout << "From: " << msg.senderAddress << "\n";
    std::cout << "Message: " << msg.payload << "\n";
    std::cout << "RSSI: " << msg.rssi << "\n";  
    return 0;
}

