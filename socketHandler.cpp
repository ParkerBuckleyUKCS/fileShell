#include "socketHandler.h"

socketHandler::socketHandler()
{
	downloading = false;
	uploading = false;

	Address.sin_family = AF_INET;
	Address.sin_addr.s_addr = INADDR_ANY;
	Address.sin_port = htons( port );

	std::string stringIP = "172.31.146.76";
	serverIP = new char[stringIP.size()];
	for(int i = 0; i < stringIP.size(); i++)
	{
		serverIP[i] = stringIP[i];
	}	
	serverIP[stringIP.size()] = '\0';
}

socketHandler::~socketHandler()
{
	close(client_fd);
	close(server_fd);
	close(new_socket);
}
/*
void socketHandler::setFilename(std::string fName)
{
	for(int i = 0; i < fName.size(); i++)
	{
		filename[i] = fName[i];
	}
	filename[fName.size()] = '\0';
}
*/
void socketHandler::setPort(std::string port)
{
	char* Cport;
	Cport = new char[port.size()];
	for(int i = 0; i < port.size(); i++)
	{
		Cport[i] = port[i];
	}	
	Cport[port.size()] = '\0';

	int portNO = atoi(Cport);
	this->port = static_cast<short>(portNO);
} 

void socketHandler::setIP(std::string tempIP)
{
	serverIP = new char[tempIP.size()];
	for(int i = 0; i < tempIP.size(); i++)
	{
		serverIP[i] = tempIP[i];
	}	
	serverIP[tempIP.size()] = '\0';
}

void socketHandler::serverConnect()
{
	//creating the socket
	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == 0)	//error	
	{
		perror("Failure in creating socket file descriptor");
		exit(EXIT_FAILURE);
	}
	//attaching the socket to port
	if ( setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT ,&opt, sizeof(opt)))
	{
		perror("Error in setting the socket opt");
		exit(EXIT_FAILURE);
	}
	//attempt to bind the port 
	auto retVal = bind(server_fd, (struct sockaddr *)&Address, sizeof(Address));
	if (retVal < 0) //error
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	//listen for the client message!
	std::cout << "Listening\n";
    	if (listen(server_fd, 3) < 0) 
    	{ 
        	perror("listen"); 
        	exit(EXIT_FAILURE); 
    	} 	
	//accept from server
	new_socket = accept(server_fd, (struct sockaddr *)&Address, (socklen_t*)&addrlen);
	if (new_socket < 0) //error
	{
		perror("Error in server acceptance after listening");
		exit(EXIT_FAILURE);
	} 

	std::cout << "Server connected successfully!" << std::endl;	
}

void socketHandler::clientConnect()
{	
	std::cout << "Connecting to Server." << std::endl; 
	client_fd = 0; 
	int valread;
	
	client_fd = socket(AF_INET, SOCK_STREAM, 0);	//create client socket
	if(client_fd < 0)//error
	{
		std::cout << "Socket creation error";
		return;
	} 	
	
	memset(&serverAddress,'0',sizeof(serverAddress));
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons( port );
	
	auto retVal = inet_pton(AF_INET, serverIP, &serverAddress.sin_addr);
	if(retVal <= 0)
	{
		std::cout << "Error in establishing Server IP Address.";		
		return;
	}
	
	retVal = connect(client_fd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
	if(retVal < 0)	//error
	{
		std::cout << "Connection Failed to Server";
		return;
	}
	std::cout << "Client Connected" << std::endl;
}

void socketHandler::setWho(std::string identity)
{
	who = identity;
}

void socketHandler::download(std::string fileName)
{
	if(who == "client")
	{
		if (!downloading && !uploading)
		{
			unsigned long long int bytes = 0;
			downloading = true;
			char buffer_read[256];
			bzero(buffer_read,256);
			int n = 1;
				
			sendCommand(fileName);	//send filename to the server
			
			fout.open(fileName , std::ios::binary | std::ios::out);
			while(n > 0){
				n = read(client_fd,buffer_read,255);
				fout.write(buffer_read,256);
				if (fout.eof()) {n = -1;}
			}				

			if(fout.eof())
				std::cout << "Transfer successful" << std::endl;
			else
				std::cout << "Error in tranfer" << std::endl;

			fout.close();
		}
	}
	else if(who == "server")
	{
		if (!downloading && !uploading)
		{
			unsigned long long int bytes = 0;
			downloading = true;
			char buffer_read[256];
			bzero(buffer_read,256);
			int n = 1;

			while (n > 0)
			{
				n = read(new_socket,buffer_read,255);
			}

			
		}
	}
	else std::cout << "Failed to set string who in the socketHandler" << std::endl;
}

void socketHandler::upload(std::string fileName)
{
	if(who == "client")
	{
		if(!downloading && !uploading)
		{
			uploading = true;
			unsigned long long int bytes = 0;
			char buffer_write[256];
			bzero(buffer_write,256);
			int n = 0;
				
			sendCommand(fileName);	//send filename to the server
			while(n > 0){
				n = write(client_fd,buffer_write,255);
			}		
		}
	}
	else if (who == "server")
	{
		if(!downloading && !uploading)
		{
			unsigned long long int bytes = 0;
			downloading = true;
			char sendBuff[256] = {0};
			std::cout << "File found" << std::endl;
			fin.open(fileName,std::ios::binary | std::ios::in);
			std::cout << "File: " << fileName << " has been opened\n";	
			
			while(fin.get(sendBuff,256))
			{					
				int n = write(new_socket,sendBuff,256);
				if (n < 0)
				{
					std::cout << "Transfer Completed" << std::endl;
				} 
			}
						
			if(fin.eof())
				std::cout << "Transfer Successful" << std::endl;
			else
				perror("Transfer Failed");

			fin.close();

		}
	}
	else std::cout << "Failed to set who in socketHandler" << std::endl;	
}

void socketHandler::sendCommand(std::string command)
{
	const int size = command.size();
	char buffer_write[size];
	bzero(buffer_write,size);

	for(int i = 0; i < size; i++)
	{
		buffer_write[i] = command[i];
	}
	buffer_write[size] = '\0';

	int n = 0;
	n = write(client_fd,buffer_write,size);
	if (n < 0)
	{
		std::cout << "Error writing command to socket" << std::endl;
		return;
	}
}
	
std::string socketHandler::recieveCommand()
{	
	const int size = 20;
	char buffer_read[size] = {0};
	bzero(buffer_read,size);

	int n = 0;
	n = read(new_socket,buffer_read,size);
	if (n < 0)
	{
		std::cout << "Error returning command from socket" << std::endl;
		return "";
	}
	
	std::string command = "";
	for(int i = 0; buffer_read[i] != '\0'; i++)
	{
		command += buffer_read[i];
	}
	return command;
}

