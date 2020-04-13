#ifndef	SCANNER_H 
#define SCANNER_H

#include <iostream>
#include <string.h>
#include <string>
#include <vector>
#include "stdio.h"

using namespace std;

class Scanner {

public:
	Scanner();
	int scan();
	std::vector<string> getTokens();
	string getStringToken(string localInputLine);	
private:
	std::vector<string> tokens;
	string inputLine;	
};
#endif	//SCANNER_H
