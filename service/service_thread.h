#ifndef ___SERVICETHREAD_HEADER___
#define ___SERVICETHREAD_HEEADER___

#include "baselib.h"

namespace gowinder
{
	namespace service
	{

		using namespace gowinder::baselib;

/// \author gowinder
/// \brief  通用服务模板
/// \date [7/30/2010]
		template<class T>
		class service_thread : public thread_base
		{
		public:
			service_thread(T* service);
			~service_thread();

			virtual bool init_thread();
		protected:
			T* _service;
		public:
			virtual bool repeat_run(DWORD internal_tick);
		};

		template<class T>
		service_thread<T>::service_thread(T* service):_service(service)
		{
		}

		template<class T>
		service_thread<T>::~service_thread()
		{
		}

		template<class T>
		bool service_thread<T>::repeat_run(DWORD internal_tick)
		{
			CHECKF(_service);

			DEBUG_TRY
			if(_service->is_open())
			_service->maintain();
			DEBUG_CATCH("service_thread::repetition_run-_service->maintain()");

			DEBUG_TRY
			bool exit_flag = false;
			if(_service->is_open())
			_service->process_event_pump(internal_tick);

			DEBUG_CATCH("service_thread::repetition_run-_service->process_event_pump()");

			if(_service->_exit_thread_flag)
			return false;

			return true;
		}

		template<class T>
		bool service_thread<T>::init_thread()
		{
			CHECKF(_service);

			return true;
		}

	}
}
#endif
