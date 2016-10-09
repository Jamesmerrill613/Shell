#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <chrono>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

const int MAX_SIZE = 128;

std::string GetInput()
{
 	std::string input;
	getline(std::cin, input);
	return input;
};

void Parse(std::string input, char* &argc, char**&argv)
{
	argv = new char*[MAX_SIZE];
	int last = 0;
	int next = 0;
	int argCount = 0;
	while (next < input.length())
	{
		next = input.find(' ', last);
		if (next == -1)
			next = input.length();
		if (last < next)
		{
			char* word = new char[MAX_SIZE];
			int j = 0;
			for (int i = 0; i < next; i++)
			{
				word[j] = input[i];
				j++;
			}
			if (argCount == 0)
			{
				argc = word;
				argc[j] = NULL;
			}
			argv[argCount] = word;
			argCount++;
			last = next + 1;
		}
	}
	argv[argCount] = NULL;
	//return argv;
};



int main()
{
	//Get INPUT
	std::cout << "[cmd]: ";
	std::string input = GetInput();
	//Parse INPUT
	char* argc = new char[MAX_SIZE];
	char**argv = new char*[MAX_SIZE];
	Parse(input, argc, argv);

//	while (true)
//	{
//		std::string input = GetInput();
//		Parse(input);
//	}


/*	while (....)
	{

		//get input
		//parse input
		switch (command)
		{
*///		default:
//      execvp(....);
//  			auto start = std::chrono::steady_clock::now();
			pid_t pid = fork();

			if (pid == 0)
			{
				execvp(argc, argv);
				perror("execvp did not run");
				std::cout << "this is the child process" << std::endl;
				exit(0);

			}
			else if (pid > 0)
			{
				waitpid(pid, NULL, 0);
				std::cout << "this is the parent process" << std::endl;
//				auto stop = std::chrono::steady_clock::now();
//				ptime += stop - start;
			}
//			else
			//	perror(0); //fork failed
//		}
//	}

	std::cout << "main is running" << std::endl;
	return 0;
};

