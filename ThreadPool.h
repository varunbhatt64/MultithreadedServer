#pragma once
#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>

typedef bool(*callback_function)(const std::string& str);

class ThreadPool
{
public:
	ThreadPool(callback_function callback, int numberOfThreads = std::thread::hardware_concurrency());
	~ThreadPool();
	void QueueWork(const std::string& buffer);
	void BeginProcessing();
	void EndProcessing();
	int GetQueueSize();
	bool IsProcessing();
	bool TaskPending();

private:
	callback_function m_callBack;
	std::condition_variable_any m_workQueueConditionVariable;
	std::vector<std::thread> m_threads;
	std::mutex m_workQueueMutex;
	std::queue<std::string> m_workQueue;
	bool m_process;
	bool m_done;
	void DoWork();
	void Process(const std::string& buffer);
};