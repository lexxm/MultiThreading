#include "stdafx.h"
#include "MultiThreadTemplate.h"
#include "BaseMultiThreadFunctionality.h"

map<int,MultiThreadTemplate*> MultiThreadTemplate::_tableOfIstance;

MultiThreadTemplate::MultiThreadTemplate( int countThreads )
: _countThreads( countThreads )
{
	initMutex( _mutexForSynchronize );
	unlockMutex( _mutexForSynchronize );
	_threadFunctionality = new OneThreadFunctionality*[_countThreads];

#ifdef _WIN32
	_poolOfThreads = new HANDLE[_countThreads];
#else
	_poolOfThreads = new pthread_t[_countThreads];
#endif

	for ( int i = 0; i < _countThreads; i++ )
	{
		_threadFunctionality[i] = new OneThreadFunctionality( &_queue, &_valueForSynchronize, _mutexForSynchronize );
		_threadFunctionality[i]->setId( i );
#ifdef _WIN32
		_poolOfThreads[i] = (HANDLE) _beginthreadex( NULL, 0, ThreadProcWork, _threadFunctionality[i], 0, NULL );
#else
		int status;
		pthread_attr_t attr;
		pthread_attr_init( &attr );
		status = pthread_create( &_poolOfThreads[i], &attr, ThreadProcWork, _threadFunctionality[i] );
#endif
	}
}

MultiThreadTemplate::~MultiThreadTemplate()
{
	for ( int i = 0; i < _countThreads; i++ )
		delete _threadFunctionality[i];
	delete[] _threadFunctionality;
	delete[] _poolOfThreads;
}

void MultiThreadTemplate::run()
{
	vector<DataStructForMultyThreading> g;
	// вот здесь надо заполнить очередь
	// MtSafeQueue<DataStructForMultyThreading> _queue;

	_queue.lockTime = 0;
	_valueForSynchronize = 0;
	int countTasks = _queue.getQueueSize();
	for ( int i = 0; i < _countThreads; i++ )
		_threadFunctionality[i]->start();

	for ( int i = 0; i < _countThreads; i++ )
		_threadFunctionality[i]->wait();
}

MultiThreadTemplate* MultiThreadTemplate::getInstance( int typeOfMtTemplate )
{
	if ( _tableOfIstance.find( typeOfMtTemplate ) != _tableOfIstance.end() )
		return _tableOfIstance[typeOfMtTemplate];

	MultiThreadTemplate* newTemplate = nullptr;
	switch ( typeOfMtTemplate )
	{
		case MT_TEMPLATE_FOR_DICTIONARY:
			newTemplate = new MultiThreadTemplate( MT_TEMPLATE_DICTIONARY_COUNT_THREADS );
			break;
		case MT_TEMPLATE_FOR_ZONE_SEARCH:
			newTemplate = new MultiThreadTemplate( MT_TEMPLATE_ZONE_SEARCH_COUNT_THREADS );
			break;
		case MT_TEMPLATE_FOR_ZONE_PROCESS:
			newTemplate = new MultiThreadTemplate( MT_TEMPLATE_ZONE_PROCESS_COUNT_THREADS );
			break;
		case MT_TEMPLATE_FOR_PRE_PROCESS:
			newTemplate = new MultiThreadTemplate( MT_TEMPLATE_PRE_PROCESS_COUNT_THREADS );
			break;
	}
	_tableOfIstance[typeOfMtTemplate] = newTemplate;
	return newTemplate;
}

void MultiThreadTemplate::stopThreads()
{
	for ( int i = 0; i < _countThreads; i++ )
#ifdef _WIN32
		CloseHandle( _poolOfThreads[i] );
#else
		pthread_cancel( _poolOfThreads[i] );
#endif
}

void MultiThreadTemplate::release( )
{
	for ( auto it : _tableOfIstance )
		it.second->stopThreads();
		
	for ( auto it : _tableOfIstance )
		delete it.second;
}
