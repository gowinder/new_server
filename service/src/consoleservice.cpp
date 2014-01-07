/*
 * consoleservice.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: go
 */

#include "consoleservice.h"


namespace gowinder
{
	namespace service
	{
		using namespace gowinder::service;
		using namespace gowinder::baselib;
		console_service::~console_service()
		{
			// TODO Auto-generated destructor stub
		}

		console_service::console_service(OBJID service_id,
		        DWORD go_tick_interval) :
				service(service_id, ST_CONSOLE_UI, go_tick_interval)
		{
		}

		void console_service::trace(std::string& log, int level)
		{
			this->post_event(0, SET_TRACE_LOG, log.length(), log.c_str(), level,
			        0, 0);
		}

		bool console_service::on_process_event_pump(ST_ADVANCE_EVENT& event,
		        bool& processed)
		{
			switch (event.type)
			{
				case SET_TRACE_LOG:
				{
					cout << event.data.data << endl;
					processed = true;
				}
				break;
			}
		}

	} /* namespace service */
} /* namespace gowinder */
