#ifndef ___SERVICEDEFINE_HEADER___
#define ___SERVICEDEFINE_HEADER___

namespace gowinder
{
	namespace service
	{

		enum
		{
			SET_SHUT_DOWN,
			SET_RECEIVE_NETMSG,
			SET_RELOAD_CONFIG,
			SET_SEGMENT_HEAD,
			SET_SEGMENT_SEGMENT,
			SET_TRACE_LOG,

			SET_SERVICE_BASE_END,
		} EM_SERVICE_EVENT_TYPE;

		enum
		{
			ST_NETWORK,
			ST_MAIN_LOGIC,
			ST_ASYNC_LOAD_DB_PROXY,
			ST_ASYNC_LOAD_DB,
			ST_ASYNC_SAVE_DB_PROXY,
			ST_ASYNC_SAVE_DB,
			ST_CONSOLE_UI,
			ST_GROUP,
			ST_SERVICE_BASE_END,
		} EM_SERVICE_TYPE;
	}
}

#endif
