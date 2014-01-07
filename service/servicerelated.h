/*
 * servicerelated.h
 *
 *  Created on: Nov 5, 2013
 *      Author: go
 */

#ifndef SERVICERELATED_H_
#define SERVICERELATED_H_

namespace gowinder
{
  namespace service
  {

    class service;
    class service_related
    {
    public:
      service_related(service* service);
      virtual
      ~service_related();
      inline service* get_service()      {return _service;}
    protected:
      service*     _service;
    };

  } /* namespace service_base */
} /* namespace gowinder */
#endif /* SERVICERELATED_H_ */
