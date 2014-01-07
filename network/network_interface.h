#ifndef ___NETWORK_INTERFACE_HEADER___
#define ___NETWORK_INTERFACE_HEADER___

namespace gowinder
{
	namespace network
	{

		class isocket_item_netmsg_protocal
		{
		public:
			virtual ~isocket_item_netmsg_protocal(){}

			virtual int check_netmsg(const char* buffer_data, int buffer_length) = 0;	//!<	check if entire msg is in buffer data, return the msg length
		};
	}
}


#endif
