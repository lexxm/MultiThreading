#pragma once
#include "crossplatformmutex.h"

void crossplatformSleep( unsigned int msec );
void waitForMultipleObjectNew( int* valueForSynchroniztion, int countTasks, MUTEX& mutexForSynchronize );
