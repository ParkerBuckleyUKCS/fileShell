#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h>
#include <stdlib.h>
#include <chrono>
 
class socketHandler {
public:
	socketHandler();
	~socketHandler();
	void setPort(std::string port);
	void setIP(std::string tempIP);
	void clientConnect();
	void serverConnect();
	//void setFilename(std::string fName);
	void download(std::string fileName);
	void upload(std::string fileName);
	void setWho(std::string identity);
	void sendCommand(std::string command);
	std::string recieveCommand();
private:
	short port = 1337;
	int client_fd, server_fd, new_socket, valread, opt = 1;
	struct sockaddr_in Address;
	struct sockaddr_in serverAddress;	
	int addrlen = sizeof(Address);
	char * serverIP;
	bool downloading, uploading;
	//char * buffer_write;
	std::string who;
	std::fstream fin, fout;
}; 
