#pragma once

#include "../win32/win_gamepad.h"

enum demoState
{
	DEMO_STATE_IDLE = 0x0,
	DEMO_STATE_RECORDING = 0x1,
	DEMO_STATE_PLAYING = 0x2,
	DEMO_STATE_COUNT = 0x3,
};

void Demo_AbortFileshare(ControllerIndex_t controllerIndex);
void Demo_AddDollyCamMarker_f();
void Demo_Back_f();
void Demo_CaptureSegmentThumbnail_f();
void Demo_ClearRenderFlag_f();
void Demo_DeleteClip_f();
void Demo_DeleteSegment_f();
void Demo_DownloadAndPlay_f();
void Demo_End();
void Demo_Forward_f();
void Demo_Frame(int msec, int scaledMsec);
void Demo_Init();
void Demo_JumpToStart_f();
void Demo_Keyboard_f();
void Demo_MatchEnded();
void Demo_MergeSegments_f();
void Demo_MoveSegment_f();
void Demo_Play_f();
void Demo_PreviewClip_f();
void Demo_PreviewSegment_f();
void Demo_Printf(int channel, const char* fmt, ...);
void Demo_RebuildHighlightReelTimeline_f();
void Demo_RegenerateHighlightReel_f();
void Demo_RegisterCommands();
void Demo_RemoveDollyCamMarker_f();
void Demo_RepositionDollyCamMarker_f();
void Demo_SaveAndUploadClip_f();
void Demo_SaveScreenshot_f();
void Demo_SaveSegment_f();
void Demo_Screenshot_f();
void Demo_SetDemoState(demoState state);
void Demo_StartClipRecord_f();
void Demo_Stop_f();
void Demo_SwitchControls_f();
void Demo_SwitchDollyCamMarker_f();
void Demo_SwitchPlayer_f();
void Demo_SwitchTransition_f();
void Demo_UpdateDollyCamMarkerParameters_f();