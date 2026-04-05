#include "node.h"
#include <iostream>

Node::Node(int node_ID){
    this->node_ID = node_ID;
    message_count = 0;
}

bool Node::receive_message(Message m){
    if(message_count==10){
        return false;
    }
    else{
        messages[message_count]=m;
        message_count++;
        return true;
    }
}


bool Node::send_message(Message m, Node& receiver){
    return receiver.receive_message(m);
}

void Node::print_node(){
    std::cout<<node_ID<<"\n";
    for(int i=0; i<message_count;i++){
        std::cout<<"Message No."<<i+1<<"\n";
        messages[i].print();
    }
}