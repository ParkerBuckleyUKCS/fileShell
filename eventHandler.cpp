#include "eventHandler.h"

eventHandler::eventHandler()
{
    	char cwd[1024]; 
    	getcwd(cwd, sizeof(cwd)); 
    	printf("\nHomeDir: %s", cwd);
	cout << endl;
	
	storeVariable("ShowTokens","0");
	storeVariable("PATH","/usr:/bin:/usr/bin");
	storeVariable("HomeDir", cwd );
}

bool eventHandler::executeCommand(int commandID, Prompter &p, socketHandler& clientSocket) 
{
	if (commandID == 0)
		return false;

	switch(commandID)
	{
		case 1:	// CONNECT <ipAddress> <port>
			if(tokens.size() == 1)
			{
				clientSocket.clientConnect();
			}
			else if (tokens.size() == 3)
			{
				clientSocket.setIP(tokens[1]);
				clientSocket.setPort(tokens[2]);
				clientSocket.clientConnect();				
			}
			else
				cout << "Proper usage is CONNECT <ipaddr> <port>\n";
			return false;		
		break;
		
		case 2: // UPLOAD
			if(tokens.size() != 2)
				cout << "Proper usage is UPLOAD <filename>";
			else
			{
				clientSocket.sendCommand("upload");
				wait(100);
				clientSocket.upload(tokens[1]);	//upload filename
			}
			return false;
		break;
		
		case 3: // NEWPROMPT
			if (tokens.size() > 1)
				p.setPrompt(tokens[1]);
			else if (tokens.size() > 2)
			       cout << "Too many arguments.\n";
			else	       
				cout << "Provide a prompt value.\n";	
			return false;
		break;
		
		case 4: // CD
			if (tokens.size() == 1)
			{
				string homeDir = getVariable("HomeDir");
				char chBuf[1024];
				for (unsigned int i = 0; i < homeDir.size(); i++)
				{
					chBuf[i] = homeDir[i];
				}	
				chBuf[homeDir.size()] = '\0';
				chdir(chBuf);
				printCWD();
			}
			else if (tokens.size() == 2)
			{
				string strNwd = tokens[1];
				char nwd[1024];
				for (unsigned int i = 0; i < strNwd.size(); i++)
				{
					nwd[i] = strNwd[i];
				}
				nwd[strNwd.size()] = '\0';
				chdir(nwd);
				printCWD();
			}
			else 
				cout << "Too many Arguments.\n";
			return false;
		break;
		
		case 5: // DOWNLOAD
			if(tokens.size() == 1)
				cout << "Proper usage is DOWNLOAD <filename>" << endl;
			else if(tokens.size() == 2)
			{
				clientSocket.sendCommand("download");
				wait(100);
				clientSocket.download(tokens[1]);	//download filename
			}
			else	cout << "Proper usage is DOWNLOAD <filename>" << endl;
			return false;
		break;
		
		case 6: // BYE
			return true;
		break;
		
		case 7: // DELETE
			{
			if (tokens.size() != 2)
				cout << "Proper usage is delete <filename>";
			else
			{ 
				clientSocket.sendCommand("delete");
				wait(100);
				clientSocket.sendCommand(tokens[1]);
			}
			return false;		// end case 7
			}
		break;
		
		case 8: //DIR
			{
			if (tokens.size() != 1)
				cout << "Proper usage is dir";
			else
			{
				clientSocket.sendCommand("dir");	
			}
			wait(100);
			string arg = "rm -r ./tmp.txt";
			system(arg.c_str());
			clientSocket.download("tmp.txt");
			wait(100);			
			cout << endl << "Directories:" << endl;
			arg = "cat tmp.txt";
			system(arg.c_str());
			return false;
			}
		break;

		case 9: // LV
		{
			map<string,string>::iterator it = localStrings.begin();
			cout << endl;
			for (auto iter = it; iter != localStrings.end(); iter++)
			{	
				cout << iter->first << ": " << iter->second << endl;
			}
			return false;
		}
		break;

		case 10: // variable =
		{
			string dummyString = getVariable(tokens[0]);
			if(dummyString != "")
			{ 
				localStrings.erase(tokens[0]);
				storeVariable(tokens[0],tokens[2]);
			}
			else storeVariable(tokens[0],tokens[2]);
			
			return false;
		}
		case 11: //LS
		{
			system("ls");
			return false;
		}	
		break;
		
	}	
}

void eventHandler::storeVariable(string varName, string data)
{
		localStrings.insert(std::pair<string, string>(varName, data));
}

string eventHandler::getVariable(string varName)
{
	//std::map<string,int>::iterator it1;
	//std::map<string,double>::iterator it2;
	std::map<string,string>::iterator it3;

	//it1 = localInts.find(varName);
	//it2 = localDoubles.find(varName);
	it3 = localStrings.find(varName);
	/*
	if (it1 != localInts.end())
		return localInts.at(varName);
	if (it2 != localDoubles.end())
		return localDoubles.at(varName);
	*/
	if (it3 != localStrings.end())
	{
		string retVal = it3->second;
		return retVal;
	}
	else
	{
		cout << "Variable with name \"" << varName << "\" not Declared.\n";
		return "";
	}
}

void eventHandler::setTokens(vector<string> externTokens)
{
	tokens.clear();	// clears all tokens from local copy, preparing for read in.
	
	for (unsigned int i = 0; i < externTokens.size(); i++) 
	{
		tokens.push_back(externTokens[i]);	//populate local tokens from outside tokens
	}
}

void eventHandler::isPrintSet()
{
	string printVar = getVariable("ShowTokens");

	if ( printVar == "1\0\n" )
	{
		for (unsigned int i = 0; i < tokens.size(); i++) 
		{
			cout << "Token = " << tokens[i] << endl;
		}
	}
}

bool eventHandler::quit()
{
	return true;
}

void eventHandler::printCWD()
{
    	char cwd[1024]; 
    	getcwd(cwd, sizeof(cwd)); 
    	printf("\nCurrent Working Dir: %s", cwd);
	cout << endl;	
}

void eventHandler::cString(string str, char retStr[])
{
	str.copy(retStr, str.size()+1);
	retStr[str.size()] = '\0';
}

void eventHandler::wait(long int mill)	//halts program control for a specified millisecond argument
{
	using namespace std::chrono;
	bool go = true;
	auto start = high_resolution_clock::now();
	while (go)
	{
		auto stop = high_resolution_clock::now();
		auto duration = duration_cast<milliseconds>(stop - start);

		if(duration.count() > mill)
		{
			go = false;
		}
	}
}

