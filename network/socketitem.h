/*
 * socketitem.h
 *
 *  Created on: Nov 11, 2013
 *      Author: go,gowinder@hotmail.com
 */

#ifndef SOCKETITEM_H_
#define SOCKETITEM_H_

#include <iostream>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "network_define.h"

namespace gowinder
{
	namespace network
	{
		using boost::asio::ip::tcp;
		class network_service;
		class socket_item
		{
		public:
			socket_item(boost::asio::io_service& ios, network_service& service, DWORD index);
			virtual ~socket_item();

			void start();
			bool send(const char* data, int length);

		protected:
			void handle_read(const boost::system::error_code& error,
			      size_t bytes_transferred);
			void handle_write(const boost::system::error_code& error);

		protected:
			boost::asio::ip::tcp::socket _socket;
			network_service&	_network_service;
			DWORD	_index;
			char	_recv_data[SOCKET_RECEIVE_DATA_SIZE];
			char	_send_data[SOCKET_SEND_DATA_SIZE];
			int		_recv_data_length;
			int		_send_data_length;

		};

	} /* namespace network */
} /* namespace gowinder */

#endif /* SOCKETITEM_H_ */
