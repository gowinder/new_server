//============================================================================
// Name        : test_database.cpp
// Author      : gowinder
// Version     :
// Copyright   : Just for fun!!!
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <vector>
using namespace std;

#include <mysql/mysql.h>
#include <boost/smart_ptr.hpp>
#include <boost/variant.hpp>

#include "baselib.h"
#include "mysqldatasource.h"
/*
typedef boost::shared_ptr<std::vector<char> > shared_ptr_string;

typedef boost::variant<unsigned int, int, std::string, time_t, float,
        boost::int64_t, double> game_data_field;

typedef boost::shared_array<game_data_field> RECORD_ROW;

typedef boost::shared_ptr<std::vector<RECORD_ROW> > shared_record_set;*/

gowinder::baselib::ilogger_tracer* gowinder::baselib::g_logger_tracer = NULL;

typedef union
{
	unsigned int _uValue;
	int _nValue;
	char* _pszValue;
	time_t _tValue;
	float _fValue;
	double _dValue;
} GAME_DATA_FIELD;

typedef GAME_DATA_FIELD* DATA_ROW;

typedef vector<DATA_ROW> DATA_ROW_VECTOR;

using namespace gowinder::baselib;
using namespace gowinder::datasource;

void fetch_row_field(MYSQL_FIELD* fields, int i, MYSQL_ROW& mysql_row,
        RECORD_ROW& row)
{
	if (mysql_row[i] == 0)
	{
		row[i] = 0;
		return;
	}
	switch (fields[i].type)
	{
	case FIELD_TYPE_TINY:
	case FIELD_TYPE_SHORT:
		row[i] = atoi(mysql_row[i]);
		break;
	case FIELD_TYPE_LONG:
		row[i] = atol(mysql_row[i]);
		break;
	case FIELD_TYPE_FLOAT:
	{
		row[i] = (float) atof(mysql_row[i]);
	}
		break;
	case FIELD_TYPE_DOUBLE:
	{
		row[i] = atof(mysql_row[i]);
	}
		break;
		/*		case FIELD_TYPE_LONGLONG:
		 {
		 row[i] = _atoi64(mysql_row[i]);
		 }
		 break;*/
	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_NEWDECIMAL:
	{
		row[i] = atoi(mysql_row[i]);
	}
		break;
		//////////////////////////////////////////////////////////////////////////
	case FIELD_TYPE_STRING:
	case FIELD_TYPE_VAR_STRING:
	case FIELD_TYPE_DATETIME:
	case FIELD_TYPE_DATE:
		row[i] = (char*) mysql_row[i];
		break;
		/*	case FIELD_TYPE_BLOB:
		 {
		 memcpy(row[i].m_szVal, mysql_row[i], row[i].FieldLen());
		 }
		 break;*/
	}
}

shared_record_set get_recordset_from_db(std::string sql)
{
	shared_record_set rs;

	MYSQL* db = mysql_init(NULL);

	mysql_thread_init();

	if (db == NULL)
	{
		return rs;
	}

	char DB_CHARSET[] = "utf8";
	mysql_options(db, MYSQL_SET_CHARSET_NAME, DB_CHARSET);

	if (mysql_real_connect(db, "192.168.96.77", "root", "asdf", "test", 3307, 0,
	CLIENT_MULTI_STATEMENTS) == NULL)
	{
		cout << "error: " << mysql_error(db) << endl;
		return rs;
	}

	if (mysql_query(db, sql.c_str()) != NULL)
	{
		cout << "error: " << mysql_error(db) << endl;
		return rs;
	}
	else
	{
		MYSQL_RES * res = mysql_store_result(db);

		unsigned int field_count = mysql_num_fields(res);
		unsigned int row_count = mysql_num_rows(res);
		MYSQL_FIELD * fields = mysql_fetch_field(res);

		rs = shared_record_set(new gowinder::datasource::record_set);

		for (int i = 0; i < row_count; i++)
		{
			mysql_data_seek(res, i);
			RECORD_ROW row(new game_data_field[field_count]);
			MYSQL_ROW mysql_row = mysql_fetch_row(res);
			for (int j = 0; j < field_count; j++)
			{
				fetch_row_field(fields, j, mysql_row, row);
			}
			rs->get_rs()->push_back(row);
		}

		mysql_free_result(res);
	}

	mysql_close(db);

	return rs;
}

/*void fetch_row_field(MYSQL_FIELD* fields, int i, MYSQL_ROW& mysql_row,
        DATA_ROW& row)
{
	if (mysql_row[i] == 0)
	{
		row[i]._nValue = 0;
		return;
	}
	switch (fields[i].type)
	{
	case FIELD_TYPE_TINY:
	case FIELD_TYPE_SHORT:
		row[i]._nValue = atoi(mysql_row[i]);
		break;
	case FIELD_TYPE_LONG:
		row[i]._nValue = atol(mysql_row[i]);
		break;
	case FIELD_TYPE_FLOAT:
	{
		row[i]._fValue = (float) atof(mysql_row[i]);
	}
		break;
	case FIELD_TYPE_DOUBLE:
	{
		row[i]._dValue = atof(mysql_row[i]);
	}
		break;
				case FIELD_TYPE_LONGLONG:
		 {
		 row[i] = _atoi64(mysql_row[i]);
		 }
		 break;
	case FIELD_TYPE_DECIMAL:
	case FIELD_TYPE_NEWDECIMAL:
	{
		row[i]._nValue = atoi(mysql_row[i]);
	}
		break;
		//////////////////////////////////////////////////////////////////////////
	case FIELD_TYPE_STRING:
	case FIELD_TYPE_VAR_STRING:
	case FIELD_TYPE_DATETIME:
	case FIELD_TYPE_DATE:
		int len = fields[i].length;
		row[i]._pszValue = new char[len + 1];
		strncpy(row[i]._pszValue, (char*) mysql_row[i], len);
		break;
			case FIELD_TYPE_BLOB:
		 {
		 memcpy(row[i].m_szVal, mysql_row[i], row[i].FieldLen());
		 }
		 break;
	}
}

DATA_ROW_VECTOR* get_recordset_from_db_(std::string sql)
{
	DATA_ROW_VECTOR* rs = new DATA_ROW_VECTOR;

	MYSQL* db = mysql_init(NULL);

	mysql_thread_init();

	if (db == NULL)
	{
		return rs;
	}

	char DB_CHARSET[] = "utf8";
	mysql_options(db, MYSQL_SET_CHARSET_NAME, DB_CHARSET);

	if (mysql_real_connect(db, "192.168.96.77", "root", "asdf", "test", 3307, 0,
	CLIENT_MULTI_STATEMENTS) == NULL)
	{
		cout << "error: " << mysql_error(db) << endl;
		return rs;
	}

	if (mysql_query(db, sql.c_str()) != NULL)
	{
		cout << "error: " << mysql_error(db) << endl;
		return rs;
	}
	else
	{
		MYSQL_RES * res = mysql_store_result(db);

		unsigned int field_count = mysql_num_fields(res);
		unsigned int row_count = mysql_num_rows(res);
		MYSQL_FIELD * fields = mysql_fetch_field(res);

//		rs = shared_record_set(new std::vector<RECORD_ROW>);

		for (int i = 0; i < row_count; i++)
		{
			mysql_data_seek(res, i);
			//	RECORD_ROW row(new game_data_field[field_count]);
			DATA_ROW row = new GAME_DATA_FIELD[field_count];
			MYSQL_ROW mysql_row = mysql_fetch_row(res);
			for (int j = 0; j < field_count; j++)
			{
				fetch_row_field(fields, j, mysql_row, row);
			}
			rs->push_back(row);
		}

		mysql_free_result(res);
	}

	mysql_close(db);

	return rs;
}*/

int main()
{
	using namespace gowinder::baselib;
	using namespace gowinder::datasource;
	using namespace std;
	if (true)
	{
		mysql_datasource db;
		bool success = db.connect("192.168.96.77", 3307, "test", "root", "asdf");
		if(success)
		{
			bool success = db.execute_sql("insert into test set test = 8, test2 = 'asdf'");
			if(success)
			{
				unsigned int id = db.get_insert_id();
				cout << id << endl;
			}


			shared_record_set rs = db.get_recordset_from_db("select * from account limit 10000");
			cout << rs->get_rs()->size() << endl;
		}

		db.close();
	}
	else
	{
		/*if (true)
		{
			shared_record_set rs = get_recordset_from_db(
			        "select * from account limit 10000");

			for (int i = 0; i < rs->size(); i++)
			{
				cout << "row ";
				RECORD_ROW row = rs->at(i);
				for (int j = 0; j < 14; j++)
				{
					cout << row[j] << "\t";
				}

				cout << endl;
			}

		}
		else
		{
			//shared_record_set rs = get_recordset_from_db("select * from account limit 2000");
			DATA_ROW_VECTOR* rs = get_recordset_from_db_(
			        "select * from account limit 10000");

			for (int i = 0; i < rs->size(); i++)
			{
				cout << "row ";
				DATA_ROW row = rs->at(i);
				for (int j = 0; j < 14; j++)
				{
					cout << row[j]._nValue << "\t";
				}

				cout << endl;
			}
		}*/
	}

	cout << "now ,Hello World, then The World!" << endl; // prints now ,Hello World, then The World!
	return 0;
}
