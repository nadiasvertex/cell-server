#include <iostream>
#include <query/cpp/cs_backend.h>

namespace cell {
namespace query {

class block
{
   std::wstringstream &s;
   int depth;
public:   
   block(std::wstringstream &_s, int _depth):s(_s), depth(_depth)
   {     
      s << L"{" << std::endl;
   }
   
   ~block()
   {
      depth--;
      indent();
      s << "}" << std::endl;      
   }
   
   void indent()
   {
      for(auto i=0; i<depth; ++i)
      {
         s << "   ";
      }
   }
};

cs_backend::cs_backend(sql_parser &_p):p(_p)
{
}

cs_backend::~cs_backend()
{
   std::wcout << s.str();
}

bool 
cs_backend::generate_select(sql::select *sq)
{
   s << L"public class select_query ";
   block cb(s,1);
   
   return true;   
}

   
bool 
cs_backend::generate_query(sql_parser::query_handle_type &q)
{
   if (q->k == sql::query::kind::SELECT)
   {
   
   }
   return true;   
}
   
bool 
cs_backend::generate()
{
   
   auto query_count = p.count_queries();
   for(auto i = 0; i< query_count; ++i)
   {
      auto q = p.query(i);
      if (generate_query(q)==false)
      {
         return false;
      }
   }
   
   return true;
}

} // end namespace query
} // end namespace cell