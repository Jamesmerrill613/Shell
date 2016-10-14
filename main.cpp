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
		for (int i = list.size(); i > 0; i--)
			std::cout << "    " << i << ") " << list[list.size() - i] << std::endl;
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
	void remove()
	{
		list.pop_back();
	}
};

std::string GetInput()
{
 	std::string input;
	getline(std::cin, input);
	return input;
};

void Parse_Files(std::string& input, std::string& infile, std::string& outfile) 
{
	infile = "";
	outfile = "";
		int posIn = input.find("<");
		int posOut = input.find(">");
		if (posIn == -1 && posOut > -1)
		{
			outfile = input.substr(posOut + 1);
			input = input.substr(0, posOut);
		}	
		else if (posIn > -1 && posOut == -1)
		{
			infile = input.substr(posIn + 1);
			input = input.substr(0, posIn);
		}
		else if (posIn > -1 && posOut > -1)
		{
			if (posIn > posOut)
			{
				infile = input.substr(posIn + 1);
				input = input.substr(0, posIn);
				outfile = input.substr(posOut + 1);
				input = input.substr(0, posOut);
			}
			else
			{
				outfile = input.substr(posOut + 1);
				input = input.substr(0, posOut);
				infile = input.substr(posIn + 1);
				input = input.substr( 0, posIn);
			
			}

		}
}

void Parse_Pipe(std::string input, std::vector<std::string>& cmds, std::string& infile, std::string& outfile)
{
	cmds.clear();
	int pos = 1;
	bool shouldContinue = true;
	while (shouldContinue)
	{
		pos = input.find("|");
		if (pos < 0)
		{
			pos = input.size();
			Parse_Files(input, infile, outfile);
			cmds.push_back(input);
			shouldContinue = false;
		}	
		if (pos < input.size() && pos > 0)
		{
			cmds.push_back(input.substr(0, pos));
			input = input.substr(pos + 1);
		}
		else if (pos == 0)
			input = input.substr(pos + 1);
	}
}


void Parse_Cmd(std::string input, std::vector<std::string>& argv)
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
	std::chrono::microseconds ptime = std::chrono::microseconds(0);

	while (true)
	{


		//GET INPUT
		std::cout << "[cmd]: ";
		std::string input = GetInput();
		history.add(input);

		//PARSE PIPE
		std::vector<std::string> cmds;
		std::string infile, outfile;
		Parse_Pipe(input, cmds, infile, outfile);


		for (int x = 0; x < cmds.size(); x++)
		//RUN THE FOLLOWING FORK FOR EACH COMMAND IN cmds 
		{





		//PARSE INPUT
		std::vector<std::string> args;
		Parse_Cmd(cmds[x], args);

		//CHECK COMMANDS
		bool shouldFork = true;
//		if (args[0] == "exit")
//			return 0;
//		if (args[0] == "ptime")
//		{
//			std::cout << "Time spent executing child process: " << ptime.count() << " microseconds" << std::endl;
//			shouldFork = false;
//		}
//		if (args[0] == "^")
//		{
//			history.remove();
//			Parse_Cmd(history.use(std::stoi(args[1], nullptr, 10)), args);
//		}
//		if (args[0] == "history")
//		{
//			history.show();
//			shouldFork = false;
//		}

		//FORK
		if (shouldFork)
		{
			char** argv = new char*[args.size()+1];
			for (int i = 0; i < args.size(); i++)
				argv[i] = const_cast<char*>(args[i].c_str());
			argv[args.size()] = NULL;
  			auto start = std::chrono::steady_clock::now();
			pid_t pid = fork();

			//CHILD
			if (pid == 0)
			{
				execvp(argv[0], argv);
				perror("execvp did not run");
				exit(0);

			}

			//PARENT
			else if (pid > 0)
			{
				waitpid(pid, NULL, 0);
				auto stop = std::chrono::steady_clock::now();
				ptime += std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
			}
			else
				perror(0); //fork failed
		}	
	}
	}
	return 0; 
}


