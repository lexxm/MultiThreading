#pragma once


typedef bool( *processFunc )( void* lParam, int threadId );

struct DataStructForMultyThreading
{
	processFunc Func;
	void* Param;
};