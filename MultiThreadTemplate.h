#pragma once

#include "OneThreadFunctionality.h"

#define MT_TEMPLATE_FOR_DICTIONARY 0
#define MT_TEMPLATE_FOR_ZONE_SEARCH 1
#define MT_TEMPLATE_FOR_ZONE_PROCESS 2
#define MT_TEMPLATE_FOR_PRE_PROCESS 3


#define MT_TEMPLATE_DICTIONARY_COUNT_THREADS 4
#define MT_TEMPLATE_ZONE_SEARCH_COUNT_THREADS 4
#define MT_TEMPLATE_ZONE_PROCESS_COUNT_THREADS 4
#define MT_TEMPLATE_PRE_PROCESS_COUNT_THREADS 4

#ifdef _WIN32
#include <process.h>
#else
#include <pthread.h>
#endif

class MultiThreadTemplate
{
protected:
	MultiThreadTemplate( int countThreads );
public:
	void run();
	~MultiThreadTemplate();
	static MultiThreadTemplate* getInstance( int typeOfMtTemplate );
	static void release( );
	void stopThreads();

	MtSafeQueue<DataStructForMultyThreading>& getQueue() { return _queue; }
protected:
	OneThreadFunctionality** _threadFunctionality;
	MtSafeQueue<DataStructForMultyThreading> _queue;

	int _valueForSynchronize;
	int _countThreads;

#ifdef _WIN32
	HANDLE* _poolOfThreads;
#else
	pthread_t* _poolOfThreads;
#endif
	MUTEX _mutexForSynchronize;
	static map<int,MultiThreadTemplate*> _tableOfIstance;
};