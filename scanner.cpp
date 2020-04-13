#include "scanner.h"

Scanner::Scanner() 
{
	inputLine = "";
}

int Scanner::scan() 
{
	tokens.clear();	//clear all tokens already in the tokens local
	string tempString;
	getline(cin, inputLine);
		
	if(cin.eof())
		return -1;	//if the person types Ctrl+D 
	if (inputLine == "\0\n")
		return -2;	
	while (inputLine.size() > 0) 
	{
		inputLine = getStringToken(inputLine);

		if (tokens.size() > 0)
		{
			if (tokens[0] == "!\0")
				return 1;
		}
	}

	
	return 1;	
}

string Scanner::getStringToken(string localInputLine) 
{
	vector<char> splitChars;
	string returnInputLine = ""; // will be concatenated to equal the remaining input line.
	string tokenPushString = "";
	unsigned int i = 0;

	while (localInputLine[i] != ' ' && i != localInputLine.size() + 1)
	{
		if (localInputLine[i] == '"')
		{
			i++;
			while (localInputLine[i] != '"' && i != localInputLine.size() + 1)
			{
				splitChars.push_back(localInputLine[i]);
				i++;
			}
			for (unsigned int i = 0; i < splitChars.size(); i++)
			{
				tokenPushString += splitChars[i];
			}
			
			tokens.push_back(tokenPushString);	//pushing the token

			for (unsigned int j = i + 1; j < localInputLine.size(); j++)
			{
				returnInputLine += localInputLine[j];
			}
			return returnInputLine;	//RETUNS the remaining untokenized inputLine
		}
		else
		{
			splitChars.push_back(localInputLine[i]);
			i++;
		}
	}
	for (unsigned int i = 0; i < splitChars.size(); i++) 
	{
		tokenPushString += splitChars[i];
	}
	
	tokens.push_back(tokenPushString);

	for (unsigned int j = i + 1; j < localInputLine.size(); j++)
	{
		returnInputLine += localInputLine[j];	
	}
	return returnInputLine;
}

std::vector<string> Scanner::getTokens() { return tokens; }
