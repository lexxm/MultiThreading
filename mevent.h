#pragma once

#include <mutex>
#include <condition_variable>

class ManualResetEvent
{
public:
	ManualResetEvent() : _signaled(false) {}

	void signal()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_signaled = true;
		_cond.notify_all();
	}

	void unsignal()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		_signaled = false;
	}


	void wait()
	{
		std::unique_lock<std::mutex> lock(_mutex);

		while ( !_signaled )
			_cond.wait(lock);
	}

private:
	std::mutex _mutex;
	std::condition_variable _cond;
	bool _signaled;
};