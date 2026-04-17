#include "node.h"
#include <iostream>
#include <sstream>
#include <vector>

Node::Node(int node_ID):lora(node_ID){
    this->node_ID = node_ID;
    message_count = 0;
    neighbor_count = 0;
    neighbor_addresses[0] = neighbor_addresses[1] = 0;
    lora.begin();
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

bool Node::receive(){
    ReceivedMessage msg = lora.receive();
    if(msg.senderAddress != 0){
        std::stringstream ss(msg.payload);
        std::string token;
        std::vector<std::string> tokens;
        while(std::getline(ss, token, '|')){
            tokens.push_back(token);
        }
        
        if(tokens.size() < 7) return false;
        
        std::string payloadStr = tokens[4] + "|" + tokens[5] + "|" + tokens[6];
        Message m = Message(std::stoi(tokens[0]), this->node_ID, std::stoi(tokens[2]), MessageType::SENSORREADING, payloadStr);
        return receive_message(m);
    }
    return false;
}

void Node::add_task(Task t){
    scheduler.add_task(t);
}

void Node::run_task(){
    scheduler.execute(*this);
    scheduler.print();
}
bool Node::add_neighbor(int address){
    if(neighbor_count==2){
        return false;
    }
    else{
        neighbor_addresses[neighbor_count]=address;
        neighbor_count++;
        return true;
    }
}

bool Node::broadcast(Message m){
    int error_count = 0;
    for(int i=0;i<neighbor_count;i++){
        if(!lora.send(neighbor_addresses[i],m.toString())){
            error_count++;
        }
    }
    if(error_count==neighbor_count){
        return false;
    }
    return true;
}

void Node::print_node(){
    if(message_count > 0){
        messages[message_count - 1].print();
    }
}