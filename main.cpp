#include "include/message.h"

int main(){
    Message m(1,2,3,MessageType::TEMPERATURE,23.3);
    m.print();
    return 0;
}
