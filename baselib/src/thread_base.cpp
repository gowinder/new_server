/*
 * CThreadBase.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: go
 */

#include "thread_base.h"
#include <boost/timer.hpp>
#include <boost/thread/pthread/thread_data.hpp>

namespace gowinder
{
namespace baselib
{

bool thread_base::is_running()
{
	return _thread.joinable();
}

bool thread_base::go_tick(DWORD interval_tick)
{
	boost::timer tick;

	bool bOk = true;
	//	真正的运行
	DEBUG_TRY
	bOk = repeat_run(interval_tick);
	DEBUG_CATCH("CThreadBase::GoTick-RepetitionRun");


	if(!bOk)
		return false;

	//	用时计算

	DEBUG_TRY
	UINT64 used_milliseconds = tick.elapsed() * 1000;
	_monitor.repetition_time_unitage += used_milliseconds;

	DWORD dwRepetitionInterval = get_repetition_run_interval();
	if(used_milliseconds < dwRepetitionInterval)
	{

	}
	DEBUG_CATCH("CThreadBase::GoTick check time");


	return true;
}

DWORD ThreadProc(LPVOID lpParameter)
{
	thread_base* pServiceThread = (thread_base*)lpParameter;

	if(pServiceThread == NULL)
		return -1;

	DWORD i64UseTime = 0;

#if defined(WIN32)
	CoInitialize(NULL);
#endif
	while(!pServiceThread->is_interruption_request())
	{
		if(!pServiceThread->is_running())
			break;

		if(!pServiceThread->go_tick(pServiceThread->get_repetition_run_interval()))
			break;
	}
#if defined(WIN32)
	CoUninitialize();
#endif
	return 0;
}



bool thread_base::start()
{
	if(_thread.joinable())
		return false;

	boost::thread_attributes thread_att;
	thread_att.set_stack_size(1024 * 1024 * 5);
	_thread = boost::thread(thread_att, boost::bind(ThreadProc, this));

	return true;
}

bool thread_base::stop(DWORD wait_tick)
{
	//	停止线程
	DEBUG_TRY

	if(!_thread.joinable())
		return false;

	_thread.interrupt();
	_thread.join();


	DEBUG_CATCH("CThreadBase::StopThread");

	return true;
}

thread_base::thread_base():
_interval(THREAD_STANDARD_REPETITIONRUN_INTERVAL)
{
	memset(&_monitor, 0, sizeof(ST_THREAD_MONITOR));

}

thread_base::~thread_base()
{

}

void thread_base::reset_unitage()
{
	_monitor.repetition_time_unitage = 0;
	_monitor.repetition_sleep_time_unitage = 0;
}

}
}

