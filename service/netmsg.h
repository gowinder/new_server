#ifndef ___NETMSG_HEADER___
#define ___NETMSG_HEADER___

#include "baselib.h"

namespace gowinder
{
	namespace service
	{

		using namespace gowinder::baselib;

		/// \author 风不息 gowinder@hotmail.com
		/// \brief  网络消息头
		/// \date [4/20/2010]
		/// \detail 在每个网络消息上都有一个固定的头结构,共16个字节
		/// \addgroup net_message
#pragma pack(push)
#pragma pack(1)
		typedef struct _NET_MSG_HEAD
		{
			UINT size;		//!< 	包大小(包括包头)
			UINT type;		//!< 	包类型
			UINT end_flag;	//!< 	网络消息结束标志位,参见EM_NET_MSG_END_FLAG
			DWORD msg_number;	//!< 网络消息序号,回送给客户端的消息要和发送到服务器的消息的序号相同,广播消息序号为0
		} NET_MSG_HEAD;
#pragma pack(pop)

		class service_base;
		/// \author 风不息 gowinder@hotmail.com
		/// \brief  网络消息接口
		/// \date [2012-29-8]
		/// \detail
		/// \return
		class inetmsg
		{
		public:
			virtual ~inetmsg()
			{
			}
			virtual bool init_msg(DWORD socket_index, const char* data,
			        const DWORD size, DWORD ip) = 0;

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 得消息缓冲区指针,就是要发送到网络上的消息
			/// \date [4/20/2010]
			virtual char* GetBuf(void) = 0;

			/// \author 风不息 gowinder@hotmail.com
			/// \brief  得消息缓冲长度
			/// \date [4/20/2010]
			virtual unsigned int get_size() = 0;

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 得消息类型
			/// \date [4/20/2010]
			virtual unsigned int get_type() = 0;

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 得接收到的消息SOCKET索引
			/// \date [4/20/2010]
			virtual DWORD get_socket_index() = 0;

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 处理消息
			/// \date [4/9/2010]
			virtual bool process() = 0;
		};

		/// \author 风不息 gowinder@hotmail.com
		/// \brief 消息构造器接口
		/// \date [4/20/2010]
		class inetmsg_builder
		{
		public:
			virtual ~inetmsg_builder()
			{
			}

			/// \author 风不息 gowinder@hotmail.com
			/// \brief 创建网络消息
			/// \detail 接收到网络数据流后,按一定的规则生成消息
			/// \date [4/9/2010]
			/// \param dwSocketIndex 索引
			/// \param idMsg 消息类型
			/// \param pbufMsg 缓冲区指针
			/// \param dwMsgSize 缓冲区长度
			/// \param dwIP 客户端IP
			virtual inetmsg* create_netmsg(DWORD socket_index, OBJID msg_id,
			        const char* buffer, DWORD msg_size, DWORD ip) = 0;
		};

	}
}

#endif
