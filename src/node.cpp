#include "node.h"
#include <iostream>
#include <sstream>
#include <vector>
#include <unistd.h>


Node::Node(int node_ID):lora(node_ID){
    this->node_ID = node_ID;
    message_count = 0;
    neighbor_count = 0;
    neighbor_addresses[0] = neighbor_addresses[1] = 0;
    lora.begin();
    neighbor_online[0]=neighbor_online[1]= true;
    last_heard[0] = last_heard[1] = time(nullptr);
}

int Node::get_node_ID(){
    return node_ID;
}

bool Node::receive_message(Message m){
    messages[message_count % 10] = m;
    message_count++;
    return true;
}

bool Node::receive(){
    ReceivedMessage msg = lora.receive();
    if(msg.senderAddress!=0){
        for(int i = 0; i < neighbor_count; i++){
            if(neighbor_addresses[i] == msg.senderAddress){
                last_heard[i] = time(nullptr);
                break;
            }
        }
        std::stringstream ss(msg.payload);
        std::string token;
        std::vector<std::string> tokens;
        while(std::getline(ss, token, '|')){
        tokens.push_back(token);
        }
        if(std::stoi(tokens[1])==node_ID){ 
            Message m = Message(msg.senderAddress, this->node_ID, this->message_count, MessageType::SENSORREADING, tokens[4].substr(9) + "|" + tokens[5] + "|" + tokens[6]);
            std::cout << "t4: " << tokens[4] << " t5: " << tokens[5] << " t6: " << tokens[6] << "\n";
            return receive_message(m);
        }
        else{
            for(int i =0; i<neighbor_count;i++){
                if(neighbor_addresses[i]==std::stoi(tokens[1])){
                    lora.send(neighbor_addresses[i],msg.payload);
                }                
            }
            return true;
        }

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
        ::sleep(1);
    }
    if(error_count==neighbor_count){
        return false;
    }
    return true;
}

void Node::check_neighbors(){
    for(int i=0; i<neighbor_count; i++){
        time_t diff = time(nullptr) - last_heard[i];
        if((time(nullptr) - last_heard[i])>30){
            if(neighbor_online[i]==true){
                neighbor_online[i]= false;
                std::cout << "Connection to Node " << neighbor_addresses[i] << " lost\n";
            }
        }
        else{
            if(neighbor_online[i]==false){
                neighbor_online[i]=true;
                std::cout<<"Reconnected\n";
            }
        }
    }
}

void Node::print_node(){
    if(message_count > 0){
        messages[(message_count - 1) % 10].print();
    }
    
}