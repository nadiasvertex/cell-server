#include <query/cpp/errors.h>
#include <query/cpp/metadata.h>

namespace cell {
namespace query {

using namespace cell::common;

expected<bool> 
metadata::table::add_column(const std::string& name, const data_type& type)
{
   auto pos = column_ids.find(name);
   if (pos!=column_ids.end())
   {
      return expected<bool>::from_exception(duplicate_column_error(this->name, name));
   }

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
	return engine::column_id();
}

} // end namespace query
} // end namespace cell
