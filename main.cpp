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
		for (int i = 1; i < list.size() + 1; i++)
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
	int size()
	{
	return list.size();
	};
};

std::string GetInput()
{
 	std::string input;
	getline(std::cin, input);
	return input;
};

void Parse(std::string input, std::vector<std::string>& argv)
{
	argv.clear();
	int pos = -1;
	bool shouldContinue = true;
	while (shouldContinue)
	{
		pos = input.find(" ");
		if (pos < 0)
			pos = input.size();

		if (pos < input.size() && pos > 0)
		{
		argv.push_back(input.substr(0,pos));
		input = input.substr(pos + 1);
		}
		else if (pos == 0)
			input= input.substr(pos + 1);
		else
		{
		argv.push_back(input);
		shouldContinue = false;
		}
	} 
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
		std::vector<std::string> args;
		Parse(input, args);

		//CHECK COMMANDS
		bool shouldFork = true;
		if (args[0] == "^")
			Parse(history.use(std::stoi(args[1], nullptr, 10)), args);
		if (args[0] == "history")
		{
			history.show();
			shouldFork = false;
		}

		if (shouldFork)
		{
			char** argv = new char*[args.size()+1];
			for (int i = 0; i < args.size(); i++)
				argv[i] = const_cast<char*>(args[i].c_str());
			argv[args.size()] = NULL;
  			auto start = std::chrono::steady_clock::now();
			pid_t pid = fork();

			if (pid == 0)
			{
				execvp(argv[0], argv);
				perror("execvp did not run");
				exit(0);

			}
			else if (pid > 0)
			{
				waitpid(pid, NULL, 0);
				auto stop = std::chrono::steady_clock::now();
			//	auto ptime += stop - start;
			}
			else
				perror(0); //fork failed
		}	
	}
	return 0; 
}


