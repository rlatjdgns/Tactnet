#include "node.h"
#include <iostream>

Node::Node(int node_ID){
    this->node_ID = node_ID;
    message_count = 0;
}

void Node::receive_message(Message m){
    if(message_count==10){
        std::cout << "Node full\n";
        return;
    }
    else{
        messages[message_count]=m;
        message_count++;
    }
}


void Node::send_message(Message m){

}

void Node::print_node(){
    std::cout<<node_ID<<"\n";
    for(int i=0; i<message_count;i++){
        messages[i].print();
    }
}