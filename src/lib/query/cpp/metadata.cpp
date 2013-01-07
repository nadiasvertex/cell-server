#include <query/cpp/errors.h>
#include <query/cpp/metadata.h>

namespace cell {
namespace query {

using namespace cell::common;

expected<bool>
metadata::table::add_column(const std::string& name,
      const data_type& type)
{
   auto pos = column_ids.find(name);
   if (pos != column_ids.end())
      {
      return expected<bool>::from_exception(
            duplicate_column_error(this->tbl_name, name));
      }

   auto id = generator.next();

   columns.insert(std::make_pair(id, type));
   column_ids.insert(std::make_pair(name, id));

   return true;
}

expected<bool>
metadata::table::drop_column(const engine::column_id& cid)
{

   return false;

}

expected<engine::column_id>
metadata::table::column(const std::string& name)
{
   auto pos = column_ids.find(name);
   if (pos == column_ids.end())
      {
      return expected<engine::column_id>::from_exception(
            unknown_column_error(this->tbl_name, name));
      }

   return pos->second;
}

expected<bool>
metadata::database::create_table(const std::string& name)
{
   auto pos = table_ids.find(name);
   if (pos != table_ids.end())
      {
      return expected<bool>::from_exception(
            duplicate_table_error(db_name, name));
      }

   class table tbl(name);
   auto id = generator.next();

   tables.insert(std::make_pair(id, tbl));
   table_ids.insert(std::make_pair(name, id));

   return true;
}

expected<bool>
metadata::database::drop_table(const engine::table_id& tid)
{

}

expected<engine::table_id>
metadata::database::table(const std::string& name)
{
   auto pos = table_ids.find(name);
   if (pos == table_ids.end())
      {
      return expected<engine::table_id>::from_exception(
            unknown_table_error(db_name, name));
      }

   return pos->second;
}

expected<bool>
metadata::create_database(const std::string& name)
{

   auto pos = database_ids.find(name);
   if (pos != database_ids.end())
      {
      return expected<bool>::from_exception(duplicate_database_error(name));
      }

   class database db(name);
   auto id = generator.next();

   databases.insert(std::make_pair(id, db));
   database_ids.insert(std::make_pair(name, id));

   return true;
}

expected<bool>
metadata::drop_database(const query::database_id& did)
{
   return false;
}

expected<query::database_id>
metadata::db(const std::string& name)
{
   auto pos = database_ids.find(name);
   if (pos == database_ids.end())
      {
      return expected<query::database_id>::from_exception(
            unknown_database_error(name));
      }

   return pos->second;
}

} // end namespace query
} // end namespace cell
