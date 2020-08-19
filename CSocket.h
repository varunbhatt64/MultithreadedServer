#pragma once
#include <string>
#include <vector>

class CSocket
{
public:
	CSocket();

	std::string GetNextMessage();

private:
	std::vector<std::string> m_messages;
	size_t m_iCurrent;
	static const size_t COUNT_TEST_MESSAGES = 1000;
};

