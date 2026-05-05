#ifndef NODE_H
#define NODE_H

#include "message.h"
#include "scheduler.h"
#include "LoRaDriver.h"
#include "BME280Driver.h"
#include <ctime>

class Node {
private:
    int       node_ID;
    Message   messages[10];       // circular buffer of received messages
    int       message_count;
    int       neighbor_addresses[2];
    int       neighbor_count;
    Scheduler scheduler;
    LoRaDriver lora;              // UART driver for RYLR998
    BME280Driver bme;             // I2C driver for BME280 sensor
    time_t    last_heard[2];      // timestamps for failover detection
    bool      neighbor_online[2]; // online status per neighbor

public:
    Node(int node_ID);
    
    // sensor
    SensorReadings read_sensor();

    // getters
    int     get_node_ID();
    int     get_neighbor_count();
    int     get_neighbor_address(int i);
    Message get_latest_message();
    bool    is_neighbor_online(int i);

    // network
    bool receive_message(Message m);
    bool receive();
    bool add_neighbor(int address);
    bool send_to(int address, Message m);
    bool broadcast(Message m);

    // task scheduling
    void add_task(Task t);
    void run_task();

    void check_neighbors();
    void print_node();
};

#endif