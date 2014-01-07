#include "basefunction.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <cstring>
#include <strstream>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/sha1.hpp>
#include <boost/lexical_cast.hpp>

#include "interface.h"

namespace gowinder
{

	namespace baselib
	{

		UINT64 get_sys_tick()
		{
			boost::posix_time::ptime pt =
			        boost::posix_time::microsec_clock::local_time();

		}

		void hex_2_char(char const* szHex, unsigned char* rch)
		{
			int h = (int) (unsigned char) (*szHex) / 16;
			int l = (int) (unsigned char) (*szHex) % 16;
			if (h >= 0 && h <= 9)
				*rch = '0' + h;
			else
				*rch = 'A' + h - 10;

			if (l >= 0 && l <= 9)
				*(rch + 1) = '0' + l;
			else
				*(rch + 1) = 'A' + l - 10;

			*(rch + 2) = ' ';
		}

		bool binary_2_hex(char const* pszSource, unsigned char* pszDesc,
		        int nInSize, int nOutSize)
		{
			int i;
			if (nOutSize < nInSize * 3 + 1)
				return false;
			for (i = 0; i < nInSize; i++, pszSource++, pszDesc += 3)
			{
				hex_2_char(pszSource, pszDesc);
			}
			pszDesc++;
			pszDesc = 0;
			return true;
		}

		std::string get_md5(const char* pData, int nLen)
		{
			return "";
		}

		std::string get_SHA1(const char* pData, int nLen)
		{
			boost::uuids::detail::sha1 sa;
			sa.process_bytes(pData, nLen);
			unsigned int digest[5];
			sa.get_digest(digest);
			std::stringstream ss;
			for (int i = 0; i < 5; i++)
				ss << hex << digest[i];

			return ss.str();
		}

////////////////////////////////////////cast ///////////////////
		int str2i(const char* pszChar)
		{
			return boost::lexical_cast<int>(pszChar);
		}

		int str2i(const std::string& str)
		{
			return str2i(str.c_str());
		}

		float str2f(const char* pszChar)
		{
			return boost::lexical_cast<float>(pszChar);
		}

		float str2f(const std::string& str)
		{
			return str2f(str.c_str());
		}

		double str2d(const char* pszChar)
		{
			double dd = boost::lexical_cast<double>(pszChar);
			cout << dd << endl;
			return dd;
		}

		double str2d(const std::string& str)
		{
			return str2d(str.c_str());
		}

		std::string i2str(int v)
		{
			return boost::lexical_cast<std::string>(v);
		}

		std::string f2str(float v)
		{
			return boost::lexical_cast<std::string>(v);
		}

		std::string d2str(double v)
		{
//	return boost::lexical_cast<std::string>(v);
			strstream ss;
			ss << v;
			return ss.str();
		}

////////////////////////////////////////////cast end 	/////////

///////////////////////// log/////////////////////////////////

		void LogProxyFunc(const char* pszFileName, int nLogLevel,
		        TiXmlElement* pElement, int nType)
		{
			assert(pElement);

			time_t ltime;
			time(&ltime);
			tm pTime;
			localtime(&ltime);
			char buffer[TEMP_BUFF_SIZE];
			sprintf(buffer, "%d-%d-%d %d:%d:%d", pTime.tm_year + 1900,
			        (pTime.tm_mon + 1) % 12, pTime.tm_mday, pTime.tm_hour,
			        pTime.tm_min, pTime.tm_sec);

			pElement->SetAttribute("log_time", buffer);

			pElement->SetAttribute(XML_ELEMENT_NODE_ATT_LEVEL, nLogLevel);

			char szLogName[TEMP_BUFF_SIZE];
			sprintf(szLogName, "%s %d-%d.log", pszFileName,
			        (pTime.tm_mon + 1) % 12, pTime.tm_mday);

			FILE* fp = NULL;
			fp = fopen(szLogName, "a+");
			if (!fp)
				return;

			//	日志级别检查

			//	转发到界面
			if (true) //g_baseParam.bLogToUI)
			{
				std::string str;

				TiXmlPrinter printer;
				printer.SetStreamPrinting();
				pElement->Accept(&printer);
				str = printer.CStr();

				if (g_logger_tracer)
					g_logger_tracer->trace(str, nType, nLogLevel);
			}

			pElement->Print(fp, 0);

			fprintf(fp, "\r\n");

			fclose(fp);
		}

		void LogTextFunc(const char* pszFileName, int nLogLevel,
		        const char* pszTitle, ...)
		{
			assert(pszFileName);
			assert(pszTitle);

			va_list argptr;
			va_start(argptr, pszTitle);
//	char	buf[TEMP_BUFF_SIZE] = _T("");
			std::auto_ptr<char> aptChar(new char[TEMP_BUFF_SIZE_HUGE]);
			memset(aptChar.get(), 0, TEMP_BUFF_SIZE_HUGE);
			vsprintf(aptChar.get(), pszTitle, argptr);
			va_end(argptr);

			TiXmlElement element(XML_ELEMENT_NODE_NAME);

			element.SetAttribute(XML_ELEMENT_NODE_ATT_TYPE, EM_LOG_TYPE_TEXT);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TITLE, aptChar.get());

			LogProxyFunc(pszFileName, nLogLevel, &element, nLogLevel);
		}

		void LogCatchFunc(const char* pszFileName, int nLogLevel,
		        const char* pszTitle, ...)
		{
			assert(pszFileName);
			assert(pszTitle);

			if (g_dwLogTypeFlag && EM_LOG_TYPE_CATCH != EM_LOG_TYPE_CATCH)
				return;

			va_list argptr;
			va_start(argptr, pszTitle);
			char buf[TEMP_BUFF_SIZE] = "";
			vsprintf(buf, pszTitle, argptr);
			va_end(argptr);

			TiXmlElement element(XML_ELEMENT_NODE_NAME);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TYPE, EM_LOG_TYPE_CATCH);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TITLE, pszTitle);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_EXCEPTION_NAME, "");
			element.SetAttribute(XML_ELEMNET_NODE_ATT_EXCEPTION_ADDRESS, "");
			element.SetAttribute(XML_ELEMNET_NODE_ATT_EXCEPTION_STACK, "");
			element.SetAttribute(XML_ELEMNET_NODE_ATT_EXCEPTION_PARAM, buf);

			LogProxyFunc(pszFileName, nLogLevel, &element, nLogLevel);
		}

		void LogAssertFunc(const char* pszFileName, int nLogLevel,
		        const char* pszTitle, const char* pszFile, int nLine)
		{
			assert(pszFileName);
			assert(pszTitle);

			if (g_dwLogTypeFlag && EM_LOG_TYPE_ASSERT != EM_LOG_TYPE_ASSERT)
				return;

			TiXmlElement element(XML_ELEMENT_NODE_NAME);	// 新建一个XML节点
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TYPE, EM_LOG_TYPE_ASSERT);//	加入一个节点类型属性
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TITLE, pszTitle);	//	加入一个节点的名称
			element.SetAttribute(XML_ELEMNET_NODE_ATT_FILE, pszFile);//	加入一个断言发生的文件名
			element.SetAttribute(XML_ELEMNET_NODE_ATT_LINE, nLine);	//	加入一个断言发生的行数

			LogProxyFunc(pszFileName, nLogLevel, &element, nLogLevel);//	调用日志代理函数
		}
		void LogCheckFunc(const char* pszFileName, int nLogLevel,
		        const char* pszTitle, const char* pszFile, int nLine)
		{
			assert(pszFileName);
			assert(pszTitle);

			if (g_dwLogTypeFlag && EM_LOG_TYPE_CHECK != EM_LOG_TYPE_CHECK)
				return;

			TiXmlElement element(XML_ELEMENT_NODE_NAME);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TYPE, EM_LOG_TYPE_CHECK);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TITLE, pszTitle);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_FILE, pszFile);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_LINE, nLine);

			LogProxyFunc(pszFileName, nLogLevel, &element, nLogLevel);
		}

		char buf[TEMP_BUFF_SIZE] = "";
		char buf1[TEMP_BUFF_SIZE] = "";
		void LogCheckFuncWithParam(const char* pszFileName, int nLogLevel,
		        const char* pszTitle, const char* pszFile, int nLine,
		        const char* pszTitleParam, ...)
		{
			assert(pszFileName);
			assert(pszTitle);

			if (g_dwLogTypeFlag && EM_LOG_TYPE_CHECK != EM_LOG_TYPE_CHECK)
				return;

			va_list argptr;
			va_start(argptr, pszTitleParam);
			//char	buf[TEMP_BUFF_SIZE] = _T("");
			vsprintf(buf, pszTitleParam, argptr);
			va_end(argptr);

			//char	buf1[TEMP_BUFF_SIZE] = _T("");
			sprintf(buf1, "%s %s", pszTitle, buf);

			TiXmlElement element(XML_ELEMENT_NODE_NAME);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TYPE, EM_LOG_TYPE_CHECK);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TITLE, buf1);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_FILE, pszFile);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_LINE, nLine);

			LogProxyFunc(pszFileName, nLogLevel, &element, nLogLevel);
		}

		void LogIfFunc(const char* pszFileName, int nLogLevel,
		        const char* pszTitle, const char* pszFile, int nLine)
		{
			assert(pszFileName);
			assert(pszTitle);

			if (g_dwLogTypeFlag && EM_LOG_TYPE_IF != EM_LOG_TYPE_IF)
				return;

			TiXmlElement element(XML_ELEMENT_NODE_NAME);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TYPE, EM_LOG_TYPE_IF);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TITLE, pszTitle);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_FILE, pszFile);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_LINE, nLine);

			LogProxyFunc(pszFileName, nLogLevel, &element, nLogLevel);
		}

		void LogDeleteCatchFunc(const char* pszFileName, int nLogLevel,
		        const char* pszTitle, const char* pszFile, int nLine)
		{
			assert(pszFileName);
			assert(pszTitle);

			if (g_dwLogTypeFlag
			        && EM_LOG_TYPE_DELETECATCH != EM_LOG_TYPE_DELETECATCH)
				return;

			TiXmlElement element(XML_ELEMENT_NODE_NAME);

			element.SetAttribute(XML_ELEMENT_NODE_ATT_TYPE,
			        EM_LOG_TYPE_DELETECATCH);
			element.SetAttribute(XML_ELEMENT_NODE_ATT_TITLE, pszTitle);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_FILE, pszFile);
			element.SetAttribute(XML_ELEMNET_NODE_ATT_LINE, nLine);

			LogProxyFunc(pszFileName, nLogLevel, &element, nLogLevel);
		}

	}

}
