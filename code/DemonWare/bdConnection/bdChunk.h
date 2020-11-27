#pragma once
enum bdChunkTypes
{
	BD_CT_INVALID = 0x0,
	BD_CT_PADDING = 0x1,
	BD_CT_DATA = 0x2,
	BD_CT_INIT = 0x3,
	BD_CT_INIT_ACK = 0x4,
	BD_CT_SACK = 0x5,
	BD_CT_HEARTBEAT = 0x6,
	BD_CT_HEARTBEAT_ACK = 0x7,
	BD_CT_ABORT = 0x8,
	BD_CT_SHUTDOWN = 0x9,
	BD_CT_SHUTDOWN_ACK = 0xA,
	BD_CT_SHUTDOWN_COMPLETE = 0xB,
	BD_CT_ERROR = 0xC,
	BD_CT_COOKIE_ECHO = 0xD,
	BD_CT_COOKIE_ACK = 0xE,
};

class bdChunk
{
	struct bdChunkData
	{
		char m_type;
		char m_flags;
		unsigned __int16 m_length;
	};

	bdChunkTypes m_type;
	bdChunk::bdChunkData m_headerData;

	bdChunk(bdChunkTypes type);
	~bdChunk();
	unsigned int getSerializedSize();
	bool isControl();
	bdChunkTypes getType(const char* const data, const unsigned int size);
	unsigned int serialize(char* data, const unsigned int size);
	char deserialize(const char* const data, const unsigned int size, unsigned int* offset);
};

