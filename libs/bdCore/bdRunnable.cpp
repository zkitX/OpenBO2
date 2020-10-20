#include "bdRunnable.h"

bdRunnable::bdRunnable()
{
	m_stop = 0;
}

bdRunnable::~bdRunnable()
{
}

void bdRunnable::start()
{
	this->m_stop = 0;
}

void bdRunnable::stop()
{
	this->m_stop = 1;
}
