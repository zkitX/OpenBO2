#pragma once

void Sys_WorkerCmdInit(void);
bool Sys_IsWorkerCmdReady(void);
void* jqCommitMemory(jqBatch*, void*, unsigned int);
void jqSafeFlush(jqBatchGroup*, unsigned __int64);
void* jqCallbackPre(jqBatch*, char const*);
void jqCallbackPost(jqBatch*, void*, bool);
void Sys_AddWorkerCmdToQueueInternal(jqWorkerCmd*, void*, jqQueue*, WorkerCmdConditional*);
void Sys_AddWorkerCmdInternal(jqWorkerCmd*, void*, WorkerCmdConditional*);
void Sys_AddWorkerCmdGroupedToQueueInternal(jqWorkerCmd*, void*, jqBatchGroup*, jqQueue*, WorkerCmdConditional*);
void Sys_AddWorkerCmdGroupedInternal(jqWorkerCmd*, void*, jqBatchGroup*, WorkerCmdConditional*);
void Sys_WaitWorkerCmdInternal(jqWorkerCmd*);
bool Sys_AssistSingleCallback(void*);
void Sys_AssistSingle(void);
bool Sys_AssistNeeded(void);
int Sys_GetWorkerQueuedCountInternal(jqWorkerCmd*);
int R_FinishedFrontendWorkerCmds(void);
void R_WaitFrontendWorkerCmds(void);
void Sys_AssistAndWaitWorkerCmdInternal(jqBatchGroup*);
void Sys_AssistAndWaitWorkerCmdInternal(jqWorkerCmd*);
void R_WaitDrawWorkerCmdsOfType(GfxWorkerData*, unsigned char, int);