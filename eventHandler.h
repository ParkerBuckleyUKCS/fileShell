#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <vector>
#include <string>
#include <iostream>
#include <map>
#include "prompter.h"
#include <unistd.h>
#include <cstring>
#include <sys/wait.h>
#include <sys/types.h>
#include <cstring>
#include <fstream>
#include <fcntl.h>
#include "socketHandler.h"
#include <chrono>

using namespace std;

class eventHandler {

friend class Prompter;
public:
	eventHandler();
	bool executeCommand(int commandID, Prompter &p, socketHandler &clientSocket);
	void storeVariable(string varName, int data);
	void storeVariable(string varName, double data);
	void storeVariable(string varName, string data);
	string getVariable(string varName);
	void setTokens(vector<string>);
	void isPrintSet();
	bool quit();
	void printCWD();
	void cString(string str, char retStr[]);
	void wait(long int mill);
private:
	map<string,int> localInts;
	map<string,double> localDoubles;
	map<string,string> localStrings;
	vector<string> tokens;
	vector<string> procs;
};
#endif
