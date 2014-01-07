#ifndef __BASEFUNCTION_HEADER__
#define __BASEFUNCTION_HEADER__

#define BOOST_ALL_NO_LIB
#define BOOST_SYSTEM_NO_LIB
#define BOOST_DATE_TIME_NO_LIB

#include "tinyxml.h"
#include "constvar.h"

namespace gowinder
{

namespace baselib
{

extern int g_dwLogTypeFlag;

UINT64 get_sys_tick();

/// \author 风不息 gowinder@hotmail.com
/// \brief 将一个字节格式化成16进度,
/// \date [9/14/2009 ]
/// \details 注意rch的长度要大于先于3个
/// \return
void hex_2_char(char const* szHex, unsigned char* rch);

/// \author 风不息 gowinder@hotmail.com
/// \brief 格式化一个缓冲区为16进制
/// \date [9/14/2009 ]
/// \details 注意pszDesc的长度应该是pszSource的3倍再+1,也就是说nOutSize要>= nInSize * 3 + 1
/// \return	成功返回true
bool binary_2_hex(char const* pszSource, unsigned char* pszDesc, int nInSize,
		int nOutSize);

std::string get_md5(const char* pData, int nLen); //!<	get md5

std::string get_SHA1(const char* pData, int nLen);

////////////////////////////////// cast ////////////////////////////

int str2i(const char* pszChar);
int str2i(const std::string& str);
float str2f(const char* pszChar);
float str2f(const std::string& str);
double str2d(const char* pszChar);
double str2d(const std::string& str);

std::string i2str(int v);
std::string f2str(float v);
std::string d2str(double v);

///////////////////////////////// end cast ////////////////////////////////

////////////////////////////////////log/////////////////////////

/// \author 风不息 gowinder@hotmail.com
/// \brief 日志代理函数，只接受XML参数
/// \date [3/24/2010]
/// \details xml节点中的属性就是日志要记录的参数,注意有几个是必需的,类型和时间
void LogProxyFunc(const char* pszFileName, int nLogLevel,
		TiXmlElement* pElement, int nType);

/// \author 风不息 gowinder@hotmail.com
/// \brief 记录字符串日志
/// \date [3/24/2010 Administrator]
/// \param pszTitle 标题
/// \param pszContent 内容
/// \details
/// \return
void LogTextFunc(const char* pszFileName, int nLogLevel, const char* pszTitle, ...);

/// \author 风不息 gowinder@hotmail.com
/// \brief 记录异常日志
/// \date [3/25/2010]
/// \details
/// \return
void LogCatchFunc(const char* pszFileName, int nLogLevel, const char* pszTitle,	...);

/// \author 风不息 gowinder@hotmail.com
/// \brief 记录ASSERT日志
/// \date [3/25/2010]
/// \param pszTitle assert内容
/// \param pszAssertFile 发生的文件
/// \param nLine 行数
/// \details
/// \return
void LogAssertFunc(const char* pszFileName, int nLogLevel, const char* pszTitle, const char* pszFile, int nLine);

/// \author 风不息 gowinder@hotmail.com
/// \brief 记录CHECK, CHECKF日志
/// \date [3/25/2010]
/// \param pszTitle assert内容
/// \param pszFile 发生的文件
/// \param nLine 行数
/// \details
/// \return
void LogCheckFuncWithParam(const char* pszFileName, int nLogLevel,
		const char* pszTitle, const char* pszFile, int nLine,
		const char* pszTitleParam, ...);

/// \author 风不息 gowinder@hotmail.com
/// \brief 记录CHECK, CHECKF日志
/// \date [3/25/2010]
/// \param pszTitle assert内容
/// \param pszFile 发生的文件
/// \param nLine 行数
/// \details
/// \return
void LogCheckFunc(const char* pszFileName, int nLogLevel, const char* pszTitle,
		const char* pszFile, int nLine);

/// \author 风不息 gowinder@hotmail.com
/// \brief 记录IF_NOT, IF_OK日志
/// \date [3/25/2010]
/// \param pszTitle assert内容
/// \param pszFile 发生的文件
/// \param nLine 行数
/// \details
/// \return
void LogIfFunc(const char* pszFileName, int nLogLevel, const char* pszTitle,
		const char* pszFile, int nLine);

/// \author 风不息 gowinder@hotmail.com
/// \brief	记录删除异常
/// \date [3/25/2010]
/// \details
/// \return
void LogDeleteCatchFunc(const char* pszFileName, int nLogLevel,
		const char* pszTitle, const char* pszFile, int nLine);

///////////////////////////////end log////////////////////////////////

}
}
#endif

