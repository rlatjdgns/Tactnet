#include "include/node.h"
#include "include/scheduler.h"
#include "include/LoRaDriver.h"
#include <iostream>
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
        n1.add_task(Task(TaskType::SEND_READINGS, 1, 10));

        while (true) {
            n1.run_task();
            ::sleep(3);
            for(int i = 0; i < 5; i++){
                if (n1.receive()) n1.print_node();
            }
            n1.check_neighbors();
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
                SensorReadings r = n2.read_sensor();
                std::string payload = build_payload(r);
                MessageType  type   = is_alert(r) ? MessageType::ERROR : MessageType::STATUS_PING;
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
                MessageType  type   = is_alert(r) ? MessageType::ERROR : MessageType::STATUS_PING;
                n3.send_to(1, Message(3, 1, 1, type, payload));
            }
        }
    }
    return 0;
}