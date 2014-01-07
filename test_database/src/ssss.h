/*
 * ssss.h
 *
 *  Created on: Nov 11, 2013
 *      Author: go,gowinder@hotmail.com
 */

#ifndef SSSS_H_
#define SSSS_H_

#include <interface.h>

namespace gowinder
{
	namespace baselib
	{

		class ssss: public gowinder::baselib::ievent_pump
		{
		public:
			ssss();
			virtual ~ssss();
		};

	} /* namespace baselib */
} /* namespace gowinder */

#endif /* SSSS_H_ */
