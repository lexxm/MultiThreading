#pragma once
#include "SoftwareException.h"

class WaitForMultipleObjectException : public SoftwareException
{
public:
	WaitForMultipleObjectException() : SoftwareException() {}
};