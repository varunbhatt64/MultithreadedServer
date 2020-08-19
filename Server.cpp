
#include <iostream>
#include <string>
#include <sstream>
#include "ThreadPool.h"
#include "CSocket.h"

std::mutex consoleMutex;

bool ProcessMessage(const std::string& message)
{
	try
	{
		std::lock_guard<std::mutex> g(consoleMutex);
		std::cout << message << "\n";
		return true;
	}
	// add catch blocks as per the funtionality. Currently it is just io.
	catch (std::exception ex)
	{
		std::cout << "Failed to process message - " << message << ". " << ex.what() << "\n";
		return false;
	}
}

int main()
{
	// make this configurable - read from ini file
	constexpr int numberOfThreads = 6;	
	constexpr int sleepTime = 5;
	std::string message;
	std::cout << "Server Started.." << "\n";
		
	CSocket socket;
	// Create threadpool with n threads and message process handler - ProcessMessage
	ThreadPool tp(ProcessMessage, numberOfThreads);
	tp.BeginProcessing();
	while (true)
	{		
		// Queue each message read from socket to thread pool worker queue

		message = socket.GetNextMessage();
		if(!message.empty())
			tp.QueueWork(socket.GetNextMessage());
		else
		{
			std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
		}		
	}

	system("PAUSE");
}
