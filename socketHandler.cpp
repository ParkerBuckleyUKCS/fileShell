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

	std::string arg = "rm -r tmp.txt";
	system(arg.c_str());
}

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
			int bytes = 0;
			int total = 0;
			downloading = true;
			char buffer_read[256];
			bzero(buffer_read,256);
			size_t n = 0;
				
			sendCommand(fileName);	//send filename to the server
			recv(client_fd, &bytes, sizeof(int), 0);
			filehandle = open(fileName.c_str(), O_CREAT | O_EXCL | O_WRONLY, 0666);
			if(filehandle < 0)
			{
				std::cout << "Error in file open/create" << std::endl;
			}
			char *f = (char*)malloc(bytes);
			memset(f,0,bytes);
	
			std::cout << "File found...";
			std::cout << "File: " << fileName << " has been opened...";				
			unsigned long long int totalDuration = 0;
			while(total < bytes)
			{	
				bool kbyteSend = false;
				using namespace std::chrono;
				auto start = high_resolution_clock::now();
				if(bytes - total < 1024)
				{
					recv(client_fd, f, 1024, 0);
					total += 1024;
					kbyteSend = true;
				}
				else
				{
					recv(client_fd, f, (bytes - total), 0);
					total += (bytes - total);
				}
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop-start);
				totalDuration += duration.count();
				if(static_cast<double>((double)totalDuration/1000000.0) > 1)
				{
					totalDuration = 0;
					if(kbyteSend)
						CSV(fileName,1,1024);
					else
						CSV(fileName,1,(bytes-total));
				}
			}
			
			std::cout << "...writing file...";
			n = write(filehandle,f,bytes);	
			int err = close(filehandle);
			if (err < 0)
				perror("Error on file close");
			if (n < 0)
			{
				std::cout << "File Write Failed" << std::endl;
			}
			else std::cout << "...File Write Completed" << std::endl;
			downloading = false;
			free(f);		
		}
	}
	else if(who == "server")
	{
		if (!downloading && !uploading)
		{
			int bytes = 0;
			int total = 0;
			downloading = true;
			char buffer_read[256];
			bzero(buffer_read,256);
			size_t n = 0;

			recv(new_socket, &bytes, sizeof(int), 0);
			filehandle = open(fileName.c_str(), O_CREAT | O_EXCL | O_WRONLY, 0666);
			if(filehandle < 0)
			{
				std::cout << "Error in file open/create" << std::endl;
			}
			char *f = (char*)malloc(bytes);
			memset(f,0,bytes);			

			unsigned long long int totalDuration = 0;
			while(total < bytes)
			{
				bool kbyteSend = false;
				using namespace std::chrono;
				auto start = high_resolution_clock::now();
				if(bytes - total < 1024)
				{
					recv(new_socket, f, 1024, 0);
					total += 1024;
					kbyteSend = true;
				}
				else
				{
					recv(new_socket, f, (bytes - total), 0);
					total += (bytes - total);
				}
				auto stop = high_resolution_clock::now();
				auto duration = duration_cast<microseconds>(stop-start);
				totalDuration += duration.count();
				if(static_cast<double>((double)totalDuration/1000000.0) > 1)
				{
					totalDuration = 0;
					if(kbyteSend)
						CSV(fileName,1,1024);
					else
						CSV(fileName,1,(bytes-total));
				}
			}
			std::cout << "File found...";
			std::cout << "File: " << fileName << " has been opened...";	
			if (bytes > 0)	
			 	n = write(filehandle,f,bytes);
			if (n < 0)
			{
				std::cout << "File Write Failed" << std::endl;
			}
			else std::cout << "File Write Completed" << std::endl;
			downloading = false;
			close(filehandle);
			free(f);
			std::cout << "File handle closed" << std::endl;
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
			int bytes = 0;
	
			sendCommand(fileName);	//send filename to the server
			stat(fileName.c_str(), &obj);	// find file size
			filehandle = open(fileName.c_str(), O_RDONLY); //open file
			bytes = obj.st_size;	// init file size
			if(filehandle < 0)
			{
				std::cout << "Error in file open/read";
			}
			else {
				std::cout << "...File Read Successfully" << std::endl;	
				send(client_fd, &bytes, sizeof(int), 0); //send file size
				sendfile(client_fd, filehandle, NULL, bytes);
			}
			close(filehandle);
			uploading = false;
		}
	}
	else if (who == "server")
	{
		if(!downloading && !uploading)
		{
			int bytes = 0;
			uploading = true;
			
			stat(fileName.c_str(), &obj);
			filehandle = open(fileName.c_str(), O_RDONLY);
			bytes = obj.st_size;

			if(filehandle < 0)
			{
				std::cout << "Error in file read for upload" << std::endl;
			}
			else 
			{
				std::cout << "...File read successful";
				send(new_socket, &bytes, sizeof(int), 0); //send file size
				sendfile(new_socket, filehandle, NULL, bytes); //send file
			}
			
			close(filehandle);
			std::cout << "...File Handle Closed" << std::endl;
			uploading = false;
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
	if (who == "client")
		n = write(client_fd,buffer_write,size);
	else
		n = write(new_socket,buffer_write,size);
	if (n < 0)
	{
		std::cout << "Error writing command to socket" << std::endl;
		return;
	}
}
	
std::string socketHandler::recieveCommand()
{	
	const int size = 40;
	char buffer_read[size] = {0};
	bzero(buffer_read,size);

	int n = 0;
	if (who == "server")
		n = read(new_socket,buffer_read,size);
	else
		n = read(client_fd,buffer_read,size);
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

void socketHandler::CSV(std::string filename, int seconds, int bytes)
{
	std::string outfile = filename + "_output.csv";
	fout.open(outfile, std::ofstream::out);
	fout << seconds << "," << bytes << std::endl;
	fout.close();
}
