Author: Parker Buckley
Project: novshell

Files: 
	main.cpp
	eventHandler.cpp
	eventHandler.h
	parser.cpp
	parser.h
	scanner.cpp
	scanner.h
	prompter.cpp
	prompter.h
	Makefile

Description:

Algorithms Used:
		SCANNING: I designed my own algorithm which does the following
			The entire input line is read in. It is tokenized using nested while loops
			by first checking for a space, and then checking for double quotes so it can
			ignore spaces until another quote. These tokens are pushed into a storage vector
			and the remaining input line is returned for further processing.
		PARSING: This is also an algorithm of my own design.
			The parser first checks if the first token is a builtin command. If it is,
			an integer code from 0-10 is returned to the eventhandler class that processes
			the input accordingly. If it is not builtin or program control command, the parser
			assumed variables assignment and checks proper syntax for that.
