/*
 * socketitem.cpp
 *
 *  Created on: Nov 11, 2013
 *      Author: go,gowinder@hotmail.com
 */
#include "networkservice.h"

namespace gowinder
{
	namespace network
	{
		using boost::asio::ip::tcp;
		socket_item::socket_item(boost::asio::io_service& ios, network_service& service, DWORD index) :
				_network_service(service), _socket(ios), _index(index)
		{

		}

		socket_item::~socket_item()
		{
			// TODO Auto-generated destructor stub
		}

		void socket_item::start()
		{
		    _socket.async_read_some(boost::asio::buffer(_recv_data, SOCKET_RECEIVE_DATA_SIZE),
		        boost::bind(&socket_item::handle_read, this,
		          boost::asio::placeholders::error,
		          boost::asio::placeholders::bytes_transferred));
		}

		bool socket_item::send(const char* data, int length)
		{

		}

		void socket_item::handle_read(const boost::system::error_code& error,
		        size_t bytes_transferred)
		{
		}

		void socket_item::handle_write(const boost::system::error_code& error)
		{
		}

	} /* namespace network */
} /* namespace gowinder */
