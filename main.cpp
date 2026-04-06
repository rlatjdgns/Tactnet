#include "include/node.h"
#include "include/scheduler.h"
#include <iostream>
void printMessage(const Message& m){
    m.print();
}


int main(){
    /*
    Message m(1,2,3,MessageType::TEMPERATURE,23.3);
    Node n1(1);
    Node n2(2);
    Node n3(3);

    n1.add_neighbor(&n2);
    n1.add_neighbor(&n3);
    n2.add_neighbor(&n1);
    n2.add_neighbor(&n3);
    n3.add_neighbor(&n1);
    n3.add_neighbor(&n2);

    if(n1.broadcast(m)){
        std::cout<<"Message broadcasted succesfully to neighbors\n";
    }
    else{
        std::cout<<"Failed to broadcast message\n";
    }
     
    n1.print_node();
    n2.print_node();
    n3.print_node();
    */

    Scheduler s = Scheduler();
    Task t1 = Task(TaskType::SEND_TEMPERATURE, 3, 10);
    Task t2 = Task(TaskType::SEND_STATUS, 2, 5);
    Task t3 = Task(TaskType::SEND_ERROR, 1, 2);

    s.add_task(t1); s.add_task(t2); s.add_task(t3);
    s.print();
    s.execute();
    s.print();
    return 0;
}

