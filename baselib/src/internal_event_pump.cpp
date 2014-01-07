#include "internal_event_pump.h"

#include "basefunction.h"
#include "basedef.h"
#include <errno.h>
#include <sys/time.h>

#include <boost/date_time.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_types.hpp>

namespace gowinder
{
namespace baselib
{

internal_event_pump::internal_event_pump(int id,
		DWORD max_event_count) :
		_pump_id(id), _max_event_count(max_event_count),
		_is_open(false)
{
	boost::timed_mutex::scoped_timed_lock lock(_mutex);
}

internal_event_pump::~internal_event_pump(void)
{
	if (is_open())
		close();
}

bool internal_event_pump::push(int from, int type, int size,
		const char* data, int param1, int param2, int param3)
{
	boost::timed_mutex::scoped_timed_lock lock(_mutex);
	if (!is_open())
		return false;

	if (size > MAX_ADV_EVENT_DATA_SIZE)
	{
		LOGERROR(LOG_FILE, EM_LOG_HIGH,
				"CAdvanceInternalEventPump (%d) size overflow %d", _pump_id,
				get_size());
		return false;
	}

	if (get_size() >= _max_event_count)
	{
		LOGERROR(LOG_FILE, EM_LOG_HIGH,
				"CAdvanceInternalEventPump (%d) size overflow %d", _pump_id,
				get_size());
		return false;
	}

	ST_ADVANCE_EVENT* new_event = NULL;
	if (_recycle_events.size() > 0)
	{
		try
		{
			new_event = _recycle_events.front();
			_recycle_events.pop_front();
		} catch (...)
		{
			int i = 0;
		}
	}

	int data_size = 0;
	BYTE* alloc_data = NULL;
	if (size > 0 && data != NULL)
	{
		alloc_data =
				auto_size_with_recycle_container<MAP_ADV_DATA, LIST_ADV_DATA>::GetAdvDataBySize(
						size, data_size);
		if (alloc_data == NULL)
			return false;
	}

	if (new_event == NULL)
		new_event = new ST_ADVANCE_EVENT;

	// [4/16/2009 �粻Ϣ gowinder@hotmail.com] 
	//	��ʼ������ 
	memset(new_event, 0, sizeof(ST_ADVANCE_EVENT));

	new_event->from = from;
	new_event->type = type;
	new_event->param1 = param1;
	new_event->param2 = param2;
	new_event->param3 = param3;
	new_event->data.data_size = size;
	new_event->data.size = data_size;

	if (alloc_data && size && data)
	{
		memcpy(alloc_data, data, size);
	}

	new_event->data.data = alloc_data;

	_events.push_back(new_event);

	_candition_pump_has_event.notify_one();

	return true;

}

bool internal_event_pump::pop(ST_ADVANCE_EVENT* pEvent)
{
	boost::timed_mutex::scoped_timed_lock lock(_mutex);

	if (!is_open())
		return false;

	DEBUG_TRY
	//CSingleLock lock(&m_csLock, TRUE);

		if (_events.size() < 1)
			return false;

		ST_ADVANCE_EVENT* stored_event = _events.front();
		pEvent->from = stored_event->from;
		pEvent->param1 = stored_event->param1;
		pEvent->param2 = stored_event->param2;
		pEvent->param3 = stored_event->param3;
		pEvent->type = stored_event->type;
		pEvent->data.data_size = stored_event->data.data_size;
		if (stored_event->data.data_size > 0)
		{
			ASSERT(pEvent->data.size >= stored_event->data.size);
			memcpy(pEvent->data.data, stored_event->data.data,
					stored_event->data.data_size);
		}
		_events.pop_front();

		stored_event->from = 0;

		_recycle_events.push_back(stored_event);
		if (stored_event->data.data != NULL && stored_event->data.size > 0)
		{
			RecycleData(stored_event->data.size, stored_event->data.data);
			memset(&stored_event->data, 0, sizeof(ST_ADVANCE_EVENT_DATA));
		}
		else
		{
			pEvent->data.data[0] = 0;
		}

	DEBUG_CATCH4("CAdvanceInternalEventPump::Pop from %d, type %d, size %d", pEvent->from, pEvent->type, pEvent->data.data_size);

	return true;
}

bool internal_event_pump::wait(DWORD dwMillSec)
{
	boost::timed_mutex::scoped_timed_lock lock(_mutex);

	if (!is_open())
		return false;

	if (get_size())
		return true;

	boost::posix_time::time_duration td = boost::posix_time::milliseconds(
			dwMillSec);
	bool wait_result = _candition_pump_has_event.timed_wait(_mutex, td);
	return wait_result;
}

bool internal_event_pump::open()
{
	if (is_open())
		return false;

	return true;
}

bool internal_event_pump::is_open()
{
	return _is_open;
}

bool internal_event_pump::close()
{
	boost::timed_mutex::scoped_timed_lock lock(_mutex);

	if (_pump_id == KI_UI)
		int i = 0;

	if (!is_open())
		return true;

	for (LIST_ADV_MSG::iterator iter = _events.begin();
			iter != _events.end(); iter++)
	{
		ST_ADVANCE_EVENT* pEvent = (*iter);

		SAFE_DELETE(pEvent->data.data);

		SAFE_DELETE(pEvent);
	}
	_events.clear();

	for (LIST_ADV_MSG::iterator iterRe = _recycle_events.begin();
			iterRe != _recycle_events.end(); iterRe++)
	{
		ST_ADVANCE_EVENT* pEvent = (*iterRe);
		SAFE_DELETE(pEvent);
	}
	_recycle_events.clear();

	_is_open = false;

	return true;
}

DWORD internal_event_pump::get_size()
{
	return (DWORD) _events.size();
}

DWORD internal_event_pump::GetSize(int index)
{
	return (DWORD) _events.size();
}

bool internal_event_pump::IsFull()
{
	if (get_size() >= _max_event_count)
	{
		return true;
	}

	return false;
}

}
}
