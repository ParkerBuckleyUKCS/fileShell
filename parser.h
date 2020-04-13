#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include "eventHandler.h"

#define NUMCOMMANDS 11
using namespace std;

class Parser
{
	friend class eventHandler;
public:
	Parser();
	vector<string> getTokens();
	bool isValidVariable();	// checks for valid syntax returning true false
	void setTokens(vector<string> externTokens);
	void printTokens();
	int isValidCommand();
	void checkSubs(eventHandler &EH);
private:
	vector<string> tokens;
	string builtIn[NUMCOMMANDS];
};
#endif
