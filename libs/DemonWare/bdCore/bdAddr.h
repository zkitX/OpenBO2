#pragma once
class bdAddr
{
private:
	bdInetAddr m_address;
	short m_port;
	unsigned int serializedSize;
public:

	bdAddr(char const*);
	bool deserialize(void const*, unsigned int, unsigned int, unsigned int&);
	bool serialize(void*, unsigned int, unsigned int, unsigned int&)const;
	unsigned int toString(char* const, unsigned int)const;
	unsigned short getPort(void)const;
	bool operator==(class bdAddr const* other)const {
		return this->m_port == other->m_port && bdSequenceNumber::operator==(&this->m_address, &other->m_address);
	}
	void set(char const*);
	bdAddr(class bdInetAddr const&, unsigned short);
	bdAddr(class bdAddr const&);
	bdAddr();
};

int bdSnprintf(char*, unsigned int, char const*, ...);
