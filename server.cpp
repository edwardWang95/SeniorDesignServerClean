#include <iostream>
#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <vector>
#include <thread>

#define ServerQueue 5

std::string ipAddress = "";
int serverfd, serverPort;
struct sockaddr_in server_addr;
int socketOption = 1;
//Client
int clientCounter = 0;
std::vector<int> clientPortList;
std::vector<pthread_t> threadList;

void setupServer();
void acceptClients();
void* handleClientThread(void *vptr);

int main() {
    setupServer();
    acceptClients();
    return 0;
}

void setupServer(){
    //Setup socket file descriptor
    if((serverfd = socket(AF_INET,SOCK_STREAM,0))==0){
        perror("setupServer[serverfd]");
        exit(1);
    }
    //Attach socket to server port
    if(setsockopt(serverfd, SOL_SOCKET, SO_REUSEADDR, &socketOption, sizeof(socketOption))) {
        perror("setupServer[sockoptResult]");
        exit(1);
    }
    std::cout << "=> Server Socket created" << std::endl;
    //setup socket address in
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(serverPort);
    //bind serverSocket socket to serverSocket address
    if (bind(serverfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("setupServer[bind]");
        exit(1);
    }
    std::cout << "=> Server port bounded to " << serverPort << std::endl;
    //listen for clients
    if(listen(serverfd, ServerQueue) < 0){
        perror("setupServer[listen]");
    }
    std::cout << "=> Server port open to clients" << std::endl;
}

void acceptClients(){
    bool running = true;
    std::cout << "=> Waiting for client..." <<  std::endl;
    while(running){
        std::cout << std::endl;
        int clientPort;
        struct sockaddr_in clientSocketAddress;
        socklen_t length;
        pthread_t pthread;
        //Logic
        length = sizeof(clientSocketAddress);
        clientPort = accept(serverfd,(struct sockaddr *)&clientSocketAddress, &length);
        if(clientPort > 0){
            std::cout << "=> Accepted Client:Port[" << clientPort << "]" << std::endl;
            clientPortList.push_back(clientPort);
            clientCounter++;
            pthread_create(&pthread, NULL, &handleClientThread, (void *)&clientPort);
        }else{
            std::string statement = "acceptClient[" + std::to_string(clientPortList.size() - 1) + "]";
            perror(statement.c_str());
        }
    }
}

/*
 * Need index/pointer to the sockfd for the client to write to it
 *
 * n = write(newsockfd,"I got your message",18);
 * */
void* handleClientThread(void *vptr)
{
    //get arguments
    int clientfd = *((int *) vptr);
    unsigned long size = 0;
    /*
    while(true)
    {
        if(map.memory == NULL)continue;
        size = map.size;
        write(clientfd, map.memory, size);
    }
    */
    std::cout << "=> New client added : " << clientfd << std::endl;
    std::string input;
    while(true)
    {
        std::cin >> input;
        input += "\n";
        if(write(clientfd, input.c_str(), input.size()) > 0){
            std::cout << "=> Sent out: " << input << " to"  <<  clientfd << std::endl;
        }else{
            std::string statement = "handleClientThread[" + std::to_string(clientfd) + "]";
            perror(statement.c_str());
            pthread_exit(NULL);
        }
    }
}