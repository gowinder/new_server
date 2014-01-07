#ifndef ___CONSTVAR_HEADER___
#define ___CONSTVAR_HEADER___


#if !defined(__WINDOWS__)
typedef unsigned int DWORD;
typedef bool BOOL;
typedef char BYTE;
typedef unsigned short WORD;
typedef char* LPSTR;
typedef const char* LPCSTR;
typedef long long INT64;
typedef unsigned long long UINT64;
typedef unsigned short USHORT;
typedef unsigned int	SOCKET;
typedef long LONG;
typedef void*	LPVOID;
typedef unsigned int UINT;
typedef unsigned long ULONG;

#define TRUE true
#define FALSE false

#define STRICMP strcasecmp
#define ATOI64	atoll
#define ATOUI64 strtoull

#else	//	 windows
#define STRICMP stricmp
#define ATOI64 _atoi64
#define ATOUI64 _strtoui64
#endif

namespace gowinder
{
namespace baselib
{

const DWORD DATABASE_SAVE_FAILED = 900000000;	//!< 	��ݿⱣ��ʧ�ܴ���?
const DWORD DATABASE_LOAD_FAILED = 900000001;	//!< 	��ݿ��ȡʧ�ܴ���
const DWORD PARAMERTER_INVALID = 900000002;	//!< 	����ָ�����?
const DWORD DATABASE_PROCEDURE_EXCEPTION = 900000005;	//!<	�洢�����?
const DWORD CRASH_EXCEPTION = 900000006;	//!<	һ�����쳣

#define		BUFSIZE		1024
#define		INDEX_NONE	-1
#define		ID_NONE 0
#define		RETURN_OK 0
#define		RETURN_CRASH 900000006
#define		RETURN_NULL_REF 1000	//!<	��ָ��
#define		RETURN_CHECK_FAILED 1001	//!<	checkʧ��
#define		RETURN_CREATE_NEW 1002	//!<	�����¶���ʧ��
#define		RETURN_INPUT_INVALIDE_PARAMETER  1003	//!<	����������
const char CONFIG_FILE_NAME[] = "server_config/gameconfig.xml";
const DWORD NET_MSG_BUFFER_SIZE = 2048 * 256;
const DWORD DATE_TIME_STR_LENGTH = 20;
const DWORD DEBUG_LOG_SIZE = 1024 * 10;	//	��־������?

const DWORD ROLE_EXPLORE_HOTPOINT_COUNT = 255;	//���̽�������ȵ��¼����

const DWORD ACCOUNT_NAME_STR_COUNT = 30;
const DWORD ACCOUNT_PWD_STR_COUNT = 33;
const DWORD SERVER_KEY_STR_COUNT = 40;

const DWORD DATABASE_PARAMETER_LEGTH = 30;	//!< ��ݿ����ӣ��û������볤��?

typedef unsigned long OBJID;

#define _USE_UTF8
#ifdef _USE_UTF8
const DWORD CHINESE_CHAR_SIZE = 3;	//	���ʹ��UTF8����,�����ַ�ռλ3���ֽ�
#else
const DWORD CHINESE_CHAR_SIZE = 2;	//	����,��ͨ��,�����ַ�ռλ2���ֽ�
#endif

#ifdef UNICODE
#define TIMESTR _T("%Y-%m-%d %H:%M:%S")
#else
#define TIMESTR "%Y-%m-%d %H:%M:%S"
#endif

#define	TEMP_BUFF_SIZE 1024 * 8
#define TEMP_BUFF_SIZE_HUGE 1024 * 32

#define LOG_FILE	"log/server_log"
#define LOG_CLIENT	"log/client_log"
#define LOG_SOCKET	"log/socket_log"
#define LOG_TIMEOUT_FILE "log/server_timeout"
#define LOG_ITEM "log/item"
#define LOG_DICT "log/dict"
#define LOG_MISSION "log/mission"
#define LOG_INIT "log/init"
#define LOG_WORLD_BOSS "log/world_boss"
#define DB_LOG_FILE "log/dberror"
#define DB_EXECUTE_FAILED_LOG_FILE "log/db_execute_failed"
#define DB_EXECUTE_LOG "log/db_execute"
#define LOG_BATTLE_COPY "log/battle_copy"
#define LOG_RANK "log/rank"
#define LOG_MAKE_COIN "log/make_coin"
#define LOG_WORLD_TIMER "log/world_timer"

#define XML_ELEMENT_NODE_NAME "event"
#define XML_ELEMENT_NODE_ATT_TYPE "type"
#define XML_ELEMENT_NODE_ATT_LEVEL  "level"
#define XML_ELEMENT_NODE_ATT_TITLE "title"
#define XML_ELEMNET_NODE_ATT_CONTENT "content"
#define XML_ELEMNET_NODE_ATT_EXCEPTION_NAME "name"
#define XML_ELEMNET_NODE_ATT_EXCEPTION_ADDRESS "address"
#define XML_ELEMNET_NODE_ATT_EXCEPTION_STACK "stack"
#define XML_ELEMNET_NODE_ATT_EXCEPTION_PARAM "param"
#define XML_ELEMNET_NODE_ATT_FILE "file"
#define XML_ELEMNET_NODE_ATT_LINE "line"

#define LOG_DB_DEADLOCK_FILE "log/db_deadlock"
#define LOG_DB_LOST_CONNECTION_FILE "log/db_lostconnection"

/// \brief �ʼ��Ƿ��Ѷ�
enum {
	MAIL_ISNOT_READ = 0, //!< δ��
	MAIL_IS_READ = 1, //!< 	�Ѷ�
};

/// \brief Ĭ���ʼ�����
enum EM_DEFAULT_MAIL_TYPE {
	MT_SYSTEM = 0,			//!< 	ϵͳ��
	MT_USER = 1,				//!< 	�����?
	MT_ALL = 0xffffffff,		//!<	ȫ���ʼ�������ʱʹ�ã�����Ϊ�������ͣ�
};

typedef enum _EM_ABNORMAL_STATE {
	EM_ABNORMAL_STATE_BAND = 1,	//!< ���?
	EM_ABNORMAL_STATE_FORBID = 2,		//!< ����
} EM_ABNORMAL_STATE;

//////////////////////////////////////////////////////////////////////////
/// \brief �ʼ�����
const DWORD MAIL_SENDER_LENGTH = 35;		//�ʼ��շ�
const DWORD MAIL_TITLE_LENGTH = 91;		//�ʼ����ⳤ��

const int MAIL_CONTENT_LIMIT_SYSTEM = 200;		//ϵͳ�ʼ�����
const int MAIL_CONTENT_LIMIT_UNION = 100;		//�����ʼ�����
const int MAIL_CONTENT_LIMIT_USER = 200;		//����ʼ�����?
const DWORD MAIL_CONTENT_LENGTH = 901;		//�ʼ�����
const DWORD NOTICE_CONTENT_LENGTH = 512;	//�������ݳ���
const DWORD MAIL_CONTENT_LIMIT_ITEM_ARRAY = 300;	//!<	ϵͳ�ʼ��е����б��ַ�ĳ���?

const DWORD USER_ACCOUNT_LENGTH = 65;	//	����˺����ռλ�ֽڳ���
const DWORD PASSWORD_LENGTH = 33;	//	���볤��

const DWORD CONST_HOTPOINT_NAME_LENGTH = 15;	//!< �ȵ���?5���֣�
const DWORD CONST_FOURCE_NAME_LENGTH = 21;	//!< �ȵ���?7���֣�
const DWORD CONST_ROLE_NAME_LENGTH = 24;	//!< �ȵ���?8���֣�

/// \author �粻Ϣ gowinder@hotmail.com
/// \brief  ��־����
/// \date [3/24/2010]
/// \details
/// \return
typedef enum _EM_LOG_TYPE	//!<	��־����
{
	EM_LOG_TYPE_TEXT = 1,				//!<	�ı�
	EM_LOG_TYPE_CATCH = 2,			//!<	�쳣
	EM_LOG_TYPE_ASSERT = 3,			//!<	����
	EM_LOG_TYPE_CHECK = 4,			//!<	���?
	EM_LOG_TYPE_IF = 5,					//!<	���?
	EM_LOG_TYPE_DELETECATCH = 6,					//!<	ɾ���쳣
	EM_LOG_TYPE_MAKE_ROLE = 7,	//!<	������־
	EM_LOG_TYPE_TIME = 8,				//!<	ʱ�����?
} EM_LOG_TYPE;

/// \author �粻Ϣ gowinder@hotmail.com
/// \brief ��־����
/// \date [3/25/2010]
/// \details
/// \return
enum _EM_LOG_LEVEL {
	EM_LOG_LOW = 1,				//!< DEBUG��
	EM_LOG_NORMAL = 2,				//!< DEBUG��
	EM_LOG_HIGH = 3,				//!< �����¼���?
	EM_LOG_EMERGENCY = 4,				//!< �����ģ��쳣����
};

/// \author �粻Ϣ gowinder@hotmail.com
/// \brief �ں˷�����
/// \date [4/12/2010]
typedef enum _EM_KERNAL_ID {
	KI_UI = 0,				//!< ����
	KI_MAINLOGIC,		//!< ���߼�
	KI_CHARGE_FEE,	//!<	��ֵ
	KI_SERVER_NETWORK,			//!< ������������
	KI_CLIENT_NETWORK,			//!< ��Ϊ�ͻ��������˺ŷ������ķ���
	KI_GROUP,				//!< 	�����?��Ұ�?���˰�,֮���?
	KI_AYSN_SAVE_DATABASE_PROXY,		//!<	��ݿ���·���
	KI_AYSN_LOAD_DATABASE_PROXY,		//!<	��ݿ��������
	KI_CHAT,
	KI_RUN_TRADE,
	KI_BATTLE,
	KI_CHAT_DATA_DISK_LOG,
	KI_WORLD_TIMER,	//!<	����ʱ��ά������
	KI_KERNAL_NORMAL_ID_END,
	KI_ASYN_SAVE_DATABASE_BEGIN = 1000,	//!<	�첽�洢��ݿ����ʼ��ʶ
	KI_ASYN_LOAD_DATABASE_BEGIN = 1100,	//!<	�첽������ݿ����ʼ��ʶ
} EM_KERNAL_ID;

typedef enum _EM_SERVICE_TYPE {
	ST_UI,	//!<	 ����
	ST_NETWORK,	//!<	����
	ST_MAINLOGIC,	//!<	 ���߼�ֻ����һ��
	ST_GROUP,	//������
	ST_CHARGE,	//!<	��ֵ
	ST_ASYN_SAVE_DB_PROXY,	//!<	��ݿ�洢�ܴ���
	ST_ASYN_SAVE_DB,	//!<	��ݿ�洢
	ST_ASYN_LOAD_DB_PROXY,	//!<	�첽������ݴ���?
	ST_ASYN_LOAD_DB,	//!<	�첽������ݷ���?
	ST_BASE_END,
	ST_OTHER = 1000,
} EM_SERVICE_TYPE;

const DWORD EM_USER_SERVICE_ID = 6;

/// \author �粻Ϣ gowinder@hotmail.com
/// \brief  �˺ŵ�¼��֤״̬
/// \date [4/7/2010]
typedef enum _EM_SOCKET_REGISTER_TYPE {
	EM_SOCKET_REGISTER_TYPE_INVALIDATE = 0,	//!< 	δ��֤
	EM_SOCKET_REGISTER_TYPE_VALIDATE = 1,	//!< 	�Ѿ���֤(������֤ͨ����δ������Ϸ,����״̬)
	EM_SOCKET_REGISTER_TYPE_INGAME = 2,	//!< 	�Ѿ���֤������Ϸ��
} EM_SOCKET_REGISTER_TYPE;

/// \author �粻Ϣ gowinder@hotmail.com
/// \brief  ��ݶ����������
/// \date [2012-24-9]
/// \detail
/// \return
enum EM_GAME_DATA_EDIT_TYPE {
	GDET_NONE = 0, GDET_UPDATE = 1, GDET_INSERT = 2, GDET_DELETE = 3,
};

/// \author �粻Ϣ gowinder@hotmail.com
/// \brief  ��ݶ����ֶ�����?
/// \date [2012-24-9]
/// \detail
/// \return
enum EM_GAME_DATA_FIELD_TYPE {
	GDFT_OBJID = 1,
	GDFT_INT = 2,
	GDFT_CHAR = 3,
	GDFT_VARCHAR = 4,
	GDFT_DATETIME = 5,
	GDFT_TIME_T = 6,
};

}
}


#endif
