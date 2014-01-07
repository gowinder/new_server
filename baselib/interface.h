#ifndef __INTERFACE__HEADER__
#define __INTERFACE__HEADER__

#include "basedef.h"
#include "struct.h"
#include <string>

namespace gowinder
{
namespace baselib
{

using namespace std;

/// \author 风不息 gowinder@hotmail.com
/// \brief  事件泵(为了与网络消息作区别,内部消息又称为"事件")
/// \date [4/7/2010]
/// \detail
class ievent_pump
{
public:
	virtual ~ievent_pump()
	{
	}

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 压入新的事件
	/// \date [4/7/2010]
	/// \detail
	/// \param nFrom	事件发送者ID
	/// \param nType 事件类型ID
	/// \param nSize 缓冲区大小
	/// \param pData 缓冲区指针
	/// \param nParam1 附加参数1
	/// \param nParam2 附加参数2
	/// \return nParam3 附加参数3
	virtual bool push(int nFrom, int nType, int nSize, const char* pData, int nParam1,
			int nParam2, int nParam3) = 0;

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 弹出最早的事件
	/// \date [4/7/2010]
	/// \detail
	/// \return
	virtual bool pop(ST_ADVANCE_EVENT* pMsg) = 0;

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 等待新的事件
	/// \date [4/7/2010]
	/// \detail
	/// \return
	virtual bool wait(DWORD dwMillSec) = 0;

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 开启事件泵
	/// \date [4/7/2010]
	/// \detail 如果不开启，事件泵不接受任何事件
	/// \return
	virtual bool open() = 0;

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 事件泵是否开启
	/// \date [4/7/2010]
	/// \detail
	/// \return
	virtual bool is_open() = 0;

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 关闭事件泵
	/// \date [4/7/2010]
	/// \detail
	/// \return
	virtual bool close() = 0;

	/// \author 风不息 gowinder@hotmail.com
	/// \brief 得当前事件泵中未处理事件的数量
	/// \date [4/7/2010]
	/// \detail
	/// \return
	virtual DWORD get_size() = 0;

// 	/// \author 风不息 gowinder@hotmail.com
// 	/// \brief 得竞争区
// 	/// \date [2012-7-10]
// 	virtual CCriticalSection*	GetCritialSection() = 0;
};

/// \author 风不息 gowinder@hotmail.com
/// \brief 网络消息分发器。
/// \date [4/7/2010]
/// \detail 网络引擎使用此接口分发接收到的网络消息到各个组件中去。需要实现自己的派生类。
/// \return
class inetmsg_dispatcher
{
public:
	/// \author 风不息 gowinder@hotmail.com
	/// \brief 分发网络消息
	/// \date [4/7/2010]
	/// \detail
	/// \param dwSocketIndex 网络引擎中收到此消息的SOCKET索引值，便于定位是哪个客户端发过来的消息。
	/// \param pData 消息指针
	/// \param dwSize 消息缓冲长度
	/// \param hSocket SOCKET句柄
	/// \param dwClientIP 发送消息的来客户端IP
	/// \return 是否分发成功
	virtual bool dispatch_msg(DWORD dwSocketIndex, BYTE* pData, DWORD dwSize,
			SOCKET hSocket, DWORD dwClientIP) = 0;
};

/// \author 风不息 gowinder@hotmail.com
/// \brief  UI日志监视器
/// \date [2012-5-9]
/// \detail
/// \return
class ilogger_tracer
{
public:
	virtual void trace(string& strLog, int nType, int nLevel) = 0;
};

extern ilogger_tracer* g_logger_tracer;

}

}

#endif
