/* Author: Parker Buckley 
 * Date Modified:
 * Description:
 * References:
 * 
 * */

#include "scanner.h"
#include "parser.h"
#include "eventHandler.h"
#include "prompter.h"

using namespace std;

int main() 
{
	Scanner myScanner;
	Parser myParser;
	eventHandler evHandler;
	Prompter p;
	socketHandler client;
	client.setWho("client");	//used to identify who is downloading in socketHandler functions.		
	bool quit = false;
	int qflag = 0;
	// if qflag recieves a value of -1 from the scanner, Ctrl + d was input.
	// if qflag recieves -2, enter was pressed but no string was appended.
	//
	while (!quit)
	{
		p.print();
		qflag = myScanner.scan();
		
		while (qflag == -2)	//detects an empty line input
		{
			p.print();
			qflag = myScanner.scan();
		}
		if (qflag == -1)	//detects a ctrl+d or bye input
			quit = true;
		else
		{
			myParser.setTokens( myScanner.getTokens() );
			myParser.checkSubs(evHandler);	
			evHandler.setTokens( myParser.getTokens() );
			evHandler.isPrintSet();
			quit = evHandler.executeCommand( myParser.isValidCommand() , p , client );
		}
	}
return 0;
}
