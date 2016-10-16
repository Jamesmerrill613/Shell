#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <chrono>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdio>

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
	while (infile.find(" ") == 0)
		infile = infile.substr(1);
	while (infile.find(" ") < infile.size())
		infile = infile.substr(0, infile.size() - 1);
	while (outfile.find(" ") == 0)
		outfile = outfile.substr(1);
	while (outfile.find(" ") < outfile.size())
		outfile = outfile.substr(0, outfile.size() - 1);
};

void Parse_Pipe(std::string input, std::vector<std::string>& cmds, std::string& infile, std::string& outfile)
{
	cmds.clear();
	while (input.size() != 0)
	{
		int pos = input.find("|");
		if (pos == 0)
			pos = input.find("|");
		else if (pos > 0)
		{
			cmds.push_back(input.substr(0, pos));
			input = input.substr(pos + 1);
		}
		else
		{
			Parse_Files(input, infile, outfile);
			cmds.push_back(input);
			input = "";
		}
	}
};


void Parse_Cmd(std::string input, std::vector<std::string>& argv)
{
	argv.clear();
	while (input.size() != 0)
	{
		int pos = input.find(" ");
		if (pos == 0)
			input = input.substr(1);
		else if (pos > 0)
		{
			argv.push_back(input.substr(0,pos));
			input = input.substr(pos + 1);
		}
		else
		{
			argv.push_back(input);
			input = "";
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

		//CREATE PIPE
		int p[2];
		int test;
		test = pipe(p);
		if (test == -1)
			perror("pipe failed");

		//CREATE FILE MARKERS
		int fin, fout;
		FILE *FIN, *FOUT;
		if (infile != "")
		{
			FIN = std::fopen(infile.c_str(), "r");
			fin = fileno(FIN);
		}
		if (outfile != "")
		{		
			FOUT = std::fopen(outfile.c_str(), "w");
			fout = fileno(FOUT);
		}
		//RUN COMMANDS IN PIPE
		for (int x = 0; x < cmds.size(); x++) 
		{
			std::vector<std::string> args;
			Parse_Cmd(cmds[x], args);

			//CHECK COMMANDS
			bool shouldFork = true;
			if (args[0] == "exit")
				return 0;
			if (args[0] == "ptime")
			{
				std::cout << "Time spent executing child process: " << ptime.count() << " microseconds" << std::endl;
				shouldFork = false;
			}
			if (args[0] == "^")
			{
				history.remove();
				Parse_Cmd(history.use(std::stoi(args[1], nullptr, 10)), args);
			}
			if (args[0] == "history")
			{
				history.show();
				shouldFork = false;
			}

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
					if (x == 0 && infile != "")
					{
						dup2(fin, 0);
						close(STDIN);
					}
					if (x == cmds.size() - 1 && outfile != "")
					{
						dup2(fout, 1);
						close(STDOUT);
					}
					if (x > 0)
					{
						dup2(p[0], 0);
						close(STDIN);
					}
					if (x < cmds.size() - 1)
					{
						dup2(p[1], 1);
						close(STDOUT);
					}
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
		//CLOSE FILE MARKERS
		if (infile != "")
			fclose(FIN);
		if (outfile != "")
			fclose(FOUT);
	}
	return 0;
};
