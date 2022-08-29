#include "stdafx.h"
#include "OneThreadFunctionality.h"


THREAD_RETURN_VALUE WINAPI ThreadProcWork( LPVOID pParam )
{
	OneThreadFunctionality* pParent = (OneThreadFunctionality*) pParam;

	if ( NULL == pParent )
		return 0;

	pParent->run();
	//__try
	//{
	//}
	//__except ( filter( GetExceptionCode(), GetExceptionInformation() ) )
	//{
	//	logError( L" UnhandledException in sending thread" );
	//	TerminateProcess( GetCurrentProcess(), 1 );
	//}

	return 0;
}

#ifdef _WIN32
OneThreadFunctionality::OneThreadFunctionality( MtSafeQueue<DataStructForMultyThreading>* queue, int* valueForSynch, HANDLE mutexForSynchronize )
#else
OneThreadFunctionality::OneThreadFunctionality( MtSafeQueue<DataStructForMultyThreading>* queue, int* valueForSynch, pthread_mutex_t& mutexForSynchronize )
#endif
: _queue( queue )
, _valueForSynchronize( valueForSynch )
, _mutexForSynchronize( mutexForSynchronize )
, _id( 0 )
, _isBreak( false )
{
	lockTime = 0;
	initMutex( _flagOfBreakMutex );
	unlockMutex( _flagOfBreakMutex );
}

OneThreadFunctionality::~OneThreadFunctionality()
{
	stop();
}

void OneThreadFunctionality::start()
{
	_eventForProcCompleate.unsignal();
	_eventForStart.signal();
}

void OneThreadFunctionality::run()
{
	lockTime = 0;
	while ( true )
	{
		_eventForStart.wait();
#if 1
		lockMutex( _flagOfBreakMutex );
		if ( _isBreak )
		{
			break;
		}
		unlockMutex( _flagOfBreakMutex );
#endif
		while ( true )
		{
			DataStructForMultyThreading* dataToProcess = _queue->take();
			if ( dataToProcess == nullptr )
				break;

			(*dataToProcess->Func)( dataToProcess->Param, _id );
			
			delete dataToProcess;

		}
		_eventForStart.unsignal();
		_eventForProcCompleate.signal();
	}
	_eventForEnd.signal();
}
#if 1
void OneThreadFunctionality::stop()
{
	lockMutex( _flagOfBreakMutex );
	_isBreak = true;
	unlockMutex( _flagOfBreakMutex );
	
	_eventForStart.signal();
	_eventForEnd.wait();
}
#endif
