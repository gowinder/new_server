/*
 * mysqldatasource.h
 *
 *  Created on: Nov 7, 2013
 *      Author: go
 */

#ifndef MYSQLDATASOURCE_H_
#define MYSQLDATASOURCE_H_

#include "baselib.h"
#include "datasource.h"
#include <mysql/mysql.h>
#include <mysql/mysqld_error.h>

namespace gowinder
{
	namespace datasource
	{

		class mysql_datasource: public idatasource
		{
		public:
			mysql_datasource();
			virtual ~mysql_datasource();

			virtual bool connect(std::string host, short port, std::string database_name, std::string user_name, std::string password);
			virtual void	close();
			virtual shared_record_set get_recordset_from_db(std::string sql);

			virtual bool execute_sql(std::string sql);
			virtual INT64 get_insert_id();
			virtual shared_record_set execute_store_procedure(std::string sql, bool force_transaction = false);
			virtual void			set_retry_count(int count) {_retry_count = count;}
			virtual int			get_retry_count()	{return _retry_count;}
		protected:
			void fetch_row_field(MYSQL_FIELD* fields, int i, MYSQL_ROW& mysql_row, RECORD_ROW& row);
			shared_record_set execute_store_procedure_one(string sql, bool force_transaction, int& error_code);
			int release_old_record();
			inline	bool	is_connected() {return _connected;}
		private:
			int	_retry_count;
			MYSQL* _db;
			bool _connected;
		};

	} /* namespace datasource */
} /* namespace gowinder */

#endif /* MYSQLDATASOURCE_H_ */
