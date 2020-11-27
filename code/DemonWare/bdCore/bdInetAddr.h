#pragma once
class bdInetAddr
{
private:
	bdInAddr m_addr;
public:
	bdInetAddr(unsigned int addr);
	bdInetAddr(const unsigned int address);
	bdInetAddr(const char* address);
	bdInetAddr* Any(bdInetAddr* result);
	bool deserialize(const void* data, const unsigned int size, const unsigned int offset, unsigned int* newOffset);
	bool serialize(void* data, const unsigned int size, const unsigned int offset, unsigned int* newOffset);
	bdInAddr* getInAddr(bdInAddr* result);
	unsigned int toString(char* const str, const unsigned int size);
	bool isValid();
	void set(bdSecurityKeyMap* this, bdSecurityKeyMapListener* listener);
};

