#pragma once
#include <windows.h>
#include <mutex>
#include <vector>
#define PTRMAXVAL ((PVOID)0x000F000000000000)
typedef uintptr_t QWORD;
using namespace std;
template<typename T>
class Singleton
{
protected:
	Singleton() {}
	~Singleton() {}

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	Singleton(Singleton&&) = delete;
	Singleton& operator=(Singleton&&) = delete;

public:
	static T& Get()
	{
		static T inst{};
		return inst;
	}
};


namespace CUtils {
	void DBG(char *format, ...);

	uint8_t* PatternScan(void* module, const char* signature);

	template<class T>
	T Read(const DWORD64 dwPtr)
	{
		if (!IsBadReadPtr((void*)dwPtr, sizeof(T)))
			return *(T*)dwPtr;
		return 0;
	}
	static __forceinline BOOLEAN IsValid(PVOID ptr)
	{
		return (ptr >= (PVOID)0x10000) && (ptr < PTRMAXVAL) && ptr != nullptr && !IsBadReadPtr(ptr, sizeof(ptr));
	}
	template <class vType>
	static BOOLEAN Write(QWORD address, vType value)
	{
		if (IsValid((vType*)(address)))
		{
			*(vType*)(address) = value;
			return TRUE;
		}
		return FALSE;
	}
	template <typename vType>
	static BOOLEAN WritePtr(std::initializer_list<QWORD> _Offsets, vType _value, bool ReadFirstOffset)
	{
		QWORD LastPtr = NULL;
		int OffsetsSize = NULL;
		std::vector<QWORD> Offsets = { NULL };
		Offsets = _Offsets;
		OffsetsSize = Offsets.size();
		LastPtr = Read<QWORD>((ReadFirstOffset ? Read<QWORD>(Offsets[0]) : Offsets[0]) + Offsets[1]);
		for (size_t i = 2; i < OffsetsSize - 1; i++)
			if (!(LastPtr = Read<QWORD>(LastPtr + Offsets[i])))
				return FALSE;
		return Write<vType>(LastPtr + Offsets[OffsetsSize - 1], _value);
	}

	float RandomFloat(float min, float max);
	void ReadData(const DWORD64 dwPtr, void *pData, int size);
	void split(const string& src, const string& separator, vector<string>& dest);
	template <typename vType>
	static vType ReadPtr(std::initializer_list<QWORD> _Offsets, bool ReadFirstOffset)
	{
		QWORD LastPtr = NULL;
		int OffsetsSize = NULL;
		std::vector<QWORD> Offsets = { NULL };
		Offsets = _Offsets;
		OffsetsSize = Offsets.size();
		LastPtr = Read<QWORD>((ReadFirstOffset ? Read<QWORD>(Offsets[0]) : Offsets[0]) + Offsets[1]);
		for (size_t i = 2; i < OffsetsSize - 1; i++)
			if (!(LastPtr = Read<QWORD>(LastPtr + Offsets[i])))
				return vType();
		return Read<vType>(LastPtr + Offsets[OffsetsSize - 1]);
	}

}



