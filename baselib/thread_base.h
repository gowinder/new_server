/*
 * Cthread_base.h
 *
 *  Created on: Nov 5, 2013
 *      Author: go
 */

#ifndef Cthread_base_H_
#define Cthread_base_H_

#include "constvar.h"
#include "basedef.h"
#include "interface.h"
#include "basefunction.h"
#include <boost/thread.hpp>

namespace gowinder
{
namespace baselib
{

const DWORD THREAD_STANDARD_REPETITIONRUN_INTERVAL = 50;

/// \author 风不息 gowinder@hotmail.com
/// \brief 线程监控
/// \date 5/8/2009
///
///
///
typedef struct _ST_THREAD_MONITOR
{
	LONG repetition_time_unitage;	//!< 	每次循环用时
	LONG repetition_time_max;		//!< 	循环最大用时

	LONG repetition_sleep_time_unitage;	//!< 	每次循环休息用时
	LONG repetition_sleep_time_max;		//!< 	循环休息最大用时
} ST_THREAD_MONITOR;

// 线程基类
class thread_base
{
protected:
	thread_base();
	virtual ~thread_base();
	DWORD _interval;
public:
	bool start();
	bool stop(DWORD wait_tick);
	virtual bool repeat_run(DWORD internal_tick) = 0;
	bool is_running();

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 不可调用
	/// \date 5/8/2009
	///
	bool go_tick(DWORD interval_tick);
	inline void set_repetition_run_interval(DWORD interval =
			THREAD_STANDARD_REPETITIONRUN_INTERVAL)
	{
		_interval = interval;
	}
	inline DWORD get_repetition_run_interval()
	{
		return _interval;
	}

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 得监控数据
	/// \date 5/8/2009
	///
	ST_THREAD_MONITOR* get_monitor()
	{
		return &_monitor;
	}

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 重置单位时间统计数据
	/// \date 5/8/2009
	///
	void reset_unitage();

	bool is_interruption_request()	{return _thread.interruption_requested();}
private:
	boost::thread _thread;
	ST_THREAD_MONITOR _monitor;	//!< 	线程监控
};

}
}

#endif /* Cthread_base_H_ */
