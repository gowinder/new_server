/*
 * servicemanager.h
 *
 *  Created on: Nov 8, 2013
 *      Author: go
 */

#ifndef SERVICEMANAGER_H_
#define SERVICEMANAGER_H_

namespace gowinder
{
	namespace service
	{
		class service;
		class service_manager
		{
		public:
			service_manager();
			virtual ~service_manager();
		};

	} /* namespace service */
} /* namespace gowinder */

#endif /* SERVICEMANAGER_H_ */
