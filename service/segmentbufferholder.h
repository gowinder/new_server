/*
 * segmentbufferholder.h
 *
 *  Created on: Nov 8, 2013
 *      Author: gowinder@hotmail.com
 */

#ifndef SEGMENTBUFFERHOLDER_H_
#define SEGMENTBUFFERHOLDER_H_

namespace gowinder
{
	namespace service
	{

		enum EM_SEGMENT_BUFFER_HOLDER_STATUS
		{
			SBHS_FREE,	//!<	空闲
			SBHS_USING,	//!<	在接收中,未接收完成
			SBHS_DONE,	//!<	全部分片段接收完成
		};

		/// \author 风不息 gowinder@hotmail.com
		/// \brief  分割事件缓冲头
		/// \date [9/24/2013]
		/// \detail
		/// \return
		struct ST_SERVICE_EVENT_SEGMENT_HEADER
		{
			int original_type;	//!<	原始事件类型
			int original_size;		//!<	原始事件缓冲长度
			int segment_count;	//!<	分割后的片段数量
		};

		class service;
		class segment_buffer_holder
		{
		public:
			segment_buffer_holder(service* service);
			virtual ~segment_buffer_holder();

			inline EM_SEGMENT_BUFFER_HOLDER_STATUS get_status()
			{
				return _status;
			}

			inline int get_p1()
			{
				return _p1;
			}
			inline int get_p2()
			{
				return _p2;
			}
			inline int get_p3()
			{
				return _p3;
			}
			inline const ST_SERVICE_EVENT_SEGMENT_HEADER& get_header()
			{
				return _header;
			}
			;
			inline int get_data_size()
			{
				return _data_size;
			}	//!<	得缓冲区内存占用大小,可能会比实际数据大

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  设置头
			/// \date [9/24/2013]
			/// \param bForce 是否强制设置头,如果为true否认为SBHS_FREE状态都会重置
			/// \detail
			/// \return
			bool set_header(ST_SERVICE_EVENT_SEGMENT_HEADER* header, int p1,
			        int p2, int p3, bool force = false);

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  , int nP1, int nP2, int nP3,
			/// \date [9/24/2013]
			/// \detail
			/// \return
			bool add_segment(const char* data, int size);

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  取出数据
			/// \date [9/24/2013]
			/// \detail 注意取出后,状态由SBHS_DONE变成SBHS_FREE
			/// \return
			const char* get_data(int& size);
		protected:
			inline void set_status(EM_SEGMENT_BUFFER_HOLDER_STATUS status)
			{
				_status = status;
			}
		protected:
			EM_SEGMENT_BUFFER_HOLDER_STATUS _status;
			ST_SERVICE_EVENT_SEGMENT_HEADER _header;
			char* _data;
			int _data_size;
			int _data_received_offset;
			int _received_segment_count;
			service* _service;
			int _p1;
			int _p2;
			int _p3;
		};

	} /* namespace service */
} /* namespace gowinder */

#endif /* SEGMENTBUFFERHOLDER_H_ */
