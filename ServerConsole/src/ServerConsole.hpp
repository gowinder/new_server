//============================================================================
// Name        : ServerConsole.cpp
// Author      : gowinder
// Version     :
// Copyright   : Just for fun!!!
// Description : Hello World in C++, Ansi-style
//============================================================================
#define BOOST_ALL_NO_LIB
#define BOOST_SYSTEM_NO_LIB
#define BOOST_DATE_TIME_NO_LIB
#include <boost/config/warning_disable.hpp>

#include <iostream>
#include <boost/timer.hpp>
#include <boost/progress.hpp>
#include <boost/date_time/gregorian/greg_date.hpp>
#include <boost/date_time/gregorian/greg_month.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_parsing.hpp>
#include <boost/smart_ptr.hpp>

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/sha1.hpp>

#include <boost/current_function.hpp>

#include "interface.h"
#include "basefunction.h"

using namespace std;
using namespace boost::gregorian;
using namespace boost::posix_time;
using namespace boost;
using namespace boost::uuids;
using namespace boost::detail;

int main()
{
	boost::timer t;
	cout << "max " << t.elapsed_max() / 3600 << " h" << endl;
	cout << "min " << t.elapsed_min() << endl;
	cout << "now ,Hello World, then The World!" << endl; // prints now ,Hello World, then The World!

	boost::progress_timer	pt;

	date d(2013, 10, 25);
//	date d_now(day_clock::local_day);l
//	cout << day_clock::local_day << endl;
	cout << to_simple_string(d) << endl;

	time_duration td(2, 15, 23);
	cout << "td: " << to_simple_string(td) << endl;

	string str_td("02:13:24");
	ptime pt_temp(d);
	pt_temp += time_duration(14, 56, 00);
	cout << "iso extended td1: " << to_iso_extended_string(pt_temp) << endl;
	cout << "iso td1: " << to_iso_string(pt_temp) << endl;

	ptime pt_local = second_clock::local_time();
	ptime pt_micro_local = microsec_clock::universal_time();
	cout << "pt local " << to_simple_string(pt_local) << endl;
	cout << "pt mlocal " << to_simple_string(pt_micro_local) << endl;

	time_duration td_from_ptime = pt_local.time_of_day();
	date date_from_ptime = pt_local.date();
	//	output YY-mm-dd HH:MM:SS
	cout << "pt formated " << to_iso_extended_string(date_from_ptime) << " " << to_simple_string(td_from_ptime) << endl;

	ptime pt_start(date(2013, 10, 24), time_duration(15, 22, 00));
	time_duration td_delta = second_clock::local_time() - pt_start;
	cout << "td_delta " << to_simple_string(td_delta) << endl;

	uuid u;
	random_generator rgen;
	u = rgen();
	cout << "random uuid " << u << endl;

	string strSHA1 = "mather fucker";
	boost::uuids::detail::sha1 sa;
	sa.process_bytes(strSHA1.c_str(), strSHA1.length());
	unsigned int digest[5];
	sa.get_digest(digest);
	for(int i = 0; i < 5; i++)
		cout << hex << digest[i];
	cout << endl;

	cout << BOOST_CURRENT_FUNCTION << endl;
	cout << dec << endl;

	string str1 = "1234";
	cout << "fuck " << GetSHA1(str1.c_str(), str1.length()) << endl;
	int x = str2i("1234");
	double ddds = boost::lexical_cast<double>("24.25");
	cout << "ddds " << ddds << endl;
	string strzz = "23.22";
	double dddd = str2d(strzz);
	string ssasaf = d2str(223.5);
	cout << "castd " << dddd << endl;
	cout << "cast " << str2i("1234") << endl;
	cout << "recast" << ssasaf << endl;

//	ptime pt_now = second_clock::local_time();
//	string str_pt_now = to_extended_iso_string(pt_now, ' ');
//	ptime pt_temp1;
//	pt_temp1 = parse_iso_extended_time<ptime>(str_pt_now, ' ');
//	cout << "now " << to_iso_string(pt_temp1) << endl;

	try
	{
		int i = 0;
		int b = 222 / i;
		int c = b;
	}
	catch(std::exception& ex)
	{
		cout << ex.what() << endl;
	}
//	boost::progress_display pd(100);
//	for(int i = 0; i < 100; i++)
//	{
//		sleep(1);
//		++pd;
//	}

	return 0;
}

class TestLoggerTracer: public ILoggerTracer
{
public:
	virtual void Trace(string& strLog, int nType, int nLevel);
};

void	TestLoggerTracer::Trace(string& strLog, int nType, int nLevel)
{
	cout << strLog << endl;
}


ILoggerTracer* g_pILoggerTracer = new TestLoggerTracer;
