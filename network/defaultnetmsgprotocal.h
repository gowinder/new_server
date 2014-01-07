/*
 * defaultnetmsgprotocal.h
 *
 *  Created on: Nov 12, 2013
 *      Author: go,gowinder@hotmail.com
 */

#ifndef DEFAULTNETMSGPROTOCAL_H_
#define DEFAULTNETMSGPROTOCAL_H_

#include <network_interface.h>

namespace gowinder
{
	namespace network
	{

		class default_netmsg_protocal: public gowinder::network::isocket_item_netmsg_protocal
		{
		public:
			default_netmsg_protocal();
			virtual ~default_netmsg_protocal();
		};

	} /* namespace network */
} /* namespace gowinder */

#endif /* DEFAULTNETMSGPROTOCAL_H_ */
