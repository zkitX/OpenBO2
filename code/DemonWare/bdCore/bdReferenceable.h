#pragma once
class bdReferenceable
{
private:
	volatile int m_refCount;
public:
	~bdReferenceable();
};