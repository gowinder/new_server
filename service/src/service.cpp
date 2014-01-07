/*
 * servicebase.cpp
 *
 *  Created on: Nov 5, 2013
 *      Author: go
 */

#include "service.h"
#include "servicemanager.h"
#include "service_define.h"

namespace gowinder
{
	namespace service
	{
		service::service(OBJID service_id, OBJID type, DWORD go_tick_interval):
		_service_id(service_id),_type(type),_netmsg_builder(NULL),
		_is_initialized(false),_is_open(false)
		,_thread(NULL),_thread_go_tick_interval(go_tick_interval)
		, _event_data_temp(NULL)
		,_service_manager(NULL)
		,_addtion_id(0)
		,_dict_manager(NULL)
		,_list_index(0)
		,_exit_thread_flag(false)
		,_segment_buffer_holder(this)
		{
			_event_pump = new internal_event_pump(service_id);

			_event_data_temp = new char[MAX_ADV_EVENT_DATA_SIZE];
		}

		service::~service(void)
		{
			SAFE_DELETE(_thread);
			SAFE_DELETE(_event_pump);
			SAFE_DELETE(_event_data_temp);
		}

		bool service::post_event(int from, int type, int size, const char* data,
		        int param1, int param2, int param3)
		{
			CHECKF(_event_pump);

			CHECKP3F(_event_pump->is_open(), "from:%d,to%d,type:%d", from,
			        get_id(), type);

			if(size > MAX_ADV_EVENT_DATA_SIZE)
			{
				//	大于最大数据缓冲块的长度就分割
				gowinder::service::ST_SERVICE_EVENT_SEGMENT_HEADER header;
				header.original_type = type;
				header.original_size = size;
				int segment_count = (int)(float)ceil((float)size / MAX_ADV_EVENT_DATA_SIZE);
				header.segment_count = segment_count;
				CHECKP2F(_event_pump->push(from, SET_SEGMENT_HEAD, sizeof(ST_SERVICE_EVENT_SEGMENT_HEADER), (BYTE*)&header, param1, param2, param3),
						"type:%d, segment-header, buffer size:%d", type, size);
				for (int i = 0; i < segment_count; i++)
				{
					int offset = i * MAX_ADV_EVENT_DATA_SIZE;
					int left = size - offset;
					int size_to_copy = left >= MAX_ADV_EVENT_DATA_SIZE ? MAX_ADV_EVENT_DATA_SIZE : left;
					CHECKP2F(_event_pump->push(from, SET_SEGMENT_SEGMENT, size_to_copy, (const char*)(data + offset), param1, param2, param3),
							"type:%d, segment-seg, segment index:%d", type, i);
				}
			}
			else
			CHECKP2F(_event_pump->push(from, type, size, data, param1, param2, param3), "to%d,type:%d", get_id(), type);

			return true;
		}

		service_thread<service>* service::create_thread()
		{
			return new service_thread<service>(this);
		}

		bool service::process_client_netmsg(DWORD socket_index,
		        const char* data, const DWORD size, DWORD ip)
		{
			NET_MSG_HEAD* netmsg_head = (NET_MSG_HEAD*) data;
			inetmsg* netmsg = NULL;

			int size_ = 0;
			int type_ = 0;
			DEBUG_TRY
				{
					size_ = netmsg_head->size;
					type_ = netmsg_head->type;

					CHECKF(netmsg_head->size == size);

					netmsg = _netmsg_builder->create_netmsg(socket_index,
					        netmsg_head->type, data, size, ip);

					if (!netmsg)
						return true;
					ASSERT(netmsg->process());

				}DEBUG_CATCH3("service::process_client_netmsg msgType(%d) size(%d)", type_, size);
			SAFE_DELETE(netmsg);

			return true;
		}

		bool service::service_start()
		{
			if (_is_open)
				return false;

			//	初始化随机数
			srand(unsigned(time(NULL)));

			CHECKF(init());

			if (_event_pump)
				CHECKF(_event_pump->open());

			CHECKF(on_service_start());

			_is_open = true;

			_exit_thread_flag = false;

			return true;
		}
		bool service::service_stop()
		{
			_is_open = false;

			if (_event_pump)
				CHECKF(_event_pump->close());

			CHECKF(on_service_stop());

			_exit_thread_flag = true;

			_is_open = false;

			return true;
		}

		bool service::init()
		{
			if (_is_initialized)
				return true;

			//	线程
			_thread = create_thread();
			CHECKF(_thread);
			_thread->set_repetition_run_interval(_thread_go_tick_interval);

			CHECKF(_thread->init_thread());

			CHECKF(on_init());

			_is_initialized = true;

			return true;
		}

		bool service::process_event_pump(DWORD interval_tick)
		{
			//	引擎已经关闭
			if (!_is_open)
			{
				return false;
			}

			//	接收事件
			DWORD event_count = 0;	//	本次处理的事件总数
			ST_ADVANCE_EVENT internal_event;
			memset(&internal_event, 0, sizeof(ST_ADVANCE_EVENT));
			BYTE eventData[MAX_ADV_EVENT_DATA_SIZE];
			internal_event.data.data = _event_data_temp;
			internal_event.data.data_size = 0;
			internal_event.data.size = MAX_ADV_EVENT_DATA_SIZE;

			//__int64	tick64 = ::GetSysTick();
			DWORD tick64 = get_sys_tick();
			while (_event_pump->pop(&internal_event))
			{
				DEBUG_TRY

					DWORD dwTickStart = get_sys_tick();
					bool bProcessed = false;
					on_process_event_pump(internal_event, bProcessed);
					if (bProcessed)
						continue;

					switch (internal_event.type)
					{
					case SET_SHUT_DOWN:
					{
						//	关闭网络引擎
						//	ServiceStop();
						OBJID id = get_id();
						save_all();
						service_stop();
						//	TRACE("close service set event %d\n", id);
						_exit_thread_flag = true;
						return true;
					}
						break;
					case SET_RECEIVE_NETMSG:
					{
						//	向某个客户端发送消息
						process_client_netmsg(internal_event.param1,
						        internal_event.data.data,
						        internal_event.data.data_size,
						        internal_event.param3);
					}
						break;
					case SET_RELOAD_CONFIG:
					{
					}
						break;
					case SET_SEGMENT_HEAD:
					{
						//	分割头
						if (_segment_buffer_holder.get_status() != SBHS_FREE)
						{
							LOGERROR(LOG_FILE, EM_LOG_HIGH,
							        "KERNAL_SEGMENT_HEAD new segment header before previous end");
							break;
						}
						CHECKF(
						        _segment_buffer_holder.set_header(
						                (ST_SERVICE_EVENT_SEGMENT_HEADER* )internal_event.data.data,
						                internal_event.param1,
						                internal_event.param2,
						                internal_event.param3));
					}
						break;
					case SET_SEGMENT_SEGMENT:
					{
						//	分割数据
						CHECKF(
						        _segment_buffer_holder.add_segment(
						                internal_event.data.data,
						                internal_event.data.data_size));
						if (_segment_buffer_holder.get_status() == SBHS_DONE)
						{
							//	收完了
							ST_ADVANCE_EVENT the_event;
							the_event.param1 = _segment_buffer_holder.get_p1();
							the_event.param2 = _segment_buffer_holder.get_p2();
							the_event.param3 = _segment_buffer_holder.get_p3();
							the_event.type =
							        _segment_buffer_holder.get_header().original_type;
							the_event.data.data =
							        (char*) _segment_buffer_holder.get_data(
							                the_event.data.data_size);
							the_event.data.size =
							        _segment_buffer_holder.get_data_size();
							std::cout << "segment count:%d\n" << _segment_buffer_holder.get_header().segment_count << std::endl;
							DEBUG_TRY
								{
									if (the_event.type == SET_RECEIVE_NETMSG)
										process_client_netmsg(the_event.param1,
										        the_event.data.data,
										        the_event.data.data_size,
										        the_event.param3);
									else
										on_process_event_pump(the_event,
										        bProcessed);
								}DEBUG_CATCH2("KERNAL_SEGMENT_SEGMENT on_process_event_pump, type:%d", the_event.type);

						}
					}
						break;
					default:
					{
						LOGERROR(LOG_FILE, EM_LOG_HIGH,
						        "Unsupport service(%d) event :%d",
						        _is_open, internal_event.type);
					}
						break;
					}
					event_count++;


				DEBUG_CATCH3("service::ProcessMsgPump type %d, service(%d)", internal_event.type, get_id())

				}

				int wait = get_sys_tick() - tick64;
				wait = wait > interval_tick || wait < 0 ?
				        0 : (interval_tick - wait);
				if (wait > 50)
					std::cout << "wait = %d\n" << wait;

				bool bResult = _event_pump->wait(wait);
#ifdef _DEBUG
				if(bResult == false)
				{
					int i = 0;
				}
#endif
				return bResult;
			}

			bool service::maintain()
			{
				if (_is_open)
				{
					CHECKF(on_maintain());
				}

				return true;
			}
		} /* namespace service */
	} /* namespace gowinder */
