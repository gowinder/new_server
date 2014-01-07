//	database interface

#ifndef ___DATASOURCEINTERFACE_HEADER___
#define ___DATASOURCEINTERFACE_HEADER___

#include <stdio.h>
#include <cstring>
#include <iostream>
#include <vector>

#include <mysql/mysql.h>
#include <boost/smart_ptr.hpp>
#include <boost/variant.hpp>

namespace gowinder
{
	namespace datasource
	{
		using namespace std;

		typedef boost::variant<unsigned int, int, std::string, time_t, float,
		        boost::int64_t, double> game_data_field;
		typedef boost::shared_array<game_data_field> RECORD_ROW;
		typedef boost::shared_ptr<std::vector<RECORD_ROW> > shared_vector_record_rows;
		typedef boost::shared_array<int> shared_array_fields_types;

		class record_set
		{
		public:
			virtual ~record_set()
			{
			}

			inline int get_fields_count() const
			{
				return _fields_count;
			}

			inline void set_fields_count(int fieldsCount)
			{
				_fields_count = fieldsCount;
			}

			inline const shared_vector_record_rows& get_rs() const
			{
				return _rs;
			}

			inline void set_rs(const shared_vector_record_rows& rs)
			{
				_rs = rs;
			}

			inline const shared_array_fields_types& get_shared_array_fields_types() const
			{
				return _shared_array_fields_types;
			}

			inline void set_shared_array_fields_types(
			        const shared_array_fields_types& sharedArrayFieldsTypes)
			{
				_shared_array_fields_types = sharedArrayFieldsTypes;
			}

		protected:
			int _fields_count;
			shared_array_fields_types _shared_array_fields_types;
			shared_vector_record_rows _rs;
		};
		typedef boost::shared_ptr<record_set> shared_record_set;

//		typedef boost::shared_ptr<std::vector<char> > shared_ptr_string;

		class idatasource
		{
		public:
			virtual ~idatasource()
			{
			}
			;
			virtual bool connect(std::string host, short port,
			        std::string database_name, std::string user_name,
			        std::string password) = 0;
			virtual void close() = 0;
			virtual shared_record_set get_recordset_from_db(
			        std::string sql) = 0;
			virtual bool execute_sql(std::string sql) = 0;
			virtual INT64 get_insert_id() = 0;
			virtual shared_record_set execute_store_procedure(std::string sql,
			        bool use_trasaction = false) = 0;
			virtual void set_retry_count(int count) = 0;
			virtual int get_retry_count() = 0;
		};

	}
}

#endif
