#include "include/node.h"
#include "include/scheduler.h"
#include "include/LoRaDriver.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <unistd.h>

// helper to build sensor payload string
static std::string build_payload(const SensorReadings& r) {
    return std::to_string(r.temperature) + "|" +
           std::to_string(r.humidity)    + "|" +
           std::to_string(r.pressure);
}

// helper to check if readings exceed safe thresholds
static bool is_alert(const SensorReadings& r) {
    return r.temperature > 35.0f || r.temperature < 10.0f || r.humidity > 95.0f;
}

int main(int argc, char* argv[]) {
    int nodeID = std::atoi(argv[1]);
    
    if (nodeID == 1) {
        Node n1(1);
        n1.add_neighbor(2);
        n1.add_task(Task(TaskType::SEND_READINGS, 1, 5));
        std::string node2_payload = "0|0|0";
        std::string node3_payload = "0|0|0";
        std::string alert = ""; 
        
        while (true) {
            n1.run_task();

            for(int i = 0; i <10; i++){
                if (n1.receive()){
                    Message latest = n1.get_latest_message();
                    if (latest.get_sender_ID() == 2) {
                        node2_payload = latest.get_payload();
                        if (latest.get_message_type() == MessageType::ERROR) alert = "Node 2 ALERT";
                    }
                    if (latest.get_sender_ID() == 3) {
                        node3_payload = latest.get_payload();
                        if (latest.get_message_type() == MessageType::ERROR) alert = "Node 3 ALERT";
                    }
                    n1.print_node();
                }
                ::usleep(200000); 
            }
            SensorReadings r1 = n1.read_sensor();
            std::ofstream file("/tmp/tactnet_data.json");
            n1.check_neighbors();
            file << "{\n";
            file << "  \"node1\": \"" << build_payload(r1) << "\",\n";
            file << "  \"node2\": \"" << node2_payload << "\",\n";
            file << "  \"node3\": \"" << node3_payload << "\",\n";
            file << "  \"node2_status\": \"" << (n1.is_neighbor_online(0) ? "online" : "offline") << "\",\n";
            file << "  \"node3_status\": \"" << (n1.is_neighbor_online(1) ? "online" : "offline") << "\",\n";
            file << "  \"alert\": \"" << alert << "\"\n";
            file << "}\n";
            file.close();                    

        }
    }

    else if (nodeID == 2) {
        Node n2(2);
        n2.add_neighbor(1);
        n2.add_neighbor(3);
        std::cout << "Node 2 listening...\n-----------------------------------\n";
        while (true) {
            if (n2.receive()) {
                n2.print_node();
                ::usleep(500000); 
                SensorReadings r = n2.read_sensor();
                std::string payload = build_payload(r);
                MessageType type = is_alert(r) ? MessageType::ERROR : MessageType::STATUS_PING;
                n2.send_to(1, Message(2, 1, 1, type, payload));
            }
        }
    }

    else if (nodeID == 3) {
        Node n3(3);
        n3.add_neighbor(1);
        std::cout << "Node 3 listening...\n-----------------------------------\n";
        while (true) {
            if (n3.receive()) {
                n3.print_node();
                SensorReadings r = n3.read_sensor();
                std::string payload = build_payload(r);
                MessageType type = is_alert(r) ? MessageType::ERROR : MessageType::STATUS_PING;
                n3.send_to(1, Message(3, 1, 1, type, payload));
            }
        }
    }
    return 0;
}