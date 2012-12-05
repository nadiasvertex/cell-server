#include <query/cpp/sql_parser.h>

namespace cell {
namespace query {
  
sql_parser::sql_parser(const std::string& query)
{
   s = new sql::Scanner(
        reinterpret_cast<const unsigned char *>(query.c_str()), 
        query.size()
   );
   p = new sql::Parser(s);
}

sql_parser::~sql_parser()
{
   delete s;
   delete p;
}

int sql_parser::parse()
{
   p->Parse();
   
   return p->errors->count;
}

} // end namespace query
} // end namespace cell