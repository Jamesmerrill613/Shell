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

class History
{
private:
	std::vector<std::string> list;
public:
	void add(std::string input)
	{
		list.push_back(input);
	};
	void show()
	{
		for (int i = i; i < list.size(); i++)
			std::cout << i << ") " << list[list.size() - i] << std::endl;
	};
	std::string use(int i)
	{
		if(i >=  list.size()|| i <= 0)
		{
			std::cerr << "No such history exists" << std::endl;
			return NULL;
		}
		else
			return list[list.size() - i];
	};
};

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
	History history;
	bool shouldContinue = true;

	while (shouldContinue)
	{

		//GET INPUT
		std::cout << "[cmd]: ";
		std::string input = GetInput();
		history.add(input);

		//PARSE INPUT
		char* argc = new char[MAX_SIZE];
		char**argv = new char*[MAX_SIZE];
		Parse(input, argc, argv);


		switch (argc)
		case "history": history.show();
		case "^":
		{
			// history.add(stoi
		}
//{
		//default:
  			auto start = std::chrono::steady_clock::now();
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
			else
				perror(0); //fork failed
		//}
	}

	history.show();
	return 0;
}

