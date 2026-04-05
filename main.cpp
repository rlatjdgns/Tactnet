#include "include/node.h"
#include <iostream>
void printMessage(const Message& m){
    m.print();
}


int main(){
    Message m(1,2,3,MessageType::TEMPERATURE,23.3);
    Node n1(1);
    Node n2(2);
    for(int i=0;i<11;i++){
        bool a = n1.send_message(m,n2);       
        if (a==true){
            std::cout<<"Success: Message sent \n";
        }
        else
            std::cout<<"Failure: Node is full\n"; 

    }
    n2.print_node();

    return 0;
}
