#ifndef __STRUCT_HEADER__
#define __STRUCT_HEADER__


#include "constvar.h"
#include "basedef.h"

#include <map>
#include <string>
#include <vector>

namespace gowinder
{
namespace baselib
{

using namespace std;
using namespace gowinder::baselib;

const	DWORD	EVENT_BUFFER_SIZE = 4096;
const	DWORD IP_STRING_LENGHT = 16;
const	DWORD	MD5_32_STRING_LENGTH = 33;

/// \author 风不息 gowinder@hotmail.com
/// \brief  内部消息(事件)结构
/// \date [4/20/2010]
struct ST_EVENT
{
public:
	int from;		//!< 事件发出引擎ID
	int type;		//!< 事件ID
	int size;		//!< 附加缓冲区大小
	int param1;		//!< 附加参数11
	int param2;		//!< 附加参数2
	int param3;		//!< 附加参数3
	BYTE data[EVENT_BUFFER_SIZE];	//!< 附加缓冲区
};

struct ST_ADVANCE_EVENT_DATA
{
	int size;	//!< 附加缓存区大小
	int data_size;	//!<	缓存区实际存储的数据大小
	char*	data;	//!< 附加缓存区指针
};

struct ST_ADVANCE_EVENT
{
public:
	int from;		//!< 事件发出引擎ID
	int type;		//!< 事件ID
	int param1;		//!< 附加参数1
	int param2;		//!< 附加参数2
	int param3;		//!< 附加参数3
	ST_ADVANCE_EVENT_DATA	data;	//!<	缓存
};

enum	EM_NET_MSG_END_FLAG
{
	EF_NORMAL = 0,
	EF_FIRST = 1,
	EF_LAST = 2,
};


const	DWORD	SOCKET_BROADCAST_MAX_SOCKET_COUNT = 250;
/// \author 风不息 gowinder@hotmail.com
/// \brief  广播用ID组
/// \date [4/20/2010]
#pragma pack(push)
#pragma pack(1)
typedef struct _ST_SOCKET_BRAODCAST_INFO
{
	int		count;
	DWORD	socket_index[SOCKET_BROADCAST_MAX_SOCKET_COUNT];
}ST_SOCKET_BRAODCAST_INFO;
#pragma pack(pop)

/// \author 风不息 gowinder@hotmail.com
/// \brief 基本参数
/// \date [8/22/2009 ]
#pragma pack(push)
#pragma pack(1)
typedef struct _ST_BASE_PARAM
{
	bool	bLogToDisk;			//!< 是否写入日志到磁盘
	bool	bLogToUI;				//!< 是否发送到介面,注意,bLogToDisk=true时,此参数才有效果
	bool	bEnableDirectWarArmyDataLog;	//!< 是否记录战场兵种战斗数值
	char		szAccountServerIP[IP_STRING_LENGHT];	//!< 	账号服务器IP
	USHORT usAccountServerPort;	//!< 	账号服务器端口
	char		szBackupAccountServerIP[IP_STRING_LENGHT];	//!< 	备份用账号服务器
	USHORT usBackupAccountServerPort;	//!< 	备份用账号服务器端口
	DWORD	dwServerIndex;	//!< 游戏服务器索引
	char		szAccountServerLoginPwd[gowinder::baselib::ACCOUNT_PWD_STR_COUNT];	//!< 登录账号服务器所需要的密码
	char		szServerKey[SERVER_KEY_STR_COUNT];		//!< 一键登录时的服务器密钥

	char		szDBServer[DATABASE_PARAMETER_LEGTH];//!< 数据库地址
	char		szDBDataBase[DATABASE_PARAMETER_LEGTH];//!< 数据库名
	char		szDBName[DATABASE_PARAMETER_LEGTH];//!< 数据库用户名
	char		szDBPassword[DATABASE_PARAMETER_LEGTH];//!< 数据库用户密码

	char		szDBLogServer[DATABASE_PARAMETER_LEGTH];//!< 日志数据库地址
	char		szDBLogDataBase[DATABASE_PARAMETER_LEGTH];//!< 日志数据库名
	char		szDBLogName[DATABASE_PARAMETER_LEGTH];//!< 日志数据库用户名
	char		szDBLogPassword[DATABASE_PARAMETER_LEGTH];//!< 日志数据库用户密码

	bool		bEnableAccountServer;	//!<	是否开启账号服务器连接
	bool		bEnableNormalLogin;	//!<	是否开启正常登录
	bool		bEnableCooperatorLogin;	//!<	是否开启运营商直接登录

	DWORD		dwProcedureDeadLockRetryCount;	//!<	存储过程执行锁死后重试次数

}ST_BASE_PARAM;
#pragma pack(pop)

extern ST_BASE_PARAM	g_baseParam;



/// \author 风不息 gowinder@hotmail.com
/// \brief 连接属性
/// \date [9/16/2009 ]
typedef struct _ST_SOCKET_EVENT_CONNECT_INFO
{
	bool	use_config;	//!<	是否使用配置表中的IP，如果使用，本结构中的地址将无效
	char	ip[IP_STRING_LENGHT];							//!< 服务器地址
	DWORD 	port;				//!< 端口
	DWORD	connection_id;	//!< 连接ID
	DWORD	socket_index;	//!< SOCKET索引号
}ST_SOCKET_EVENT_CONNECT_INFO;



typedef struct _ST_SOCKET_REGISTER_INFO
{
	OBJID account_id;		//!<	账号ID
	DWORD tag;		//!<	标志
}ST_SOCKET_REGISTER_INFO;

/// \author 风不息 gowinder@hotmail.com
/// \brief  数据对象字段联合
/// \date [2012-24-9]
/// \detail
/// \return
typedef union
{
	unsigned	int _uValue;
	int _nValue;
	char*	_pszValue;
	time_t	_tValue;
}GAME_DATA_FIELD;

//////////////////////////////////////////////////////////////////////////


///////////////////////////////////容器
typedef vector<OBJID>	VECT_OBJID;
typedef map<OBJID, OBJID>	MAP_OBJID;

typedef vector<int>	VECT_INT;


}

}



#endif
