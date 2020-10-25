#include "pch.h"

bdLogSubscriber* g_logSubscriberList;

int bdPrintf(char const* format, ...)
{
	iobuf* v1; // eax
	va_list ap; // [esp+Ch] [ebp+Ch]

	va_start(ap, format);
	v1 = __iob_func();
	return vfprintf((FILE*)v1 + 1, format, ap);
}

bdLogSubscriber::bdLogSubscriber()
{
}

bdLogSubscriber::~bdLogSubscriber()
{
}

char bdLogSubscriber::addChannel(char* channel)
{
	char v3; // bl
	unsigned int v4; // ecx
	char** channels; // eax

	v3 = 0;
	v4 = 0;
	channels = this->m_channels;
	while (v4 < 5)
	{
		if (!*channels)
		{
			v3 = 1;
			*channels = channel;
		}
		++v4;
		++channels;
		if (v3)
		{
			if (!bdLinkable::find(g_logSubscriberList))
				bdLinkable::link((bdLinkable **)&g_logSubscriberList);
			return v3;
		}
	}
	return v3;
}

void bdLogSubscriber::logMessage(const bdLogMessageType type, const char* const channelName, const char* const file, const char* const function, const unsigned int line, const char* const msg)
{
	int v8; // [esp+Ch] [ebp-8h]
	const char** v9; // [esp+10h] [ebp-4h]

	v9 = (const char**)this->m_channels;
	v8 = 5;
	do
	{
		if (*v9)
		{
			if (bdDelimSubstr(channelName, *v9, "\\/"))
				this->publish(type, channelName, file, function, line, msg);
		}
		++v9;
		--v8;
	} while (v8);
}

void bdLogSubscriber::publish(bdLogMessageType type, char const* const __formal, char const* const file, char const* const a5, unsigned int line, char const* const msg)
{
	const char* startMsg;
	const char* fileMsg;

	startMsg = " ";
	switch (type) {
	case bdLogMessageType::BD_LOG_WARNING:
		startMsg = " WARNING: ";
		break;
	case bdLogMessageType::BD_LOG_ERROR:
		startMsg = " ERROR: ";
		break;
	default:
		startMsg = " INFO: ";
		break;
	}
	fileMsg = strrchr(file, '\\');
	if (fileMsg)
		fileMsg = fileMsg + 1;
	else
		fileMsg = file;
	bdPrintf("%s(%u):%s%s\n", fileMsg, line, startMsg, msg);
}