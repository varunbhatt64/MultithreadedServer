#include "CSocket.h"


CSocket::CSocket()
	:m_iCurrent(0)
{
	m_messages.reserve(COUNT_TEST_MESSAGES);
	for (size_t i = 0; i < COUNT_TEST_MESSAGES; ++i)
	{
		std::string strMessage = "Message " + std::to_string(i);
		m_messages.push_back(strMessage);
	}
}

std::string CSocket::GetNextMessage()
{
	std::string strCurrentValue = m_messages[m_iCurrent++];
	if (m_iCurrent == COUNT_TEST_MESSAGES)
	{
		m_iCurrent = 0;
	}
	return strCurrentValue;
}

