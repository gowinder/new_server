#ifndef ___ADVANCEINTERNALEVENT_HEADER___
#define ___ADVANCEINTERNALEVENT_HEADER___

#include "constvar.h"
#include "struct.h"
#include "interface.h"
#include "basefunction.h"
#include "auto_size_with_recycle_container.h"

#include <vector>
#include <list>
#include <queue>
#include <pthread.h>

#include <boost/thread.hpp>

namespace gowinder
{
namespace baselib
{

using namespace std;

typedef list<ST_ADVANCE_EVENT*> LIST_ADV_MSG;

typedef list<BYTE*> LIST_ADV_DATA;

typedef map<int, LIST_ADV_DATA*> MAP_ADV_DATA;

const DWORD ADV_EVENT_PUMP_MAX_EVENT_COUNT = 50000;

/// \author �粻Ϣ gowinder@hotmail.com
/// \brief  �߼��ڲ���Ϣ��,�Ա䳤��ݶ������Ż�?
/// \date [2012-17-8]
/// \detail 
/// \return	
class internal_event_pump: public ievent_pump,
		public auto_size_with_recycle_container<MAP_ADV_DATA, LIST_ADV_DATA>
{
public:
	internal_event_pump(int nID, DWORD max_event_count =
			ADV_EVENT_PUMP_MAX_EVENT_COUNT);
	virtual ~internal_event_pump(void);
public:
	virtual bool push(int from, int type, int size, const char* data, int param1,
			int param2, int param3);
	virtual bool pop(ST_ADVANCE_EVENT* msg);
	virtual bool wait(DWORD milli_seconds);
	virtual bool open();
	virtual bool is_open();
	virtual bool close();
	virtual DWORD get_size();
	virtual DWORD GetSize(int index);
	virtual bool IsFull();
private:
	LIST_ADV_MSG _events;		//!<	��Ϣ����
	LIST_ADV_MSG _recycle_events;	//!<	��������Ϣ����

//	::CCriticalSection	m_csLock;		//	ɾ�������?
	//	HANDLE	m_hHaveEvent;	//	�Ƿ�����Ϣ���?

	bool _is_open;	//	�Ƿ�ʼ����

	OBJID _pump_id;
	DWORD _max_event_count;

//	pthread_mutex_t* _pMutex;		//	ɾ�������?
//	pthread_cond_t* _pCond;
	boost::timed_mutex _mutex;
	boost::condition_variable_any _candition_pump_has_event;

};

}

}

#endif
