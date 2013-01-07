#include <string>
#include <unordered_map>
#include <vector>

#include <query/cpp/data_type.h>
#include <query/cpp/database_id.h>
#include <data_engine/cpp/table_id.h>
#include <data_engine/cpp/column_id.h>
#include <common/cpp/expected.h>

namespace cell {
namespace query {

using namespace cell::common;

/** 
 * Stores metadata information about
 * the various databases available.
 */
class metadata {
public:
	class table {
	public:
		typedef std::unordered_map<std::string, engine::column_id> id_map_type;

		typedef std::unordered_map<engine::column_id, data_type,
				engine::column_id_hash> column_map_type;

	private:
		std::string tbl_name;

		id_map_type column_ids;

		column_map_type columns;

		engine::column_id generator;

	public:
		table(const std::string& _name) :
				tbl_name(_name) {
		}

		expected<bool> add_column(const std::string& name,
				const data_type& type);

		expected<bool> drop_column(const engine::column_id& cid);

		expected<engine::column_id> column(const std::string& name);

		const std::string& name() const {
			return tbl_name;
		}

		column_map_type& column_map() {
			return columns;
		}

		const id_map_type column_name_map() const {
			return column_ids;
		}
	};

	//==---------------------------------------------------------------------==//
public:
	class database {
	public:
		typedef std::unordered_map<std::string, engine::table_id> id_map_type;

		typedef std::unordered_map<engine::table_id, table,
				engine::table_id_hash> table_map_type;
	private:
		std::string db_name;

		id_map_type table_ids;

		table_map_type tables;

		engine::table_id generator;

	public:
		database(const std::string& _name) :
				db_name(_name) {
		}

		expected<bool> create_table(const std::string& name);

		expected<bool> drop_table(const engine::table_id& tid);

		const std::string& name() const {
			return db_name;
		}

		expected<engine::table_id> table(const std::string& name);

		table_map_type& table_map() {
			return tables;
		}

		const id_map_type& table_name_map() const {
			return table_ids;
		}
	};

	typedef std::unordered_map<std::string, query::database_id> id_map_type;

	typedef std::unordered_map<query::database_id, database,
			query::database_id_hash> db_map_type;

	//==-- Member variables -------------------------------------------------==//
private:
	id_map_type database_ids;

	db_map_type databases;

	query::database_id generator;

	//==--- Implementation ---------------------------------------------------==//
public:
	metadata() {
	}

	expected<bool> create_database(const std::string& name);

	expected<bool> drop_database(const query::database_id& did);

	expected<query::database_id> db(const std::string& name);

	db_map_type& database_map() {
		return databases;
	}

	const id_map_type& database_name_map() const {
		return database_ids;
	}

};

} // end namespace query
} // end namespace cell
