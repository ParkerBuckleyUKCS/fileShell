# Client-Server FTP in C++


## Files: 
* main.cpp	// for client shell
* server.cpp
* eventHandler.cpp
* eventHandler.h
* parser.cpp
* parser.h
* scanner.cpp
* scanner.h
* prompter.cpp
* prompter.h
* socketHandler.h	
* socketHandler.cpp
* Makefile

## Description:
This client server pair allows for file transfer and directory navigation clientside.
The clientside has a fully functional shell, with pwd, cd, ls functionalities.
Primitive builtin commands are as follows:
* CONNECT [ipaddress] [port]
* UPLOAD [filename]
* DOWNLOAD [filename]
* NEWPROMPT [prompt]	//changes the prompt for the shell
* CD [argument] 		//change dir, takes the same args as system("cd")
* CD 			// using CD with no argument Prints the working directory!
* BYE			//exits the shell safetly and frees all pointers/sockets
* DELETE [filename]	//deletes a file on the server
* DIR			//pulls file information from the server
* LV			//lists environment variables in the shell
* LS			//list files in current dir
* $[variablename]		//dereferences environment variable	
* [variablename] = "[value]"	//stores a var in the client environment.
				MUST USE spaces in between <variablename> and = and <value>
				<value> must have "" quotations
	
## Default Shell Environment variables:
* HomeDir : stores the working directory of the shell.
	You may use CD $HomeDir to return to the HomeDir path.
* PATH : default value is /usr:/bin:/usr/bin
* ShowTokens : A Boolean to show the tokens being extracted by the client.
	Default value is zero. To turn on, type ShowTokens = 1
	
## Building:
call 'make clean' to remove all of the .o objects for the linker.
call 'make' to build the fileClient executable.
call 'make fileServer' to build the fileServer executable.
