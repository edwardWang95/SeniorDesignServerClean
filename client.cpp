#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include <thread>

#define ServerQueue 5

char* ipAddress;
int serverPort;
struct sockaddr_in server_addr;
int serverfd = 0;
int valRead = 0;
char buffer[1024] = {0};

void setupClient();
void recieveInput();

int main(int argc, char* argv[]) {
    ipAddress = argv[1];
    serverPort = atoi(argv[2]);
    setupClient();
    recieveInput();
    return 0;
}

void setupClient(){
    //Setup socket file descriptor
    if((serverfd = socket(AF_INET,SOCK_STREAM,0))==0){
        perror("setupServer[serverfd]");
        exit(1);
    }
    std::cout << "=> Client Socket created" << std::endl;
    //setup socket address in
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(ipAddress);    //server finds: INADDR_ANY
    //server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(serverPort);
    //bind serverSocket socket to serverSocket address
    if (connect(serverfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("connectServer[connect]");
        exit(1);
    }
    std::cout << "=> Client port bounded to " << serverPort << std::endl;
}

void recieveInput(){
    bool running = true;
    std::cout << "=> Waiting for server..." <<  std::endl;
    while(running){
        valRead = read(serverfd, buffer, 1024);
        printf("Read:\t%s", buffer);
    }
}