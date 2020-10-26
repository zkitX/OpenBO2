#include "pch.h"

static bdLogSubscriber* g_logSubscriberList;

int bdPrintf(char const* format, ...)
{
	iobuf* v1; // eax
	va_list ap; // [esp+Ch] [ebp+Ch]

	va_start(ap, format);
	v1 = __iob_func();
	return vfprintf((FILE*)v1 + 1, format, ap);
}

void bdLogMessage(const bdLogMessageType type, const char* const baseChannel, const char* const channel, const char* const file, const char* const function, const unsigned int line, const char* const format, ...)
{
	unsigned int v7; // edi
	const char* v8; // edi
	bdLogSubscriber* i; // esi
	char channelNameBuffer[256]; // [esp+10h] [ebp-204h]
	char message[256]; // [esp+110h] [ebp-104h]
	va_list ap; // [esp+238h] [ebp+24h]

	va_start(ap, format);
	if (g_logSubscriberList)
	{
		memset(message, 0, 0x100u);
		v7 = _vscprintf(format, ap);
		vsnprintf_s(message, 0x100u, 0xFFFFFFFF, format, ap);
		if (v7 >= 0x100)
			bdLogMessage(
				bdLogMessageType::BD_LOG_WARNING,
				"warn/",
				"bdPlatformLog",
				__FILE__,
				__FUNCTION__,
				0xDAu,
				"Message truncated.",
				0);
		v8 = baseChannel;
		if (baseChannel && channel)
		{
			if (bdSnprintf(channelNameBuffer, 0x100u, "%s%s", baseChannel, channel) >= 256)
				bdLogMessage(
					bdLogMessageType::BD_LOG_WARNING,
					"warn/",
					"bdPlatformLog",
					__FILE__,
					__FUNCTION__,
					0xE8u,
					"Channel name truncated: %s%s",
					baseChannel,
					channel);
			v8 = channelNameBuffer;
		}
		for (i = g_logSubscriberList; i; i = (bdLogSubscriber*)bdLinkable::getNext(i))
			bdLogSubscriber::logMessage(i, type, v8, file, function, line, message);
	}
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

void bdLogSubscriber::logMessage(bdLogSubscriber* logsub, const bdLogMessageType type, const char* const channelName, const char* const file, const char* const function, const unsigned int line, const char* const msg)
{
	int v8; // [esp+Ch] [ebp-8h]
	const char** v9; // [esp+10h] [ebp-4h]

	v9 = (const char**)logsub->m_channels;
	v8 = 5;
	do
	{
		if (*v9)
		{
			if (bdDelimSubstr(channelName, *v9, "\\/"))
				logsub->publish(type, channelName, file, function, line, msg);
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