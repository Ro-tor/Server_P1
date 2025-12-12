#include<iostream>
#include<string>
#include<cstring>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>


const int SERVERPORT = 8080;
const int BUFFER_SIZE = 1025;
const char* SERVERIP ="127.0.0.1"; 
int main(){
    int clientSocket = socket(AF_INET, 
                              SOCK_STREAM, 
                              0);
	if(clientSocket < 0) {
		return 1;
	}
    sockaddr_in serverAddr;
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(SERVERPORT);
    inet_pton(AF_INET,SERVERIP,&serverAddr.sin_addr);

    if( connect(clientSocket,
                (sockaddr*)&serverAddr,
                sizeof(serverAddr) )
        < 0
        ) { return 2; }

    for (int i = 0 ; i < 3; i++){
        std::string message="ping";
        send(   clientSocket,
                message.c_str(),
                message.length(),
                0 );

        char buffer[BUFFER_SIZE];

        int byteRecieved = recv(clientSocket,
                                buffer,
                                BUFFER_SIZE - 1,
                                0);
        if (byteRecieved > 0){
            std::cout << buffer << std::endl;
        }
    } // for
    send(
        clientSocket,
        "exit", 
        4,
        0   );//close server
    close (clientSocket);
    return 0;
}