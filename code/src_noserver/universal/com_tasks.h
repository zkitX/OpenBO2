#pragma once

#include <win32/win_gamepad.h>

enum TaskState
{
	TASK_STATE_INVALID = 0x0,
	TASK_STATE_INIT = 0x1,
	TASK_STATE_INPROGRESS = 0x2,
	TASK_STATE_INCALLBACK = 0x3,
	TASK_STATE_CHILDCOMPLETE = 0x4,
	TASK_STATE_CHILDFAILED = 0x5,
	TASK_STATE_COMPLETED = 0x6,
	TASK_STATE_FAILED = 0x7,
	TASK_STATE_CANCELLED = 0x8,
};

enum TaskGroup
{
	TASK_GROUP_UNKNOWN = 0x0,
	TASK_GROUP_XSESSION = 0x1,
};

struct bdReference<bdRemoteTask>
{
	bdRemoteTask* m_ptr;
};

struct TaskRecord
{
	TaskRecord* next;
	const TaskDefinition* definition;
	TaskState state;
	ControllerIndex_t controllerIndex;
	unsigned int lastPoll;
	unsigned int lastPollMS;
	unsigned int id;
	TaskGroup group;
	int startMS;
	int timeOut;
	int timeIn;
	bdReference<bdRemoteTask> remoteTask;
	TaskRecord* nestedTask;
	void* payload;
	bool isChildTask;
	bool cancelImmediately;
	bool skipAllCallbacksAfterComplete;
};

unsigned int g_taskProcessNesting;

int ChunkAllocate(int, int);
void ChunkFree(int, int);
void* Task_Allocate(int);
void Task_Deallocate(void*, int);
int TaskManager2_GetTaskRuleErrors(void);
bool TaskManager2_IsTimedIn(struct TaskRecord*);
void TaskManager2_ProcessXOverlappedTask(struct TaskRecord*);
void TaskManager2_RevertTask(struct TaskRecord*);
void TaskManager2_ClearTask(struct TaskRecord*);
void TaskManager2_ClearTasks(struct TaskDefinition const*);
void TaskManager2_SkipCallbacksForTaskAfterComplete(struct TaskRecord*);
void TaskManager2_SkipCallbacksForTasksAfterComplete(struct TaskDefinition const*);
bool TaskManger2_TaskGetInProgressForControllerByName(char const*, enum ControllerIndex_t);
bool TaskManager2_TaskIsPending(struct TaskRecord const*);
void TaskManager2_DumpTasks(void);
void TaskManager2_RunTaskRules(struct TaskRecord*);
void TaskManager2_RecordTaskInfo(struct TaskRecord*);
void TaskManager2_Init(void);
void TaskManager2_ReportTaskComplete(struct TaskRecord*, bool, char const*, unsigned int);
void TaskManager2_FreeAllPendingTasksInternal(enum ControllerIndex_t);
void TaskManager2_HandleTimedOutTask(struct TaskRecord*);
void TaskManager2_ProcessLocalTask(struct TaskRecord*);
void TaskManager2_CancelEndlessTasks(enum ControllerIndex_t);
void TaskManager2_ProcessDemonwareTask(struct TaskRecord*);
void TaskManager2_ProcessNestedTask(struct TaskRecord*);
struct TaskRecord* TaskManager2_CreateTask(struct TaskDefinition const*, enum ControllerIndex_t, struct TaskRecord*, int);
void TaskManager2_EnumTasksInProgress(void (*)(struct TaskRecord*, void*), void*);
struct TaskRecord* TaskManager2_TaskGetInProgressForController(struct TaskDefinition const*, enum ControllerIndex_t);
struct TaskRecord* TaskManager2_TaskGetInProgress(struct TaskDefinition const*);
struct TaskRecord* TaskManager2_TaskGetInProgressCallBackSkipCallbacks(struct TaskDefinition const*);
bool TaskManager2_TaskIsInProgressForController(struct TaskDefinition const*, enum ControllerIndex_t);
bool TaskManager2_TaskIsInProgress(struct TaskDefinition const*);
int TaskManager2_CountTasksInProgress(struct TaskDefinition const*);
int TaskManager2_CountTasksInProgressForControllerByType(int, unsigned __int64);
void TaskManager2_FreeDeadTasks(enum ControllerIndex_t);
void TaskManager2_ProcessTasks(enum ControllerIndex_t);
void TaskManager2_StartTask(struct TaskRecord*);
struct TaskRecord* TaskManager2_SetupNestedTask(struct TaskDefinition const*, enum ControllerIndex_t, struct TaskRecord*, void*);
struct TaskRecord* TaskManager2_SetupRemoteTask(struct TaskDefinition const*, enum ControllerIndex_t, class bdRemoteTask*, void*);
void TaskManager2_ComErrorCleanup(void);