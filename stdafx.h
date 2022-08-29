// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#ifdef _WIN32
#include <tchar.h>
#include <windows.h>
typedef unsigned int THREAD_RETURN_VALUE;
#else
#include <unistd.h>
#include <pthread.h>
typedef unsigned int DWORD;
#define WINAPI
typedef void* LPVOID;
typedef void* THREAD_RETURN_VALUE;
#endif

#include "SoftwareException.h"
#include <math.h>

#include <vector>
#include <string.h>
#include <queue>
#include <map>
#include "xtimer.h"

typedef unsigned char BYTE;
using namespace std;
