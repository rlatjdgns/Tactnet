#include "include/node.h"
void printMessage(const Message& m){
    m.print();
}


int main(){
    Message m(1,2,3,MessageType::TEMPERATURE,23.3);
    printMessage(m);

    Node n1(1);
    n1.receive_message(m);
    n1.print_node();

    return 0;
}
