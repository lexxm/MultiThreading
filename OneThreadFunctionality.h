#pragma once

#include "MT_queue.h"
#include "DataStructForMultyThreading.h"
#include "mevent.h"

#ifdef _WIN32
typedef unsigned int THREAD_RETURN_VALUE;
#else
typedef void* THREAD_RETURN_VALUE;
#define WINAPI
#endif

THREAD_RETURN_VALUE WINAPI ThreadProcWork( void* pParam );

class OneThreadFunctionality
{
public:
#ifdef _WIN32
	OneThreadFunctionality( MtSafeQueue<DataStructForMultyThreading>* queue, int* valueForSynch, HANDLE mutexForSynchronize );
#else
	OneThreadFunctionality( MtSafeQueue<DataStructForMultyThreading>* queue, int* valueForSynch, pthread_mutex_t& mutexForSynchronize );
#endif
	~OneThreadFunctionality();
	void run();
	void start();
	void wait()	{ _eventForProcCompleate.wait(); }
	void stop();
	void setId( int i ) { _id = i; }
	double lockTime;

protected:
	MtSafeQueue<DataStructForMultyThreading>* _queue;
	ManualResetEvent _eventForStart;
	ManualResetEvent _eventForEnd;
	ManualResetEvent _eventForProcCompleate;
	bool _isBreak;
#ifdef _WIN32
	HANDLE _mutexForSynchronize;
	HANDLE _flagOfBreakMutex;
#else
	pthread_mutex_t& _mutexForSynchronize;
	pthread_mutex_t _flagOfBreakMutex;
#endif
	int* _valueForSynchronize;
	int _id;
};

