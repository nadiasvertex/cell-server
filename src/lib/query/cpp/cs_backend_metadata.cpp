#include <iostream>
#include <query/cpp/cs_backend_metadata.h>

namespace cell {
namespace query {

cs_backend_metadata::cs_backend_metadata(metadata& _m)
      :
            m(_m)
{

}

cs_backend_metadata::~cs_backend_metadata()
{
}

void cs_backend_metadata::generate_type(const data_type& t)
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

void cs_backend_metadata::generate_table(metadata::table& t)
{
   s << "\t\tpublic class " << t.name().c_str() << " : itable {" << std::endl;
   for (const auto& col : t.column_name_map())
      {
      const auto& name = col.first;
      const auto& id = col.second;
      const auto& type = t.column_map().find(id)->second;

      s << "\t\t\tpublic ";
      generate_type(type);
      s << " " << name.c_str() << ";" << std::endl;
      }

   s << "\t\t\tpublic ifilter filter(Func<" << t.name().c_str() <<  ", bool> expr) {" << std::endl;
   s << "\t\t\t\texpr_list.Add(expr);" << std::endl;
   s << "\t\t\t}" << std::endl;
   s << "\t\t\tList<System.Linq.Expressions.Expression> expr_list;" << std::endl;
   s << "\t\t}" << std::endl;
}

void cs_backend_metadata::generate_database(metadata::database& d)
{
   s << "\tnamespace " << d.name().c_str() << " {" << std::endl;
   s << "\t\tpublic class database : idatabase {" << std::endl;
   s << "\t\t\tpublic static string name {" << std::endl;
   s << "\t\t\t\tget { return \"" << d.name().c_str() << "\"; }" << std::endl;
   //s << "\t\t\t\tset { }" << std::endl;
   s << "\t\t\t}" << std::endl;
   s << "\t\t}" << std::endl;
   for (const auto& t : d.table_name_map())
      {
      const auto& name = t.first;
      const auto& id = t.second;
      auto& tbl = d.table_map().find(id)->second;

      generate_table(tbl);
      }
   s << "\t}" << std::endl;
}

void cs_backend_metadata::generate()
{
   s << "using System;" << std::endl;
   s << "namespace schema {" << std::endl;
   for (const auto& db : m.database_name_map())
      {
      const auto& name = db.first;
      const auto& id = db.second;
      auto& d = m.database_map().find(id)->second;

      generate_database(d);
      }
   s << "}" << std::endl;

   std::wcout << s.str();
}

} // end namespace query
} // end namespace cell
