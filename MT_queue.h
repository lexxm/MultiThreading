#pragma once

#include <list>
#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
#include "crossplatformmutex.h"
#include "SoftwareException.h"
#include "xtimer.h"
#include <queue>

#define QUEUE_VER
#ifndef QUEUE_VER
#include "boost\lockfree\queue.hpp"
#endif

template<typename T> class MtSafeQueue
{
public:
	/// Конструктор
	MtSafeQueue( bool useMaxCountPackets = false );

	/// Деструктор
	~MtSafeQueue();

	/// Добавить новый готовый к отправке клиенту пакет в конец очереди
	void put( T* in_packet );

	/// Взять пакет из начала очереди
	T* take();

	/// Удалить из очереди все пакеты, кроме управляющих
	void clearPacketQueue();

	bool isEmpty();

	unsigned int getQueueSize();
	double lockTime;
protected:
#ifdef QUEUE_VER
	queue<T*> _packets;
#else
	boost::lockfree::queue<T*> _packets;//(2048)
#endif
	MUTEX _queueMutex;
	static const int MAX_QUEUE_SIZE = 512;	/// Максимальное число пакетов в очереди
	bool _useMaxCountPackets;
};

template<typename T> MtSafeQueue<T>::MtSafeQueue( bool useMaxCountPackets )
: _useMaxCountPackets( useMaxCountPackets )
{
	lockTime = 0;
	initMutex( _queueMutex );
	unlockMutex( _queueMutex );
}

template<typename T> MtSafeQueue<T>::~MtSafeQueue()
{
	clearPacketQueue();

#ifdef _WIN32
	if ( NULL != _queueMutex )
		CloseHandle( _queueMutex );
#else
	//if ( _queueMutex == PTHREAD_MUTEX_INITIALIZER )
	pthread_mutex_destroy( &_queueMutex );
#endif
}

template<typename T> void MtSafeQueue<T>::put( T *in_packet )
{
#ifdef QUEUE_VER
	lockMutex( _queueMutex );
	if ( _useMaxCountPackets && _packets.size() >= MAX_QUEUE_SIZE )
		delete in_packet;
	else
#endif
		_packets.push( in_packet );
#ifdef QUEUE_VER
	unlockMutex( _queueMutex );
#endif
}

template<typename T> T* MtSafeQueue<T>::take()
{
	T *pPacket = nullptr;
#ifdef QUEUE_VER
	CXTimer t0;
	lockMutex( _queueMutex );
	lockTime += t0.GetElapsedMSec();
#endif
	if ( _packets.empty() )
	{
#ifdef QUEUE_VER
		unlockMutex( _queueMutex );
#endif
		return nullptr;
	}

	//printf( "queue size: %d\n", _packets.size() );
#ifdef QUEUE_VER
	pPacket = _packets.front();
	_packets.pop();
	unlockMutex( _queueMutex );
#else
	_packets.pop( pPacket );
#endif

	return pPacket;
}

template<typename T> void MtSafeQueue<T>::clearPacketQueue()
{
#ifdef QUEUE_VER
	lockMutex( _queueMutex );
#endif
	while ( !_packets.empty() )
	{
#ifdef QUEUE_VER
		T* p = _packets.front();
		_packets.pop();
#else
		T* p;
		_packets.pop( p );
#endif
		delete p;
	}

#ifdef QUEUE_VER
	unlockMutex( _queueMutex );
#endif
}

template<typename T> bool MtSafeQueue<T>::isEmpty()
{
	bool isQueueEmpty = false;

#ifdef QUEUE_VER
	lockMutex( _queueMutex );
#endif
	isQueueEmpty = _packets.empty();
#ifdef QUEUE_VER
	unlockMutex( _queueMutex );
#endif
	return isQueueEmpty;
}

template<typename T> unsigned int MtSafeQueue<T>::getQueueSize()
{
	size_t size = 0;

#ifdef QUEUE_VER
	lockMutex( _queueMutex );
	size = _packets.size();
	unlockMutex( _queueMutex );
#endif

	return (unsigned int) size;
}
