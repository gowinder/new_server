/*
 * idictionarymanager.h
 *
 *  Created on: Nov 5, 2013
 *      Author: go
 */

#ifndef IDICTIONARYMANAGER_H_
#define IDICTIONARYMANAGER_H_

#include "servicerelated.h"
namespace gowinder
{
  namespace service
  {

	  class service;
    class idictionary_manager : public service_related
    {
    public:
      idictionary_manager(service* service);
      virtual
      ~idictionary_manager();
    };

  } /* namespace service_base */
} /* namespace gowinder */
#endif /* IDICTIONARYMANAGER_H_ */
