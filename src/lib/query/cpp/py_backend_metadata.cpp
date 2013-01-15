#include <iostream>
#include <query/cpp/py_backend_metadata.h>

namespace cell {
namespace query {

py_backend_metadata::py_backend_metadata(metadata& _m)
      :
            m(_m)
{

}

py_backend_metadata::~py_backend_metadata()
{
}

void py_backend_metadata::generate_type(const data_type& t)
{
   switch (t.k)
      {
      case data_type::kind::BOOL:
         s << "bool";
      break;

      case data_type::kind::TINYINT:
         s << "char";
      break;

      case data_type::kind::SMALLINT:
         s << "short";
      break;

      case data_type::kind::INT:
         s << "int";
      break;

      case data_type::kind::BIGINT:
         s << "long";
      break;

      case data_type::kind::FLOAT:
         s << "float";
      break;

      case data_type::kind::DOUBLE:
         s << "double";
      break;

      case data_type::kind::VARCHAR:
         s << "string";
      break;

      case data_type::kind::DECIMAL:
         s << "decimal";
      break;
      }
}

void py_backend_metadata::generate_table(metadata::table& t)
{
   s << "class " << t.name().c_str() << "(object):" << std::endl;
   s << "\t__slots__=[";
   for (const auto& col : t.column_name_map())
      {
      const auto& name = col.first;
      const auto& id = col.second;
      //const auto& type = t.column_map().find(id)->second;
      s << "'" << name.c_str() << "',";
      }
   s << "]" << std::endl;

   s << "\tcol_map={";
      for (const auto& col : t.column_name_map())
         {
         const auto& name = col.first;
         const auto& id = col.second;
         s << "'" << name.c_str() << "':" <<  id.as_uint64() << ",";
         }
      s << "}" << std::endl;

   s<< "# end table " << t.name().c_str() << std::endl;
}

void py_backend_metadata::generate_database(metadata::database& d)
{
   s << "from cell.query import filter" << std::endl;
   s << "name = \"" << d.name().c_str() << "\"" << std::endl;
   for (const auto& t : d.table_name_map())
      {
      const auto& name = t.first;
      const auto& id = t.second;
      auto& tbl = d.table_map().find(id)->second;

      generate_table(tbl);
      }
}

void py_backend_metadata::generate()
{
   for (const auto& db : m.database_name_map())
      {
      const auto& name = db.first;
      const auto& id = db.second;
      auto& d = m.database_map().find(id)->second;

      generate_database(d);
      }

   std::wcout << s.str();
}

} // end namespace query
} // end namespace cell
