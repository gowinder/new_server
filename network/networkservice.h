/*
 * networkservice.h
 *
 *  Created on: Nov 5, 2013
 *      Author: gowinder@hotmail.com
 */

#ifndef NETWORKSERVICE_H_
#define NETWORKSERVICE_H_


#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "baselib.h"
#include "service.h"
#include "thread_base.h"
#include "socketitem.h"
#include "network_interface.h"

namespace gowinder
{
	namespace network
	{
		using namespace gowinder::baselib;
		using namespace gowinder::service;
		using boost::asio::ip::tcp;
		class network_service: public gowinder::service::service
		{
		public:
			network_service(OBJID id, DWORD interval_tick);
			virtual ~network_service();

			inline const boost::asio::io_service& get_io_service() const
			{
				return _io_service;
			}

			inline const isocket_item_netmsg_protocal& get_netmsg_protocal() const
			{
				return _netmsg_protocal;
			}

			inline void set_netmsg_protocal(
			        const isocket_item_netmsg_protocal& netmsg_protocal)
			{
				_netmsg_protocal = netmsg_protocal;
			}

		protected:
			virtual bool on_init();
			virtual bool on_service_start();
			virtual bool on_service_stop();
			virtual bool on_process_event_pump(ST_ADVANCE_EVENT& event, bool& processed);

		protected:
			boost::asio::io_service	_io_service;
			isocket_item_netmsg_protocal*	_netmsg_protocal;
		};


	} /* namespace network */
} /* namespace gowinder */
#endif /* NETWORKSERVICE_H_ */
