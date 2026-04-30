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
    neighbor_online[0]=neighbor_online[1]= true;
    last_heard[0] = last_heard[1] = time(nullptr);
    lora.begin();
    bme.begin();
}

SensorReadings Node::read_sensor(){return bme.read();}

int Node::get_node_ID(){return node_ID;}
int Node::get_neighbor_count(){return neighbor_count;}
int Node::get_neighbor_address(int i){return neighbor_addresses[i];}

bool Node::receive_message(Message m){
    messages[message_count % 10] = m;
    message_count++;
    return true;
}

bool Node::receive() {
    ReceivedMessage msg = lora.receive();
    if (msg.senderAddress == 0) return false;

    // update last heard for this neighbor
    for (int i = 0; i < neighbor_count; i++) {
        if (neighbor_addresses[i] == msg.senderAddress) {
            last_heard[i] = time(nullptr);
            break;
        }
    }
    // parse payload fields split by |
    std::stringstream ss(msg.payload);
    std::string token;
    std::vector<std::string> tokens;
    while (std::getline(ss, token, '|')) {
        tokens.push_back(token);
    }

    std::string sensorPayload = tokens[4].substr(9) + "|" + tokens[5] + "|" + tokens[6];
    int destinationID = std::stoi(tokens[1]);

    if (destinationID == node_ID) {
        MessageType type;
        if(tokens[3] == "S") type = MessageType::SENSORREADING;
        else if(tokens[3] == "P") type = MessageType::STATUS_PING;
        else if(tokens[3] == "E") type = MessageType::ERROR;
        else type = MessageType::SENSORREADING;
        Message m(msg.senderAddress, node_ID, message_count, type, sensorPayload);
        return receive_message(m);
    } else {
        Message m(msg.senderAddress, node_ID, message_count, MessageType::SENSORREADING, sensorPayload);
        receive_message(m);
        for (int i = 0; i < neighbor_count; i++) {
            if (neighbor_addresses[i] == destinationID) {
                std::cout<<"Relaying to "<<neighbor_addresses[i]<<"\n"
                lora.send(neighbor_addresses[i], msg.payload);
            }
        }
        return true;
    }
}

void Node::add_task(Task t)  { scheduler.add_task(t); }
void Node::run_task()        { scheduler.execute(*this); }

bool Node::add_neighbor(int address){
    if(neighbor_count==2) return false;
    neighbor_addresses[neighbor_count++] = address;
    return true;
}

bool Node::send_to(int address, Message m){
    return lora.send(address,m.toString());
}

bool Node::broadcast(Message m) {
    int error_count = 0;
    for (int i = 0; i < neighbor_count; i++) {
        if (!lora.send(neighbor_addresses[i], m.toString())) error_count++;
        ::sleep(1);
    }
    return error_count < neighbor_count;
}

void Node::check_neighbors() {
    for (int i = 0; i < neighbor_count; i++) {
        bool timed_out = (time(nullptr) - last_heard[i]) > 30;
        std::cout << "diff for Node " << neighbor_addresses[i] << ": " << (time(nullptr) - last_heard[i]) << "\n";
        if (timed_out && neighbor_online[i]) {
            neighbor_online[i] = false;
            std::cout << "Connection to Node " << neighbor_addresses[i] << " lost\n";
        } else if (!timed_out && !neighbor_online[i]) {
            neighbor_online[i] = true;
            std::cout << "Node " << neighbor_addresses[i] << " reconnected\n";
        }
    }
}


void Node::print_node(){
    if(message_count > 0){
        messages[(message_count - 1) % 10].print();
    }
    
}