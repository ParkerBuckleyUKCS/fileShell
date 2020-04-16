#include <iostream>
#include <fstream>
#include <limits>	//for filesize
#include <string>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> //networking
#include <stdlib.h> 
#include <netinet/in.h> //networking
#include <arpa/inet.h>	//networking
#include <string.h>
#include <chrono>	//for bitrate measurement
#include <fcntl.h> // for O_RDONLY
#include <sys/stat.h> //filesize?
#include <sys/sendfile.h> //sendfile()
#include <vector>	//for csvdata
 
class socketHandler {
public:
	socketHandler();
	~socketHandler();
	void setPort(std::string port);
	void setIP(std::string tempIP);
	void clientConnect();	//establish sockets
	void serverConnect();	//establish sockets
	void download(std::string fileName);
	void upload(std::string fileName);
	void setWho(std::string identity);	//used to call proper member functions for server/client
	void sendCommand(std::string command);
	std::string recieveCommand();	//used for sending commands to/from server
	void CSV(std::string filename);	//output for CSV file 
private:
	short port = 1337;
	int client_fd, server_fd, new_socket, valread, opt = 1;
	struct sockaddr_in Address;
	struct sockaddr_in serverAddress;	
	int addrlen = sizeof(Address);
	char * serverIP;
	bool downloading, uploading;
	std::string who;	//identifies client/server
	std::fstream fin, fout;
	int filehandle;		//used to establish download buffers with malloc
	struct stat obj;	//used for taking filesize
	std::vector<long int> csvData;
}; 
