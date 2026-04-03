#include "include/message.h"
void printMessage(const Message& m){
    m.print();
}


int main(){
    Message m(1,2,3,MessageType::TEMPERATURE,23.3);
    printMessage(m);
    return 0;
}
