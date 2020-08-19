#include "ThreadPool.h"

ThreadPool::ThreadPool(callback_function callback, int numberOfThreads) : m_done(false)
{
	m_callBack = callback;
	if (numberOfThreads == 0) {
		numberOfThreads = 1;
	}

	for (size_t i = 0; i < numberOfThreads; i++)
	{
		m_threads.push_back(std::thread(&ThreadPool::DoWork, this));
	}
}

ThreadPool::~ThreadPool()
{
	m_done = true;

	m_workQueueConditionVariable.notify_all();
	for (auto& thread : m_threads)
	{
		if (thread.joinable())
			thread.join();
	}
}

void ThreadPool::QueueWork(const std::string& buffer)
{
	std::lock_guard<std::mutex> g(m_workQueueMutex);
	m_workQueue.push(buffer);
	m_workQueueConditionVariable.notify_one();
}

void ThreadPool::DoWork()
{
	// make this sleep time configurable
	constexpr int sleepTime = 5;

	while (!m_done)
	{
		if (m_process)
		{
			std::string message;
			{
				std::unique_lock<std::mutex> g(m_workQueueMutex);
				m_workQueueConditionVariable.wait(g, [&] {
					return (!m_workQueue.empty() || m_done);
					});

				message = m_workQueue.front();
				m_workQueue.pop();
			}

			Process(message);
		}
		else
		{			
			std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
		}
	}
}

void ThreadPool::Process(const std::string& message)
{	
	if (m_callBack)
	{
		if (!m_callBack(message))
			QueueWork(message); // if message was not processed, requeue it so some other thread will pick it
	}
}

void ThreadPool::BeginProcessing()
{
	m_process = true;
}
void ThreadPool::EndProcessing()
{
	m_process = false;
}
int ThreadPool::GetQueueSize()
{
	return m_workQueue.size();
}
bool ThreadPool::IsProcessing()
{
	return m_process;
}

bool ThreadPool::TaskPending()
{
	return m_workQueue.size() != 0;
}