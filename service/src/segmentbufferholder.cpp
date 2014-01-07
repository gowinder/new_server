/*
 * segmentbufferholder.cpp
 *
 *  Created on: Nov 8, 2013
 *      Author: go
 */

#include <segmentbufferholder.h>
#include "service.h"
namespace gowinder
{
	namespace service
	{

		segment_buffer_holder::segment_buffer_holder(service* service):
				_status(SBHS_FREE), _data(NULL), _data_size(0), _received_segment_count(
				        0), _data_received_offset(0), _service(service), _p1(0), _p2(
				        0), _p3(0)
		{
			ASSERT(_service);
			memset(&_header, 0, sizeof(ST_SERVICE_EVENT_SEGMENT_HEADER));
		}

		segment_buffer_holder::~segment_buffer_holder()
		{
			if(_data)
				SAFE_DELETE(_data);
		}


		bool		segment_buffer_holder::set_header(ST_SERVICE_EVENT_SEGMENT_HEADER* header, int p1,
		        int p2, int p3, bool force)
		{
			CHECKF(header);
			if(get_status() != SBHS_FREE && !force)
			{
				LOGERROR(LOG_FILE, EM_LOG_HIGH, "CSegmentBufferHolder::SetHeader set new header when status=%d, event type=%d, new event type=%d",
						get_status(), _header.original_type, header->original_type);
				return false;
			}
			memcpy(&_header, header, sizeof(ST_SERVICE_EVENT_SEGMENT_HEADER));

			if(_data == NULL)
			{
				_data = new BYTE[_header.original_size];
				_data_size = _header.original_size;
			}
			else
			{
				if(_data_size < _header.original_size)
				{
					SAFE_DELETE(_data);
					_data = new BYTE[_header.original_size];
					_data_size = _header.original_size;
				}
			}
			CHECKF(_data != NULL);

			_received_segment_count = 0;
			_data_received_offset = 0;

			_p1 = p1;
			_p2 = p2;
			_p3 = p3;

			set_status(SBHS_USING);

			return true;
		}

		bool		segment_buffer_holder::add_segment(const char* data, int size)
		{
			CHECKP2F(get_status() == SBHS_USING, "received segment:%d,event type:%d", _received_segment_count, _header.original_type);
			CHECKF(_data);
			CHECKF(data);
			CHECKF(_data_size - _data_received_offset >= size);

			memcpy(_data + _data_received_offset, data, size);

			_data_received_offset += size;
			_received_segment_count++;

			if(_received_segment_count == _header.segment_count)
			{
				//	收完了
				CHECKF(_data_received_offset == _header.original_size);
				set_status(SBHS_DONE);
			}

			return true;
		}

		const char*	segment_buffer_holder::get_data(int& size)
		{
			set_status(SBHS_FREE);
			size = _header.original_size;
			return _data;
		}

	} /* namespace service */
} /* namespace gowinder */
