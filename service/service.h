/*
 * servicebase.h
 *
 *  Created on: Nov 5, 2013
 *      Author: go
 */

#ifndef SERVICEBASE_H_
#define SERVICEBASE_H_

#include "baselib.h"
#include "service_thread.h"
#include "netmsg.h"
#include "idictionarymanager.h"
#include "segmentbufferholder.h"

namespace gowinder
{
	namespace service
	{
		using namespace gowinder::baselib;

		const int THREAD_STANDARD_REPETITIONRUN_INTERVAL = 50;
		class service_manager;

		class service
		{
		public:
			service(OBJID service_id, OBJID type, DWORD go_tick_interval =
			        THREAD_STANDARD_REPETITIONRUN_INTERVAL);
			virtual ~service(void);

			/// \brief 推送内部事件
			/// \author gowinder
			/// \param nFrom 发出ID
			/// \param nType 事件类型
			///	\param pData 数据缓冲区
			/// \param nSize 缓冲区大小
			virtual bool post_event(int from, int type, int size,
			        const char* data, int param1, int param2, int param3);

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  得服务ID
			/// \date [2012-28-8]
			/// \detail
			/// \return
			inline OBJID get_id()
			{
				return _service_id;
			}

			idictionary_manager* GetDictionaryManager()
			{
				return _dict_manager;
			}

			inline void set_service_manager(service_manager* manager)
			{
				_service_manager = manager;
			}
			inline service_manager* get_service_manager()
			{
				return _service_manager;
			}

			bool init();
			bool service_start();

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  更新服务指针
			/// \date [2012-6-9]
			/// \detail
			/// \return
			virtual void update_service()
			{
			}
			;

			bool process_event_pump(DWORD interval_tick);
			void set_netmsg_builder(inetmsg_builder* builder)
			{
				_netmsg_builder = builder;
			}

			bool maintain();

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  返回服务类型
			/// \date [2012-22-8]
			/// \detail
			/// \return
			inline OBJID get_type()
			{
				return _type;
			}

			inline OBJID get_additional_id()
			{
				return _addtion_id;
			}

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  是否初始化
			/// \date [2012-8-10]
			/// \detail
			/// \return
			bool is_open()
			{
				return _is_open;
			}
		protected:

			virtual service_thread<service>* create_thread();

			/// \author gowinder
			/// \brief  处理客户端发来的网络消息
			/// \date [7/30/2010]
			virtual bool process_client_netmsg(DWORD socket_index,
			        const char* data, const DWORD size, DWORD ip);

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 处理事件
			/// \date [4/20/2010]
			/// \detail 派生类可在此函数中处理自己的事件
			/// \param bProcessed 如果在派生类中处理了此事件,此参数设置为true,否则设置为false,留给基类处理
			virtual bool on_process_event_pump(ST_ADVANCE_EVENT& event,
			        bool& processed)
			{
				return true;
			}

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 当引擎启动时调用
			/// \date [4/20/2010]
			virtual bool on_service_start() = 0;

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 在引擎关闭时调用
			/// \date [4/20/2010]
			virtual bool on_service_stop()
			{
				return true;
			}

			/// \author gowinder
			/// \brief  维护
			/// \date [6/8/2010]
			/// \detail 派生类重载后使用，也可以不重载
			virtual bool on_maintain()
			{
				return true;
			}

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 保存全部
			/// \date [4/20/2010]
			/// \detail 一般在关闭引擎时调用,保存全部内存数据到数据库
			virtual bool save_all()
			{
				return true;
			}

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  外部不能直接停止,需要调用 SendStopServiceEvent
			/// \date [2012-4-9]
			/// \detail
			/// \return
			bool service_stop();

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  得队列中还有的数量
			/// \date [2012-27-9]
			/// \detail
			/// \return
			int get_event_count_in_pump()
			{
				CHECKF(_event_pump);
				return _event_pump->get_size();
			}

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  更新线程监控数据
			/// \date [8/20/2013]
			/// \detail
			/// \return
			virtual void UpdateThreadMonitor()
			{
			}
			;

			virtual bool	on_init()=0;

		private:
			internal_event_pump* _event_pump;
		protected:
			friend class service_thread<service>;
			gowinder::service::service_thread<service>* _thread;	//!<  	主线程

			int _list_index;

			inetmsg_builder* _netmsg_builder;		//!<	消息构造器
			OBJID _service_id;	//!< 	引擎编号
			bool _is_open;	//!< 	引擎是否启动
			OBJID _type;	//!<	 服务类型
			OBJID _addtion_id;	//!<	附加ID,网络及数据库服务使用
			idictionary_manager* _dict_manager;
			bool _is_initialized;	//!<	是否已经初始化
			service_manager* _service_manager;
			DWORD _thread_go_tick_interval;
			char* _event_data_temp;
			bool _exit_thread_flag;
			segment_buffer_holder	_segment_buffer_holder;
		};

	} /* namespace service_base */
} /* namespace gowinder */
#endif /* SERVICEBASE_H_ */
