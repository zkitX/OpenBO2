#pragma once

enum print_msg_dest_t
{
	CON_DEST_CONSOLE = 0x0,
	CON_DEST_MINICON = 0x1,
	CON_DEST_ERROR = 0x2,
	CON_DEST_GAME_FIRST = 0x3,
	CON_DEST_GAME1 = 0x3,
	CON_DEST_GAME2 = 0x4,
	CON_DEST_GAME3 = 0x5,
	CON_DEST_GAME4 = 0x6,
	CON_DEST_GAME_LAST = 0x6,
	CON_DEST_COUNT = 0x7,
};

struct PrintChannel
{
	char name[32];
	bool allowScript;
};

struct PrintChannelGlob
{
	PrintChannel openChannels[256];
	unsigned int filters[7][8];
};
 
PrintChannelGlob pcGlob;

bool Con_IsChannelVisible(print_msg_dest_t dest, int channel, int errorFlags);

class con_channels
{
};

