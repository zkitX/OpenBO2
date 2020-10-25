#pragma once

enum class bdLogMessageType
{
	BD_LOG_INFO = 0x0,
	BD_LOG_WARNING = 0x1,
	BD_LOG_ERROR = 0x2
};

int bdPrintf(char const*, ...);

class bdLogSubscriber : bdLinkable {
private:
	char* m_channels[5];
public:
	bdLogSubscriber();
	~bdLogSubscriber();
	char addChannel(char* channel);
	void logMessage(const bdLogMessageType type, const char* const channelName, const char* const file, const char* const function, const unsigned int line, const char* const msg);
	void publish(bdLogMessageType type, char const* const __formal, char const* const file, char const* const a5, unsigned int line, char const* const msg);
};