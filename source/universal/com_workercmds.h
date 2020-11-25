#pragma once

class jqBatchGroup 
{
	union {
		struct {
			int QueuedBatchCount;
			int ExecutingBatchCount;
		};
		unsigned __int64 BatchCount;
	};
};