#include <pch.h>

void bdRunnable::start()
{
	this->m_stop = 0;
}

void bdRunnable::stop()
{
	this->m_stop = 1;
}
