#include "node.h"
#include <iostream>

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
    if(msg.senderAddress!=0){
        std::string payloadStr = msg.payload;
        size_t pos = payloadStr.find("Payload: ");
        std::cout << "payloadStr: '" << payloadStr << "'\n";
        if(pos != std::string::npos){
            payloadStr = payloadStr.substr(pos + 9);
        }
        Message m = Message(this->node_ID, msg.senderAddress, this->message_count, MessageType::TEMPERATURE, std::stod(payloadStr));
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
    std::cout<<node_ID<<"\n";
    for(int i=0; i<message_count;i++){
        std::cout<<"Message No."<<i+1<<"\n";
        messages[i].print();
    }
}