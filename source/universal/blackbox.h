#pragma once
#include "../defs.h"

#include "error.h"
#include "../win32/win_gamepad.h"

class blackbox
{
public:
	// _________________________________________________________________________________
	// Structure definitions
	// _________________________________________________________________________________
	struct BBMsgHeader
	{
		char ident[8];
		int magic;
		unsigned int createTime;
		unsigned int sendTime;
		unsigned int flags;
	};
	struct bb_msg_t
	{
		char* data;
		int maxsize;
		int cursize;
		bool overflow;
		char* pppHashTable;
		char pppBits;
		unsigned int pppHash;
		int pppBitOffset;
		int pppBitCount;
	};
	// _________________________________________________________________________________
	// Public variable declarations
	// _________________________________________________________________________________
	bool s_blackboxInitialized;

	// _________________________________________________________________________________
	// Function declarations
	// _________________________________________________________________________________
	static void BB_Alert(const char* filename, int line, const char* type, const char* msg);
	char BB_CheckThrottle(const char* name);
	unsigned int BB_HashString(const char* str, int len);
	void BB_Init();
	void BB_InitDefinitions();
	void BB_MsgInit(bb_msg_t* msg, void* data, int maxsize);
	void BB_Notify(bool daily, const char* recipient, const char* msg, bool immediate);
	char BB_ParseParameter(const char* string, int len);
	void BB_Print(win_gamepad::ControllerIndex_t controllerIndex, const char* name, const char* fmt, ...);
	int BB_RegisterHighWaterMark(const char* name);
	void BB_ResetHighWaterMark(const char* name);
	void BB_ResetHighWaterMarks();
	void BB_Send(win_gamepad::ControllerIndex_t controllerIndex, bool force, bool bandwidthLimited);
	void BB_Send_f();
	void BB_SetHighWaterMark(int idx, unsigned int value);
	void BB_SetThrottle(const char* name, int throttle);
	void BB_SetupMsg();
	void BB_Start();
	void BB_Start_f();
	void BB_Throttle_f();
	void BB_WriteFormatParameters(bb_msg_t* msg, int parameterCount, const char* parameters, char* args);
	void BB_WriteInstanceData(win_gamepad::ControllerIndex_t controllerIndex);
	void BB_WriteInt8(bb_msg_t* msg, int c);
	void BB_WriteString(bb_msg_t* msg, const char* str, int len);
	void BB_WriteVarUInt32(bb_msg_t* msg, unsigned int c);
	void BB_WriteVarUInt64(unsigned __int64 c);
private:
	// _________________________________________________________________________________
	// Private variable declarations
	// _________________________________________________________________________________
	bool s_blackboxIsInteresting;
	char byte_C68B68; // UNKNOWN

};

