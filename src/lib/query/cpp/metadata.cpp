#include <query/cpp/errors.h>
#include <query/cpp/metadata.h>

namespace cell {
namespace query {

using namespace cell::common;

expected<bool> metadata::table::add_column(const std::string& name,
		const data_type& type) {
	auto pos = column_ids.find(name);
	if (pos != column_ids.end()) {
		return expected<bool>::from_exception(
				duplicate_column_error(this->tbl_name, name));
	}

	auto id = generator.next();

	columns.insert(std::make_pair(id, type));

	return true;
}

expected<bool> metadata::table::drop_column(const engine::column_id& cid) {

	return false;

}

expected<engine::column_id> metadata::table::column(const std::string& name) {
	auto pos = column_ids.find(name);
	if (pos == column_ids.end()) {
		return expected<engine::column_id>::from_exception(
				unknown_column_error(this->tbl_name, name));
	}

	return pos->second;
}

expected<bool> metadata::create_database(const std::string& name) {

	auto pos = database_ids.find(name);
	if (pos != database_ids.end()) {
		return expected<bool>::from_exception(duplicate_database_error(name));
	}

	class database db(name);
	auto id = generator.next();

	databases.insert(std::make_pair(id, db));

	return true;
}

expected<bool> metadata::drop_database(const query::database_id& did) {
	return false;
}

expected<query::database_id> metadata::db(const std::string& name) {
	return query::database_id();
}

} // end namespace query
} // end namespace cell
