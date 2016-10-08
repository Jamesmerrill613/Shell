#include <stdio.h>
//#include <unistd.h>
#include <chrono>
#include <string>
#include <iostream>
#include <algorithm>
#include <vector>


std::string GetInput()
{
	std::string input;
	getline(std::cin, input);
	return input;
};

void Parse(std::string input)
{
	std::vector<std::string> line;
	int current = 0;
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] == ' ')
			line.push_back();


	}
};


//void RunChild()
//{
//	execvp(....);
//	perror();
//	exit();
//};
//
//void RunParent()
//{
//	waitpid(pid);
//	auto stop = std::chrono::steady_clock::now();
//	ptime += stop - start;
//};


int main()
{

	while (true)
	{
		std::string input = GetInput();
		Parse(input);
	}


	//while (....)
	//{
	//	 
	//	//get input
	//	//parse input
	//	switch (command)
	//	{
	//	default:
	//		auto start = std::chrono::steady_clock::now();
	//		auto pid = fork();
	//		
	//		if (pid == 0)
	//			RunChild();

	//		else if (pid > 0)
	//			RunParent();

	//		else
	//			perror(); //fork failed
	//	}
	//}

	return 0;
};