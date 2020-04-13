#include "parser.h"

using namespace std;

Parser::Parser() 
{
	tokens.clear();

	string arrTemp[NUMCOMMANDS] = { "CONNECT","UPLOAD","NEWPROMPT","CD","DOWNLOAD","BYE","DELETE","DIR","LV" };
	
	for (int i = 0; i < NUMCOMMANDS; i++) 
	{
		builtIn[i] = arrTemp[i];
	}
}

std::vector<string> Parser::getTokens() { return tokens; }

bool Parser::isValidVariable() 
{
	// there are two cases, 
	// (1) first command is a valid variable name followed by an equals sign.
	// (2) first command is a builtIn command followed by valid sytntac	
	//first, check first argument and decide if it is a variable name or builtIni
	if (!tokens.empty())
	{
		regex isVar ("^[a-zA-Z_$][a-zA-Z0-9_$]*.{1}");
		bool isVarName = regex_match(tokens[0], isVar);

		for (unsigned int i= 0; i < tokens[0].size(); i++) 
		{
			if ((tokens[0])[i] == ' ') { isVarName = false; }
		}
		//cout << "\n Is Valid Variable: " << isVarName << endl;	DEBUG
		return isVarName;
	}
	else
		return false;
}

int Parser::isValidCommand() 
{
	int i;
	for (i = 0; i < NUMCOMMANDS; i++)
	{	//comparinf the substring not including the terminating char to the token
		bool isMatch = true;
		for (unsigned int j = 0; j < builtIn[i].size(); j++)
		{
			if (j < tokens[0].size())
			{
				if ((tokens[0])[j] != (builtIn[i])[j])	//this is just checking if the first token is equal to a builtin command
					isMatch = false;
			}
			else isMatch = false;
		}

		if (isMatch)
			return (i+1);
	} 
	if (isValidVariable())	//this branch checks for variable = proper syntax
	{
		if (tokens.size() == 2)
		{
			if (tokens[1] == "=\0\n" )
				cout << "No value to assign to " << tokens[0] << endl;
		}
		else if (tokens.size() == 3)
		{
			if (tokens[1] == "=\0" )
				return 10;	// this is the INDEX of the variable assignment
		}
		else if (tokens.size() > 3)
		{
			if (tokens[1] == "=\0")
				cout << "Too many arguments for variable assignment.\n";
			return 0;
		}
		else
		{
			cout << "Invalid Command\n";
			return -1;
		}
	}
	else
	{	
		cout << "Invalid command.\n";
		return 0;				//returns -1 on invalid syntax
	}
}

void Parser::setTokens(vector<string> externTokens)
{
	tokens.clear();	// clears all tokens from local copy, preparing for read in.
	
	for (unsigned int i = 0; i < externTokens.size(); i++) 
	{
		tokens.push_back(externTokens[i]);	//populate local tokens from outside tokens
	}
}

void Parser::printTokens()
{
	for (unsigned int i = 0; i < tokens.size(); i++) 
	{
		cout << "Token = " << tokens[i] << endl;
	}
}

void Parser::checkSubs(eventHandler &EH)
{
	for (unsigned int j = 0; j < tokens.size(); j++)
	{

		char localToken[tokens[j].size()];
		for (unsigned int i = 0; i < tokens[j].size()-1; i++)
		{
			localToken[i] = (tokens[j])[i+1];
		}	
		if ((tokens[j])[0] == '$')
		{
			tokens[j] = EH.getVariable(localToken);
		}
	}
}

