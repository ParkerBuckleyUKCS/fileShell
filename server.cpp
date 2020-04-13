#include <iostream>
#include "socketHandler.h"
#include <unistd.h>
#include <dirent.h>
#include <vector>
#include <chrono>

using namespace std;
//GLOBAL VARIABLES
vector<string> fileNames;
fstream fin, fout;

void getFileNames()
{
	char cwd[256];
	getcwd(cwd, sizeof(cwd));
	std::string CWD = "";

	for (int i = 0; cwd[i] != '\0'; i++)
		CWD += cwd[i];

	DIR *dir;
	struct dirent *ent;
	if((dir = opendir(cwd)) != NULL)	//open at the path of the current working directory
	{
		fileNames.clear();
		//cout << "File list: ";
		while((ent = readdir(dir)) != NULL)
		{
			CWD = "";
			strcpy(cwd,ent->d_name);
			for (int i = 0; cwd[i] != '\0'; i++)
				CWD += cwd[i];

			//cout << CWD << ",";
			fileNames.push_back(CWD);
		}
	closedir(dir);
	}	
	else perror("cannot open .");
}

void wait(long int mill)	//halts program control for a specified millisecond argument
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

bool compareFileName(std::string fname)
{
	for(unsigned int i = 0; i < fileNames.size(); i++)
	{
		if(fname == fileNames[i])
		{
			return true;
		}
	}
	return false;
}

int main()
{
	string command;
	socketHandler S;
	S.serverConnect();
	S.setWho("server");

	cout << "Who is set" << endl;	
	while(true)
	{
		getFileNames();	//index local filenames
		command = "";
		command = S.recieveCommand();
	
		if (command != "")	//'' is returned on an error from recieve command
		{	
		cout << "Command: " << command;

			if(command == "download")	//server needs to upload
			{
				string filename = "";
				while(filename == "")
				{
					cout << "recieving...";
					filename = S.recieveCommand();
					wait(100);
				}
					if(compareFileName(filename))	//filename exists in dir
					{
						S.upload(filename);
					}
					else	//filename does not exist in dir 
					{
						cout << "File does not exist" << endl;
					}
			}
		}
	}
	return 0;
}

