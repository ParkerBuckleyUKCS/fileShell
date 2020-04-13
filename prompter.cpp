#include "prompter.h"

Prompter::Prompter()
{
	prompt = "fileShell > ";
}


void Prompter::print()
{
	cout << prompt;
}

void Prompter::setPrompt(string newPrompt)
{
	prompt = newPrompt;
}
