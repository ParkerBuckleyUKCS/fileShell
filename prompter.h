#ifndef PROMPTER_H
#define PROMPTER_H

#include <iostream>
#include <string>

using namespace std;

class Prompter
{
public:
	Prompter();
	void print();
	void setPrompt(string newPrompt);
private:
	string prompt;
};
#endif
