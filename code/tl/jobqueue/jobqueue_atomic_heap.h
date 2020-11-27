#pragma once
#pragma warning(push)
#pragma warning(disable:28112)
#pragma warning(disable:26451)

#include "jobqueue.h"
#include "../base/include/tl_system.h"
#include "../defs.h"

unsigned __int64 tlAtomicAnd(volatile unsigned __int64* var, unsigned __int64 value) {
	volatile signed __int64* m_var;
	signed __int64 v;

	m_var = (volatile signed __int64*)var;
	for (v = *var; _InterlockedCompareExchange64(m_var, value & v, v) != v; v = *var)
	{
		Sleep(0);
		m_var = (volatile signed __int64*)var;
	}
	return value & v;
}

unsigned __int64 tlAtomicOr(volatile unsigned __int64* var, unsigned __int64 value)
{
	volatile signed __int64* m_var;
	signed __int64 v;

	m_var = (volatile signed __int64*)var;
	for (v = *var; _InterlockedCompareExchange64(m_var, value | v, v) != v; v = *var)
	{
		Sleep(0);
		m_var = (volatile signed __int64*)var;
	}
	return value | v;
}

class jqAtomicHeap {

public:

	struct LevelInfo
	{
		unsigned int BlockSize;
		int NBlocks;
		int NCells;
		unsigned __int64* CellAvailable;
		unsigned __int64* CellAllocated;
	};

	jqAtomicHeap* ThisPtr;
	tlAtomicMutex Mutex;
	char* HeapBase;
	unsigned int HeapSize;
	unsigned int BlockSize;
	volatile unsigned int TotalUsed;
	volatile unsigned int TotalBlocks;
	int NLevels;
	jqAtomicHeap::LevelInfo Levels[11];
	char* LevelData;

	bool AllocBlock(LevelInfo** FitLevel, int* FitSlot) {
		jqAtomicHeap::LevelInfo** v3; // edi
		int v4; // ebx
		unsigned __int64* v5; // edx
		int v6; // eax
		int v7; // edi
		int v8; // esi
		unsigned int v9; // ebx
		unsigned int v10; // edx
		signed __int64 v11; // rax
		int v12; // ebx
		int v13; // ecx
		unsigned int v15; // [esp+Ch] [ebp-18h]
		jqAtomicHeap::LevelInfo* v16; // [esp+14h] [ebp-10h]
		jqAtomicHeap* i; // [esp+18h] [ebp-Ch]
		int v18; // [esp+1Ch] [ebp-8h]
		int v19; // [esp+20h] [ebp-4h]

		v3 = FitLevel;
		for (i = this; *v3 < &this->Levels[this->NLevels]; ++ * v3)
		{
			v4 = (*v3)->NCells;
			v5 = (*v3)->CellAvailable;
			v6 = 0;
			v16 = *v3;
			if (v4 > 0)
			{
				while (1)
				{
					v7 = v5[v6];
					v8 = HIDWORD(v5[v6]);
					v15 = v5[v6];
					if (v5[v6])
						break;
					if (++v6 >= v4) {
						v3 = FitLevel;
					}
				}
				v18 = v6;
				v9 = 0x80000000;
				v10 = 0;
				v19 = 0;
				if (v8 >= 0)
				{
					do
					{
						++v19;
						v10 = __PAIR64__(v9, v10) >> 1;
						v9 >>= 1;
					} while (!(v8 & v9 | v7 & v10));
				}
				*FitSlot = 63 - v19 + (v6 << 6);
				v11 = 1i64 << ((63 - v19) & 0x3F);
				v12 = v7 & ~(_DWORD)v11;
				LODWORD(v11) = v7;
				v13 = v8 & ~HIDWORD(v11);
				HIDWORD(v11) = v8;
				if (_InterlockedCompareExchange64(
					(volatile signed __int64*)&v16->CellAvailable[v18],
					__SPAIR64__(v13, v12),
					v11) == __PAIR64__(v8, v15))
					return 1;
				v3 = FitLevel;
			}
		}
		return 0;
	}

	int SplitBlock(LevelInfo* Level, int Slot, LevelInfo* LevelTo) {

		jqAtomicHeap::LevelInfo* v4; // edi
		char v5; // cl
		volatile signed __int64* v6; // esi
		__int64 v8; // [esp+10h] [ebp-10h]
		signed __int64 i; // [esp+18h] [ebp-8h]
		jqAtomicHeap::LevelInfo* Levela; // [esp+28h] [ebp+8h]

		v4 = Level;
		if (Level > LevelTo)
		{
			do
			{
				v5 = (2 * Slot + 1) & 0x3F;
				Levela = v4 - 1;
				Slot *= 2;
				v8 = 1i64 << v5;
				v6 = (volatile signed __int64*)&v4[-1].CellAvailable[(Slot + 1) / 64];
				for (i = *v6; _InterlockedCompareExchange64(v6, v8 | i, i) != i; i = *v6)
					Sleep(0);
				--v4;
			} while (Levela > LevelTo);
		}
		tlAtomicOr(&v4->CellAllocated[Slot / 64], 1i64 << (Slot & 0x3F));
		return Slot;
	}

	int AllocLevel(int LevelIdx) {
		LevelInfo* lvlInfo;
		LevelInfo* FitLevel;
		int retnSplitBlock;

		lvlInfo = &this->Levels[LevelIdx];
		FitLevel = &this->Levels[LevelIdx];
		LevelIdx = 0;
		if (!AllocBlock(&FitLevel, &LevelIdx))
			return 0;
		retnSplitBlock = SplitBlock(FitLevel, LevelIdx, lvlInfo);
		_InterlockedExchangeAdd(&this->ThisPtr->TotalBlocks, 1u);
		_InterlockedExchangeAdd(&this->ThisPtr->TotalUsed, lvlInfo->BlockSize);
		return this->HeapBase[retnSplitBlock * lvlInfo->BlockSize];
	}

	int FindLevelForSize(unsigned int Size) {
		return (this->BlockSize < Size)
			+ (2 * this->BlockSize < Size)
			+ (4 * this->BlockSize < Size)
			+ (8 * this->BlockSize < Size)
			+ (16 * this->BlockSize < Size)
			+ (32 * this->BlockSize < Size)
			+ (this->BlockSize << 6 < Size)
			+ (this->BlockSize << 7 < Size)
			+ (this->BlockSize << 8 < Size)
			+ (this->BlockSize << 9 < Size)
			+ (this->BlockSize << 10 < Size);
	}

	void* Alloc(unsigned int Size, unsigned int Align) {
		unsigned int inSize;
		int foundLevel;
		int retnAllocLevel;
		bool isLocked;

		this->Mutex.Lock();
		inSize = Size;
		if (Size < Align)
			inSize = Align;
		if (inSize <= this->HeapSize)
		{
			foundLevel = this->FindLevelForSize(inSize);
			retnAllocLevel = this->AllocLevel(foundLevel);
			isLocked = this->Mutex.LockCount-- == 1;
			if (isLocked)
			{
				Size = 0;
				InterlockedExchange(&Size, 0);
				this->Mutex.ThreadId = 0i64;
			}
			return (void*)retnAllocLevel;
		}
		else
		{
			tlPrintf("Size (%d) > HeapSize (%d), return NULL\n", inSize, this->HeapSize);
			return 0;
		}
	}

	void FindAllocatedBlock(unsigned int Offset, LevelInfo** FitLevel, int* FitSlot) {
		int offsetOverBlocksize;
		jqAtomicHeap::LevelInfo* lvlInfoLoop;

		*FitLevel = this->Levels;
		if (this->Levels < &this->Levels[this->NLevels])
		{
			do
			{
				offsetOverBlocksize = Offset / (*FitLevel)->BlockSize;
				*FitSlot = offsetOverBlocksize;
				if ((*FitLevel)->CellAllocated[offsetOverBlocksize / 64] & (1i64 << (offsetOverBlocksize & 0x3F)))
					break;
				lvlInfoLoop = *FitLevel + 1;
				*FitLevel = lvlInfoLoop;
			} while (lvlInfoLoop < &this->Levels[this->NLevels]);
		}
		if (*FitLevel >= &this->Levels[this->NLevels]
			&& _tlAssert(
				__FILE__,
				__LINE__,
				"FitLevel < Levels+NLevels",
				"jqAtomicHeap error detected, block not found in heap."))
		{
			__debugbreak();
		}
	}

	void MergeBlocks(jqAtomicHeap::LevelInfo** FitLevel, int* FitSlot) {
		jqAtomicHeap::LevelInfo** m_FitLevel; // edi
		int* v4; // ecx
		int v5; // ecx
		int v6; // esi
		int v7; // ebx
		unsigned __int64* v8; // edi
		signed __int64 v9; // rax
		volatile signed __int64* v10; // edi
		unsigned int v11; // esi
		__int64 v12; // [esp+Ch] [ebp-10h]

		m_FitLevel = FitLevel;
		if (*FitLevel < (jqAtomicHeap::LevelInfo*)&this->HeapSize + this->NLevels)
		{
			v4 = FitSlot;
		LABEL_3:
			v5 = *v4 ^ 1;
			v6 = v5 / 64;
			v7 = 1i64 << (v5 & 0x3F);
			v12 = 1i64 << (v5 & 0x3F);
			while (1)
			{
				v8 = (*m_FitLevel)->CellAvailable;
				HIDWORD(v9) = HIDWORD(v8[v6]);
				v10 = (volatile signed __int64*)&v8[v6];
				if (!(HIDWORD(v9) & HIDWORD(v12) | *(_DWORD*)v10 & v7))
					break;
				LODWORD(v9) = *(_DWORD*)v10;
				v11 = *(_DWORD*)v10;
				if (_InterlockedCompareExchange64(v10, __SPAIR64__(HIDWORD(v9) & (unsigned int)~HIDWORD(v12), v11 & ~v7), v9) == __PAIR64__(HIDWORD(v9), v11))
				{
					m_FitLevel = FitLevel;
					++* FitLevel;
					v4 = FitSlot;
					*FitSlot >>= 1;
					if (*FitLevel < (jqAtomicHeap::LevelInfo*)&this->HeapSize + this->NLevels)
						goto LABEL_3;
					return;
				}
				m_FitLevel = FitLevel;
				v7 = 1i64 << (v5 & 0x3F);
				v6 = v5 / 64;
			}
		}
	}

	void Free(void* Ptr) {
		DWORD* m_Ptr; // ebx
		int v5; // esi
		__int64 v6; // rax
		unsigned int v7; // ebx
		unsigned int v9; // [esp-Ch] [ebp-20h]
		int FitSlot; // [esp+10h] [ebp-4h]

		this->Mutex.Lock();
		v9 = (char *)Ptr - this->HeapBase;
		FitSlot = 0;
		FindAllocatedBlock(v9, (LevelInfo **)&Ptr, &FitSlot);
		m_Ptr = (DWORD*)Ptr;
		tlAtomicAnd((volatile unsigned __int64*)(*((_DWORD*)Ptr + 4) + 8 * (FitSlot / 64)), ~(1i64 << (FitSlot & 0x3F)));
		_InterlockedExchangeAdd(&this->ThisPtr->TotalBlocks, 0xFFFFFFFF);
		_InterlockedExchangeAdd(&this->ThisPtr->TotalUsed, -*m_Ptr);
		MergeBlocks((LevelInfo**)&Ptr, &FitSlot);
		v5 = 8 * (FitSlot / 64);
		v6 = 1i64 << (FitSlot & 0x3F);
		v7 = v6;
		LODWORD(v6) = HIDWORD(v6) & *(_DWORD*)(*((_DWORD*)Ptr + 3) + v5 + 4) | v6 & *(_DWORD*)(*((_DWORD*)Ptr + 3) + v5);
		FitSlot = (1i64 << (FitSlot & 0x3F)) >> 32;
		if (v6
			&& _tlAssert(
				__FILE__,
				__LINE__,
				"(jqGet(&FitLevel->CellAvailable[BlockCell(FitSlot)]) & BlockBit(FitSlot)) == 0",
				"jqAtomicHeap error detected, invalid tree state."))
		{
			__debugbreak();
		}
		tlAtomicOr((volatile unsigned __int64*)(v5 + *((_DWORD*)Ptr + 3)), __PAIR64__(FitSlot, v7));
		if (this->Mutex.LockCount-- == 1)
		{
			Ptr = 0;
			InterlockedExchange((volatile LONG*)&Ptr, 0);
			this->Mutex.ThreadId = 0i64;
		}
	}

	void Init(void* _HeapBase, unsigned int _HeapSize, unsigned int _BlockSize) {
		unsigned int v5; // edx
		int v6; // ebx
		unsigned int v7; // edi
		unsigned int v8; // eax
		int v9; // ecx
		int v10; // edi
		int v11; // ecx
		char* v12; // eax
		char* v13; // edi
		int v14; // ebx
		unsigned __int64** v15; // ecx
		signed int v16; // kr04_4
		_DWORD* v17; // esi
		int i; // [esp+14h] [ebp+8h]
		int ia; // [esp+14h] [ebp+8h]

		this->BlockSize = _BlockSize;
		v5 = this->BlockSize;
		this->HeapBase = (char*)_HeapBase;
		this->HeapSize = _HeapSize;
		v6 = 0;
		v7 = this->HeapSize;
		this->ThisPtr = this;
		v8 = v5;
		this->TotalUsed = 0;
		this->TotalBlocks = 0;
		this->NLevels = 1;
		if (v5 < v7)
		{
			v9 = 1;
			do
			{
				++v9;
				v8 *= 2;
				this->NLevels = v9;
			} while (v8 < v7);
		}
		if (v5 << (LOBYTE(this->NLevels) - 1) != v7
			&& _tlAssert(
				__FILE__,
				__LINE__,
				"(BlockSize << (NLevels-1)) == HeapSize",
				"HeapSize must be a power of 2 of BlockSize."))
		{
			__debugbreak();
		}
		if (this->NLevels > 11
			&& _tlAssert(
				__FILE__,
				__LINE__,
				"NLevels <= JQ_ATOMIC_HEAP_MAX_LEVELS",
				"Maximum heap depth is 11 levels."))
		{
			__debugbreak();
		}
		i = 0;
		if (this->NLevels > 0)
		{
			v10 = (int)&this->Levels[0].NBlocks;
			do
			{
				v10 += 20;
				*(_DWORD*)(v10 - 24) = this->BlockSize << v6;
				v11 = this->NLevels - v6++ - 1;
				*(_DWORD*)(v10 - 20) = 1 << v11;
				*(_DWORD*)(v10 - 16) = (unsigned int)((1 << v11) + 63) >> 6;
				i += (int)((*(_DWORD*)(v10 - 20) + 1023) & 0xFFFFFC00) / 8;
			} while (v6 < this->NLevels);
		}
		v12 = (char*)tlMemAlloc(2 * i, 0x80u, 0);
		this->LevelData = v12;
		memset(v12, 0, 2 * static_cast<size_t>(i));
		v13 = this->LevelData;
		v14 = (int)&v13[i];
		ia = 0;
		if (this->NLevels > 0)
		{
			v15 = &this->Levels[0].CellAvailable;
			do
			{
				v16 = ((unsigned int)*(v15 - 2) + 1023) & 0xFFFFFC00;
				*v15 = (unsigned __int64*)v13;
				v15[1] = (unsigned __int64*)v14;
				v13 += v16 / 8;
				v14 += v16 / 8;
				v15 += 5;
				++ia;
			} while (ia < this->NLevels);
		}
		v17 = (_DWORD*)*((_DWORD*)&this->ThisPtr + 5 * this->NLevels + 10);
		*v17 = 1;
		v17[1] = 0;
	}
};