/*
 * networkservice.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: go
 */

#include "../networkservice.h"

namespace gowinder
{
	namespace network
	{

		network_service::network_service(OBJID id, DWORD interval_tick):service(id, ST_NETWORK, interval_tick)
		,_netmsg_protocal(NULL)
		{

		}

		network_service::~network_service()
		{
			// TODO Auto-generated destructor stub
		}

		bool network_service::on_init()
		{
			return true;
		}

		bool network_service::on_service_start()
		{
			return true;
		}

		bool network_service::on_service_stop()
		{
			return true;
		}

		bool network_service::on_process_event_pump(ST_ADVANCE_EVENT& event,
		        bool& processed)
		{
			return true;
		}

	} /* namespace network */
} /* namespace gowinder */
