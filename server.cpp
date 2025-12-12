#include<iostream>
#include<string>
#include<cstring>
#include<arpa/inet.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>

const int PORT = 8080;
const int BUFFER_SIZE = 1025;

int main() {
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(serverSocket < 0) {
		return 1;
	}
	sockaddr_in serverAddr;//структура
	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(PORT);
	if (    bind(serverSocket, 
                 (sockaddr*)&serverAddr, 
                 sizeof(serverAddr)
                )
                < 0
        ) {
		close(serverSocket);
		std::cout << "bind fail" << std::endl;
        return 1;
	}
	if (listen(serverSocket, 3) < 0) {
		close(serverSocket);
        return 2;
	}
    std::cout << "Socket bind successfully to port " << PORT << std::endl;

    sockaddr_in clientAddr;
	socklen_t clientAddrLen ;//Size = sizeof(clientAddr);
	int clientSocket = accept(serverSocket, 
                            (sockaddr*)&clientAddr,
                             &clientAddrLen);
	if (clientSocket < 0) {
		close(serverSocket);
        return 3;
	}
	
    char clientIP[INET_ADDRSTRLEN]; // это МАССИВ!!!!

	inet_ntop(AF_INET, 
            &clientAddr.sin_addr, 
            clientIP, 
            INET_ADDRSTRLEN);

	char BUFFER[BUFFER_SIZE];
	while (true) {
		memset(BUFFER, 0, BUFFER_SIZE);
		int byteRecieved = recv(clientSocket, BUFFER, BUFFER_SIZE - 1, 0);
		if (byteRecieved <= 0) {
			break;
		}
		std::string message(BUFFER);
		std::cout << message << std::endl;
		if (message == "exit") {
			break;
		}
		std::string responce;
		if (message == "ping") {
			responce = "pong";
		}
		else {
			responce = "unknown";
		}
        
		send(clientSocket, 
            responce.c_str(), 
            responce.length(), 
            0);
	}
	close(clientSocket);
	close(serverSocket);
}