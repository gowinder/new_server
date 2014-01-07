/*
 * mysqldatasource.cpp
 *
 *  Created on: Nov 7, 2013
 *      Author: go
 */

#include "mysqldatasource.h"
#include <boost/lexical_cast.hpp>

namespace gowinder
{
	namespace datasource
	{
		using namespace gowinder::baselib;

		mysql_datasource::mysql_datasource() :
				_retry_count(0), _db(NULL), _connected(false)
		{
			_db = NULL;
		}

		mysql_datasource::~mysql_datasource()
		{
			close();
		}

		bool mysql_datasource::connect(std::string host, short port,
		        std::string database_name, std::string user_name,
		        std::string password)
		{
			try
			{
				/*cout << "fuck " << endl;
				 if (is_connected())
				 {
				 return true;
				 }

				 MYSQL* db = mysql_init(NULL);
				 my_bool b = mysql_thread_init();
				 if(b != 0)
				 {
				 cout << "error " << mysql_error(db) << endl;
				 return false;
				 }

				 const string char_set = "utf8";
				 if(mysql_options(db, MYSQL_SET_CHARSET_NAME, char_set.c_str()) != 0)
				 return false;

				 if (!mysql_real_connect(db, host.c_str(), user_name.c_str(),password.c_str(), database_name.c_str(), port, NULL, CLIENT_MULTI_STATEMENTS))
				 {
				 cout << "error: " << mysql_error(db) << endl;
				 return false;
				 }*/

				_db = mysql_init(NULL);
				if (mysql_thread_init() != 0)
					return false;

				const string char_set = "utf8";
				if (mysql_options(_db, MYSQL_SET_CHARSET_NAME, char_set.c_str())
				        != 0)
					return false;

				if (!mysql_real_connect(_db, host.c_str(), user_name.c_str(),
				        password.c_str(), database_name.c_str(), port, NULL,
				        CLIENT_MULTI_STATEMENTS))
				{
					cout << "error: " << mysql_error(_db) << endl;
					return false;
				}

				_connected = true;
				return true;
			} catch (...)
			{
				std::cout << "connect crashed" << endl;
			}

		}

		void mysql_datasource::close()
		{
			if (_db)
			{
				mysql_close(_db);
				_db = NULL;
			}

		}

		shared_record_set mysql_datasource::get_recordset_from_db(
		        std::string sql)
		{
			shared_record_set rs;

			if (!is_connected())
				return rs;

			if (mysql_query(_db, sql.c_str()) != NULL)
			{
				cout << "error: " << mysql_error(_db) << endl;
				return rs;
			}

			MYSQL_RES * res = mysql_store_result(_db);

			unsigned int field_count = mysql_num_fields(res);
			unsigned int row_count = mysql_num_rows(res);
			MYSQL_FIELD * fields = mysql_fetch_field(res);

			rs = shared_record_set(new record_set);

			shared_vector_record_rows rows(new std::vector<RECORD_ROW>);
			rs->set_rs(rows);
			rs->set_fields_count(field_count);
			shared_array_fields_types fields_types(new int[field_count]);
			rs->set_shared_array_fields_types(fields_types);
			//	assign field_type array
			for (int i = 0; i < field_count; i++)
			{
				fields_types[i] = fields[i].type;
			}

			//	assign recordset data
			for (int i = 0; i < row_count; i++)
			{
				mysql_data_seek(res, i);
				RECORD_ROW row(new game_data_field[field_count]);
				MYSQL_ROW mysql_row = mysql_fetch_row(res);
				for (int j = 0; j < field_count; j++)
				{
					fetch_row_field(fields, j, mysql_row, row);
				}
				rows->push_back(row);
			}

			mysql_free_result(res);

			return rs;
		}

		bool mysql_datasource::execute_sql(std::string sql)
		{
			if (!is_connected())
				return false;

			if (mysql_query(_db, sql.c_str()) != NULL)
			{
				LOGERROR(DB_LOG_FILE, EM_LOG_HIGH,
				        "mysql_datasource::execute_sql:%s-%s-%d", sql.c_str(),
				        mysql_error(_db), mysql_errno(_db));

				return false;
			}
			else
			{
				do
				{
					MYSQL_RES * result = mysql_store_result(_db);
					int affect_rows = mysql_affected_rows(_db);
					mysql_free_result(result);
				} while (!mysql_next_result(_db));

				return true;
			}
		}
		INT64 mysql_datasource::get_insert_id()
		{
			if (_db)
				return mysql_insert_id(_db);
			else
				return 0;

			/*shared_record_set rs = this->get_recordset_from_db(
			        "SELECT LAST_INSERT_ID()");
			if (rs->get_rs().get() == NULL)
				return 0;
			if (rs->get_rs().get()->size() < 1)
				return 0;

			return boost::get<INT64>(rs->get_rs().get()->at(0)[0]);*/
		}

		int mysql_datasource::release_old_record()
		{
			if (!is_connected())
				return 0;
			int count_to_release = 0;
			DEBUG_TRY
				{
					do
					{
						MYSQL_RES* result = mysql_store_result(_db);
						if (result != NULL)
							mysql_free_result(result);
						count_to_release++;
					} while (!mysql_next_result(_db));
				}DEBUG_CATCH("catch mysql_datasource::release_old_record()");

			return count_to_release;
		}

		shared_record_set mysql_datasource::execute_store_procedure_one(
		        string sql, bool force_transaction, int& error_code)
		{
			//	只支持一个返回记录集
			shared_record_set rs;
			DEBUG_TRY
				{
					if (force_transaction)
						execute_sql("start transaction;");

//				SafeCopy(m_szLastExecuteStoreProcedure, szSql, SQL_QUERY_SENTENCE_LENGTH);
					rs = get_recordset_from_db(sql);

					if (rs.get() == NULL)
					{
						if (force_transaction)
							execute_sql("rollback;");
						error_code = mysql_errno(_db);
						return rs;
					}
					else //if (rs->at(0)[0] != RETURN_OK)
					{
						RECORD_ROW row = rs->get_rs()->at(0);
						if (boost::get<int>(row[0]) != RETURN_OK)
							;
						{
							if (force_transaction)
								execute_sql("rollback;");
							error_code = mysql_errno(_db);
							return rs;
						}
					}

					release_old_record(); // 调用存储过程,除了存储过程中返回的结果,还会返回一个存储过程的状态结果,如果不清除掉,调用COMMIT,就会出现不同步的异常
					if (force_transaction)
						execute_sql("commit;");

					return rs;
				}DEBUG_CATCH2("mysql_datasource::execute_store_procedure_one '%s'", sql.c_str());

		}

		shared_record_set mysql_datasource::execute_store_procedure(
		        std::string sql, bool force_transaction)
		{
			int error_code = 0;
			int retry_count = 0;
			shared_record_set rs;
			while (retry_count < _retry_count)
			{
				error_code = 0;
				rs = execute_store_procedure_one(sql, force_transaction,
				        error_code);
				if (error_code == ER_LOCK_DEADLOCK)
				{
					LOGERROR(LOG_DB_DEADLOCK_FILE, EM_LOG_NORMAL,
					        "CMyDatabase::ExecuteStoreProcedure %s get deadlock, retry=%d",
					        sql.c_str(), retry_count);
					retry_count++;
				}
				else
					return rs;
			}

			execute_sql("start transaction;");

//			SafeCopy(m_szLastExecuteStoreProcedure, szSql,
//			        SQL_QUERY_SENTENCE_LENGTH);
			rs = get_recordset_from_db(sql);

			if (rs.get() == NULL)
			{

				execute_sql("rollback;");
				return rs;
			}
			else //if (boost::get<int>(rs.get()[0]) != RETURN_OK)
			{
				RECORD_ROW row = rs->get_rs()->at(0);
				if (boost::get<int>(row[0]) != RETURN_OK)
				{
					release_old_record();
					execute_sql("rollback;");
					return rs;
				}
			}

			release_old_record(); // 调用存储过程,除了存储过程中返回的结果,还会返回一个存储过程的状态结果,如果不清除掉,调用COMMIT,就会出现不同步的异常
			execute_sql("commit;");

			return rs;
		}

		void mysql_datasource::fetch_row_field(MYSQL_FIELD* fields, int i,
		        MYSQL_ROW& mysql_row, RECORD_ROW& row)
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
			case FIELD_TYPE_LONGLONG:
				row[i] = boost::lexical_cast<INT64>((char*) mysql_row[i]);
				break;
			default:
				std::cout
				        << "mysql_datasource::fetch_row_field unsupport field type "
				        << fields[i].type << endl;
			}
		}

	} /* namespace datasource */
} /* namespace gowinder */
