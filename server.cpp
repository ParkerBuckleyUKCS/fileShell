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
			if (CWD != ".." && CWD != ".")
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
	S.setWho("server");	//allows for proper socketHandler member functions to be called

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
					cout << "...recieving...";
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
			else if(command == "upload")	//server need to download
			{
				string filename = "";
				while(filename == "")
				{
					cout << "...recieving...";
					filename = S.recieveCommand();
					wait(100);
				}
				if(compareFileName(filename))
				{
					string arg = "rm -r " + filename;
					system(arg.c_str());//file already exists
					S.download(filename);
					//OVERWRITE!
				}
				else
				{
					S.download(filename);
				}
			}
			else if(command == "dir")
			{		
				string arg = "rm -r tmp.txt";		
				cout << "...removing tmp files...";
				system(arg.c_str());
				arg = "ls > tmp.txt";
				cout << "...writing dirnames...";
				system(arg.c_str());
				S.recieveCommand();
				cout << "uploading...";
				S.upload("tmp.txt");
				cout << "done." << endl;

			}
			else if(command == "delete")
			{
				string filename = "";
				while(filename == "")
				{
					cout << "...recieving...";
					filename = S.recieveCommand();
					wait(100);
				}
					string arg = "rm -r " + filename;
					system(arg.c_str());
					cout << filename << " deleted" << endl;
			}
		}
	}
	return 0;
}

