#pragma once
class bdSequenceNumber
{
private:
	int m_seqNum;
public:
	bdSequenceNumber(const bdSequenceNumber* last, const unsigned int seqNumber, const unsigned int bits);

	bool operator!=(const bdSequenceNumber* other) {
		return this->m_seqNum != other->m_seqNum;
	}

	bool operator!=(const bdInetAddr* other) {

	}
};

