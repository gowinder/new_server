/*
 * consoleservice.h
 *
 *  Created on: Nov 11, 2013
 *      Author: go
 */

#ifndef CONSOLESERVICE_H_
#define CONSOLESERVICE_H_

#include "baselib.h"
#include "service.h"
#include "service_define.h"

namespace gowinder
{
	namespace service
	{

		class console_service: public gowinder::service::service, public ilogger_tracer
		{
		public:
			console_service(OBJID service_id, DWORD go_tick_interval);
			virtual ~console_service();
			virtual void trace(std::string& log, int level);
		protected:
			virtual service_thread<service>* create_thread()	{return NULL;}
			virtual bool on_process_event_pump(ST_ADVANCE_EVENT& event, bool &processed);
		};

	} /* namespace service */
} /* namespace gowinder */

#endif /* CONSOLESERVICE_H_ */
