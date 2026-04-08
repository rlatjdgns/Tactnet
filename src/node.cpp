#include "node.h"
#include <iostream>

Node::Node(int node_ID){
    this->node_ID = node_ID;
    message_count = 0;
    neighbor_count = 0;
    neighbors[0]=nullptr;
    neighbors[1]=nullptr;
}

int Node::get_node_ID(){
    return node_ID;
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

bool Node::send_message(Message m, Node* receiver){
    if(receiver==nullptr){
        return false;
    }
    return receiver->receive_message(m);
}
void Node::add_task(Task t){
    scheduler.add_task(t);
}

void Node::run_task(){
    scheduler.execute(*this);
    scheduler.print();
}
bool Node::add_neighbor(Node* neighbor_node){
    if(neighbor_count==2){
        return false;
    }
    else{
        neighbors[neighbor_count]=neighbor_node;
        neighbor_count++;
        return true;
    }
}

bool Node::broadcast(Message m){
    int error_count = 0;
    for(int i=0;i<neighbor_count;i++){
        if(!send_message(m,neighbors[i])){
            error_count++;
        }
    }
    if(error_count==neighbor_count){
        return false;
    }
    return true;
}
void Node::print_node(){
    std::cout<<node_ID<<"\n";
    for(int i=0; i<message_count;i++){
        std::cout<<"Message No."<<i+1<<"\n";
        messages[i].print();
    }
}