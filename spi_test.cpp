#include <fcntl.h>   
#include <unistd.h> 
#include <iostream>

int main(){
    int fd = open("/dev/spidev0.0", O_RDWR);
    if(fd==-1){
        std::cout<<"Failiure\n";
    }
    else{
        std::cout<<"Success\n";
    }
    close(fd);
    return 0;
}
